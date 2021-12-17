#include "window.h"
#include "config.h"
#include "qprocess.h"

static void startProcess(const QString &pName);
static void killProcess(const QString &pName);

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    startProcess("http");
}

Window::~Window()
{
}

//程序退出
void Window::appExit()
{
    if(appStart) stopAllPro();
    Config::writeConfig();
    killProcess("http");
    qApp->exit();
}

static void startProcess(const QString &pName)
{
    QProcess *p = new QProcess;
    QString appDir = QString("\"%1/%2.exe\"").arg(qApp->applicationDirPath()).arg(pName);
    p->start(appDir);
}

static void killProcess(const QString &pName)
{
    QProcess *p = new QProcess;
    p->start(QString("taskkill /im %1.exe /f").arg(pName));
}

void Window::startAllPro()
{
    if(Config::IdexxEnabled) startProcess("idexx");
    if(Config::BC2600Enabled) startProcess("bc2600");
    if(Config::V200Enabled) startProcess("v200");
    if(Config::VB1Enabled) startProcess("vb1");
    if(Config::POCHEnabled) startProcess("poch");
    if(Config::Mini8Enabled) startProcess("mini8");
    if(Config::DIANOEnabled) startProcess("diano");
    if(Config::EDANEnabled) startProcess("edan");
    if(Config::H120Enabled) startProcess("h120");
    if(Config::MPointEnabled) startProcess("mpoint");
}

void Window::stopAllPro()
{
    if(Config::IdexxEnabled) killProcess("idexx");
    if(Config::BC2600Enabled) killProcess("bc2600");
    if(Config::V200Enabled) killProcess("v200");
    if(Config::VB1Enabled) killProcess("vb1");
    if(Config::POCHEnabled) killProcess("poch");
    if(Config::Mini8Enabled) killProcess("mini8");
    if(Config::DIANOEnabled) killProcess("diano");
    if(Config::EDANEnabled) killProcess("edan");
    if(Config::H120Enabled) killProcess("h120");
    if(Config::MPointEnabled) killProcess("mpoint");
}
