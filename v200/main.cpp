#include "config.h"
#include "qsharedmemory.h"
#include "v200worker.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedMemory mem("V200");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    V200Worker v200;
    emit v200.initTcpIP(Config::V200IP, Config::V200Port);

    return a.exec();
}
