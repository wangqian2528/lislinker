#include "window.h"
#include "qsharedmemory.h"
#include "trayicon.h"
#include "config.h"

#include <QApplication>
#include <QTextCodec>
#include <QTimer>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSharedMemory mem("LinkerBox");
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
    w.setWindowTitle("LinkerBox");

    TrayIcon::Instance()->setMainWidget(&w);
    TrayIcon::Instance()->setIcon(QIcon(":/image/hb_on.png"));
    TrayIcon::Instance()->setToolTip("LinkerBox");
    TrayIcon::Instance()->setVisible(true);

    QObject::connect(TrayIcon::Instance(), SIGNAL(trayIconExit()), &w, SLOT(appExit()));

    QProcess p;
    p.setWorkingDirectory(qApp->applicationDirPath() + "/web");
    p.start(qApp->applicationDirPath() + "/web/start.bat");

    return a.exec();
}
