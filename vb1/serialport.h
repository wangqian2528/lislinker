#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <memory>
#include <QDebug>
#include <QSerialPort>
#include <QTimerEvent>

using namespace  std;

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

public slots:
    void comInit(const QString &portName);

signals:
    void updateSerialDataFrame(const QByteArray &data);

protected:
    virtual void timerEvent(QTimerEvent *event) override;

private slots:
    void readReady();

private:
    bool lookForHeadTail();

private:
    unique_ptr<QSerialPort> mSerialPort = nullptr;
    QByteArray mReceivedData;
    int mCountTimerID = 0;
};

#endif // SERIALPORT_H
