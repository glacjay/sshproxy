#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QApplication>

#include "constants.hpp"
#include "MainDialog.hpp"

int main(int argc, char *argv[])
{
    char *passwd = getenv("GLACJAY_SSHPROXY_PASSWORD");
    if (passwd && strlen(passwd))
    {
        printf("%s\n", passwd);
        exit(0);
    }

    QApplication app(argc, argv);
    MainDialog dlg;
    dlg.show();
    return app.exec();
}
