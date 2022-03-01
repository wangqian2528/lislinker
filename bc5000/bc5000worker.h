#ifndef BC5000WORKER_H
#define BC5000WORKER_H

#include <QCoreApplication>
#include <QObject>
#include <memory>
#include <QDebug>
#include <QThread>
#include <QTextStream>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>

#include "tcpworker.h"

#include <stdint.h>

#define SERIAL_NUMBER_1         1
#define SERIAL_NUMBER_2         2
#define SERIAL_NUMBER_3         3
#define SERIAL_NUMBER_4         4
#define SERIAL_NUMBER_5         5
#define SERIAL_NUMBER_6         6
#define SERIAL_NUMBER_7         7
#define SERIAL_NUMBER_8         8
#define SERIAL_NUMBER_9         9
#define SERIAL_NUMBER_10        10
#define SERIAL_NUMBER_11        11
#define SERIAL_NUMBER_12        12
#define SERIAL_NUMBER_13        13
#define SERIAL_NUMBER_14        14
#define SERIAL_NUMBER_15        15
#define SERIAL_NUMBER_16        16
#define SERIAL_NUMBER_17        17
#define SERIAL_NUMBER_18        18
#define SERIAL_NUMBER_19        19
#define SERIAL_NUMBER_20        20
#define SERIAL_NUMBER_24        24
#define SERIAL_NUMBER_32        32
#define SERIAL_NUMBER_33        33

class BC5000Worker : public QObject
{
    Q_OBJECT
public:
    explicit BC5000Worker(QObject *parent = nullptr);

    enum MessageType {
        DEFAULT = 0, ORU_R01 = 1, ORM_O01 = 2
    };

signals:
    void initTcpIP(const QString &ip, int port);
    void updateMsgCompleted();
    void updateOrderCmd();

private slots:
    void parseData(const QByteArray &data);

    void requestPack();
    void requestOrder();

private:
    void parseMSH(const QString &msg);
    void parsePID(const QString &pid);
    void parsePIV(const QString &pv1);
    void parseOBR(const QString &obr);
    void parseOBX(const QString &obx);
    void parseMSA(const QString &msa);
    void clearJson();

private:
    unique_ptr<TcpWorker> mTcpWorker = nullptr;
    unique_ptr<QThread> mThread = nullptr;

    MessageType mMsgType = DEFAULT;
    QJsonObject mJsonObject;
    QJsonArray mPatientArray;
    QJsonArray mObxArray;
    QString mMsgTime = "";
    QString mMSHContent = "";
    QString mMsgControlID = "";
    QString mFillerNum = "";
};

#endif // BC5000WORKER_H
