#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[pocH]
bool Config::POCHEnabled = false;               //配置使能
QString Config::POCHComPort = "";               //串口号

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[POCH]
    set.beginGroup("POCH");
    Config::POCHEnabled = set.value("Enabled", Config::POCHEnabled).toBool();
    Config::POCHComPort = set.value("ComPort", Config::POCHComPort).toString();
    set.endGroup();
}
