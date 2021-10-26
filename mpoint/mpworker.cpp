#include "mpworker.h"

MPWorker::MPWorker(QObject *parent) : QObject(parent)
{
    mSerialWorker = unique_ptr<SerialPort>(new SerialPort);
    mThread = unique_ptr<QThread>(new QThread);
    mSerialWorker->moveToThread(mThread.get());

    connect(this, &MPWorker::initSerial, mSerialWorker.get(), &SerialPort::comInit);
    connect(mSerialWorker.get(), &SerialPort::updateSerialDataFrame, this, &MPWorker::parseData);

    mThread->start();
}

const char taeget_string[7] = {0x02,0x72,0x65,0x73,0x75,0x6C,0x74};
void MPWorker::parseData(const QByteArray &data)
{
    if(memcmp(taeget_string, data.data(), 7) != 0)
        return;

    //去掉头尾和CRC
    QByteArray r_data = data.right(data.size() - 7);
    r_data.chop(3);

    if(r_data.at(0) != '{' || r_data.at(r_data.size()-1) != '}') return;

    r_data = r_data.right(r_data.size() - 1);
    r_data.chop(1);

    QString result = r_data;
    QStringList res_list = result.split(";");

    QJsonObject root_Obj;
    QJsonArray result_arry;

    for(int i = 0; i < res_list.size(); i++)
    {
        QString item = res_list.at(i);
        QStringList item_list = item.split(":");
        mapRes.insert(item_list.at(0), item_list.at(1));
    }

    jsonInsert(result_arry, "INR", mapRes["INR"]);
    jsonInsert(result_arry, "PT", mapRes["PT"]);
    jsonInsert(result_arry, "APTT", mapRes["APTT"]);
    jsonInsert(result_arry, "FIB", mapRes["FIB"]);
    jsonInsert(result_arry, "TT", mapRes["TT"]);
    jsonInsert(result_arry, "RATIO", mapRes["RATIO"]);
    jsonInsert(result_arry, "QC", mapRes["QC"]);

    root_Obj.insert("results", result_arry);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QString strDir = QString("%1/analysis/MPOINT/result").arg(qApp->applicationDirPath());
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkpath(strDir);
    }

    QString fileName = QString("%1/analysis/MPOINT/result/%2.json").arg(qApp->applicationDirPath()).arg(mapRes["PID"]);
    QFile jsonfile(fileName);
    jsonfile.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream in(&jsonfile);
    in << strJson;
    jsonfile.close();
}

void MPWorker::jsonInsert(QJsonArray &arr, const QString &key, const QString &value)
{
    QJsonObject tmpObj;

    tmpObj["key"] = key;
    tmpObj["value"] = value;

    arr.append(tmpObj);
}
