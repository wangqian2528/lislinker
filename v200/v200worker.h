#ifndef V200WORKER_H
#define V200WORKER_H

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

class V200Worker : public QObject
{
    Q_OBJECT
public:
    explicit V200Worker(QObject *parent = nullptr);

signals:
    void initTcpIP(const QString &ip, int port);

private slots:
    void parseData(const QByteArray &data);

private:
    void someInit();


private:
    unique_ptr<TcpWorker> mTcpWorker = nullptr;
    unique_ptr<QThread> mThread = nullptr;

    QStringList m_ItemList;
    QStringList m_UnitList;
    QStringList m_QualSemiList;
    QStringList m_ValueType;
};

#endif // V200WORKER_H
