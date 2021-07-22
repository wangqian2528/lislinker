#ifndef SERIALPORTWORKER_HPP
#define SERIALPORTWORKER_HPP

#include <QObject>
#include <memory>
#include <QDebug>
#include <QSerialPort>
#include <QTimerEvent>

using namespace  std;

class SerialPortWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortWorker(QObject *parent = nullptr);

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
    std::unique_ptr<QSerialPort> mSerialPort = nullptr;
    QByteArray mReceivedData;
    int mCountTimerID = 0;
};

#endif // SERIALPORTWORKER_HPP
