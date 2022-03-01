#include <QCoreApplication>
#include "config.h"
#include "qsharedmemory.h"
#include <QTextCodec>
#include "bc5000worker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedMemory mem("BC5000");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    BC5000Worker bc5000;
    emit bc5000.initTcpIP(Config::BC5000IP, Config::BC5000Port);

    return a.exec();
}
