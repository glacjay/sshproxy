#include <assert.h>

#include <QApplication>
#include <QAction>
#include <QDateTime>
#include <QGroupBox>
#include <QLabel>
#include <QMenu>
#include <QSpacerItem>
#include <QSplitter>
#include <QVBoxLayout>

#include "MainDialog.hpp"

MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , mHostEdit(new QLineEdit)
    , mPortEdit(new QLineEdit)
    , mUserEdit(new QLineEdit)
    , mPswdEdit(new QLineEdit)
    , mLAddrEdit(new QLineEdit)
    , mLPortEdit(new QLineEdit)
    , mWaitEdit(new QLineEdit)
    , mCtrlBtn(new QPushButton(tr("Start")))
    , mQuitBtn(new QPushButton(tr("Quit")))
    , mLogList(new QListWidget)
    , mInfoIcon(QPixmap(":/icon-info.png"))
    , mWarnIcon(QPixmap(":/icon-warn.png"))
    , mErrorIcon(QPixmap(":/icon-error.png"))
    , mStoppedIcon(QPixmap(":/icon-stopped.png"))
    , mConnectingIcon(QPixmap(":/icon-connecting.png"))
    , mConnectedIcon(QPixmap(":/icon-connected.png"))
    , mSleepingIcon(QPixmap(":/icon-sleeping.png"))
    , mSettings(new QSettings(QSettings::IniFormat, QSettings::UserScope,
                              "glacjay", "sshproxy", this))
    , mIsKeepRunning(false)
    , mProcess(new QProcess(this))
    , mRestartTimer(new QTimer(this))
{
    setWindowTitle(tr("SSH Proxy"));
    setWindowIcon(mConnectedIcon);

    mHostEdit->setText(mSettings->value("ssh/host").toString());
    mPortEdit->setText(mSettings->value("ssh/port", "22").toString());
    mUserEdit->setText(mSettings->value("ssh/user").toString());
    mLAddrEdit->setText(mSettings->value("ssh/laddr", "127.0.0.1").toString());
    mLPortEdit->setText(mSettings->value("ssh/lport", "1077").toString());
    mWaitEdit->setText(mSettings->value("ssh/wait", "10").toString());

    mPortEdit->setValidator(new QIntValidator(1, 65535));
    mPswdEdit->setEchoMode(QLineEdit::Password);
    mLPortEdit->setValidator(new QIntValidator(1, 65535));
    mWaitEdit->setValidator(new QIntValidator);
    mLogList->setWordWrap(true);

    mProcess->setProcessChannelMode(QProcess::MergedChannels);

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
    inputLayout->insertStretch(-1);
    inputLayout->addWidget(mCtrlBtn);
    inputLayout->addWidget(mQuitBtn);

    QGroupBox *inputGroup = new QGroupBox(tr("Settings"));
    inputGroup->setLayout(inputLayout);

    QVBoxLayout *logLayout = new QVBoxLayout;
    logLayout->addWidget(mLogList);

    QGroupBox *logGroup = new QGroupBox(tr("Log"));
    logGroup->setLayout(logLayout);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(inputGroup);
    splitter->addWidget(logGroup);
    splitter->setStretchFactor(1, 5);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(splitter);

    setLayout(mainLayout);
    resize(1000, 600);

    connect(mCtrlBtn, SIGNAL(clicked(void)), this, SLOT(on_mCtrlBtn_clicked(void)));
    connect(mQuitBtn, SIGNAL(clicked(void)), this, SLOT(onQuit(void)));

    mTray = new QSystemTrayIcon(mStoppedIcon);
    mTray->show();

    setSshStatus(StatStopped);

    connect(mTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(on_mTray_activated(QSystemTrayIcon::ActivationReason)));

    QAction *toggleAction = new QAction(tr("Toggle Main Dialog"), this);
    connect(toggleAction, SIGNAL(triggered(void)),
            this, SLOT(on_toggleAction_triggered(void)));

    QAction *quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered(void)),
            this, SLOT(onQuit(void)));

    QMenu *trayMenu = new QMenu;
    trayMenu->addAction(toggleAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);

    mTray->setContextMenu(trayMenu);

    connect(qApp, SIGNAL(aboutToQuit(void)), this, SLOT(onQuit(void)));

    connect(mProcess, SIGNAL(started(void)), this, SLOT(on_mProcess_started(void)));
    connect(mProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(on_mProcess_finished(int, QProcess::ExitStatus)));
    connect(mProcess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(on_mProcess_error(QProcess::ProcessError)));
    connect(mProcess, SIGNAL(readyReadStandardOutput(void)),
            this, SLOT(on_mProcess_readyReadStandardOutput(void)));

    connect(mRestartTimer, SIGNAL(timeout(void)), this, SLOT(startRunning(void)));

    if (mHostEdit->text().isEmpty())
        mHostEdit->setFocus();
    else if (mPortEdit->text().isEmpty())
        mPortEdit->setFocus();
    else if (mUserEdit->text().isEmpty())
        mUserEdit->setFocus();
    else if (mPswdEdit->text().isEmpty())
        mPswdEdit->setFocus();
    else if (mLAddrEdit->text().isEmpty())
        mLAddrEdit->setFocus();
    else if (mLPortEdit->text().isEmpty())
        mLPortEdit->setFocus();
    else if (mWaitEdit->text().isEmpty())
        mWaitEdit->setFocus();
}

MainDialog::~MainDialog(void)
{
    stopRunning();
}

void MainDialog::closeEvent(QCloseEvent *event)
{
    this->setVisible(false);
    event->ignore();
}

