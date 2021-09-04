#include "tcpworker.h"

TcpWorker::TcpWorker(QObject *parent) : QObject(parent)
{

}

void TcpWorker::socketInit(const QString &ip, int port)
{
    mTcpSocket.reset(new QTcpSocket);
    mTcpSocket->abort();
    mTcpSocket->connectToHost(ip, port, QAbstractSocket::ReadWrite, QAbstractSocket::IPv4Protocol);
    mTcpSocket->setProxy(QNetworkProxy::NoProxy);
    connect(mTcpSocket.get(), &QTcpSocket::readyRead, this, &TcpWorker::readReady);
}

void TcpWorker::readReady()
{
    mReceivedData += mTcpSocket->readAll();
    mCountTimerID = startTimer(2000);
}

const char rspCMD[] = {0x10,0x02,0x00,0x0B,0x6F,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x03};
const char ackNewOB[] = {0x10,0x02,0x00,0x01,0x6E,0x10,0x03};

void TcpWorker::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == mCountTimerID)
    {
        if(lookForHeadTail())
        {
            emit updateTcpDataFrame(mReceivedData);
        }
        else
        {
            mTcpSocket->write(rspCMD, 17);
        }

        mReceivedData.clear();
        killTimer(mCountTimerID);
    }
}


bool TcpWorker::lookForHeadTail()
{
    if(mReceivedData.size() == 7 && mReceivedData == QByteArray(ackNewOB, 7))
        return false;
    else
        return true;
}
