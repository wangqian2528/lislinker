#ifndef TRAYICON_H
#define TRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>

class QMenu;

class TrayIcon : public QObject
{
    Q_OBJECT
public:
    static TrayIcon *Instance();
    explicit TrayIcon(QObject *parent = nullptr);

private:
    static QScopedPointer<TrayIcon> self;
    QWidget *mainWidget;            //对应所属主窗体
    QSystemTrayIcon *trayIcon;      //托盘对象
    QMenu *menu;                    //右键菜单
    QAction *openAction;
    QAction *quitAction;

private slots:
    void iconIsActived(QSystemTrayIcon::ActivationReason reason);

public:
    bool getVisible()       const;

public Q_SLOTS:

    void setMainWidget(QWidget *mainWidget);//设置所属主窗体
    void showMessage(const QString &title, const QString &msg,
                     QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information, int msecs = 5000);//显示消息
    void setIcon(const QIcon &icon);//设置图标
    void setToolTip(const QString &tip);//设置提示信息
    void setVisible(bool visible);//设置是否可见

Q_SIGNALS:
    void trayIconExit();

};

#endif // TRAYICON_H
