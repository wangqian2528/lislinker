#include "dianoworker.h"
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

DianoWorker::DianoWorker(QObject *parent) : QObject(parent)
{
    mSerialWorker = unique_ptr<SerialPort>(new SerialPort);
    mThread = unique_ptr<QThread>(new QThread);
    mSerialWorker->moveToThread(mThread.get());

    connect(this, &DianoWorker::initSerial, mSerialWorker.get(), &SerialPort::comInit);
    connect(mSerialWorker.get(), &SerialPort::updateSerialDataFrame, this, &DianoWorker::parseData);

    mThread->start();
}

void DianoWorker::parseData(const QByteArray &data)
{
    qInfo()<<data;
    QStringList list = QString(data).split("|", QString::KeepEmptyParts);

    QString name = QString(list.at(3));
    QString valueLeft = QString(list.at(4));
    QString valueRight = QString(list.at(5));
    QString refLeft = QString(list.at(6));
    QString refRight = QString(list.at(7));
    QString micValue = QString(list.at(8));
    QString unit = QString(list.at(9));
    QString result = QString(list.at(10));

    QJsonObject root_Obj;
    root_Obj.insert("name", name);
    root_Obj.insert("valueLeft", valueLeft.toInt());
    root_Obj.insert("valueRight", valueRight.toInt());
    root_Obj.insert("refLeft", refLeft.toInt());
    root_Obj.insert("refRight", refRight.toInt());
    root_Obj.insert("micValue", micValue);
    root_Obj.insert("unit", unit);
    root_Obj.insert("result", result);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QString strDir = QString("%1/analysis/DIANO/result").arg(qApp->applicationDirPath());
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkpath(strDir);
    }

    QString fileName = QString("%1/analysis/DIANO/result/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile jsonfile(fileName);
    jsonfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly);
    QTextStream in(&jsonfile);
    in << strJson;
    jsonfile.close();
}
