#include "config.h"
#include "qsettings.h"
#include "qfile.h"
#include "qdebug.h"

//配置文件文件路径及名称
QString Config::ConfigFile = "update.ini";

//[UPDATE]
QString Config::s_ver = "0.0.0";//版本号
QString Config::s_url = "https://lislinker.oss-cn-shanghai.aliyuncs.com/update.json";//服务端地址

bool Config::checkConfig()
{
    QFile file(Config::ConfigFile);
    if (file.size() == 0)
    {
        writeConfig();
        return false;
    }
    return true;
}

void Config::readConfig()
{
    if (!checkConfig())
    {
        return;
    }

    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[UPDATE]
    set.beginGroup("UPDATE");
    Config::s_ver = set.value("ver", Config::s_ver).toString();
    Config::s_url = set.value("url", Config::s_url).toString();
    set.endGroup();
}

void Config::writeConfig()
{
    QSettings set(Config::ConfigFile, QSettings::IniFormat);

    //[UPDATE]
    set.beginGroup("UPDATE");
    set.setValue("ver", Config::s_ver);
    set.setValue("url", Config::s_url);
    set.endGroup();
}
