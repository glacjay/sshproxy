#include <QtGui/QApplication>
#include <QtGui/QAction>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>

#include "MainDialog.hpp"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    mHostEdit(new QLineEdit),
    mPortEdit(new QLineEdit("22")),
    mUserEdit(new QLineEdit),
    mPswdEdit(new QLineEdit),
    mLAddrEdit(new QLineEdit("127.0.0.1")),
    mLPortEdit(new QLineEdit("1077")),
    mWaitEdit(new QLineEdit("10")),
    mCtrlBtn(new QPushButton(tr("Connect"))),
    mQuitBtn(new QPushButton(tr("Quit"))),
    mLogList(new QListWidget)
{
    setWindowTitle(tr("SSH Proxy"));

    QLabel *hostLabel = new QLabel(tr("SSH Host:"));
    hostLabel->setBuddy(mHostEdit);
    QLabel *portLabel = new QLabel(tr("SSH Port:"));
    portLabel->setBuddy(mPortEdit);
    QLabel *userLabel = new QLabel(tr("Username:"));
    userLabel->setBuddy(mUserEdit);
    QLabel *pswdLabel = new QLabel(tr("Password:"));
    pswdLabel->setBuddy(mPswdEdit);
    QLabel *lAddrLabel = new QLabel(tr("Listen Addr:"));
    lAddrLabel->setBuddy(mLAddrEdit);
    QLabel *lPortLabel = new QLabel(tr("Listen Port:"));
    lPortLabel->setBuddy(mLPortEdit);
    QLabel *waitLabel = new QLabel(tr("Seconds wait to reconnect:"));
    waitLabel->setBuddy(mWaitEdit);

    mPswdEdit->setEchoMode(QLineEdit::Password);

    QVBoxLayout *inputLayout = new QVBoxLayout;
    inputLayout->addWidget(hostLabel);
    inputLayout->addWidget(mHostEdit);
    inputLayout->addWidget(portLabel);
    inputLayout->addWidget(mPortEdit);
    inputLayout->addWidget(userLabel);
    inputLayout->addWidget(mUserEdit);
    inputLayout->addWidget(pswdLabel);
    inputLayout->addWidget(mPswdEdit);
    inputLayout->addWidget(lAddrLabel);
    inputLayout->addWidget(mLAddrEdit);
    inputLayout->addWidget(lPortLabel);
    inputLayout->addWidget(mLPortEdit);
    inputLayout->addWidget(waitLabel);
    inputLayout->addWidget(mWaitEdit);

    QGroupBox *inputGroup = new QGroupBox(tr("Settings"));
    inputGroup->setLayout(inputLayout);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(inputGroup);
    leftLayout->insertStretch(-1);
    leftLayout->addWidget(mCtrlBtn);
    leftLayout->addWidget(mQuitBtn);

    QWidget *leftWidget = new QWidget;
    leftWidget->setLayout(leftLayout);

    QVBoxLayout *logLayout = new QVBoxLayout;
    logLayout->addWidget(mLogList);

    QGroupBox *logGroup = new QGroupBox(tr("Log"));
    logGroup->setLayout(logLayout);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(leftWidget);
    splitter->addWidget(logGroup);
    splitter->setStretchFactor(1, 5);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(splitter);

    setLayout(mainLayout);
    resize(800, 600);

    connect(mCtrlBtn, SIGNAL(clicked(void)), this, SLOT(on_mCtrlBtn_clicked(void)));
    connect(mQuitBtn, SIGNAL(clicked(void)), this, SLOT(on_mQuitBtn_clicked(void)));

    QPixmap pixmap(":/icon.png");
    QIcon icon(pixmap);
    mTray = new QSystemTrayIcon(icon);
    mTray->show();

    qApp->setWindowIcon(icon);

    connect(mTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(on_mTray_activated(QSystemTrayIcon::ActivationReason)));

    QAction *toggleAction = new QAction(tr("Toggle Main Dialog"), this);
    connect(toggleAction, SIGNAL(triggered(void)),
            this, SLOT(on_toggleAction_triggered(void)));

    QAction *quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered(void)),
            this, SLOT(on_quitAction_triggered(void)));

    QMenu *trayMenu = new QMenu;
    trayMenu->addAction(toggleAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    mTray->setContextMenu(trayMenu);
}

MainDialog::~MainDialog(void)
{
}

void MainDialog::closeEvent(QCloseEvent *event)
{
    this->setVisible(false);
    event->ignore();
}

void MainDialog::on_mCtrlBtn_clicked(void)
{
    mLogList->addItem("connect");
}

void MainDialog::on_mQuitBtn_clicked(void)
{
    qApp->exit(0);
}

void MainDialog::on_mTray_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::DoubleClick:
            this->setVisible(!this->isVisible());
            break;
        case QSystemTrayIcon::Unknown:
        case QSystemTrayIcon::Context:
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::MiddleClick:
            break;
    }
}

void MainDialog::on_toggleAction_triggered(void)
{
    this->setVisible(!this->isVisible());
}

void MainDialog::on_quitAction_triggered(void)
{
    qApp->exit(0);
}
