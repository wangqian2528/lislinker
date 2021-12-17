#ifndef HBFAWORKER_H
#define HBFAWORKER_H

#include <QCoreApplication>
#include <QObject>
#include <memory>
#include <QDebug>
#include <QThread>
#include <QTextStream>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "tcpworker.h"

#include <stdint.h>

class HBFAWorker : public QObject
{
    Q_OBJECT
public:
    explicit HBFAWorker(QObject *parent = nullptr);

signals:
    void initTcpIP(const QString &ip, int port);
    void sendRsp(const QString &data);

private slots:
    void parseData(const QString &data);

private:
//    void someInit();

private:
    unique_ptr<TcpWorker> mTcpWorker = nullptr;
    unique_ptr<QThread> mThread = nullptr;
};

#endif // V200WORKER_H
