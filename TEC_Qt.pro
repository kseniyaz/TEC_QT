TEMPLATE = app
CONFIG += console c++11 c++17
CONFIG -= app_bundle
#CONFIG -= qt
QT       += core gui printsupport

QT += charts

INCLUDEPATH += C:\Windows\local\boost_1_74_0\boost_mingw810_32\include\boost-1_74
LIBS += -LC:\Windows\local\boost_1_74_0\boost_mingw810_32\lib\
        -llibboost_date_time-mgw8-mt-x32-1_74

SOURCES += \
    GFileParser.cpp \
    IFileParser.cpp \
    NFileParser.cpp \
    OFileParser.cpp \
    dateTime.cpp \
        main.cpp \
    mainwindow.cpp \
    models.cpp \
    opening_window.cpp \
    qcustomplot.cpp \
    utils.cpp \
    worker.cpp


DISTFILES += \
    .gitignore \
    TEC_Qt.pro.user

HEADERS += \
    GFileParser.h \
    IFileParser.h \
    NFileParser.h \
    OFileParser.h \
    Tec.h \
    dateTime.h \
    mainwindow.h \
    models.h \
    opening_window.h \
    qcustomplot.h \
    utils.h \
    worker.h

FORMS += \
    mainwindow.ui \
    opening_window.ui
