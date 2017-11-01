include(gtest_dependency.pri)

QT += core

INCLUDEPATH += C:\Users\mdzie\Desktop\Qt\PasswordManager\src\include C:\Users\mdzie\Desktop\Qt\PasswordManager\src\Shared
LIBS += -LC:\Users\mdzie\Desktop\Qt\PasswordManager\lib\release-32\release -lShared

TEMPLATE = app
CONFIG += console c++11
#CONFIG -= app_bundle
CONFIG += thread

HEADERS +=     tst_passwordmanager.h \
    tst_accountmanager.h \
    tst_abstractfilemanagertest.h

SOURCES +=     main.cpp
