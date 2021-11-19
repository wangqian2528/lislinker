#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QTextStream>
#include <QNetworkInterface>
#include <QEvent>
#include <QFileDialog>

class QUdpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();

public slots:
    void appExit();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void readUdpData();
    void paraUdpData(QString &str);

    void on_cbx_auto_start_toggled(bool checked);
    void on_btn_start_clicked();

    void on_gb_idexx_toggled(bool arg1);
    void on_btn_idexx_request_clicked();
    void on_btn_idexx_result_clicked();

    void on_gb_bc2600_toggled(bool arg1);
    void on_cbx_bc2600_activated(const QString &arg1);

    void on_gb_pcr_mini8_toggled(bool arg1);
    void on_btn_mini8_request_clicked();
    void on_btn_mini8_result_clicked();

    void on_gb_poch_toggled(bool arg1);
    void on_cbx_poch_activated(const QString &arg1);

    void on_gb_V200_toggled(bool arg1);
    void on_le_v200_ip_textEdited(const QString &arg1);
    void on_le_v200_port_textEdited(const QString &arg1);

    void on_gb_vb1_toggled(bool arg1);
    void on_cbx_vb1_activated(const QString &arg1);

    void on_gb_diano_toggled(bool arg1);
    void on_cbx_diano_activated(const QString &arg1);

    void on_gb_edan_toggled(bool arg1);
    void on_le_edan_port_textEdited(const QString &arg1);

    void on_cbx_diano_baud_currentIndexChanged(const QString &arg1);

    void on_gb_h120_toggled(bool arg1);
    void on_cbx_h120_activated(const QString &arg1);

    void on_gb_mpoint_toggled(bool arg1);
    void on_cbx_mpoint_activated(const QString &arg1);

private:
    Ui::Window *ui;

    QUdpSocket *udp;

    bool appStart = false;

    void initStyle();
    void initConnectPage();
    void initDevicePage();
    void initAppPorcess();

    void startAllPro();
    void stopAllPro();
    void saveLog();
};
#endif // WINDOW_H
