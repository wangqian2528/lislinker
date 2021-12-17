#include "tcpworker.h"

TcpWorker::TcpWorker(QObject *parent) : QObject(parent)
{

}

void TcpWorker::socketInit(const QString &ip, int port)
{
    mWebSocket.reset(new QWebSocket);
    url = QUrl(QString("ws://%1:%2").arg(ip).arg(port));
    qDebug()<<url;
    mWebSocket->open(url);
    connect(mWebSocket.get(), &QWebSocket::textMessageReceived, this, &TcpWorker::readReady);
    connect(mWebSocket.get(), &QWebSocket::disconnected, this, &TcpWorker::disconnect);
}

void TcpWorker::readReady(const QString &msg)
{
    qDebug()<<"get info";
    emit updateTcpDataFrame(msg);
}

void TcpWorker::socketSend(const QString &data)
{
    mWebSocket->sendTextMessage(data);
}

void TcpWorker::disconnect(void)
{
    mWebSocket->open(url);
}
