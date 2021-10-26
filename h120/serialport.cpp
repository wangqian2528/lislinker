#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{

}

void SerialPort::comInit(const QString &portName)
{
    mSerialPort.reset(new QSerialPort);
    mSerialPort->setPortName(portName);

    if (!mSerialPort->isOpen())
    {
        if (mSerialPort->open(QIODevice::ReadWrite))
        {
            /* 设置串口参数 */
            mSerialPort->setBaudRate(QSerialPort::Baud9600);
            mSerialPort->setDataBits(QSerialPort::Data8);
            mSerialPort->setParity(QSerialPort::NoParity);
            mSerialPort->setStopBits(QSerialPort::OneStop);
            mSerialPort->setFlowControl(QSerialPort::NoFlowControl);
            mSerialPort->clear(QSerialPort::AllDirections);
            mSerialPort->setDataTerminalReady(true);
            mSerialPort->setRequestToSend(true);
            connect(mSerialPort.get(), &QSerialPort::readyRead, this, &SerialPort::readReady);
        }
    }
}

void SerialPort::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mCountTimerID)
    {
        emit updateSerialDataFrame(mReceivedData);
        mReceivedData.clear();
        killTimer(mCountTimerID);
    }
}

void SerialPort::readReady()
{
    mReceivedData += mSerialPort->readAll();
    mCountTimerID = startTimer(1000);
}
