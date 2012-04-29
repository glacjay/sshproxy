#include <QApplication>

#include "MainDialog.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainDialog dlg;
    dlg.show();
    return app.exec();
}
