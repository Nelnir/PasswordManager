#-------------------------------------------------
#
# Project created by QtCreator 2017-10-22T14:15:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PasswordManager
TEMPLATE = app
LIBS += -LC:\Users\mdzie\Desktop\Qt\PasswordManager\lib\release-32\release -lShared
INCLUDEPATH += C:\Users\mdzie\Desktop\Qt\PasswordManager\src\include C:\Users\mdzie\Desktop\Qt\PasswordManager\src\Shared

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_FILE = passmgr.rc

SOURCES += \
        main.cpp \
        passwordmanager.cpp \
    animatedqpushbutton.cpp \
    animatedqlineedit.cpp \
    accountwidget.cpp \
    accountwidgettitle.cpp \
    accountwidgetabstractlabel.cpp \
    accountwidgetlabel.cpp

HEADERS += \
        passwordmanager.h \
    animatedqpushbutton.h \
    animationmanager.h \
    animatedqlineedit.h \
    include/passwordmanagermodel.h \
    include/accountmanager.h \
    include/passwordmanagercontroller.h \
    accountwidget.h \
    accountwidgettitle.h \
    accountwidgetabstractlabel.h \
    accountwidgetlabel.h \
    include/simplecrypt.h

FORMS += \
        passwordmanager.ui

RESOURCES += \
    media/resources.qrc
