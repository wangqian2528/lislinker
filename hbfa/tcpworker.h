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
#include <QWebSocket>

using namespace  std;

class TcpWorker : public QObject
{
    Q_OBJECT
public:
    explicit TcpWorker(QObject *parent = nullptr);

public slots:
    void socketInit(const QString &ip, int port);
    void socketSend(const QString &data);

signals:
//    void updateTcpDataFrame(const QByteArray &data);
    void updateTcpDataFrame(const QString &data);

protected:
//    virtual void timerEvent(QTimerEvent *event) override;

private slots:
    void readReady(const QString &msg);
    void disconnect(void);

private:
    unique_ptr<QWebSocket> mWebSocket = nullptr;
    QUrl url;
};

#endif // TCPWORKER_H
