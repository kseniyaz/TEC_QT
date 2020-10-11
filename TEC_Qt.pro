TEMPLATE = app
CONFIG += c++11 c++17
CONFIG -= app_bundle
#CONFIG -= qt
QT       += core gui printsupport charts

INCLUDEPATH += C:\Windows\local\boost_1_74_0\boost_mingw810_32\include\boost-1_74
LIBS += -LC:\Windows\local\boost_1_74_0\boost_mingw810_32\lib\
        -llibboost_date_time-mgw8-mt-x32-1_74

SOURCES += \
    dep/TEClib/GFileParser.cpp \
    dep/TEClib/IFileParser.cpp \
    dep/TEClib/NFileParser.cpp \
    dep/TEClib/OFileParser.cpp \
    dep/TEClib/Tec.cpp \
    dep/TEClib/dateTime.cpp \
    dep/TEClib/models.cpp \
    dep/TEClib/utils.cpp \
        main.cpp \
    mainwindow.cpp \
    opening_window.cpp \
    qcustomplot.cpp \
    worker.cpp


DISTFILES += \
    .gitignore \
    TEC_Qt.pro.user \
    dep/TEClib/.gitignore

HEADERS += \
    dep/TEClib/GFileParser.h \
    dep/TEClib/IFileParser.h \
    dep/TEClib/NFileParser.h \
    dep/TEClib/OFileParser.h \
    dep/TEClib/Tec.h \
    dep/TEClib/dateTime.h \
    dep/TEClib/models.h \
    dep/TEClib/utils.h \
    mainwindow.h \
    opening_window.h \
    qcustomplot.h \
    worker.h

FORMS += \
    mainwindow.ui \
    opening_window.ui
