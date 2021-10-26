#ifndef MPWORKER_H
#define MPWORKER_H

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

class MPWorker : public QObject
{
    Q_OBJECT
public:
    explicit MPWorker(QObject *parent = nullptr);

signals:
    void initSerial(const QString &portName);

private slots:
    void parseData(const QByteArray &data);

private:
    void MPWorker::jsonInsert(QJsonArray &arr, const QString &key, const QString &value);

private:
    unique_ptr<SerialPort> mSerialWorker = nullptr;
    unique_ptr<QThread> mThread = nullptr;
    QMap<QString, QString> mapRes;
};

#endif // MPWORKER_H
