#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static QString ConfigFile;

    //[UPDATE]
    static QString s_ver;                   //版本号
    static QString s_url;                   //服务端地址

    static void readConfig();               //读取配置文件,在main函数最开始加载程序载入
    static void writeConfig();              //写入配置文件,在更改配置文件程序关闭时调用
    static bool checkConfig();              //校验配置文件
};

#endif // CONFIG_H
