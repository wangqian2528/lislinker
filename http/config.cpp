#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";
//[APP]
bool Config::AppAutoStart = false;              //自启动选项
int Config::AppListenPort = 7790;               //监听端口号
//[HTTP]
int Config::HttpPort = 7788;                    //http端口号

//[IDEXX]
bool Config::IdexxEnabled = false;              //配置使能
QString Config::IdexxRequestFolder = "";        //工单保存路径
QString Config::IdexxResultFolder = "";         //报告保存路径

//[BC2600]
bool Config::BC2600Enabled = false;             //配置使能
QString Config::BC2600ComPort = "";             //串口号

//[Mini8]
bool Config::Mini8Enabled = false;              //配置使能
QString Config::Mini8RequestFolder = "";        //工单保存路径
QString Config::Mini8ResultFolder = "";         //报告保存路径

//[V200]
bool Config::V200Enabled = false;               //配置使能
QString Config::V200IP = "";                    //IP地址
int Config::V200Port = 0;                       //端口号

//[VB1]
bool Config::VB1Enabled = false;                //配置使能
QString Config::VB1ComPort = "";                //串口号

//[pocH]
bool Config::POCHEnabled = false;               //配置使能
QString Config::POCHComPort = "";               //串口号

//[DIANO]
bool Config::DIANOEnabled = false;               //配置使能
QString Config::DIANOComPort = "";               //串口号
int Config::DIANOComBaud = 9600;                 //波特率

//[EDAN]
bool Config::EDANEnabled = false;                //配置使能
int Config::EDANPort = 0;                        //端口号

//MPOINT
bool Config::MPointEnabled = false;              //配置使能
QString Config::MPointComPort = "";              //串口

//H120尿机
bool Config::H120Enabled = false;                //配置使能
QString Config::H120ComPort = "";                //串口

bool Config::checkConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(Config::ConfigFile);
    if (file.size() == 0)
    {
        qDebug()<< "no config file, now creat!";
        writeConfig();
        return false;
    }
    return true;
}

void Config::readConfig()
{
    if (!checkConfig())
    {
        return;
    }

    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[APP]
    set.beginGroup("APP");
    Config::AppAutoStart = set.value("AutoStart", Config::AppAutoStart).toBool();
    Config::AppListenPort = set.value("ListenPort", Config::AppListenPort).toInt();
    set.endGroup();

    //[HTTP]
    set.beginGroup("HTTP");
    Config::HttpPort = set.value("HttpPort", Config::HttpPort).toInt();
    set.endGroup();

    //[IDEXX]
    set.beginGroup("IDEXX");
    Config::IdexxEnabled = set.value("Enabled", Config::IdexxEnabled).toBool();
    Config::IdexxRequestFolder = set.value("RequestFolder", Config::IdexxRequestFolder).toString();
    Config::IdexxResultFolder = set.value("ResultFolder", Config::IdexxResultFolder).toString();
    set.endGroup();

    //[BC2600]
    set.beginGroup("BC2600");
    Config::BC2600Enabled = set.value("Enabled", Config::BC2600Enabled).toBool();
    Config::BC2600ComPort = set.value("ComPort", Config::BC2600ComPort).toString();
    set.endGroup();

    //[Mini8]
    set.beginGroup("Mini8");
    Config::Mini8Enabled = set.value("Enabled", Config::Mini8Enabled).toBool();
    Config::Mini8RequestFolder = set.value("RequestFolder", Config::Mini8RequestFolder).toString();
    Config::Mini8ResultFolder = set.value("ResultFolder", Config::Mini8ResultFolder).toString();
    set.endGroup();

    //[V200]
    set.beginGroup("V200");
    Config::V200Enabled = set.value("Enabled", Config::V200Enabled).toBool();
    Config::V200IP = set.value("IP", Config::V200IP).toString();
    Config::V200Port = set.value("Port", Config::V200Port).toInt();
    set.endGroup();

    //[VB1]
    set.beginGroup("VB1");
    Config::VB1Enabled = set.value("Enabled", Config::VB1Enabled).toBool();
    Config::VB1ComPort = set.value("ComPort", Config::VB1ComPort).toString();
    set.endGroup();

    //[POCH]
    set.beginGroup("POCH");
    Config::POCHEnabled = set.value("Enabled", Config::POCHEnabled).toBool();
    Config::POCHComPort = set.value("ComPort", Config::POCHComPort).toString();
    set.endGroup();

    //[DIANO]
    set.beginGroup("DIANO");
    Config::DIANOEnabled = set.value("Enabled", Config::DIANOEnabled).toBool();
    Config::DIANOComPort = set.value("ComPort", Config::DIANOComPort).toString();
    Config::DIANOComBaud = set.value("ComBaud", Config::DIANOComBaud).toInt();
    set.endGroup();

    //[EDAN]
    set.beginGroup("EDAN");
    Config::EDANEnabled = set.value("Enabled", Config::EDANEnabled).toBool();
    Config::EDANPort = set.value("Port", Config::EDANPort).toInt();
    set.endGroup();

    //[MPOINT]
    set.beginGroup("MPOINT");
    Config::MPointEnabled = set.value("Enabled", Config::MPointEnabled).toBool();
    Config::MPointComPort = set.value("ComPort", Config::MPointComPort).toString();
    set.endGroup();

    //[H120]
    set.beginGroup("H120");
    Config::H120Enabled = set.value("Enabled", Config::H120Enabled).toBool();
    Config::H120ComPort = set.value("ComPort", Config::H120ComPort).toString();
    set.endGroup();
}

