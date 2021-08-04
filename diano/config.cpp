#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[DIANO]
bool Config::DIANOEnabled = false;               //配置使能
QString Config::DIANOComPort = "";               //串口号
int Config::DIANOComBaud = 9600;                 //波特率

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[DIANO]
    set.beginGroup("DIANO");
    Config::DIANOEnabled = set.value("Enabled", Config::DIANOEnabled).toBool();
    Config::DIANOComPort = set.value("ComPort", Config::DIANOComPort).toString();
    Config::DIANOComBaud = set.value("ComBaud", Config::DIANOComBaud).toInt();
    set.endGroup();
}
