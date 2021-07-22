#ifndef UPDATEWORKER_H
#define UPDATEWORKER_H

#include <QObject>
#include <QCoreApplication>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class UpdateWorker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateWorker(QObject *parent = nullptr);

public slots:
    void startUpdate();
    void startDownload();

signals:
    void sendExitUpdate();
    void sendRmoteInfo(const QString &ver, const QString &date, const QString &note);
    void sendUpdateResult(bool);
    void sendUpdateProgress(int, int);

private slots:
    void replyRemoteInfo(QNetworkReply *reply);

    void doProcessReadyRead();
    void doProcessFinished();
    void doProcessError(QNetworkReply::NetworkError code);

private:
    void jsonParse(QByteArray);
    void copyPatch();

private:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;
    QFile *m_file;

    QJsonArray m_packArry;

    int m_packNum = 0;
    int m_packId = 0;

    int m_packTotalSize = 0;
    int m_packGetSize = 0;

    QString m_remoteVer;
};

#endif // UPDATEWORKER_H