void MainDialog::on_mCtrlBtn_clicked(void)
{
    if (mCtrlBtn->text() == tr("Start"))
        startRunning();
    else if (mCtrlBtn->text() == tr("Stop"))
        stopRunning();
    else
        assert(false);
}

void MainDialog::onQuit(void)
{
    saveSettings();
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

void MainDialog::on_mProcess_started(void)
{
    addLogMsg(LogInfo, "connecting to the SSH server...");
    setSshStatus(StatConnecting);
}

void MainDialog::on_mProcess_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    addLogMsg(LogInfo, QString("process finished with code %1 and status %2")
              .arg(exitCode)
              .arg(exitStatus == QProcess::NormalExit ? "NormalExit" : "CrashExit"));
    setSshStatus(StatSleeping);

    mRestartTimer->start(1000 * mWaitEdit->text().toInt());
}

void MainDialog::on_mProcess_error(QProcess::ProcessError error)
{
    addLogMsg(LogWarn, QString("process error %1").arg(error));
    setSshStatus(StatSleeping);
    mProcess->kill();

    mRestartTimer->start(1000 * mWaitEdit->text().toInt());
}

void MainDialog::on_mProcess_readyReadStandardOutput(void)
{
    char line[1024];
    while (mProcess->readLine(line, sizeof(line)) > 0)
    {
        QString msg(line);
        addLogMsg(LogInfo, msg.remove('\n'));

        if (msg.contains("Entering interactive "))
            setSshStatus(StatConnected);
    }
}

void MainDialog::setSshStatus(SshStatus status)
{
    switch (status)
    {
        case StatStopped:
            qApp->setWindowIcon(mStoppedIcon);
            mTray->setIcon(mStoppedIcon);
            break;
        case StatConnecting:
            qApp->setWindowIcon(mConnectingIcon);
            mTray->setIcon(mConnectingIcon);
            break;
        case StatConnected:
            qApp->setWindowIcon(mConnectedIcon);
            mTray->setIcon(mConnectedIcon);
            break;
        case StatSleeping:
            qApp->setWindowIcon(mSleepingIcon);
            mTray->setIcon(mSleepingIcon);
            break;
        default:
            assert(false);
    }
}

void MainDialog::addLogMsg(LogLevel level, const QString &msg)
{
    QIcon icon;
    switch (level)
    {
        case LogDebug:
            /* No icon for debugging msg */
            break;
        case LogInfo:
            icon = mInfoIcon;
            break;
        case LogWarn:
            icon = mWarnIcon;
            break;
        case LogError:
            icon = mErrorIcon;
            break;
        default:
            assert(false);
    }

    while (mLogList->count() >= MaxTotalLog)
        mLogList->removeItemWidget(mLogList->item(0));

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    mLogList->addItem(new QListWidgetItem(icon, QString("%1 %2").arg(time).arg(msg)));
    mLogList->scrollToBottom();
}

void MainDialog::saveSettings(void)
{
    mSettings->setValue("ssh/host", mHostEdit->text());
    mSettings->setValue("ssh/port", mPortEdit->text());
    mSettings->setValue("ssh/user", mUserEdit->text());
    mSettings->setValue("ssh/laddr", mLAddrEdit->text());
    mSettings->setValue("ssh/lport", mLPortEdit->text());
    mSettings->setValue("ssh/wait", mWaitEdit->text());
}

void MainDialog::startRunning(void)
{
    mIsKeepRunning = true;
    mCtrlBtn->setText(tr("Stop"));

    if (mHostEdit->text().isEmpty())
    {
        addLogMsg(LogError, "SSH server's host name may not be empty!");
        mHostEdit->setFocus();
        return;
    }

    if (mPortEdit->text().toInt() == 0)
    {
        addLogMsg(LogWarn, "SSH server's port may not be empty. I assume you mean 22");
        mPortEdit->setText("22");
    }

    if (mUserEdit->text().isEmpty())
    {
        addLogMsg(LogError, "Username may not be empty!");
        mUserEdit->setFocus();
        return;
    }

    if (mPswdEdit->text().isEmpty())
    {
        addLogMsg(LogWarn, "Password should not be empty in general.");
        mPswdEdit->setFocus();
    }

    if (mLAddrEdit->text().isEmpty())
    {
        addLogMsg(LogWarn, "Local listen address is empty. I assume you mean 127.0.0.1");
        mLAddrEdit->setText("127.0.0.1");
    }

    if (mLPortEdit->text().toInt() == 0)
    {
        addLogMsg(LogError, "Local listen port may not be empty!");
        mLPortEdit->setText("1077");
        mLPortEdit->setFocus();
        return;
    }

    QStringList arguments;
    arguments << "-p" << mPortEdit->text();
    arguments << "-vN";
    arguments << "-D" << QString("%1:%2").arg(mLAddrEdit->text()).arg(mLPortEdit->text());
    arguments << "-o" << "StrictHostKeyChecking=no";
    arguments << "-o" << "PreferredAuthentications=password";
    arguments << "-o" << "ServerAliveInterval=20";
    arguments << "-l" << mUserEdit->text();
    arguments << mHostEdit->text();

    QStringList env;
    env << "DISPLAY=:1";
    env << QString("SSH_ASKPASS=%1").arg(QCoreApplication::applicationFilePath());
    env << QString("GLACJAY_SSHPROXY_PASSWORD=%1").arg(mPswdEdit->text());
    mProcess->setEnvironment(env);

    mProcess->start("/usr/bin/ssh", arguments);
}

void MainDialog::stopRunning(void)
{
    mIsKeepRunning = false;
    mCtrlBtn->setText(tr("Start"));

    if (mProcess->state() != QProcess::NotRunning)
        mProcess->kill();
}
