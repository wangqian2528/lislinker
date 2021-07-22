#include "pochworker.h"
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

PochWorker::PochWorker(QObject *parent) : QObject(parent)
{
    mSerialWorker = unique_ptr<SerialPort>(new SerialPort);
    mThread = unique_ptr<QThread>(new QThread);
    mSerialWorker->moveToThread(mThread.get());

    connect(this, &PochWorker::initSerial, mSerialWorker.get(), &SerialPort::comInit);
    connect(mSerialWorker.get(), &SerialPort::updateSerialDataFrame, this, &PochWorker::parseData);

    mThread->start();
}

void PochWorker::parseData(const QByteArray &data)
{
    QByteArray recv(data.left(171));
    QByteArray tmp = QByteArray::fromHex(recv.toHex());
    QDataStream turnStream(&recv, QIODevice::ReadOnly);
    turnStream.setByteOrder(QDataStream::BigEndian);

    turnStream.readRawData(mDataFrame1.head, 1);
    turnStream.readRawData(mDataFrame1.code1, 1);
    turnStream.readRawData(mDataFrame1.code2, 1);
    turnStream.readRawData(mDataFrame1.devCode, 1);
    turnStream.readRawData(mDataFrame1.devID, 40);
    turnStream.readRawData(mDataFrame1.year, 4);
    turnStream.readRawData(mDataFrame1.month, 2);
    turnStream.readRawData(mDataFrame1.day, 2);
    turnStream.readRawData(mDataFrame1.info, 1);
    turnStream.readRawData(mDataFrame1.samID, 15);
    turnStream.readRawData(mDataFrame1.anaInfo, 6);
    turnStream.readRawData(mDataFrame1.species, 1);
    turnStream.readRawData(mDataFrame1.WBC, 5);
    turnStream.readRawData(mDataFrame1.RBC, 5);
    turnStream.readRawData(mDataFrame1.HGB, 5);
    turnStream.readRawData(mDataFrame1.HCT, 5);
    turnStream.readRawData(mDataFrame1.MCV, 5);
    turnStream.readRawData(mDataFrame1.MCH, 5);
    turnStream.readRawData(mDataFrame1.MCHC, 5);
    turnStream.readRawData(mDataFrame1.PLT, 5);
    turnStream.readRawData(mDataFrame1.LYM, 5);
    turnStream.readRawData(mDataFrame1.OTHR, 5);
    turnStream.readRawData(mDataFrame1.EO, 5);
    turnStream.readRawData(mDataFrame1.LYM_, 5);
    turnStream.readRawData(mDataFrame1.OTHR_, 5);
    turnStream.readRawData(mDataFrame1.EO_, 5);
    turnStream.readRawData(mDataFrame1.RDW_SD, 5);
    turnStream.readRawData(mDataFrame1.RDW_CV, 5);
    turnStream.readRawData(mDataFrame1.PDW, 5);
    turnStream.readRawData(mDataFrame1.MPV, 5);
    turnStream.readRawData(mDataFrame1.P_LCR, 5);
    turnStream.readRawData(mDataFrame1.tail, 1);

//    QByteArray recv1(data);
//    recv1.remove(0, 171);
//    recv1 = recv1.left(204);
//    QByteArray tmp1 = QByteArray::fromHex(recv1.toHex());
//    QDataStream turnStream1(&recv1, QIODevice::ReadOnly);
//    turnStream1.setByteOrder(QDataStream::BigEndian);

//    turnStream1.readRawData(mDataFrame2.head, 1);
//    turnStream1.readRawData(mDataFrame2.code1, 1);
//    turnStream1.readRawData(mDataFrame2.code2, 1);
//    turnStream1.readRawData(mDataFrame2.WBC_WL, 100);
//    turnStream1.readRawData(mDataFrame2.RBC_WL, 100);
//    turnStream1.readRawData(mDataFrame2.tail, 1);

//    QByteArray recv2(data);
//    recv2.remove(0, 375);
//    QByteArray tmp2 = QByteArray::fromHex(recv2.toHex());
//    QDataStream turnStream2(&recv2, QIODevice::ReadOnly);
//    turnStream2.setByteOrder(QDataStream::BigEndian);

//    turnStream.readRawData(mDataFrame3.head, 1);
//    turnStream.readRawData(mDataFrame3.code1, 1);
//    turnStream.readRawData(mDataFrame3.code2, 1);
//    turnStream.readRawData(mDataFrame3.PLT_WL, 80);
//    turnStream.readRawData(mDataFrame3.WBC_LD, 2);
//    turnStream.readRawData(mDataFrame3.WBC_T1, 2);
//    turnStream.readRawData(mDataFrame3.WBC_T2, 2);
//    turnStream.readRawData(mDataFrame3.WBC_UD, 2);
//    turnStream.readRawData(mDataFrame3.RBC_LD, 2);
//    turnStream.readRawData(mDataFrame3.RBC_UD, 2);
//    turnStream.readRawData(mDataFrame3.PLT_LD, 2);
//    turnStream.readRawData(mDataFrame3.PLT_UD, 2);
//    turnStream.readRawData(mDataFrame3.RESERVE, 128);
//    turnStream.readRawData(mDataFrame2.tail, 1);

    packMsg();
}

