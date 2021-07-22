#include "window.h"
#include "ui_window.h"
#include "flatui.h"
#include "qfile.h"
#include "config.h"
#include "qprocess.h"
#include "qsharedmemory.h"
#include "qmutex.h"
#include "qdatetime.h"
#include "qudpsocket.h"

#include "QtSerialPort/QSerialPortInfo"
#include "QtSerialPort/QSerialPort"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    initStyle();
    initConnectPage();
    initDevicePage();
    initAppPorcess();
}

Window::~Window()
{
    delete ui;
}

//程序退出
void Window::appExit()
{
    if(appStart) stopAllPro();
    Config::writeConfig();
    qApp->exit();
}

//重载窗口关闭事件
void Window::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

//QSS初始化 界面风格类Flat UI
void Window::initStyle()
{
    QString qss;
    QFile file(":/qss/flatwhite.css");
    if (file.open(QFile::ReadOnly))
    {
        QStringList list;
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line;
            in >> line;
            list << line;
        }
        qss = list.join("\n");
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(paletteColor));
        qApp->setStyleSheet(qss);
        file.close();
    }
    this->setWindowFlags(windowFlags() & ~ Qt::WindowMinimizeButtonHint);
}

//获取local IP V4地址
static QHostAddress getHostIPV4()
{
    foreach(const QHostAddress &hostAddress, QNetworkInterface::allAddresses())
        if ( hostAddress != QHostAddress::LocalHost && hostAddress.toIPv4Address() )
            return hostAddress;
    return QHostAddress::LocalHost;
}

//初始化连接页面显示
void Window::initConnectPage()
{
    //输入框样式
    FlatUI::setLineEditQss(ui->le_server_ip, 5, 2, "#DCE4EC", "#1ABC9C");
    FlatUI::setLineEditQss(ui->le_server_port, 5, 2, "#DCE4EC", "#1ABC9C");

    //显示IP 端口号
    ui->le_server_ip->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->le_server_port->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->le_server_ip->setText(getHostIPV4().toString());
    ui->le_server_port->setText(QString::number(Config::HttpPort));

    //读取显示自动连接状态
    ui->cbx_auto_start->setChecked(Config::AppAutoStart);
    ui->cbx_auto_start->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));

    //启动按钮状态显示
    ui->btn_start->setText(QStringLiteral("启动"));
    FlatUI::setPushButtonQss(ui->btn_start, 5, 8, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
    ui->btn_start->setIcon(QIcon(":/image/start.png"));
    ui->btn_start->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
}

static QStringList getAllComPortName()
{
    QStringList nameList;
    QSerialPort temp_serial;
    foreach (const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    {
        temp_serial.setPort(Info);
        if(temp_serial.open(QIODevice::ReadWrite))
        {
            //如果串口是可以读写方式打开的
            nameList << (Info.portName());
            temp_serial.close();
        }
    }
    return nameList;
}

//初始化设备页面显示
void Window::initDevicePage()
{
    //group box 字体
    ui->gb_idexx->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->gb_bc2600->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->gb_V200->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->gb_vb1->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->gb_poch->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->gb_pcr_mini8->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->gb_diano->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->gb_edan->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));

    //idexx
    ui->gb_idexx->setChecked(Config::IdexxEnabled);
    ui->le_idexx_request->setText(Config::IdexxRequestFolder);
    ui->le_idexx_result->setText(Config::IdexxResultFolder);
    ui->le_idexx_request->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->le_idexx_result->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));

    //bc2600
    ui->gb_bc2600->setChecked(Config::BC2600Enabled);
    ui->cbx_bc2600->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->cbx_bc2600->addItems(getAllComPortName());
    ui->cbx_bc2600->setCurrentIndex(ui->cbx_bc2600->findText(Config::BC2600ComPort));
    ui->cbx_bc2600->installEventFilter(this);

    //mini8
    ui->gb_pcr_mini8->setChecked(Config::Mini8Enabled);
    ui->le_mini8_request->setText(Config::Mini8RequestFolder);
    ui->le_mini8_result->setText(Config::Mini8ResultFolder);
    ui->le_mini8_request->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->le_mini8_result->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));

    //POCH
    ui->gb_poch->setChecked(Config::POCHEnabled);
    ui->cbx_poch->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->cbx_poch->addItems(getAllComPortName());
    ui->cbx_poch->setCurrentIndex(ui->cbx_vb1->findText(Config::POCHComPort));
    ui->cbx_poch->installEventFilter(this);

    //v200
    ui->gb_V200->setChecked(Config::V200Enabled);
    ui->le_v200_ip->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->le_v200_port->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->le_v200_ip->setText(Config::V200IP);
    ui->le_v200_port->setText(QString("%1").arg(Config::V200Port));

    //VB1
    ui->gb_vb1->setChecked(Config::VB1Enabled);
    ui->cbx_vb1->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->cbx_vb1->addItems(getAllComPortName());
    ui->cbx_vb1->setCurrentIndex(ui->cbx_vb1->findText(Config::VB1ComPort));
    ui->cbx_vb1->installEventFilter(this);

    //DIANO
    ui->gb_diano->setChecked(Config::DIANOEnabled);
    ui->cbx_diano->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->cbx_diano->addItems(getAllComPortName());
    ui->cbx_diano->setCurrentIndex(ui->cbx_vb1->findText(Config::DIANOComPort));
    ui->cbx_diano->installEventFilter(this);

    //EDAN
    ui->gb_edan->setChecked(Config::V200Enabled);
    ui->le_edan_port->setFont(QFont("Microsoft Yahei", 11, QFont::Bold));
    ui->le_edan_port->setText(QString("%1").arg(Config::V200Port));
}

