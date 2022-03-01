#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static QString ConfigFile;

    //[BC5000]
    static bool BC5000Enabled;                //配置使能
    static QString BC5000IP;                  //IP地址
    static int BC5000Port;                    //端口号

    static void readConfig();               //读取配置文件,在main函数最开始加载程序载入
};

#endif // CONFIG_H
