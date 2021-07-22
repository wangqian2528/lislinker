#include "trayicon.h"

#include <QMutex>
#include <QMenu>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

QScopedPointer<TrayIcon> TrayIcon::self;
TrayIcon *TrayIcon::Instance()
{
    if (self.isNull())
    {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull())
        {
            self.reset(new TrayIcon);
        }
    }
    return self.data();
}

TrayIcon::TrayIcon(QObject *parent) : QObject(parent)
{
    mainWidget = 0;
    trayIcon = new QSystemTrayIcon(this);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
    menu = new QMenu(QApplication::desktop());
}

void TrayIcon::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
    {
        mainWidget->showNormal();
        break;
    }

    default:
        break;
    }
}

bool TrayIcon::getVisible() const
{
    return trayIcon->isVisible();
}

void TrayIcon::setMainWidget(QWidget *mainWidget)
{
    this->mainWidget = mainWidget;

    openAction = menu->addAction(QIcon(":/image/open.png"), QStringLiteral("主界面"), mainWidget, SLOT(showNormal()));
    quitAction = menu->addAction(QIcon(":/image/quit.png"), QStringLiteral("退出"), this, SIGNAL(trayIconExit()));

    trayIcon->setContextMenu(menu);
}

void TrayIcon::showMessage(const QString &title, const QString &msg, QSystemTrayIcon::MessageIcon icon, int msecs)
{
    trayIcon->showMessage(title, msg, icon, msecs);
}

void TrayIcon::setIcon(const QIcon &icon)
{
    trayIcon->setIcon(icon);
}

void TrayIcon::setToolTip(const QString &tip)
{
    trayIcon->setToolTip(tip);
}

void TrayIcon::setVisible(bool visible)
{
    trayIcon->setVisible(visible);
}