bool Window::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->cbx_bc2600)
    {
        if(event->type() == QEvent::MouseButtonPress && ui->cbx_bc2600->isEnabled())
        {
            ui->cbx_bc2600->clear();
            ui->cbx_bc2600->addItems(getAllComPortName());
        }
    }
    else if(obj == ui->cbx_vb1)
    {
        if(event->type() == QEvent::MouseButtonPress && ui->cbx_vb1->isEnabled())
        {
            ui->cbx_vb1->clear();
            ui->cbx_vb1->addItems(getAllComPortName());
        }
    }
    else if(obj == ui->cbx_poch)
    {
        if(event->type() == QEvent::MouseButtonPress && ui->cbx_poch->isEnabled())
        {
            ui->cbx_poch->clear();
            ui->cbx_poch->addItems(getAllComPortName());
        }
    }
    else if(obj == ui->cbx_diano)
    {
        if(event->type() == QEvent::MouseButtonPress && ui->cbx_diano->isEnabled())
        {
            ui->cbx_diano->clear();
            ui->cbx_diano->addItems(getAllComPortName());
        }
    }
    return QWidget::eventFilter(obj, event);
}

void Window::on_cbx_auto_start_toggled(bool checked)
{
    Config::AppAutoStart = checked;
}

void Window::on_btn_start_clicked()
{
    if(appStart == false)
    {
        ui->btn_start->setText(QStringLiteral("停止"));
        FlatUI::setPushButtonQss(ui->btn_start, 5, 8, "#E74C3C", "#FFFFFF", "#EC7064", "#FFF5E7", "#DC2D1A", "#F5A996");
        ui->btn_start->setIcon(QIcon(":/image/pause.png"));
        setWindowIcon(QIcon(":/image/hb_on.png"));
        ui->tab_device->setEnabled(false);
        Config::writeConfig();
        startAllPro();
    }
    else
    {
        ui->btn_start->setText(QStringLiteral("启动"));
        FlatUI::setPushButtonQss(ui->btn_start, 5, 8, "#1ABC9C", "#E6F8F5", "#2EE1C1", "#FFFFFF", "#16A086", "#A7EEE6");
        ui->btn_start->setIcon(QIcon(":/image/start.png"));
        setWindowIcon(QIcon(":/image/hb_off.png"));
        ui->tab_device->setEnabled(true);
        stopAllPro();
    }

    appStart = !appStart;
}

void Window::on_gb_idexx_toggled(bool arg1)
{
    Config::IdexxEnabled = arg1;
}

void Window::on_btn_idexx_request_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择工单保存路径…"), "./");
    if(filePath != NULL)
    {
        ui->le_idexx_request->setText(filePath);
        Config::IdexxRequestFolder = filePath;
    }
}

void Window::on_btn_idexx_result_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择报告保存路径…"), "./");
    if(filePath != NULL)
    {
        ui->le_idexx_result->setText(filePath);
        Config::IdexxResultFolder = filePath;
    }
}

void Window::on_gb_bc2600_toggled(bool arg1)
{
    Config::BC2600Enabled = arg1;
}

void Window::on_cbx_bc2600_activated(const QString &arg1)
{
    Config::BC2600ComPort = arg1;
}

void Window::on_gb_pcr_mini8_toggled(bool arg1)
{
    Config::Mini8Enabled = arg1;
}

