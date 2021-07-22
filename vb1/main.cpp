#include "config.h"
#include "qsharedmemory.h"
#include "vb1worker.h"

#include <QCoreApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedMemory mem("VB1");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    VB1Worker vb1;
    emit vb1.initSerial(Config::VB1ComPort);

    return a.exec();
}
