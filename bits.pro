#-------------------------------------------------
#
# Project created by QtCreator 2011-08-07T03:15:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bits
TEMPLATE = app

# INCLUDEPATH += c:/QtSDK/desktop/qt/4.7.4/msvc2008/include
# INCLUDEPATH += c:/Qt/Qt5.1.1/5.1.1/Src/qtbase/include

SOURCES += main.cpp \
    hexedit.cpp \
    bits.cpp \
    bitfield.cpp \
    BitButton.cpp \
    shiftop.cpp \
    formatgroup.cpp

HEADERS  += \
    controlgroup.h \
    bits.ui.h \
    bits.h \
    bitfield.h \
    BitButton.h \
    hexedit.h \
    shiftop.h \
    formatgroup.h

#FORMS    += \
#    bits.ui

debug {
    DESTDIR = $$PWD/debug
}

release {
    DESTDIR = $$PWD/release
}
