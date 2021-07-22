#ifndef POCHWORKER_H
#define POCHWORKER_H

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
#include "dataframe.h"

class PochWorker : public QObject
{
    Q_OBJECT
public:
    explicit PochWorker(QObject *parent = nullptr);

signals:
    void initSerial(const QString &portName);

private slots:
    void parseData(const QByteArray &data);

private:
    void packMsg();
    float charToFloat(const char *data, const int length);
    int charToInt(const char *data, const int length);
    void jsonInsert(QJsonArray &arr, const QString &key, const QString &unit, const float value);
private:
    unique_ptr<SerialPort> mSerialWorker = nullptr;
    unique_ptr<QThread> mThread = nullptr;

    DataFrameD1 mDataFrame1;
    DataFrameD2 mDataFrame2;
    DataFrameD3 mDataFrame3;
};

#endif // POCHWORKER_H
