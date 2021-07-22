#include "mini8worker.h"
#include "config.h"
#include "qsharedmemory.h"

#include <QCoreApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedMemory mem("MINI8");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    Mini8Worker mini8;

    return a.exec();
}
