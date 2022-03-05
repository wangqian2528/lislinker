#ifndef TCPWORKER_H
#define TCPWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <memory>
#include <QDebug>
#include <QTimerEvent>
#include <QNetworkProxy>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTimer>

using namespace  std;

class TcpWorker : public QObject
{
    Q_OBJECT
public:
    explicit TcpWorker(QObject *parent = nullptr);

public slots:
    void socketInit(const QString &ip, int port);

signals:
    void updateTcpDataFrame(const QByteArray &data);

private slots:
    void readReady();
    void readError(QAbstractSocket::SocketError error);
    void disco();

private:
    unique_ptr<QTcpSocket> mTcpSocket = nullptr;
    QString aimIP;
    int aimPort;
};

#endif // TCPWORKER_H