void PochWorker::packMsg()
{
    QJsonObject root_Obj;
    QJsonArray result_arry;

    jsonInsert(result_arry, "WBC", "10^9/uL", charToFloat(mDataFrame1.WBC, 5) / 10);
    jsonInsert(result_arry, "RBC", "10^12/uL", charToFloat(mDataFrame1.RBC, 5) / 100);
    jsonInsert(result_arry, "HGB", "g/L", charToFloat(mDataFrame1.HGB, 5) / 1);
    jsonInsert(result_arry, "HCT", "%", charToFloat(mDataFrame1.HCT, 5) / 1000);
    jsonInsert(result_arry, "MCV", "fL", charToFloat(mDataFrame1.MCV, 5) / 10);
    jsonInsert(result_arry, "MCH", "pg", charToFloat(mDataFrame1.MCH, 5) / 10);
    jsonInsert(result_arry, "MCHC", "g/L", charToFloat(mDataFrame1.MCHC, 5) / 1);
    jsonInsert(result_arry, "PLT", "10^9/uL", charToFloat(mDataFrame1.PLT, 5) / 1);
    jsonInsert(result_arry, "LYM%", "%", charToFloat(mDataFrame1.LYM, 5) / 1000);
    jsonInsert(result_arry, "OTHR%", "%", charToFloat(mDataFrame1.OTHR, 5) / 1000);
    jsonInsert(result_arry, "EO%", "%", charToFloat(mDataFrame1.EO, 5) / 1000);
    jsonInsert(result_arry, "LYM#", "10^9/uL", charToFloat(mDataFrame1.LYM_, 5) / 10);
    jsonInsert(result_arry, "OTHR#", "10^9/uL", charToFloat(mDataFrame1.OTHR_, 5) / 10);
    jsonInsert(result_arry, "EO#", "10^9/uL", charToFloat(mDataFrame1.EO_, 5) / 10);
    jsonInsert(result_arry, "RDW-SD", "fL", charToFloat(mDataFrame1.RDW_SD, 5) / 10);
    jsonInsert(result_arry, "RDW-CV", "%", charToFloat(mDataFrame1.RDW_CV, 5) / 1000);
    jsonInsert(result_arry, "PDW", "fL", charToFloat(mDataFrame1.PDW, 5) / 10);
    jsonInsert(result_arry, "MPV", "fL", charToFloat(mDataFrame1.MPV, 5) / 10);
    jsonInsert(result_arry, "P-LCR", "%", charToFloat(mDataFrame1.P_LCR, 5) / 1000);

    root_Obj.insert("results", result_arry);

//    QJsonArray WBC_WL_arry;
//    for(int i=0; i<=99;i++)
//    {
//        WBC_WL_arry.append(charToInt(&mDataFrame2.WBC_WL[i], 2));
//    }
//    root_Obj.insert("WBC_WL", WBC_WL_arry);

//    QJsonArray RBC_WL_arry;
//    for(int i=0; i<=99;i++)
//    {
//        RBC_WL_arry.append(charToInt(&mDataFrame2.RBC_WL[i], 2));
//    }
//    root_Obj.insert("RBC_WL", RBC_WL_arry);

//    QJsonArray PLT_WL_arry;
//    for(int i=0; i<=79;i++)
//    {
//        PLT_WL_arry.append(charToInt(&mDataFrame3.PLT_WL[i], 2));
//    }
//    root_Obj.insert("PLT_WL", PLT_WL_arry);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QString strDir = QString("%1/analysis/POCH/result").arg(qApp->applicationDirPath());
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkpath(strDir);
    }

    QString fileName = QString("%1/analysis/POCH/result/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile jsonfile(fileName);
    jsonfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly);
    QTextStream in(&jsonfile);
    in << strJson;
    jsonfile.close();
}

float PochWorker::charToFloat(const char *data, const int length)
{
    QByteArray tmpArray(data, length-1);
    QByteArray text = QByteArray::fromHex(tmpArray.toHex());
    QString textContent = text.data();
    return textContent.toFloat();
}

int PochWorker::charToInt(const char *data, const int length)
{
    QByteArray tmpArray(data, length-1);
    QByteArray text = QByteArray::fromHex(tmpArray.toHex());
    QString textContent = text.data();
    return textContent.toInt();
}

void PochWorker::jsonInsert(QJsonArray &arr, const QString &key, const QString &unit, const float value)
{
    QJsonObject tmpObj;
    tmpObj["title"] = key;
    tmpObj["unit"] = unit;
    tmpObj["value"] = QString::number(value);
    arr.append(tmpObj);
}
