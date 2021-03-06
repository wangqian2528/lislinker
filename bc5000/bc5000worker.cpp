#include "bc5000worker.h"

#define QJSONTIME qPrintable(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"))

BC5000Worker::BC5000Worker(QObject *parent) : QObject(parent)
{
    mTcpWorker = unique_ptr<TcpWorker>(new TcpWorker);
    mThread = unique_ptr<QThread>(new QThread);
    mTcpWorker->moveToThread(mThread.get());

    connect(this, &BC5000Worker::initTcpIP, mTcpWorker.get(), &TcpWorker::socketInit);
    connect(mTcpWorker.get(), &TcpWorker::updateTcpDataFrame, this, &BC5000Worker::parseData);

    connect(this, &BC5000Worker::updateMsgCompleted, this, &BC5000Worker::requestPack);
    connect(this, &BC5000Worker::updateOrderCmd, this, &BC5000Worker::requestOrder);

    mThread->start();
}

void BC5000Worker::parseData(const QByteArray &data)
{
    QString msg = QString(data);
    QStringList msgList = msg.split("\r");
    for (int i = 0; i < msgList.count(); i++) {
        QString content = msgList.at(i);
        if (content.indexOf("MSH") == 0) {
            mMSHContent = content;
            parseMSH(content);
        } else if (content.indexOf("PID") == 0) {
            parsePID(content);
        } else if (content.indexOf("PV1") == 0) {
            parsePIV(content);
        } else if (content.indexOf("OBR") == 0) {
            parseOBR(content);
        } else if (content.indexOf("OBX") == 0) {
            parseOBX(content);
        }
    }

    if (mMsgType == ORU_R01) {
        emit updateMsgCompleted();
    } else if (mMsgType == ORM_O01) {
        emit updateOrderCmd();
    }

    QJsonDocument jsonDocument;
    jsonDocument.setObject(mJsonObject);
    QString fileName = QString("%1/analysis/BC5000/result/%2.json").arg(qApp->applicationDirPath()).arg(QJSONTIME);
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite)) {
        file.write(jsonDocument.toJson());
    }
    file.close();

    clearJson();
}

void BC5000Worker::parseMSH(const QString &msg) {
    QStringList list = msg.split("|");
    if (list.count() < SERIAL_NUMBER_19) {
        for (int i = 0; i < (SERIAL_NUMBER_19 - list.count()); i++) {
            list.append("");
        }
    }
    QJsonArray jsonArray;
    QJsonObject jsonChildObj;
    mJsonObject.insert("versionID", list.at(SERIAL_NUMBER_11));                 // HL7?????????
    jsonChildObj.insert("sendAPP", list.at(SERIAL_NUMBER_2));                   // ?????????????????????
    jsonChildObj.insert("sendFacility", list.at(SERIAL_NUMBER_3));              // ???????????????
    jsonChildObj.insert("msgControlID", list.at(SERIAL_NUMBER_9));              // ????????????ID
    jsonChildObj.insert("processID", list.at(SERIAL_NUMBER_10));                // P: ?????????????????????????????? | Q: ????????????????????????
    jsonChildObj.insert("msgDateTime", list.at(SERIAL_NUMBER_6));               // ??????????????????
    jsonArray.append(jsonChildObj);
    mJsonObject.insert("MSH", jsonArray);
    QString msgType = list.at(SERIAL_NUMBER_8);
    mMsgTime = list.at(SERIAL_NUMBER_6);
    mMsgControlID = list.at(SERIAL_NUMBER_9);
    if (msgType == QStringLiteral("ORU^R01")) {
        mMsgType = ORU_R01;
    } else if (msgType == QStringLiteral("ORM^O01")) {
        mMsgType = ORM_O01;
    } else {
        mMsgType = DEFAULT;
    }
}

