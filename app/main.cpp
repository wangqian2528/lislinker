#include "window.h"
#include "qsharedmemory.h"
#include "trayicon.h"
#include "config.h"

#include <QApplication>
#include <QTextCodec>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedMemory mem("LISLinker");
    if (!mem.create(1))
    {
        return 0;
    }
    mem.create(1);

    a.setFont(QFont("Microsoft Yahei", 10));

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);

    Config::ConfigFile = qApp->applicationDirPath() + "/config.ini";
    Config::readConfig();

    Window w;
    w.setWindowTitle("LIS Linker");
    w.setWindowIcon(QIcon(":/image/hb_off.png"));
    w.show();

    TrayIcon::Instance()->setMainWidget(&w);
    TrayIcon::Instance()->setIcon(w.windowIcon());
    TrayIcon::Instance()->setToolTip(w.windowTitle());
    TrayIcon::Instance()->setVisible(true);
    QObject::connect(TrayIcon::Instance(), SIGNAL(trayIconExit()), &w, SLOT(appExit()));
    QObject::connect(&w, SIGNAL(windowIconChanged(QIcon)), TrayIcon::Instance(), SLOT(setIcon(QIcon)));

    return a.exec();
}
