#ifndef HEADER__MAIN_DIALOG_HPP
#define HEADER__MAIN_DIALOG_HPP

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    MainDialog(QWidget *parent=0);
    ~MainDialog(void);

private:
#if 0
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
#endif
};

#endif
