#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static QString ConfigFile;

    //[Mini8]
    static bool Mini8Enabled;               //配置使能
    static QString Mini8RequestFolder;      //工单保存路径
    static QString Mini8ResultFolder;       //报告保存路径

    static void readConfig();               //读取配置文件,在main函数最开始加载程序载入
};

#endif // CONFIG_H
