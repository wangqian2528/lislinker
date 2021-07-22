#include "vb1worker.h"
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

VB1Worker::VB1Worker(QObject *parent) : QObject(parent)
{
    mSerialWorker = unique_ptr<SerialPort>(new SerialPort);
    mThread = unique_ptr<QThread>(new QThread);
    mSerialWorker->moveToThread(mThread.get());

    connect(this, &VB1Worker::initSerial, mSerialWorker.get(), &SerialPort::comInit);
    connect(mSerialWorker.get(), &SerialPort::updateSerialDataFrame, this, &VB1Worker::parseData);

    mThread->start();
}

void VB1Worker::parseData(const QByteArray &data)
{
    qInfo()<<data;

    QStringList list = QString(data).split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    list.removeAt(list.size()-1);

    int result_index;

    for(int i = 0; i < list.size(); i++)
    {
        QString str = QString(list.at(i)).replace(QRegExp("\\s{1,}"), " ");

        QStringList strList = str.split(" ");
        if(QString(strList.at(0)) == QString("Total"))
        {
            result_index = i+1;
            break;
        }
    }

    QJsonObject root_Obj;
    QJsonArray result_arry;

    for(int i = result_index; i < list.size(); i++)
    {
        QStringList resList = QString(list.at(i)).split(QRegExp("\\s{2,}"));
        if(resList.size() < 4)
        {
            resList.append(QString(" "));
        }
        qDebug()<<resList;
        jsonInsert(result_arry, resList.at(0), resList.at(1), resList.at(2), resList.at(3));
    }

    root_Obj.insert("results", result_arry);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QString strDir = QString("%1/analysis/VB1/result").arg(qApp->applicationDirPath());
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkpath(strDir);
    }

    QString fileName = QString("%1/analysis/VB1/result/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile jsonfile(fileName);
    jsonfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly);
    QTextStream in(&jsonfile);
    in << strJson;
    jsonfile.close();
}

void VB1Worker::jsonInsert(QJsonArray &arr, const QString &key, const QString &value, const QString &range, const QString &unit)
{
    QJsonObject tmpObj;
    QStringList strList = range.split("-");

    tmpObj["title"] = key;
    tmpObj["value"] = value;
    tmpObj["unit"] = unit;
    tmpObj["min"] = QString(strList.at(0));
    tmpObj["max"] = QString(strList.at(1));
    arr.append(tmpObj);
}
