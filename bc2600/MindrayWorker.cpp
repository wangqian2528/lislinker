#include "MindrayWorker.hpp"
#include <QCoreApplication>
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

using namespace  std;

MindrayWorker::MindrayWorker(QObject *parent) : QObject(parent)
{
    mSerialWorker = std::unique_ptr<SerialPortWorker>(new SerialPortWorker);
    mThread = std::unique_ptr<QThread>(new QThread);
    mSerialWorker->moveToThread(mThread.get());
    connect(this, &MindrayWorker::initSerial, mSerialWorker.get(), &SerialPortWorker::comInit);
    connect(mSerialWorker.get(), &SerialPortWorker::updateSerialDataFrame, this, &MindrayWorker::parseData);
    mThread->start();
    getLocalConfig();
}

void MindrayWorker::packMsg()
{
    QJsonObject jsonObject;
    QJsonArray jsonArray;

    jsonInsert(jsonArray, "WBC", "10^9/L", charToFloat(mDataFrame.wbc, WBC_LENGTH) / 10);
    jsonInsert(jsonArray, "Lymph#", "10^9/L", charToFloat(mDataFrame.lymph, WBC_LENGTH) / 10);
    jsonInsert(jsonArray, "Mon#", "10^9/L", charToFloat(mDataFrame.mon, MON_LENGTH) / 10);
    jsonInsert(jsonArray, "Gran#", "10^9/L", charToFloat(mDataFrame.gran, GRAN_LENGTH) / 10);
    jsonInsert(jsonArray, "Lymph%", "%", charToFloat(mDataFrame.lymph_, 3) / 10);
    jsonInsert(jsonArray, "Mon%", "%", charToFloat(mDataFrame.mon_, 3) / 10);
    jsonInsert(jsonArray, "Gran%", "%", charToFloat(mDataFrame.gran_, 3) / 10);
    jsonInsert(jsonArray, "RBC", "10^12/L", charToFloat(mDataFrame.rbc, RBC_LENGTH) / 100);
    jsonInsert(jsonArray, "HGB", "g/L", charToFloat(mDataFrame.hgb, HGB_LENGTH));
    jsonInsert(jsonArray, "MCHC", "g/L", charToFloat(mDataFrame.mchc, MCHC_LENGTH));
    jsonInsert(jsonArray, "MCV", "fL", charToFloat(mDataFrame.mcv, MCV_LENGTH) / 10);
    jsonInsert(jsonArray, "MCH", "pg", charToFloat(mDataFrame.mch, MCH_LENGTH) / 10);
    jsonInsert(jsonArray, "RDW", "%", charToFloat(mDataFrame.rdw, RDW_LENGTH) / 10);
    jsonInsert(jsonArray, "HCT", "%", charToFloat(mDataFrame.hct, HCT_LENGTH) / 10);
    jsonInsert(jsonArray, "PLT", "10^9/L", charToFloat(mDataFrame.plt, PLT_LENGTH));
    jsonInsert(jsonArray, "MPV", "fL", charToFloat(mDataFrame.mpv, MPV_LENGTH) / 10);
    jsonInsert(jsonArray, "PDW", "", charToFloat(mDataFrame.pdw, PDW_LENGTH) / 10);
    jsonInsert(jsonArray, "PCT", "%", charToFloat(mDataFrame.pct, PCT_LENGTH) / 1000);
    jsonInsert(jsonArray, "Eos", "%", charToFloat(mDataFrame.eos, EOS_LENGTH) / 10);

    jsonObject.insert("results", QJsonValue(jsonArray));

    QJsonDocument jsonDocument;
    jsonDocument.setObject(jsonObject);
    QByteArray root_str = jsonDocument.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QDir dir(mStorePath);
    if(!dir.exists())
    {
        dir.mkpath(mStorePath);
    }

    QString saveFileName = QString("%1.json").arg(QJSONTIME);
    QFile file(mStorePath + "/" + saveFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly);
    QTextStream in(&file);
    in << strJson;
    file.close();
}

