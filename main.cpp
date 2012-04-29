#include <QApplication>

#include <libssh/callbacks.h>

#include "constants.hpp"
#include "MainDialog.hpp"

int main(int argc, char *argv[])
{
    ssh_threads_set_callbacks(ssh_threads_get_pthread());
    ssh_init();

    qRegisterMetaType<SshStatus>("SshStatus");
    qRegisterMetaType<LogLevel>("LogLevel");

    QApplication app(argc, argv);
    MainDialog dlg;
    dlg.show();
    return app.exec();
}
