#include <assert.h>

#include <libssh/callbacks.h>
#include <libssh/libssh.h>

#include "SshThread.hpp"

SshThread::SshThread(const SshConfig *config, QObject *parent)
    : QThread(parent)
    , mToStop(false)
    , mConfig(config)
{
}

SshThread::~SshThread(void)
{
}

void SshThread::stop(void)
{
    mToStop = true;
}

void SshThread::logLibssh(int /*priority*/, const char *message)
{
    emit logging(LogInfo, message);
}

extern "C"
void libssh_log_cb(ssh_session /*session*/, int priority,
                   const char *message, void *userdata)
{
    SshThread *thread = (SshThread *) userdata;
    thread->logLibssh(priority, message);
}

void SshThread::run(void)
{
    ssh_session session = ssh_new();
    if (!session)
    {
        emit logging(LogError, "ssh_new()");
        goto done;
    }

    struct ssh_callbacks_struct cb;
    cb.userdata = this;
    cb.log_function = libssh_log_cb;
    ssh_callbacks_init(&cb);
    ssh_set_callbacks(session, &cb);

    {
        int verbosity = SSH_LOG_RARE;
        ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    }
    ssh_options_set(session, SSH_OPTIONS_HOST, mConfig->host.toUtf8().constData());
    ssh_options_set(session, SSH_OPTIONS_PORT, &mConfig->port);

    while (!mToStop)
    {
        int ret;

        emit sshStatusChanged(StatConnecting);
        emit logging(LogInfo, "Connecting to SSH server ...");

        ret = ssh_connect(session);
        if (ret != SSH_OK)
        {
            emit logging(LogError, "Failed to connect to SSH server.");
            goto restart;
        }

#if 1
        ret = ssh_userauth_password(session, 0, mConfig->pswd.toUtf8().constData());
        if (ret != SSH_AUTH_SUCCESS)
        {
            emit logging(LogError, "Failed to authenticate.");
            goto restart;
        }
#endif

restart:
        ssh_disconnect(session);

        emit sshStatusChanged(StatSleeping);
        emit logging(LogInfo, QString("Wait %1 seconds to reconnect ...").arg(mConfig->wait));
        for (int i = 0; i < mConfig->wait; ++i)
        {
            if (mToStop)
                goto done;
            sleep(1);
        }
    }

done:
    ssh_free(session);

    mToStop = false;
}
