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
#include "qprocess.h"

#include "QtSerialPort/QSerialPortInfo"
#include "QtSerialPort/QSerialPort"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))
#define LOG_PRINT(a) Log::Instance()->sendLog(a);

std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map;
std::string HttpServer::s_web_dir = "./web";
mg_serve_http_opts HttpServer::s_server_option;

static bool g_dev_status = false;

static QStringList getAllComPortName()
{
    QStringList nameList;
    QSerialPort temp_serial;
    foreach (const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    {
        temp_serial.setPort(Info);
        if (temp_serial.open(QIODevice::ReadWrite))
        {
            //如果串口是可以读写方式打开的
            nameList << (Info.portName());
            temp_serial.close();
        }
    }
    return nameList;
}

static void startProcess(const QString &pName)
{
    QProcess *p = new QProcess;
    QString appDir = QString("\"%1/%2.exe\"").arg(qApp->applicationDirPath()).arg(pName);
    qDebug()<<appDir;
    p->start(appDir);
}

static void killProcess(const QString &pName)
{
    QProcess *p = new QProcess;
    p->start(QString("taskkill /im %1.exe /f").arg(pName));
}

static void startAllPro(void)
{
    if(Config::IdexxEnabled) startProcess("idexx");
    if(Config::BC2600Enabled) startProcess("bc2600");
    if(Config::V200Enabled) startProcess("v200");
    if(Config::VB1Enabled) startProcess("vb1");
    if(Config::POCHEnabled) startProcess("poch");
    if(Config::Mini8Enabled) startProcess("mini8");
    if(Config::DIANOEnabled) startProcess("diano");
    if(Config::EDANEnabled) startProcess("edan");
    if(Config::H120Enabled) startProcess("h120");
    if(Config::MPointEnabled) startProcess("mpoint");
}

static void stopAllPro(void)
{
    if(Config::IdexxEnabled) killProcess("idexx");
    if(Config::BC2600Enabled) killProcess("bc2600");
    if(Config::V200Enabled) killProcess("v200");
    if(Config::VB1Enabled) killProcess("vb1");
    if(Config::POCHEnabled) killProcess("poch");
    if(Config::Mini8Enabled) killProcess("mini8");
    if(Config::DIANOEnabled) killProcess("diano");
    if(Config::EDANEnabled) killProcess("edan");
    if(Config::H120Enabled) killProcess("h120");
    if(Config::MPointEnabled) killProcess("mpoint");
}

bool handle_ResultIndex(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    Q_UNUSED(url)
    Q_UNUSED(body)

    QString analysisDir = QString("%1/analysis/").arg(qApp->applicationDirPath());

    QStringList devList;

    devList << "IDEXX"
            << "BC2600"
            << "V200"
            << "VB1"
            << "MINI8"
            << "POCH"
            << "EDAN"
            << "DIANO"
            << "MPOINT"
            << "H120";

    QJsonObject root_Obj;

    for (int index = 0; index < devList.size(); index++)
    {
        QJsonArray resArry;
        QString resultDir = analysisDir + devList.at(index) + QString("/result");
        QDir dir(resultDir);
        dir.mkpath(resultDir);
        QStringList filters;
        filters << QString("*.json");
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setNameFilters(filters);
        int dir_count = dir.count();

        for (int i = 0; i < dir_count; i++)
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
    Q_UNUSED(url)

    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if (err_rpt.error != QJsonParseError::NoError)
    {
        rsp_callback(c, "NG");
        return false;
    }

    QJsonObject root_Obj = root_Doc.object();
    QString fileName = QString("%1/analysis/%2/result/%3.json").arg(qApp->applicationDirPath()).arg(root_Obj.value("dev").toString()).arg(root_Obj.value("number").toString());
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::ExistingOnly))
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
    Q_UNUSED(url)

    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if (err_rpt.error != QJsonParseError::NoError)
    {
        rsp_callback(c, "NG");
        return false;
    }

    QJsonObject root_Obj = root_Doc.object();
    QString fileName = QString("%1/analysis/%2/result/%3.json").arg(qApp->applicationDirPath()).arg(root_Obj.value("dev").toString()).arg(root_Obj.value("number").toString());
    QFile file(fileName);
    if (file.exists())
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
    Q_UNUSED(url)

    //构建jsonDoc
    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if (err_rpt.error != QJsonParseError::NoError)
    {
        rsp_callback(c, "NG");
        return false;
    }

    QString dir_name = QString("%1/analysis/IDEXX/request").arg(qApp->applicationDirPath());
    QDir req_dir = QDir(dir_name);
    if (!req_dir.exists())
    {
        req_dir.mkpath(dir_name);
    }

    QString fileName = QString("%1/analysis/IDEXX/request/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly)) //only new create
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
    Q_UNUSED(url)

    //构建jsonDoc
    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if (err_rpt.error != QJsonParseError::NoError)
    {
        qInfo() << "json file error!";
        rsp_callback(c, "NG");
        return false;
    }

    QString dir_name = QString("%1/analysis/MINI8/request").arg(qApp->applicationDirPath());
    QDir req_dir = QDir(dir_name);
    if (!req_dir.exists())
    {
        req_dir.mkpath(dir_name);
    }

    QString fileName = QString("%1/analysis/MINI8/request/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly)) //only new create
    {
        qInfo() << "dir error!" << fileName;
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

bool handle_dev_auto(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    Q_UNUSED(url)

    //构建jsonDoc
    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if (err_rpt.error != QJsonParseError::NoError)
    {
        rsp_callback(c, "NG");
        return false;
    }

    QJsonObject root_Obj = root_Doc.object();

    Config::AppAutoStart = root_Obj.value("auto").toBool();
    Config::writeConfig();

    rsp_callback(c, "OK");
    return true;
}

bool handle_dev_start(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    Q_UNUSED(url)
    Q_UNUSED(body)

    g_dev_status = true;
    startAllPro();

    rsp_callback(c, "OK");
    return true;
}

bool handle_dev_stop(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    Q_UNUSED(url)
    Q_UNUSED(body)

    g_dev_status = false;
    stopAllPro();

    rsp_callback(c, "OK");
    return true;
}

bool handle_dev_status(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    Q_UNUSED(url)
    Q_UNUSED(body)

    QJsonObject root_Obj;
    root_Obj.insert("status", g_dev_status);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    rsp_callback(c, strJson.toStdString());
    return true;
}

bool handle_dev_reset(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    Q_UNUSED(url)
    Q_UNUSED(body)

    rsp_callback(c, "OK");
    return true;
}

bool handle_dev_setInfo(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    Q_UNUSED(url)
    Q_UNUSED(body)

    QJsonObject root_Obj;
    QJsonArray serial_array;

    QJsonObject bc2600;
    QJsonObject V200;
    QJsonObject VB1;
    QJsonObject POCH;
    QJsonObject DIANO;
    QJsonObject EDAN;
    QJsonObject MPOINT;
    QJsonObject H120;

    QStringList serial_list = getAllComPortName();
    for (int i = 0; i < serial_list.size(); i++)
    {
        serial_array.append(serial_list.at(i));
    }
    root_Obj.insert("SerialPort", serial_array);

    bc2600.insert("Enable", Config::BC2600Enabled);
    bc2600.insert("ComPort", Config::BC2600ComPort);
    root_Obj.insert("bc2600", bc2600);

    V200.insert("Enable", Config::V200Enabled);
    V200.insert("IP", Config::V200IP);
    V200.insert("Port", Config::V200Port);
    root_Obj.insert("V200", V200);

    VB1.insert("Enable", Config::VB1Enabled);
    VB1.insert("ComPort", Config::VB1ComPort);
    root_Obj.insert("VB1", VB1);

    POCH.insert("Enable", Config::POCHEnabled);
    POCH.insert("ComPort", Config::POCHComPort);
    root_Obj.insert("POCH", POCH);

    DIANO.insert("Enable", Config::DIANOEnabled);
    DIANO.insert("ComPort", Config::DIANOComPort);
    DIANO.insert("Baud", Config::DIANOComBaud);
    root_Obj.insert("DIANO", DIANO);

    EDAN.insert("Enable", Config::EDANEnabled);
    EDAN.insert("Port", Config::EDANPort);
    root_Obj.insert("EDAN", EDAN);

    MPOINT.insert("Enable", Config::MPointEnabled);
    MPOINT.insert("ComPort", Config::MPointComPort);
    root_Obj.insert("MPOINT", MPOINT);

    H120.insert("Enable", Config::H120Enabled);
    H120.insert("ComPort", Config::H120ComPort);
    root_Obj.insert("H120", H120);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    rsp_callback(c, strJson.toStdString());
    return true;
}

bool handle_dev_setEdit(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
    Q_UNUSED(url)

    //构建jsonDoc
    QString jsonQstr = QString::fromStdString(body);
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(jsonQstr.toLocal8Bit().data(), &err_rpt);
    if (err_rpt.error != QJsonParseError::NoError)
    {
        rsp_callback(c, "NG");
        return false;
    }

    QJsonObject root_Obj = root_Doc.object();
    QJsonObject child_obj;
    QJsonValue value;

    if (root_Obj.contains("BC2600"))
    {
        value = root_Obj.value("BC2600");
        child_obj = value.toObject();

        Config::BC2600Enabled = child_obj.value("Enable").toBool();
        Config::BC2600ComPort = child_obj.value("ComPort").toString();
    }

    if (root_Obj.contains("V200"))
    {
        value = root_Obj.value("V200");
        child_obj = value.toObject();

        Config::V200Enabled = child_obj.value("Enable").toBool();
        Config::V200IP = child_obj.value("IP").toString();
        Config::V200Port = child_obj.value("Port").toInt();
    }

    if (root_Obj.contains("VB1"))
    {
        value = root_Obj.value("VB1");
        child_obj = value.toObject();

        Config::VB1Enabled = child_obj.value("Enable").toBool();
        Config::VB1ComPort = child_obj.value("ComPort").toString();
    }

    if (root_Obj.contains("POCH"))
    {
        value = root_Obj.value("POCH");
        child_obj = value.toObject();

        Config::POCHEnabled = child_obj.value("Enable").toBool();
        Config::POCHComPort = child_obj.value("ComPort").toString();
    }

    if (root_Obj.contains("DIANO"))
    {
        value = root_Obj.value("DIANO");
        child_obj = value.toObject();

        Config::DIANOEnabled = child_obj.value("Enable").toBool();
        Config::DIANOComPort = child_obj.value("ComPort").toString();
        Config::DIANOComBaud = child_obj.value("Baud").toInt();
    }

    if (root_Obj.contains("EDAN"))
    {
        value = root_Obj.value("EDAN");
        child_obj = value.toObject();

        Config::EDANPort = child_obj.value("Port").toInt();
    }

    if (root_Obj.contains("MPOINT"))
    {
        value = root_Obj.value("MPOINT");
        child_obj = value.toObject();

        Config::MPointEnabled = child_obj.value("Enable").toBool();
        Config::MPointComPort = child_obj.value("ComPort").toString();
    }

    if (root_Obj.contains("H120"))
    {
        value = root_Obj.value("H120");
        child_obj = value.toObject();

        Config::H120Enabled = child_obj.value("Enable").toBool();
        Config::H120ComPort = child_obj.value("ComPort").toString();
    }

    Config::writeConfig();

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

    if(Config::AppAutoStart)
    {
        g_dev_status = true;
        startAllPro();
    }

    QThread thread;
    thread.start();

    HttpServer http_server;

    //get all report indexx
    http_server.AddHandler("/api/ResultIndex", handle_ResultIndex);     //获取结果列表
    http_server.AddHandler("/api/ResultData", handle_ResultData);       //获取结果内容
    http_server.AddHandler("/api/ResultConfirm", handle_ResultConfirm); //获取结果确认

    http_server.AddHandler("/api/IDEXX/RequestNew", handle_IDEXX_RequestNew); //idexx下发工单
    http_server.AddHandler("/api/MINI8/RequestNew", handle_MINI8_RequestNew); //idexx下发工单

    http_server.AddHandler("/api/dev/auto", handle_dev_auto);     //开机启动
    http_server.AddHandler("/api/dev/start", handle_dev_start);   //启动
    http_server.AddHandler("/api/dev/stop", handle_dev_stop);     //停止
    http_server.AddHandler("/api/dev/status", handle_dev_status); //状态
    http_server.AddHandler("/api/dev/reset", handle_dev_reset);   //复位

    http_server.AddHandler("/api/setInfo", handle_dev_setInfo); //查询
    http_server.AddHandler("/api/setEdit", handle_dev_setEdit); //设置

    http_server.Init(QString("%1").arg(Config::HttpPort).toStdString());
    http_server.moveToThread(&thread);
    QTimer::singleShot(100, &http_server, SLOT(Start()));

    return a.exec();
}
