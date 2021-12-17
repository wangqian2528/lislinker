#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[HBFA]
bool Config::HBFAEnabled = false;               //配置使能
QString Config::HBFAIP = "";                    //IP地址
int Config::HBFAPort = 0;                       //端口号

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[HBFA]
    set.beginGroup("HBFA");
    Config::HBFAEnabled = set.value("Enabled", Config::HBFAEnabled).toBool();
    Config::HBFAIP = set.value("IP", Config::HBFAIP).toString();
    Config::HBFAPort = set.value("Port", Config::HBFAPort).toInt();
    set.endGroup();
}
