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
            connect(mSerialPort.get(), &QSerialPort::readyRead, this, &SerialPort::readReady);
        }
    }
}

void SerialPort::timerEvent(QTimerEvent *event)
{
    char ack = 0x06;
    if (event->timerId() == mCountTimerID)
    {
//        if (lookForHeadTail())
//        {
//            emit updateSerialDataFrame(mReceivedData);
//            mReceivedData.clear();
//        }

//        mSerialPort->write(&ack, 1);
//        killTimer(mCountTimerID);
        mSerialPort->write(&ack, 1);
//        qInfo()<<"data start";
//        qInfo()<<mReceivedData.size();
//        qInfo()<<mReceivedData.data();
        if(mReceivedData.size() > 1)
        {
            emit updateSerialDataFrame(mReceivedData);
        }
        mReceivedData.clear();
        killTimer(mCountTimerID);
    }
}

void SerialPort::readReady()
{
    mReceivedData += mSerialPort->readAll();
    mCountTimerID = startTimer(500);
}

bool SerialPort::lookForHeadTail()
{
    if(mReceivedData.at(mReceivedData.size()-1) == 0x04)
    {
        return true;
    }
    else
    {
        return false;
    }
}
