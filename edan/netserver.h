#ifndef NETSERVER_H
#define NETSERVER_H

#include <QObject>
#include <QtNetWork>
#include <QNetworkProxy>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <memory>
#include <QDebug>
#include <QTimerEvent>

using namespace  std;

class NetServer : public QObject
{
    Q_OBJECT
public:
    explicit NetServer(QObject *parent = nullptr);

public slots:
    void netInit(int portName);
    void rspAck(const QString &rsp);

signals:
    void updateNetDataFrame(const QByteArray &data);

protected:
    virtual void timerEvent(QTimerEvent *event) override;

private slots:
    void readReady();
    void newConnect();
    void disConnect();

private:
    unique_ptr<QTcpServer> mTcpServer = nullptr;
    unique_ptr<QTcpSocket> mTcpSocket = nullptr;
    QByteArray mReceivedData;
    int mCountTimerID = 0;
};

#endif // NETSERVER_H