void MindrayWorker::parseData(const QByteArray &data)
{
    QByteArray recv(data);

    recv = recv.remove(0, 2);
    QByteArray tmp = QByteArray::fromHex(recv.toHex());
    QDataStream turnStream(&recv, QIODevice::ReadOnly);
    turnStream.setByteOrder(QDataStream::BigEndian);

    turnStream.readRawData(mDataFrame.number, NUMNER_LENGTH);
    turnStream.readRawData(mDataFrame.mode, MODE_LENGTH);
    turnStream.readRawData(mDataFrame.month, MONTH_LENGTH);
    turnStream.readRawData(mDataFrame.day, DAY_LENGTH);
    turnStream.readRawData(mDataFrame.year, YEAR_LENGTH);
    turnStream.readRawData(mDataFrame.hour, HOUR_LENGTH);
    turnStream.readRawData(mDataFrame.minute, MINUTE_LENGTH);
    turnStream.readRawData(mDataFrame.wbc, WBC_LENGTH);
    turnStream.readRawData(mDataFrame.lymph, LYMPH_LENGTH);
    turnStream.readRawData(mDataFrame.mon, MON_LENGTH);
    turnStream.readRawData(mDataFrame.gran, GRAN_LENGTH);
    turnStream.readRawData(mDataFrame.lymph_, 3);
    turnStream.readRawData(mDataFrame.mon_, 3);
    turnStream.readRawData(mDataFrame.gran_, 3);
    turnStream.readRawData(mDataFrame.rbc, RBC_LENGTH);
    turnStream.readRawData(mDataFrame.hgb, HGB_LENGTH);
    turnStream.readRawData(mDataFrame.mchc, MCHC_LENGTH);
    turnStream.readRawData(mDataFrame.mcv, MCV_LENGTH);
    turnStream.readRawData(mDataFrame.mch, MCH_LENGTH);
    turnStream.readRawData(mDataFrame.rdw, RDW_LENGTH);
    turnStream.readRawData(mDataFrame.hct, HCT_LENGTH);
    turnStream.readRawData(mDataFrame.plt, PLT_LENGTH);
    turnStream.readRawData(mDataFrame.mpv, MPV_LENGTH);
    turnStream.readRawData(mDataFrame.pdw, PDW_LENGTH);
    turnStream.readRawData(mDataFrame.pct, PCT_LENGTH);
    turnStream.readRawData(mDataFrame.reserved1, RESERVEDA_LENGTH);
    turnStream.readRawData(mDataFrame.eos, EOS_LENGTH);
    turnStream.readRawData(mDataFrame.reserved2, RESERVEDB_LENGTH);
    turnStream.readRawData(mDataFrame.type, TYPE_LENGTH);

    packMsg();
}

QString MindrayWorker::charToString(const char *data, const int length)
{
    QByteArray tmpArray(data, length);
    QByteArray text = QByteArray::fromHex(tmpArray.toHex());

    QString textContent = text.data();

    return textContent;
}

float MindrayWorker::charToFloat(const char *data, const int length)
{
    QByteArray tmpArray(data, length);
    QByteArray text = QByteArray::fromHex(tmpArray.toHex());

    QString textContent = text.data();

    return textContent.toFloat();
}

void MindrayWorker::jsonInsert(QJsonArray &arr, const QString &key, const QString &unit, const float value)
{
    QJsonObject tmpObj;
    tmpObj["title"] = key;
    tmpObj["unit"] = unit;
    tmpObj["value"] = QString::number(value);
    arr.append(tmpObj);
}

void MindrayWorker::getLocalConfig()
{
    QString configFile = qApp->applicationDirPath() + "/config.ini";

    QSettings setting(configFile, QSettings::IniFormat);
    setting.beginGroup("BC2600");
    mCOM = setting.value("ComPort").toString();
    setting.endGroup();

    mStorePath = QString("%1/analysis/BC2600/result").arg(qApp->applicationDirPath());

    emit initSerial(mCOM);
}
