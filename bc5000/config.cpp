#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[BC5000]
bool Config::BC5000Enabled = false;               //配置使能
QString Config::BC5000IP = "";                    //IP地址
int Config::BC5000Port = 0;                       //端口号

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[BC5000]
    set.beginGroup("BC5000");
    Config::BC5000Enabled = set.value("Enabled", Config::BC5000Enabled).toBool();
    Config::BC5000IP = set.value("IP", Config::BC5000IP).toString();
    Config::BC5000Port = set.value("Port", Config::BC5000Port).toInt();
    set.endGroup();
}
