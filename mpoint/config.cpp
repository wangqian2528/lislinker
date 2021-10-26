#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//MPOINT
bool Config::MPointEnabled = false;              //配置使能
QString Config::MPointComPort = "";              //串口

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[MPOINT]
    set.beginGroup("MPOINT");
    Config::MPointEnabled = set.value("Enabled", Config::MPointEnabled).toBool();
    Config::MPointComPort = set.value("ComPort", Config::MPointComPort).toString();
    set.endGroup();
}
