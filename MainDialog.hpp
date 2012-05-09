#ifndef HEADER__MAIN_DIALOG_HPP
#define HEADER__MAIN_DIALOG_HPP

#include <QCloseEvent>
#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QProcess>
#include <QPushButton>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QTimer>

#include "constants.hpp"

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

    void on_mProcess_started(void);
    void on_mProcess_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_mProcess_error(QProcess::ProcessError error);
    void on_mProcess_readyReadStandardOutput(void);

    void setSshStatus(SshStatus status);
    void addLogMsg(LogLevel level, const QString &msg);

    void startRunning(void);

private:
    void saveSettings(void);

    void stopRunning(void);

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

    QIcon mInfoIcon;
    QIcon mWarnIcon;
    QIcon mErrorIcon;

    QSystemTrayIcon *mTray;

    QIcon mStoppedIcon;
    QIcon mConnectingIcon;
    QIcon mConnectedIcon;
    QIcon mSleepingIcon;

    QSettings *mSettings;

    bool mIsKeepRunning;
    QProcess *mProcess;
    QTimer *mRestartTimer;
};

#endif
