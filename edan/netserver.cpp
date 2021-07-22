#include "netserver.h"

NetServer::NetServer(QObject *parent) : QObject(parent)
{

}

void NetServer::netInit(int portName)
{
    qDebug()<<"portName:"<<portName;
    mTcpServer.reset(new QTcpServer);
    mTcpServer->listen(QHostAddress::AnyIPv4, portName);
    connect(mTcpServer.get(), &QTcpServer::newConnection, this, &NetServer::newConnect);
}

void NetServer::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == mCountTimerID)
    {
        emit updateNetDataFrame(mReceivedData);
        mReceivedData.clear();
        killTimer(mCountTimerID);
    }
}

void NetServer::newConnect()
{
    mTcpSocket = mTcpServer->nextPendingConnection();
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &NetServer::readReady);
}

void NetServer::readReady()
{
    mReceivedData += mTcpSocket->readAll();
    mCountTimerID = startTimer(500);
}
