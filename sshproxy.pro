TEMPLATE = app
TARGET = sshproxy
DEPENDPATH += .
INCLUDEPATH += .
QT += widgets

HEADERS += \
    MainDialog.hpp \

SOURCES += \
    main.cpp \
    MainDialog.cpp \

RESOURCES += \
    sshproxy.qrc \
