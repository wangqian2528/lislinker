#ifndef VB1WORKER_H
#define VB1WORKER_H

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

#include "serialport.h"

class VB1Worker : public QObject
{
    Q_OBJECT
public:
    explicit VB1Worker(QObject *parent = nullptr);

signals:
    void initSerial(const QString &portName);

private slots:
    void parseData(const QByteArray &data);

private:
    void jsonInsert(QJsonArray &arr, const QString &key, const QString &unit, const QString &value, const QString &range);

private:
    unique_ptr<SerialPort> mSerialWorker = nullptr;
    unique_ptr<QThread> mThread = nullptr;
};

#endif // VB1WORKER_H