void BC5000Worker::parsePID(const QString &pid) {

    QStringList list = pid.split("|");
    int count = SERIAL_NUMBER_9 - list.count();
    if (list.count() < SERIAL_NUMBER_9) {
        for (int i = 0; i < count; i++) {
            list.append("");
        }
    }
    QJsonObject jsonChildObj;
    jsonChildObj.insert("patientNum", list.at(SERIAL_NUMBER_3).split("^^^^MR").at(0));              // ?????????
    if (list.at(SERIAL_NUMBER_5).indexOf("^") > -1) {
        jsonChildObj.insert("petName", list.at(SERIAL_NUMBER_5).split("^").at(0));                  // ?????????
        jsonChildObj.insert("ownerName", list.at(SERIAL_NUMBER_5).split("^").at(1));                // ?????????
    }
    jsonChildObj.insert("birthDate", list.at(SERIAL_NUMBER_7));                                     // ???????????? | ???????????????
    jsonChildObj.insert("sex", list.at(SERIAL_NUMBER_8));                                           // ??????
    mPatientArray.append(jsonChildObj);

    mJsonObject.insert("PMSG", mPatientArray);
}

void BC5000Worker::parsePIV(const QString &pv1) {
    QStringList list = pv1.split("|");
    int count = SERIAL_NUMBER_3 - list.count();
    if (list.count() < SERIAL_NUMBER_3) {
        for (int i = 0; i < count; i++) {
            list.append("");
        }
    }

    QJsonArray jsonArray;
    QJsonObject jsonChildObj;
    jsonChildObj.insert("species", list.at(SERIAL_NUMBER_2));                   // ????????????

    mPatientArray.append(jsonChildObj);

    mJsonObject.insert("PMSG", mPatientArray);
}

void BC5000Worker::parseOBR(const QString &obr) {
    QStringList list = obr.split("|");
    int count = SERIAL_NUMBER_33 - list.count();
    if (list.count() < SERIAL_NUMBER_33) {
        for (int i = 0; i < count; i++) {
            list.append("");
        }
    }
    QJsonArray jsonArray;
    QJsonObject jsonChildObj;
    mFillerNum = list.at(SERIAL_NUMBER_3);
    jsonChildObj.insert("fillerNum", mFillerNum);                               // ????????????
    jsonChildObj.insert("universalID", list.at(SERIAL_NUMBER_4));               // ??????????????????
    jsonChildObj.insert("requestdTime", list.at(SERIAL_NUMBER_6));              // ????????????
    jsonChildObj.insert("observationTime", list.at(SERIAL_NUMBER_7));           // ????????????
    jsonChildObj.insert("veterinarian", list.at(SERIAL_NUMBER_10));             //
    jsonChildObj.insert("clinicalInfo", list.at(SERIAL_NUMBER_13));             // ????????????
    jsonChildObj.insert("specimenTime", list.at(SERIAL_NUMBER_14));             // ????????????
    jsonChildObj.insert("diagnostic", list.at(SERIAL_NUMBER_24));               // ????????????ID
    jsonChildObj.insert("principal", list.at(SERIAL_NUMBER_32));                // ?????????/?????????
    jsonArray.append(jsonChildObj);
    mJsonObject.insert("OBR", jsonArray);
}

void BC5000Worker::parseOBX(const QString &obx) {
    QStringList list = obx.split("|");
    int count = SERIAL_NUMBER_14 - list.count();
    if (list.count() < SERIAL_NUMBER_14) {
        for (int i = 0; i < count; i++) {
            list.append("");
        }
    }
    QJsonObject jsonChildObj;
    QString obIdentifier = list.at(SERIAL_NUMBER_3);
    if (obIdentifier.indexOf("^") > -1) {
        jsonChildObj["observationID"] = obIdentifier.split("^").at(0);              // ??????????????????
        jsonChildObj["title"] = obIdentifier.split("^").at(1);                      // ??????????????????
        jsonChildObj["encodeSys"] = obIdentifier.split("^").at(2);                  // ????????????????????????
    }
    jsonChildObj["value"] = list.at(SERIAL_NUMBER_5);                           // ??????????????????
    jsonChildObj["unit"] = list.at(SERIAL_NUMBER_6);                            // ??????????????????
    jsonChildObj["range"] = list.at(SERIAL_NUMBER_7);                           // ??????????????????
    jsonChildObj["abnormalFlags"] = list.at(SERIAL_NUMBER_8);                   // ??????????????????
    jsonChildObj["observResult"] = list.at(SERIAL_NUMBER_11);                   // ??????????????????
    jsonChildObj["userDefine"] = list.at(SERIAL_NUMBER_13);                     // ???????????????
    mObxArray.append(jsonChildObj);
    mJsonObject.insert("OBX", mObxArray);
}