void Window::on_btn_mini8_request_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择工单保存路径…"), "./");
    if(filePath != NULL)
    {
        ui->le_mini8_request->setText(filePath);
        Config::Mini8RequestFolder = filePath;
    }
}

void Window::on_btn_mini8_result_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, QStringLiteral("请选择报告保存路径…"), "./");
    if(filePath != NULL)
    {
        ui->le_mini8_result->setText(filePath);
        Config::Mini8ResultFolder = filePath;
    }
}

void Window::on_gb_poch_toggled(bool arg1)
{
    Config::POCHEnabled = arg1;
}

void Window::on_cbx_poch_activated(const QString &arg1)
{
    Config::POCHComPort = arg1;
}

void Window::on_gb_V200_toggled(bool arg1)
{
    Config::V200Enabled = arg1;
}

void Window::on_le_v200_ip_textEdited(const QString &arg1)
{
    Config::V200IP = arg1;
}

void Window::on_le_v200_port_textEdited(const QString &arg1)
{
    Config::V200Port = arg1.toInt();
}

void Window::on_gb_vb1_toggled(bool arg1)
{
    Config::VB1Enabled = arg1;
}

void Window::on_cbx_vb1_activated(const QString &arg1)
{
    Config::VB1ComPort = arg1;
}

void Window::on_gb_diano_toggled(bool arg1)
{
    Config::DIANOEnabled = arg1;
}

void Window::on_cbx_diano_activated(const QString &arg1)
{
    Config::DIANOComPort = arg1;
}

void Window::on_gb_edan_toggled(bool arg1)
{
    Config::EDANEnabled = arg1;
}

void Window::on_le_edan_port_textEdited(const QString &arg1)
{
    Config::EDANPort = arg1.toInt();
}

static void startProcess(const QString &pName)
{
    QProcess *p = new QProcess;
    QString appDir = QString("\"%1/%2.exe\"").arg(qApp->applicationDirPath()).arg(pName);
    qDebug()<<appDir;
    p->start(appDir);
}

static void killProcess(const QString &pName)
{
    QProcess *p = new QProcess;
    p->start(QString("taskkill /im %1.exe /f").arg(pName));
}

void Window::startAllPro()
{
    startProcess("http");
    if(Config::IdexxEnabled) startProcess("idexx");
    if(Config::BC2600Enabled) startProcess("bc2600");
    if(Config::V200Enabled) startProcess("v200");
    if(Config::VB1Enabled) startProcess("vb1");
    if(Config::POCHEnabled) startProcess("poch");
    if(Config::Mini8Enabled) startProcess("mini8");
    if(Config::DIANOEnabled) startProcess("diano");
    if(Config::EDANEnabled) startProcess("edan");
    ui->tb_log->clear();
}

void Window::stopAllPro()
{
    killProcess("http");
    if(Config::IdexxEnabled) killProcess("idexx");
    if(Config::BC2600Enabled) killProcess("bc2600");
    if(Config::V200Enabled) killProcess("v200");
    if(Config::VB1Enabled) killProcess("vb1");
    if(Config::POCHEnabled) killProcess("poch");
    if(Config::Mini8Enabled) killProcess("mini8");
    if(Config::DIANOEnabled) killProcess("diano");
    if(Config::EDANEnabled) killProcess("edan");
    saveLog();
    ui->tb_log->setText(QStringLiteral("运行日志显示在这里"));
}

void Window::saveLog()
{
    QString strDir = QString("%1/log").arg(qApp->applicationDirPath());
    QDir dir(strDir);
    if(!dir.exists())
    {
        dir.mkpath(strDir);
    }

    QFile file(this);
    QString fileName = QString("%1/log/%2.log").arg(qApp->applicationDirPath()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH_mm_ss"));
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    QTextStream logStream(&file);
    logStream << ui->tb_log->toPlainText() << "\n";
    file.close();
}

void Window::initAppPorcess()
{
    udp = new QUdpSocket(this);
    udp->bind(Config::AppListenPort);
    connect(udp, SIGNAL(readyRead()), this, SLOT(readUdpData()));

    if(Config::AppAutoStart)
    {
        on_btn_start_clicked();
        setWindowIcon(QIcon(":/image/hb_on.png"));
    }
}

void Window::readUdpData()
{
    QByteArray tempData;
    QString udpdata;
    do
    {
        tempData.resize(udp->pendingDatagramSize());
        udp->readDatagram(tempData.data(), tempData.size());
        udpdata = QLatin1String(tempData);
    }
    while (udp->hasPendingDatagrams());
    ui->tb_log->append(QString("[%1]:%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH:mm:ss")).arg(udpdata));
}


