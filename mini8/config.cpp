#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "config.ini";

//[Mini8]
bool Config::Mini8Enabled = false;              //配置使能
QString Config::Mini8RequestFolder = "";        //工单保存路径
QString Config::Mini8ResultFolder = "";         //报告保存路径

void Config::readConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[Mini8]
    set.beginGroup("Mini8");
    Config::Mini8Enabled = set.value("Enabled", Config::Mini8Enabled).toBool();
    Config::Mini8RequestFolder = set.value("RequestFolder", Config::Mini8RequestFolder).toString();
    Config::Mini8ResultFolder = set.value("ResultFolder", Config::Mini8ResultFolder).toString();
    set.endGroup();
}
