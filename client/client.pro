TEMPLATE = app
QT += widgets network

CONFIG += c++14
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += ../common/include

SOURCES += \
    src/main.cpp \
    src/clientcontroller.cpp \
    src/connecttohostwidget.cpp \
    src/clientmainwindow.cpp \
    src/loginwidget.cpp \
    ../common/src/messages.cpp \
    src/forgotpasswordwidget.cpp \
    src/accountwidget.cpp \
    src/createnewaccountwidget.cpp

HEADERS += \
    src/clientcontroller.h \
    src/connecttohostwidget.h \
    src/clientmainwindow.h \
    src/loginwidget.h \
    ../common/include/messages.h \
    src/forgotpasswordwidget.h \
    src/accountwidget.h \
    src/createnewaccountwidget.h

FORMS += \
    src/connecttohostwidget.ui \
    src/clientmainwindow.ui \
    src/loginwidget.ui \
    src/forgotpasswordwidget.ui \
    src/accountwidget.ui \
    src/createnewaccountwidget.ui
