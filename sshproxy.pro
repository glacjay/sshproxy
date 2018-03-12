TEMPLATE = app
TARGET = sshproxy
DEPENDPATH += .
INCLUDEPATH += .
QT += widgets

HEADERS += \
    constants.hpp \
    MainDialog.hpp \
    Indicator.hpp \

SOURCES += \
    main.cpp \
    MainDialog.cpp \
    Indicator.cpp \

RESOURCES += \
    sshproxy.qrc \
