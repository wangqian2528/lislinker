#include <QCoreApplication>

#include "mini8worker.h"
#include "config.h"

Mini8Worker::Mini8Worker(QObject *parent) : QObject(parent)
{
    mTimer.setInterval(5000);
    connect(&mTimer, &QTimer::timeout, this, &Mini8Worker::analysis);
    mTimer.start();
}

void Mini8Worker::analysis()
{
    requestHandle();
    resultHandle();
}

void Mini8Worker::requestHandle()
{
    QString requestDir = QString("%1/analysis/MINI8/request").arg(qApp->applicationDirPath());
    QDir dir(requestDir);
    if(!dir.exists())
    {
        return;
    }

    //过滤文件后缀.json
    QStringList filters;
    filters<<QString("*.json");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);

    int dir_count = dir.count();
    for(int i=0; i<dir_count; i++)
    {
        QString file_name = QString("%1/analysis/MINI8/request/%2").arg(qApp->applicationDirPath()).arg(dir[i]);
        QString target_name = QString("%1/%2").arg(Config::Mini8RequestFolder).arg(dir[i]);

        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
        {
            file.remove();
            continue;
        }
        file.close();
        QFile::copy(file_name, target_name);
        file.remove();
    }
}

void Mini8Worker::resultHandle()
{
    QString requestDir = Config::Mini8ResultFolder;
    QDir dir(requestDir);
    if(!dir.exists())
    {
        return;
    }

    //过滤文件后缀.json
    QStringList filters;
    filters<<QString("*.json");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);

    int dir_count = dir.count();
    for(int i=0; i<dir_count; i++)
    {
        QStringList resName = QString(dir[i]).split("_");
        QString resPName = resName.at(0) + resName.at(1) + ".json";

        QString file_name = QString("%1/%2").arg(Config::Mini8ResultFolder).arg(dir[i]);
        QString target_name = QString("%1/analysis/MINI8/result/%2").arg(qApp->applicationDirPath()).arg(resPName);

        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
        {
            file.remove();
            continue;
        }
        file.close();
        QFile::copy(file_name, target_name);
        file.remove();
    }
}
