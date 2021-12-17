#include "config.h"
#include "qsharedmemory.h"
#include "hbfaworker.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedMemory mem("HBFA");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    HBFAWorker hbfa;
    emit hbfa.initTcpIP(Config::HBFAIP, Config::HBFAPort);

    return a.exec();
}
