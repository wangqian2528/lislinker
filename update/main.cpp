#include "widget.h"
#include "qsharedmemory.h"
#include "config.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    QSharedMemory mem("update");
    if (!mem.create(1))
    {
        return 0;
    }

    Config::ConfigFile = qApp->applicationDirPath() + "/update.ini";
    Config::readConfig();

    Widget w;
    w.show();
    return a.exec();
}
