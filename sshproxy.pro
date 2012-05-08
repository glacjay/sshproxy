TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

QT += network
LIBS += -lssh -lssh_threads

HEADERS += \
    MainDialog.hpp \

SOURCES += \
    main.cpp \
    MainDialog.cpp \

RESOURCES += \
    sshproxy.qrc \
