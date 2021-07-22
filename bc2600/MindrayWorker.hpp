#ifndef MINDRAYWORKER_H
#define MINDRAYWORKER_H

#include <QDateTime>
#include <memory>
#include <QDebug>
#include <QThread>
#include <QTextStream>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSerialPortInfo>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTimerEvent>
#include <QTextStream>

#include "SerialPortWorker.hpp"
#include "GeneralProtocol.hpp"

class MindrayWorker : public QObject
{
    Q_OBJECT

public:
    MindrayWorker(QObject *parent = nullptr);

signals:
    void initSerial(const QString &portName);

private slots:
    void parseData(const QByteArray &data);

private:
    void packMsg();
    QString charToString(const char *data, const int length);
    float charToFloat(const char *data, const int length);

    void jsonInsert(QJsonArray &arr, const QString &key, const QString &unit, const float value);
    void getLocalConfig();

private:
    std::unique_ptr<SerialPortWorker> mSerialWorker = nullptr;
    std::unique_ptr<QThread> mThread = nullptr;
    SampleDataFrame mDataFrame;

    QString mCOM = "";
    QString mStorePath = "";
};
#endif // MINDRAYWORKER
