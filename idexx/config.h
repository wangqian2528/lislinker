#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static QString ConfigFile;

    //[APP]
    static int AppListenPort;               //监听端口

    //[IDEXX]
    static bool IdexxEnabled;               //配置使能
    static QString IdexxRequestFolder;      //工单保存路径
    static QString IdexxResultFolder;       //报告保存路径

    static void readConfig();               //读取配置文件,在main函数最开始加载程序载入
};

#endif // CONFIG_H
