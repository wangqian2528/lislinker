#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";
//[APP]
int Config::AppListenPort = 7790;               //监听端口号
//[HTTP]
int Config::HttpPort = 7788;                    //http端口号

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[APP]
    set.beginGroup("APP");
    Config::AppListenPort = set.value("ListenPort", Config::AppListenPort).toInt();
    set.endGroup();

    //[HTTP]
    set.beginGroup("HTTP");
    Config::HttpPort = set.value("HttpPort", Config::HttpPort).toInt();
    set.endGroup();

}
