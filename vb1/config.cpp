#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[VB1]
bool Config::VB1Enabled = false;                //配置使能
QString Config::VB1ComPort = "";                //串口号

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[VB1]
    set.beginGroup("VB1");
    Config::VB1Enabled = set.value("Enabled", Config::VB1Enabled).toBool();
    Config::VB1ComPort = set.value("ComPort", Config::VB1ComPort).toString();
    set.endGroup();
}
