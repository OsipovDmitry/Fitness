TEMPLATE = app
QT += widgets sql network

CONFIG += c++14
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ../common/include


SOURCES += \
    src/main.cpp \
    src/database.cpp \
    src/servercontroller.cpp \
    ../common/src/messages.cpp

HEADERS += \
    src/database.h \
    src/servercontroller.h \
    ../common/include/messages.h

RESOURCES += \
    testdata.qrc
