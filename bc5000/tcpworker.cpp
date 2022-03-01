#include "tcpworker.h"

TcpWorker::TcpWorker(QObject *parent) : QObject(parent)
{

}

void TcpWorker::socketInit(const QString &ip, int port)
{
    mTcpSocket = std::unique_ptr<QTcpSocket>(new QTcpSocket);
    mTcpSocket->abort();

    connect(mTcpSocket.get(), static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error), this, &TcpWorker::readError);
    connect(mTcpSocket.get(), &QTcpSocket::readyRead, this, &TcpWorker::readReady);

    mTcpSocket->connectToHost(ip, port);
    if (mTcpSocket->waitForConnected(1000))
    {
        qInfo()<<"connect failed";
    }
}

void TcpWorker::readReady()
{
    QByteArray buffer =  mTcpSocket->readAll();
    qInfo()<<buffer;
    int startIndex = buffer.indexOf(0x0B);
    int endIndex = buffer.lastIndexOf((quint8)0x1C0D);
    if(startIndex != -1 && endIndex != -1)
    {
        emit updateTcpDataFrame(buffer.mid(startIndex + 1, endIndex - 2));
    }
}

void TcpWorker::readError(QAbstractSocket::SocketError error)
{
//    Q_UNUSED(error)
    qInfo()<<"connect error"<<error;
    mTcpSocket->disconnectFromHost();
}
