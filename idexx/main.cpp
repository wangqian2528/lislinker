#include "config.h"
#include "qsharedmemory.h"
#include "analysis.h"

#include <QCoreApplication>
#include <QTimer>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSharedMemory mem("IDEXX");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    Analysis ana;
    ana.start(5000);

    return a.exec();
}
