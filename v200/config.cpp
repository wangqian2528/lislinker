#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[V200]
bool Config::V200Enabled = false;               //配置使能
QString Config::V200IP = "";                    //IP地址
int Config::V200Port = 0;                       //端口号

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[V200]
    set.beginGroup("V200");
    Config::V200Enabled = set.value("Enabled", Config::V200Enabled).toBool();
    Config::V200IP = set.value("IP", Config::V200IP).toString();
    Config::V200Port = set.value("Port", Config::V200Port).toInt();
    set.endGroup();
}
