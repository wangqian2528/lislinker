#include "updateworker.h"
#include "config.h"

UpdateWorker::UpdateWorker(QObject *parent) : QObject(parent)
{

}

void UpdateWorker::startUpdate()
{
    QString update_path = qApp->applicationDirPath() + "/update";
    QDir dir(update_path);
    dir.mkpath(update_path);

    m_manager = new QNetworkAccessManager(this);
    m_file = new QFile(this);
    QNetworkRequest request;
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyRemoteInfo(QNetworkReply*)));
    request.setUrl(QUrl(Config::s_url));
    m_manager->get(request);
}

void UpdateWorker::replyRemoteInfo(QNetworkReply *reply)
{
    disconnect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyRemoteInfo(QNetworkReply*)));
    if(reply->error() != QNetworkReply::NoError)
    {
        emit sendExitUpdate();
        return;
    }
    QByteArray info = reply->readAll();
    jsonParse(info);
}

void UpdateWorker::jsonParse(QByteArray info)
{
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(info.data(), &err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        emit sendExitUpdate();
        return;
    }

    QJsonObject root_Obj = root_Doc.object();
    QJsonObject update_Obj = root_Obj.value("update").toObject();

    QString remote_ver = update_Obj.value("version").toString();
    QString remote_data = update_Obj.value("date").toString();
    QString remote_note = update_Obj.value("note").toString();

    m_remoteVer = remote_ver;

    m_packArry = root_Obj.value("package").toArray();
    m_packNum = m_packArry.size();
    m_packId = 0;

    if(remote_ver != Config::s_ver)
    {
        emit sendRmoteInfo(remote_ver, remote_data, remote_note);
    }
    else
    {
        emit sendExitUpdate();
    }
}

void UpdateWorker::startDownload()
{
    QNetworkRequest request;
    if(m_packId < m_packNum)
    {
        QJsonObject pck_Obj = m_packArry.at(m_packId).toObject();
        QString url = pck_Obj.value("url").toString();
        request.setUrl(QUrl(url));
        m_reply = m_manager->get(request);

        connect(m_reply, &QNetworkReply::readyRead, this, &UpdateWorker::doProcessReadyRead);//可读
        connect(m_reply, &QNetworkReply::finished, this, &UpdateWorker::doProcessFinished);//结束
        connect(m_reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &UpdateWorker::doProcessError);//异常

        QString fileName = qApp->applicationDirPath() + "/update/" + pck_Obj.value("patch").toString();
        m_file->setFileName(fileName);
        m_file->open(QIODevice::WriteOnly|QIODevice::Truncate);

        m_packTotalSize = pck_Obj.value("size").toInt();
        m_packGetSize = 0;

        m_packId++;
    }
    else
    {
        copyPatch();
    }
}

void UpdateWorker::doProcessReadyRead()//读取并写入
{
    while(!m_reply->atEnd())
    {
        QByteArray ba = m_reply->readAll();
        m_packGetSize += ba.size();
        sendUpdateProgress(m_packGetSize, m_packTotalSize);
        m_file->write(ba);
    }
}

void UpdateWorker::doProcessFinished()
{
    m_file->close();
    startDownload();
}

void UpdateWorker::doProcessError(QNetworkReply::NetworkError code)
{
    if(code != QNetworkReply::NoError)
    {
        emit sendUpdateResult(false);
    }
}

void UpdateWorker::copyPatch()
{
    QString patchDir = QString("%1/update").arg(qApp->applicationDirPath());
    QDir dir(patchDir);
    if(!dir.exists())
    {
        emit sendUpdateResult(false);
        return;
    }

    QStringList filters;
    filters<<QString("*.exe");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);

    int dir_count = dir.count();
    for(int i=0; i<dir_count; i++)
    {
        QString file_name = QString("%1/update/%2").arg(qApp->applicationDirPath()).arg(dir[i]);
        QString target_name = QString("%1/%2").arg(qApp->applicationDirPath()).arg(dir[i]);

        QFile target_file(target_name);
        if(target_file.exists())
        {
            if(!target_file.remove())
            {
                emit sendUpdateResult(false);
                return;
            }
        }

        if(!QFile::copy(file_name, target_name))
        {
            emit sendUpdateResult(false);
            return;
        }

        QFile tmp_file(file_name);
        tmp_file.remove();
    }

    Config::s_ver = m_remoteVer;
    Config::writeConfig();
    emit sendUpdateResult(true);
}
