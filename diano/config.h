#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static QString ConfigFile;

    //[DIANO]
    static bool DIANOEnabled;               //配置使能
    static QString DIANOComPort;            //串口
    static int DIANOComBaud;                //波特率

    static void readConfig();               //读取配置文件,在main函数最开始加载程序载入
};

#endif // CONFIG_H
