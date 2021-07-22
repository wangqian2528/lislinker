#include "SerialPortWorker.hpp"
#include <QFile>

SerialPortWorker::SerialPortWorker(QObject *parent) : QObject(parent)
{

}

void SerialPortWorker::comInit(const QString &portName)
{
    mSerialPort.reset(new QSerialPort);
    mSerialPort->setPortName(portName);

    if (!mSerialPort->isOpen())
    {
        if (mSerialPort->open(QIODevice::ReadWrite))
        {
            if (mSerialPort->setBaudRate(QSerialPort::Baud9600) &&
                    mSerialPort->setDataBits(QSerialPort::Data7) &&
                    mSerialPort->setParity(QSerialPort::NoParity) &&
                    mSerialPort->setStopBits(QSerialPort::OneStop) &&
                    mSerialPort->setFlowControl(QSerialPort::NoFlowControl))
            {
                mSerialPort->clear(QSerialPort::AllDirections);
                connect(mSerialPort.get(), &QSerialPort::readyRead, this, &SerialPortWorker::readReady);
            }
        }
    }
}

void SerialPortWorker::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == mCountTimerID)
    {
        if (lookForHeadTail())
        {
            emit updateSerialDataFrame(mReceivedData);
            mReceivedData.clear();
            killTimer(mCountTimerID);
        }
    }
}

void SerialPortWorker::readReady()
{
    mReceivedData += mSerialPort->readAll();
    mCountTimerID = startTimer(1000);
}

bool SerialPortWorker::lookForHeadTail()
{
    if ((mReceivedData.indexOf(0x02) != 0) &&
            (mReceivedData.lastIndexOf(0x1A) != mReceivedData.length()))
    {
        return false;
    }
    else
    {
        return true;
    }
}
