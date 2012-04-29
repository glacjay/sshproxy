#ifndef HEADER__SSH_THREAD_HPP
#define HEADER__SSH_THREAD_HPP

#include <QString>
#include <QThread>

#include "constants.hpp"

struct SshConfig
{
    QString host;
    int port;
    QString user;
    QString pswd;
    QString laddr;
    int lport;
    int wait;
};

class SshThread : public QThread
{
    Q_OBJECT

public:
    SshThread(const SshConfig *config, QObject *parent=0);
    ~SshThread(void);

    void stop(void);

    /* for internal using only */
    void logLibssh(int priority, const char *message);

signals:
    void sshStatusChanged(SshStatus status);
    void logging(LogLevel level, const QString &msg);

protected:
    void run(void);

private:
    volatile bool mToStop;
    const SshConfig *mConfig;
};

#endif
