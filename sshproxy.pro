TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

LIBS += -lssh -lssh_threads

HEADERS += \
    MainDialog.hpp \
    SshThread.hpp \

SOURCES += \
    main.cpp \
    MainDialog.cpp \
    SshThread.cpp \

RESOURCES += \
    sshproxy.qrc \
