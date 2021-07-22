#include "edanworker.h"
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

EdanWorker::EdanWorker(QObject *parent) : QObject(parent)
{
    mNetWorker = std::unique_ptr<NetServer>(new NetServer);
    mThread = std::unique_ptr<QThread>(new QThread);
    mNetWorker->moveToThread(mThread.get());

    connect(this, &EdanWorker::initNet, mNetWorker.get(), &NetServer::netInit);
    connect(mNetWorker.get(), &NetServer::updateNetDataFrame, this, &EdanWorker::parseData);

    mThread->start();
}

void EdanWorker::parseData(const QByteArray &data)
{
    QStringList list = QString(data).split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    QJsonArray result_arry;
    for(int i = 0; i < list.size(); i++)
    {
        QStringList itemList = QString(list.at(i)).split("|");
        qInfo()<<itemList.at(0);
        if(itemList.at(0)=="OBX" && (itemList.at(3)=="0"||itemList.at(3)=="1"))
        {
            jsonInsert(result_arry, itemList.at(4), itemList.at(6), itemList.at(5), itemList.at(7));
        }
    }

    QJsonObject root_Obj;
    root_Obj.insert("results", result_arry);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QString strDir = QString("%1/analysis/EDAN/result").arg(qApp->applicationDirPath());
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkpath(strDir);
    }

    QString fileName = QString("%1/analysis/EDAN/result/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile jsonfile(fileName);
    jsonfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly);
    QTextStream in(&jsonfile);
    in << strJson;
    jsonfile.close();
}

void EdanWorker::jsonInsert(QJsonArray &arr, const QString &key, const QString &unit, const QString &value, const QString &ref)
{
    QJsonObject tmpObj;
    tmpObj["name"] = key;
    tmpObj["unit"] = unit;
    tmpObj["value"] = value;
    tmpObj["reference"] = ref;
    arr.append(tmpObj);
}
