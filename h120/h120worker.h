#ifndef H120WORKER_H
#define H120WORKER_H

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

class H120Worker : public QObject
{
    Q_OBJECT
public:
    explicit H120Worker(QObject *parent = nullptr);

signals:
    void initSerial(const QString &portName);

private slots:
    void parseData(const QByteArray &data);

private:
    void jsonInsert(QJsonArray &arr, const QString &key, const QString &value, const QString &purity, const QString &unit);

private:
    unique_ptr<SerialPort> mSerialWorker = nullptr;
    unique_ptr<QThread> mThread = nullptr;
};

#endif // H120WORKER_H
