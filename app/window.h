#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QApplication>

class QUdpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

public slots:
    void appExit();

private:
    Ui::Window *ui;

    bool appStart = false;

    void startAllPro();
    void stopAllPro();
};
#endif // WINDOW_H
