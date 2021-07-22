#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[APP]
int Config::AppListenPort = 7790;               //监听端口号

//[IDEXX]
bool Config::IdexxEnabled = false;              //配置使能
QString Config::IdexxRequestFolder = "";        //工单保存路径
QString Config::IdexxResultFolder = "";         //报告保存路径

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[APP]
    set.beginGroup("APP");
    Config::AppListenPort = set.value("ListenPort", Config::AppListenPort).toInt();
    set.endGroup();

    //[IDEXX]
    set.beginGroup("IDEXX");
    Config::IdexxEnabled = set.value("Enabled", Config::IdexxEnabled).toBool();
    Config::IdexxRequestFolder = set.value("RequestFolder", Config::IdexxRequestFolder).toString();
    Config::IdexxResultFolder = set.value("ResultFolder", Config::IdexxResultFolder).toString();
    set.endGroup();
}
