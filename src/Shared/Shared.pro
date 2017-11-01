#-------------------------------------------------
#
# Project created by QtCreator 2017-10-22T17:00:28
#
#-------------------------------------------------

QT       -= gui
QT       += core

INCLUDEPATH += C:\Users\mdzie\Desktop\Qt\PasswordManager\src\include

TARGET = Shared
TEMPLATE = lib

DEFINES += SHARED_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += accountmanager.cpp \
    passwordmanagermodel.cpp \
    passwordmanagercontroller.cpp \
    usermanager.cpp \
    copydata.cpp \
    simplecrypt.cpp

HEADERS += \
        shared_global.h \ 
    ../include/accountmanager.h \
    ../include/passwordmanagermodel.h \
    ../include/passwordmanagercontroller.h \
    ../include/usermanager.h \
    ../include/abstractfilemanager.h \
    ../include/copydata.h \
    ../include/simplecrypt.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
