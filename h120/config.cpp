#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//H120尿机
bool Config::H120Enabled = false;                //配置使能
QString Config::H120ComPort = "";                //串口

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[H120]
    set.beginGroup("H120");
    Config::H120Enabled = set.value("Enabled", Config::H120Enabled).toBool();
    Config::H120ComPort = set.value("ComPort", Config::H120ComPort).toString();
    set.endGroup();
}
