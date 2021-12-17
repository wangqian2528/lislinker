#include "qsharedmemory.h"
#include "config.h"

#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

#include "http_server.h"
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))
#define LOG_PRINT(a) Log::Instance()->sendLog(a);

std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map;

bool handle_ResultIndex(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    QString analysisDir = QString("%1/analysis/").arg(qApp->applicationDirPath());

    QStringList devList;

    devList<<"IDEXX"<<"BC2600"<<"V200"<<"VB1"<<"MINI8"<<"POCH"<<"EDAN"<<"DIANO"<<"MPOINT"<<"H120"<<"HBFA";

    QJsonObject root_Obj;

    for(int index = 0; index < devList.size(); index++)
    {
        QJsonArray resArry;
        QString resultDir = analysisDir + devList.at(index) + QString("/result");
        QDir dir(resultDir);
        dir.mkpath(resultDir);
        QStringList filters;
        filters<<QString("*.json");
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setNameFilters(filters);
        int dir_count = dir.count();

        for(int i=0; i<dir_count; i++)
        {
            QFileInfo fileinfo(dir[i]);
            QString file_name = fileinfo.completeBaseName();
            resArry.append(file_name);
        }
        root_Obj.insert(devList.at(index), resArry);
    }

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    rsp_callback(c, strJson.toStdString());
    return true;
}

bool handle_ResultData(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        rsp_callback(c, "NG");
        return false;
    }

    QJsonObject root_Obj = root_Doc.object();
    QString fileName = QString("%1/analysis/%2/result/%3.json").arg(qApp->applicationDirPath()).arg(root_Obj.value("dev").toString()).arg(root_Obj.value("number").toString());
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::ExistingOnly))
    {
        rsp_callback(c, "NG");
        return false;
    }

    QString data = file.readAll();
    file.close();

    rsp_callback(c, data.toStdString());
    return true;
}

bool handle_ResultConfirm(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        rsp_callback(c, "NG");
        return false;
    }

    QJsonObject root_Obj = root_Doc.object();
    QString fileName = QString("%1/analysis/%2/result/%3.json").arg(qApp->applicationDirPath()).arg(root_Obj.value("dev").toString()).arg(root_Obj.value("number").toString());
    QFile file(fileName);
    if(file.exists())
    {
        file.remove();
    }
    else
    {
        rsp_callback(c, "NG");
        return false;
    }

    rsp_callback(c, "OK");
    return true;
}

bool handle_IDEXX_RequestNew(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    //构建jsonDoc
    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        rsp_callback(c, "NG");
        return false;
    }

    QString dir_name = QString("%1/analysis/IDEXX/request").arg(qApp->applicationDirPath());
    QDir req_dir = QDir(dir_name);
    if(!req_dir.exists())
    {
        req_dir.mkpath(dir_name);
    }

    QString fileName = QString("%1/analysis/IDEXX/request/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly)) //only new create
    {
        rsp_callback(c, "NG");
        return false;
    }

    //format json file
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QTextStream in(&file);
    in << strJson;
    file.close();

    rsp_callback(c, "OK");
    return true;
}

bool handle_MINI8_RequestNew(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    //构建jsonDoc
    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        qInfo()<<"json file error!";
        rsp_callback(c, "NG");
        return false;
    }

    QString dir_name = QString("%1/analysis/MINI8/request").arg(qApp->applicationDirPath());
    QDir req_dir = QDir(dir_name);
    if(!req_dir.exists())
    {
        req_dir.mkpath(dir_name);
    }

    QString fileName = QString("%1/analysis/MINI8/request/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile file(fileName);
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly)) //only new create
    {
        qInfo()<<"dir error!"<<fileName;
        rsp_callback(c, "NG");
        return false;
    }

    //format json file
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QTextStream in(&file);
    in << strJson;
    file.close();

    rsp_callback(c, "OK");
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedMemory mem("HTTP");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    QThread thread;
    thread.start();

    HttpServer http_server;

    //get all report indexx
    http_server.AddHandler("/api/ResultIndex", handle_ResultIndex);             //获取结果列表
    http_server.AddHandler("/api/ResultData", handle_ResultData);               //获取结果内容
    http_server.AddHandler("/api/ResultConfirm", handle_ResultConfirm);         //获取结果确认

    http_server.AddHandler("/api/IDEXX/RequestNew", handle_IDEXX_RequestNew);   //idexx下发工单
    http_server.AddHandler("/api/MINI8/RequestNew", handle_MINI8_RequestNew);   //idexx下发工单

    http_server.Init(QString("%1").arg(Config::HttpPort).toStdString());
    http_server.moveToThread(&thread);
    QTimer::singleShot(100, &http_server, SLOT(Start()));

    return a.exec();
}
