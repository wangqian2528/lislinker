#ifndef EDANWORKER_H
#define EDANWORKER_H

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

#include "netserver.h"

class EdanWorker : public QObject
{
    Q_OBJECT
public:
    explicit EdanWorker(QObject *parent = nullptr);

signals:
    void initNet(int portName);

private slots:
    void parseData(const QByteArray &data);
    void jsonInsert(QJsonArray &arr, const QString &key, const QString &unit, const QString &value, const QString &ref);

private:
    std::unique_ptr<NetServer> mNetWorker = nullptr;
    std::unique_ptr<QThread> mThread = nullptr;
};

#endif // EDANWORKER_H
