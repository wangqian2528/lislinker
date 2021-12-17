#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static QString ConfigFile;

    //HBFA
    static bool HBFAEnabled;                //配置使能
    static QString HBFAIP;                  //IP地址
    static int HBFAPort;                    //端口号

    static void readConfig();               //读取配置文件,在main函数最开始加载程序载入
};

#endif // CONFIG_H