void BC5000Worker::parseMSA(const QString &msa) {
    QStringList list = msa.split("|");
    if (list.count() < SERIAL_NUMBER_7) {
        for (int i = 0; i < (SERIAL_NUMBER_7 - list.count()); i++) {
            list.append("");
        }
    }
    QJsonArray jsonArray;
    QJsonObject jsonChildObj;
    jsonChildObj.insert("acCode", list.at(SERIAL_NUMBER_1));
    jsonChildObj.insert("msgID", list.at(SERIAL_NUMBER_2));
    jsonChildObj.insert("errorCondition", list.at(SERIAL_NUMBER_6));
    jsonArray.append(jsonChildObj);
    mJsonObject.insert("MSA", jsonArray);
}

void BC5000Worker::clearJson() {
    QStringList jsonKeys = mJsonObject.keys();
    for (int i = 0; i < jsonKeys.count(); i++) {
        mJsonObject.remove(jsonKeys.at(i));
    }
    int patientArraySize = mPatientArray.size();
    for (int i = 0; i < patientArraySize; i++) {
        mPatientArray.removeFirst();
    }
    int obxArraySize = mObxArray.size();
    for (int i = 0; i < obxArraySize; i++) {
        mObxArray.removeFirst();
    }
}

void BC5000Worker::requestPack() {
    QString msa = QString("MSA|AA|%1").arg(mMsgControlID);

    QByteArray sendArray;
    sendArray.append(0x0B);
    sendArray.append(mMSHContent.toLatin1());
    sendArray.append(msa.toLatin1());
    sendArray.append(0x1C);
    sendArray.append(0x0D);

//    mTcpClient->write(sendArray);
}

void BC5000Worker::requestOrder() {
    QString msh = "MSH|^~\\&|||||20210409154446||ORR^O02|1|P|2.3.1||||||UNICODE";
    QString msa = "MSA|AA|2||||";
    QString pid = "PID|1||NO-4B00003^^^^MR||Bob^Jerry||20150105|male";
    QString pv1 = "PV1|1|Species\rORC|AF|AT-JK00927";
    QString obr = "OBR|1||AT-JK00927|00001^Automated Count^99MRC||20150318141722||||Bill|||_d_|20150318141722||||||||||HM";
    QString obx = "OBX|1|NM|30525-0^Age^LN||2|mo|||||F";
    QString obx2 = "OBX|2|IS|08001^Take Mode^99MRC||O||||||F";
    QString obx3 = "OBX|3|IS|08002^Blood Mode^99MRC||P||||||F";
    QString obx4 = "OBX|4|IS|08003^Test Mode^99MRC||CBC+DIFF||||||F";
    QString obx5 = "OBX|5|IS|01002^Ref Group^99MRC||0||||||F";
    QString obx6 = "OBX|6|IS|01003^Species^99MRC||724||||||F";
    QString obx7 = "OBX|7|XPN|01004^Master Name^99MRC||Bob^Jerry||||||F";

    QByteArray sendArray;
    sendArray.append(0x0B);
    sendArray.append(msh.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(msa.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(pid.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(pv1.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(obr.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(obx.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(obx2.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(obx3.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(obx4.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(obx5.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(obx6.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(obx7.toLatin1());
    sendArray.append(0x0D);
    sendArray.append(0x1C);
    sendArray.append(0x0D);

//    mTcpClient->write(sendArray);
}
