#include <QCoreApplication>
#include <QFile>
#include <QLockFile>
#include <QSharedMemory>
#include <QTextCodec>

#include "MindrayWorker.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    MindrayWorker mindrayWorker;

    QSharedMemory singleton(a.applicationName());
    if(!singleton.create(1))
    {
        return 0;
    }

    return a.exec();
}

