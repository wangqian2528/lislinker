#include "analysis.h"
#include "qmutex.h"
#include "config.h"
#include "qdatetime.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#define QXMLTIME qPrintable(QDateTime::currentDateTime().toString("MM/dd/yyyy HH:mm:ss"))

QScopedPointer<Analysis> Analysis::self;
Analysis *Analysis::Instance()
{
    if (self.isNull())
    {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull())
        {
            self.reset(new Analysis());
        }
    }
    return self.data();
}

Analysis::Analysis(QObject *parent) : QObject(parent)
{
    m_time = new QTimer(this);
    connect(m_time, SIGNAL(timeout()), this, SLOT(AnalysisHandle()));
}

void Analysis::start(int s)
{
    m_time->start(s);
}

void Analysis::AnalysisHandle()
{
    requestHandle();
    resultHandle();
}

void Analysis::requestHandle()
{
    QString requestDir = QString("%1/analysis/IDEXX/request").arg(qApp->applicationDirPath());
    QDir dir(requestDir);
    if(!dir.exists())
    {
        dir.mkpath(requestDir);
    }

    //过滤文件后缀.json
    QStringList filters;
    filters<<QString("*.json");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);

    int dir_count = dir.count();
    for(int i=0; i<dir_count; i++)
    {
        QString file_name = QString("%1/analysis/IDEXX/request/%2").arg(qApp->applicationDirPath()).arg(dir[i]);
        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
        {
            file.remove();
            continue;
        }

        QString data = file.readAll();
        file.close();

        json2xml(data);

        file.remove();
    }
}

void Analysis::resultHandle()
{
    QDir dir(Config::IdexxResultFolder);
    if(!dir.exists())
    {
        dir.mkpath(Config::IdexxResultFolder);
    }

    //过滤文件后缀.json
    QStringList filters;
    filters<<QString("*.xml");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);
    int dir_count = dir.count();
    for(int i=0; i<dir_count; i++)
    {
        QString file_name = QString("%1/%2").arg(Config::IdexxResultFolder).arg(dir[i]);
        xml2json(file_name);
    }
}

