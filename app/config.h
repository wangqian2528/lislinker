#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static QString ConfigFile;

    //[APP]
    static bool AppAutoStart;               //自启动选项
    static int AppListenPort;               //监听端口

    //[HTTP]
    static int HttpPort;                    //websocket端口号

    //[IDEXX]
    static bool IdexxEnabled;               //配置使能
    static QString IdexxRequestFolder;      //工单保存路径
    static QString IdexxResultFolder;       //报告保存路径

    //[BC2600]
    static bool BC2600Enabled;              //配置使能
    static QString BC2600ComPort;           //串口号

    //[Mini8]
    static bool Mini8Enabled;               //配置使能
    static QString Mini8RequestFolder;      //工单保存路径
    static QString Mini8ResultFolder;       //报告保存路径

    //[V200]
    static bool V200Enabled;                //配置使能
    static QString V200IP;                  //IP地址
    static int V200Port;                    //端口号

    //[VB1]
    static bool VB1Enabled;                 //配置使能
    static QString VB1ComPort;              //串口

    //[pocH]
    static bool POCHEnabled;                //配置使能
    static QString POCHComPort;             //串口

    //[DIANO]
    static bool DIANOEnabled;               //配置使能
    static QString DIANOComPort;            //串口
    static int DIANOComBaud;                //波特率

    //EDAN
    static bool EDANEnabled;                //配置使能
    static int EDANPort;                    //端口号

    //MPOINT
    static bool MPointEnabled;              //配置使能
    static QString MPointComPort;           //串口

    //H120
    static bool H120Enabled;                //配置使能
    static QString H120ComPort;             //串口

    //HBFA
    static bool HBFAEnabled;                //配置使能
    static QString HBFAIP;                  //IP地址
    static int HBFAPort;                    //端口号

    static void readConfig();               //读取配置文件,在main函数最开始加载程序载入
    static void writeConfig();              //写入配置文件,在更改配置文件程序关闭时调用
    static bool checkConfig();              //校验配置文件
};

#endif // CONFIG_H
