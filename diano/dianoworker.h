#ifndef DIANOWORKER_H
#define DIANOWORKER_H

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

class DianoWorker : public QObject
{
    Q_OBJECT
public:
    explicit DianoWorker(QObject *parent = nullptr);

signals:
    void initSerial(const QString &portName);

private slots:
    void parseData(const QByteArray &data);

private:

private:
    unique_ptr<SerialPort> mSerialWorker = nullptr;
    unique_ptr<QThread> mThread = nullptr;
};

#endif // DIANOWORKER_H
