#include "v200worker.h"
#include "qdatetime.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

V200Worker::V200Worker(QObject *parent) : QObject(parent)
{
    someInit();
    mTcpWorker = unique_ptr<TcpWorker>(new TcpWorker);
    mThread = unique_ptr<QThread>(new QThread);
    mTcpWorker->moveToThread(mThread.get());

    connect(this, &V200Worker::initTcpIP, mTcpWorker.get(), &TcpWorker::socketInit);
    connect(mTcpWorker.get(), &TcpWorker::updateTcpDataFrame, this, &V200Worker::parseData);

    mThread->start();
}

const char ackDevOB[] = {0x10,0x02,0x00,0x06,0x6F,0x01,0x00,0x00,0x00,0x00,0x10,0x03};

void V200Worker::parseData(const QByteArray &data)
{
    QByteArray recv(data);
    recv.remove(0, 12);
    char *msg = recv.data();

    quint16 itemID = quint8(msg[27])<<8 | quint8(msg[28]);
    quint8 uintID = quint8(msg[29]);
    quint8 valTypeID = quint8(msg[30]);
    quint8 aborsemiID = quint8(msg[31]);

    float value1 = ((quint8(msg[32])<<8) | (quint8(msg[33]))) /(pow(10,quint8(msg[34])));
    float value2 = ((quint8(msg[35])<<8) | (quint8(msg[36]))) /(pow(10,quint8(msg[37])));
//    float vmin = ((quint8(msg[38])<<8) | (quint8(msg[39]))) /(pow(10,quint8(msg[40])));
//    float vmax = ((quint8(msg[41])<<8) | (quint8(msg[42]))) /(pow(10,quint8(msg[43])));
    float coi = ((quint8(msg[44])<<8) | (quint8(msg[45]))) /(pow(10,quint8(msg[46])));

    QString strItem = m_ItemList.at(itemID-0x00C0);
    QString strUnit = m_UnitList.at(uintID);

    QString strValue1 = QString::number(value1, 'f', 2);
    QString strValue2 = QString::number(value2, 'f', 2);

    QString strValue;

    switch(valTypeID)
    {
    case 0x00:
        strValue = "Invalid";
        break;
    case 0x02:
        strValue = "="+strValue1;
        break;
    case 0x03:
        strValue = "<"+strValue1;
        break;
    case 0x04:
        strValue = "=<"+strValue1;
        break;
    case 0x05:
        strValue = ">"+strValue1;
        break;
    case 0x06:
        strValue = ">="+strValue1;
        break;
    case 0x07:
        strValue = strValue2+"-"+strValue2;
        break;
    default:
        strValue = " ";
        break;
    }

    QString straborsemi = m_QualSemiList.at(aborsemiID);
    QString strCOI = QString::number(coi, 'f', 2);

    QJsonObject root_Obj;
    root_Obj.insert("title", strItem);
    root_Obj.insert("unit", strUnit);
    root_Obj.insert("value", strValue);
    root_Obj.insert("aborsemi", straborsemi);
    root_Obj.insert("coi", strCOI);

    QJsonDocument root_Doc;
    root_Doc.setObject(root_Obj);
    QByteArray root_str = root_Doc.toJson(QJsonDocument::Indented);
    QString strJson(root_str);

    QString strDir = QString("%1/analysis/V200/result").arg(qApp->applicationDirPath());
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkpath(strDir);
    }

    QString fileName = QString("%1/analysis/V200/result/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile jsonfile(fileName);
    jsonfile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::NewOnly);
    QTextStream in(&jsonfile);
    in << strJson;
    jsonfile.close();
}


void V200Worker::someInit()
{
    m_ItemList<<"fTnI"<<"VitaminD"<<"CAV Ag"<<"CIV Ag"<<"Toxoplasma Ab"<<"FCoV Ag"<<"Giardia Ag"<<"CAV Ab"\
              <<"FPV Ab(2)"<<"FCV Ab(2)"<<"FHV Ab(2)"<<"CPV Ab(2)"<<"CDV Ab(2)"<<"FMD-NSP"<<"SDMA"<<"cProBNP"\
              <<"FPV Ag"<<"CCV Ag"<<"CDV Ag"<<"CPV Ag"<<"cProg"<<"CA15-3"<<"FHW Ag"<<"FeLV Ag"\
              <<"FIV Ab"<<"Anapl Ab"<<"Lyme Ab"<<"Ehrl Ab"<<"CHW Ag"<<"AIV Ag"<<"FPV Ab"<<"FCV Ab"\
              <<"FHV Ab"<<"CPV Ab"<<"CDV Ab"<<"fProBNP"<<"cTSH"<<"fPL"<<"cPL"<<"cTnI"\
              <<"D-Dimer"<<"fBile-Acid"<<"cBile Acid"<<"fTT4"<<"cTT4"<<"cCortisol"<<"fSAA"<<"cCRP";

    m_UnitList<<" "<<"ug/ml"<<"ug/dL"<<"nmol/L"<<"umol/L"<<"ug/L"<<"ng/ml"<<"pmol/L"<<"mmol/mol"<<"%"<<"mmol/L"\
              <<"mg/L"<<"mg/mmol"<<"mg/g"<<"mg/dL"<<"mIU/mL"<<"pg/mL";



    m_QualSemiList<<" "<<"Negative"<<"Positive"<<"Abnormal"<<"Normal"<<"Negative titer 0"<<"Low titer 1"<<"Low titer 2"\
                  <<"Medium titer 3"<<"High titer 4"<<"High titer 5"<<"High titer 6"<<"High titer 3.5"<<"High titer 4.5"<<"High titer 5.5";

}






