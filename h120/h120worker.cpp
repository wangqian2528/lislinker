#include "h120worker.h"
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

H120Worker::H120Worker(QObject *parent) : QObject(parent)
{
    mSerialWorker = unique_ptr<SerialPort>(new SerialPort);
    mThread = unique_ptr<QThread>(new QThread);
    mSerialWorker->moveToThread(mThread.get());

    connect(this, &H120Worker::initSerial, mSerialWorker.get(), &SerialPort::comInit);
    connect(mSerialWorker.get(), &SerialPort::updateSerialDataFrame, this, &H120Worker::parseData);

    mThread->start();
}

void H120Worker::parseData(const QByteArray &data)
{
    QStringList list = QString(data).split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    list.removeAt(list.size()-1);
    list.removeFirst();list.removeFirst();list.removeFirst();

    QJsonObject root_Obj;
    QJsonArray result_arry;

    for(int i = 0; i < list.size(); i++)
    {
        QString str = QString(list.at(i)).simplified();
        QStringList strList = str.split(" ");

        for(int i = strList.size(); i < 4; i++)
        {
            strList.insert(2,"-");
        }
        qInfo()<<strList;
        jsonInsert(result_arry, strList.at(0), strList.at(1), strList.at(2), strList.at(3));
    }

    root_Obj.insert("results", result_arry);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QString strDir = QString("%1/analysis/H120/result").arg(qApp->applicationDirPath());
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkpath(strDir);
    }

    QString fileName = QString("%1/analysis/H120/result/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile jsonfile(fileName);
    jsonfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly);
    QTextStream in(&jsonfile);
    in << strJson;
    jsonfile.close();
}

void H120Worker::jsonInsert(QJsonArray &arr, const QString &key, const QString &value, const QString &purity, const QString &unit)
{
    QJsonObject tmpObj;

    tmpObj["key"] = key;
    tmpObj["value"] = value;
    tmpObj["purity"] = purity;
    tmpObj["unit"] = unit;

    arr.append(tmpObj);
}
