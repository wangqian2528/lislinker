#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[EDAN]
bool Config::EDANEnabled = false;                //配置使能
int Config::EDANPort = 0;                        //端口号

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[EDAN]
    set.beginGroup("EDAN");
    Config::EDANEnabled = set.value("Enabled", Config::EDANEnabled).toBool();
    Config::EDANPort = set.value("Port", Config::EDANPort).toInt();
    set.endGroup();
}
