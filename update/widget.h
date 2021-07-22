#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QProcess>
#include <QThread>
#include <QTimer>

#include "updateworker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void showUpdateInfo(const QString &info);
    void showUpdatePorgress(int, int);
    void showUpdateChoose(const QString &ver, const QString &date, const QString &note);
    void showUpdateResult(bool);

    void exitUpdate();//退出升级程序并启动主程序
signals:
    void sendStartUpdate();
    void sendStartDownload();

private slots:

private:


private:
    Ui::Widget *ui;

    UpdateWorker *m_UpdateWorker;
    QThread *m_thread;
};
#endif // WIDGET_H