void Config::writeConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[APP]
    set.beginGroup("APP");
    set.setValue("AutoStart", Config::AppAutoStart);
    set.setValue("ListenPort", Config::AppListenPort);
    set.endGroup();

    //[HTTP]
    set.beginGroup("HTTP");
    set.setValue("HttpPort", Config::HttpPort);
    set.endGroup();

    //[Idexx]
    set.beginGroup("IDEXX");
    set.setValue("Enabled", Config::IdexxEnabled);
    set.setValue("RequestFolder", Config::IdexxRequestFolder);
    set.setValue("ResultFolder", Config::IdexxResultFolder);
    set.endGroup();

    //[BC2600]
    set.beginGroup("BC2600");
    set.setValue("Enabled", Config::BC2600Enabled);
    set.setValue("ComPort", Config::BC2600ComPort);
    set.endGroup();

    //[Mini8]
    set.beginGroup("Mini8");
    set.setValue("Enabled", Config::Mini8Enabled);
    set.setValue("RequestFolder", Config::Mini8RequestFolder);
    set.setValue("ResultFolder", Config::Mini8ResultFolder);
    set.endGroup();

    //[V200]
    set.beginGroup("V200");
    set.setValue("Enabled", Config::V200Enabled);
    set.setValue("IP", Config::V200IP);
    set.setValue("Port", Config::V200Port);
    set.endGroup();

    //[VB1]
    set.beginGroup("VB1");
    set.setValue("Enabled", Config::VB1Enabled);
    set.setValue("ComPort", Config::VB1ComPort);
    set.endGroup();

    //[POCH]
    set.beginGroup("POCH");
    set.setValue("Enabled", Config::POCHEnabled);
    set.setValue("ComPort", Config::POCHComPort);
    set.endGroup();

    //[DIANO]
    set.beginGroup("DIANO");
    set.setValue("Enabled", Config::DIANOEnabled);
    set.setValue("ComPort", Config::DIANOComPort);
    set.setValue("ComBaud", Config::DIANOComBaud);
    set.endGroup();

    //[EDAN]
    set.beginGroup("EDAN");
    set.setValue("Enabled", Config::EDANEnabled);
    set.setValue("Port", Config::EDANPort);
    set.endGroup();

    //[MPOINT]
    set.beginGroup("MPOINT");
    set.setValue("Enabled", Config::MPointEnabled);
    set.setValue("ComPort", Config::MPointComPort);
    set.endGroup();

    //[H120]
    set.beginGroup("H120");
    set.setValue("Enabled", Config::H120Enabled);
    set.setValue("ComPort", Config::H120ComPort);
    set.endGroup();
}
