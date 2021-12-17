#include "hbfaworker.h"
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

HBFAWorker::HBFAWorker(QObject *parent) : QObject(parent)
{
    mTcpWorker = unique_ptr<TcpWorker>(new TcpWorker);
    mThread = unique_ptr<QThread>(new QThread);
    mTcpWorker->moveToThread(mThread.get());

    connect(this, &HBFAWorker::initTcpIP, mTcpWorker.get(), &TcpWorker::socketInit);
    connect(this, &HBFAWorker::sendRsp, mTcpWorker.get(), &TcpWorker::socketSend);
    connect(mTcpWorker.get(), &TcpWorker::updateTcpDataFrame, this, &HBFAWorker::parseData);

    mThread->start();
}

void HBFAWorker::parseData(const QString &data)
{
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(data.toLocal8Bit().data(), &err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        return;
    }

    QJsonObject root_Obj = root_Doc.object();
    QString requestNum = root_Obj.value("sample_ID").toString();

    QJsonObject rsp_Obj;
    rsp_Obj.insert("sample_ID", requestNum);

    QJsonDocument rsp_Doc;
    rsp_Doc.setObject(rsp_Obj);
    QByteArray rsp_str = rsp_Doc.toJson(QJsonDocument::Indented);
    QString rspStrJson(rsp_str);
    emit sendRsp(rspStrJson);

    QString fileName = QString("%1/analysis/HBFA/result/%2.json").arg(qApp->applicationDirPath()).arg(requestNum);
    QFile jsonfile(fileName);
    jsonfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly);
    QTextStream in(&jsonfile);

    QString trStr = QString(data.toLocal8Bit().data());
    in << trStr;
    jsonfile.close();
}
