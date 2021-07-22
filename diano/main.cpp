#include "config.h"
#include "qsharedmemory.h"
#include "dianoworker.h"

#include <QCoreApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedMemory mem("DIANO");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    DianoWorker diano;
    emit diano.initSerial(Config::DIANOComPort);

    return a.exec();
}