bool Analysis::json2xml(const QString &s)
{
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(s.toLocal8Bit().data(), &err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        return false;
    }

    QJsonObject root_Obj = root_Doc.object();
    QString requestNum = root_Obj.value("requisition_number").toString();
    QJsonObject client_Obj = root_Obj.value("client").toObject();
    QJsonObject patient_Obj = root_Obj.value("patient").toObject();
    QJsonObject patient_weight_Obj = patient_Obj.value("patient_weight").toObject();
    QJsonObject doctor_Obj = root_Obj.value("doctor").toObject();
    QString service = root_Obj.value("service_add").toString();

    QString xmlName = QString("%1/%2.xml").arg(Config::IdexxRequestFolder).arg(requestNum);
    QFile file(xmlName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeDTD(QString("<!DOCTYPE message SYSTEM \"work_request_20.dtd\">"));
    writer.writeStartElement("message");
    writer.writeAttribute("message_id", "");
    writer.writeAttribute("message_dt", QXMLTIME);
    writer.writeAttribute("message_type", "Work_Request");
    writer.writeAttribute("message_sub_type", "New");
    writer.writeAttribute("message_dtd_version_number", "2.0");

    writer.writeStartElement("header");
    writer.writeStartElement("from_application_id");
    writer.writeCharacters("");
    writer.writeEndElement();
    writer.writeStartElement("to_application_id");
    writer.writeCharacters("");
    writer.writeEndElement();
    writer.writeEndElement();

    writer.writeStartElement("body");
    writer.writeStartElement("work_request");
    writer.writeAttribute("requisition_number", requestNum);

    writer.writeStartElement("client");
    writer.writeAttribute("client_id", client_Obj.value("client_id").toString());
    writer.writeStartElement("first_name");
    writer.writeCharacters(client_Obj.value("first_name").toString());
    writer.writeEndElement();
    writer.writeStartElement("last_name");
    writer.writeCharacters(client_Obj.value("last_name").toString());
    writer.writeEndElement();
    writer.writeEndElement();//</client>

    writer.writeStartElement("patient");
    writer.writeAttribute("patient_id", patient_Obj.value("patient_id").toString());
    writer.writeAttribute("patient_species", patient_Obj.value("patient_species").toString());
    writer.writeAttribute("patient_gender", patient_Obj.value("patient_gender").toString());
    writer.writeStartElement("patient_name");
    writer.writeCharacters(patient_Obj.value("patient_name").toString());
    writer.writeEndElement();
    writer.writeStartElement("patient_breed");
    writer.writeCharacters(patient_Obj.value("patient_breed").toString());
    writer.writeEndElement();
    writer.writeStartElement("patient_birth_dt");
    writer.writeCharacters(patient_Obj.value("patient_birth_dt").toString());
    writer.writeEndElement();
    writer.writeStartElement("patient_weight");
    writer.writeAttribute("patient_weight_uom", patient_weight_Obj.value("patient_weight_uom").toString());
    writer.writeStartElement("weight");
    writer.writeCharacters(patient_weight_Obj.value("weight").toString());
    writer.writeEndElement();
    writer.writeEndElement();//</patient_weight>
    writer.writeEndElement();//</patient>

    writer.writeStartElement("doctor");
    writer.writeStartElement("first_name");
    writer.writeCharacters(doctor_Obj.value("first_name").toString());
    writer.writeEndElement();
    writer.writeStartElement("last_name");
    writer.writeCharacters(doctor_Obj.value("last_name").toString());
    writer.writeEndElement();
    writer.writeEndElement();//</doctor>

    writer.writeStartElement("service_add");
    writer.writeStartElement("service_cd");
    writer.writeCharacters(service);
    writer.writeEndElement();
    writer.writeEndElement();//</service_add>

    writer.writeEndElement();//</work_request>
    writer.writeEndElement();//</body>
    writer.writeEndElement();//</message>
    writer.writeEndDocument();

    file.close();
    return true;
}

bool Analysis::xml2json(const QString &s)
{
    QFile file(s);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        file.remove();
        return false;
    }

    QXmlStreamReader reader(&file);
    QString requestNum;
    QJsonObject root_Obj;
    QJsonArray arry_Result;
    QJsonArray instrument_notes;

    while(!reader.atEnd())
    {
        reader.readNext();

        if(reader.isStartElement() && reader.name().toString() == "result")
        {
            requestNum = reader.attributes().value("requisition_number").toString();
            if(requestNum == QString(""))
            {
                file.close();
                file.remove();
                return false;
            }
        }

        if(reader.isStartElement() && reader.name().toString() == "assay_result")
        {
            QString assay_name = reader.attributes().value("assay_name").toString();
            reader.readNext();
            reader.readNext();
            QString uom = reader.readElementText();
            reader.readNext();
            reader.readNext();
            QString value = reader.readElementText();
            reader.readNext();
            reader.readNext();
            reader.readNext();
            reader.readNext();
            QString critical_low = reader.readElementText();
            reader.readNext();
            reader.readNext();
            QString low = reader.readElementText();
            reader.readNext();
            reader.readNext();
            QString high = reader.readElementText();
            reader.readNext();
            reader.readNext();
            QString critical_high = reader.readElementText();

            QJsonObject result_obj;
            result_obj.insert("assay_name", assay_name);
            result_obj.insert("uom", uom);
            result_obj.insert("value", value);
            result_obj.insert("critical_low", critical_low);
            result_obj.insert("low", low);
            result_obj.insert("high", high);
            result_obj.insert("critical_high", critical_high);
            arry_Result.append(result_obj);
        }

        if(reader.isStartElement() && reader.name().toString() == "instrument_note")
        {
            QString instrument_note = reader.readElementText();
            instrument_notes.append(instrument_note);
        }
    }

    file.close();
    file.remove();

    root_Obj.insert("results", arry_Result);
    root_Obj.insert("instrument_notes", instrument_notes);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QString fileName = QString("%1/analysis/IDEXX/result/%2.json").arg(qApp->applicationDirPath()).arg(requestNum);
    QFile jsonfile(fileName);
    if(!jsonfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly))
    {
        return false;
    }

    QTextStream in(&jsonfile);
    in << strJson;
    jsonfile.close();

    return true;
}
