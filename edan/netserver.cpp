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
        if(mReceivedData.size() >= 100)
            emit updateNetDataFrame(mReceivedData);
        mReceivedData.clear();
        killTimer(mCountTimerID);
    }
}

void NetServer::newConnect()
{
    qInfo()<<"new connect";
    mTcpSocket.reset(mTcpServer->nextPendingConnection());
    connect(mTcpSocket.get(), &QTcpSocket::readyRead, this, &NetServer::readReady);
    connect(mTcpSocket.get(), &QTcpSocket::disconnected, this, &NetServer::disConnect);
}

void NetServer::disConnect()
{
    qInfo()<<"dis connect";
    disconnect(mTcpSocket.get(), &QTcpSocket::readyRead, this, &NetServer::readReady);
    disconnect(mTcpSocket.get(), &QTcpSocket::disconnected, this, &NetServer::disConnect);
    mReceivedData.clear();
}

void NetServer::readReady()
{
    mReceivedData += mTcpSocket.get()->readAll();
    mCountTimerID = startTimer(1000);
}

void NetServer::rspAck(const QString &rsp)
{
    QByteArray ba = rsp.toLatin1();
    mTcpSocket.get()->write(ba.data());
}
