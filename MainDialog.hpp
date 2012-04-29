#ifndef HEADER__MAIN_DIALOG_HPP
#define HEADER__MAIN_DIALOG_HPP

#include <QCloseEvent>
#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QSystemTrayIcon>

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent=0);
    ~MainDialog(void);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void onQuit(void);

    void on_mCtrlBtn_clicked(void);
    void on_mTray_activated(QSystemTrayIcon::ActivationReason reason);
    void on_toggleAction_triggered(void);

private:
    void saveSettings(void);

private:
    QLineEdit *mHostEdit;
    QLineEdit *mPortEdit;
    QLineEdit *mUserEdit;
    QLineEdit *mPswdEdit;
    QLineEdit *mLAddrEdit;
    QLineEdit *mLPortEdit;
    QLineEdit *mWaitEdit;

    QPushButton *mCtrlBtn;
    QPushButton *mQuitBtn;

    QListWidget *mLogList;

    QSystemTrayIcon *mTray;

    QSettings *mSettings;
};

#endif
