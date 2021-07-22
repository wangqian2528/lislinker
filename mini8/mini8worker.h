#ifndef MINI8WORKER_H
#define MINI8WORKER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QDebug>

class Mini8Worker : public QObject
{
    Q_OBJECT
public:
    explicit Mini8Worker(QObject *parent = nullptr);

signals:

private slots:

    void analysis();

private:

    void requestHandle();
    void resultHandle();

private:

    QTimer mTimer;

};

#endif // MINI8WORKER_H
