#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static QString ConfigFile;

    //[APP]
    static int AppListenPort;               //监听端口

    //[HTTP]
    static int HttpPort;                    //http端口号

    static void readConfig();               //读取配置文件,在main函数最开始加载程序载入
};

#endif // CONFIG_H
