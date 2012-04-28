#ifndef HEADER__MAIN_DIALOG_HPP
#define HEADER__MAIN_DIALOG_HPP

#include <QtGui/QCloseEvent>
#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSystemTrayIcon>

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent=0);
    ~MainDialog(void);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_mCtrlBtn_clicked(void);
    void on_mQuitBtn_clicked(void);
    void on_mTray_activated(QSystemTrayIcon::ActivationReason reason);
    void on_toggleAction_triggered(void);
    void on_quitAction_triggered(void);

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
};

#endif
