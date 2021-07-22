#include "widget.h"
#include "ui_widget.h"
#include "flatui.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    FlatUI::setProgressQss(ui->proBar, 25, 0, 10, "#E8EDF2", "#1ABC9C");
    showUpdatePorgress(0, 0);
    ui->label->setFont(QFont("Microsoft Yahei", 15));
    showUpdateInfo(QStringLiteral("检查更新中"));

    m_UpdateWorker = new UpdateWorker();
    m_thread = new QThread();
    m_UpdateWorker->moveToThread(m_thread);

    connect(this, &Widget::sendStartUpdate, m_UpdateWorker, &UpdateWorker::startUpdate);
    connect(this, &Widget::sendStartDownload, m_UpdateWorker, &UpdateWorker::startDownload);

    connect(m_UpdateWorker, &UpdateWorker::sendExitUpdate, this, &Widget::exitUpdate);
    connect(m_UpdateWorker, &UpdateWorker::sendRmoteInfo, this, &Widget::showUpdateChoose);
    connect(m_UpdateWorker, &UpdateWorker::sendUpdateResult, this, &Widget::showUpdateResult);
    connect(m_UpdateWorker, &UpdateWorker::sendUpdateProgress, this, &Widget::showUpdatePorgress);

    m_thread->start();


    QTimer::singleShot(500, this, [&](){
        emit sendStartUpdate();
    });
}

Widget::~Widget()
{
    m_thread->exit();
    m_thread->wait();
    delete m_UpdateWorker;
    delete m_thread;
    delete ui;
}

void Widget::showUpdateInfo(const QString &info)
{
    ui->label->setText(info);
}

void Widget::showUpdatePorgress(int val, int maxVal)
{
    ui->proBar->setMaximum(maxVal);
    ui->proBar->setValue(val);
}

void Widget::showUpdateChoose(const QString &ver, const QString &date, const QString &note)
{
    int ret = QMessageBox::information(this,
                                       QStringLiteral("是否升级"),
                                       QStringLiteral(" 版本号：%1\n\r发布日期：%2\n\r更新内容：%3\n\r").arg(ver).arg(date).arg(note),
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Ok);
    switch(ret)
    {
    case QMessageBox::Ok:
        showUpdateInfo(QStringLiteral("升级中"));
        emit sendStartDownload();
        break;
    case QMessageBox::Cancel:
        exitUpdate();
        break;
    }
}

void Widget::showUpdateResult(bool ret)
{
    showUpdateInfo(QStringLiteral("升级结束"));
    if(ret)
    {
        QMessageBox::information(this, QStringLiteral("升级完成"), QStringLiteral("升级成功"));
    }
    else
    {
        QMessageBox::information(this, QStringLiteral("升级完成"), QStringLiteral("升级失败"));
    }
    exitUpdate();
}

void Widget::exitUpdate()
{
    QProcess *p = new QProcess;
    QString appDir = QString("\"%1/%2.exe\"").arg(qApp->applicationDirPath()).arg("app");
    p->start(appDir);
    qApp->quit();
}
