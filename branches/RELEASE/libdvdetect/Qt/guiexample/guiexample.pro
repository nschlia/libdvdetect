#-------------------------------------------------
#
# Project created by QtCreator 2013-05-15T21:02:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = guiexample
TEMPLATE = app

SOURCES += main.cpp \
    aboutdlg.cpp \
    dvdexplorerdlg.cpp \
    searchtextdlg.cpp \ 
    explorerutils.cpp \
    editdetailsdlg.cpp
INCLUDEPATH += \
    ../.. \
    ../../lib \
    ../../include \
    ../../include/dvdetect

HEADERS  += \
    ../../config.h \
    ../../include/dvdetect/dvdcell.h \
    ../../include/dvdetect/dvddatabase.h \
    ../../include/dvdetect/dvdetect.h \
    ../../include/dvdetect/dvdetectbase.h \
    ../../include/dvdetect/dvdetectc++.h \
    ../../include/dvdetect/dvdetectdll.h \
    ../../include/dvdetect/dvdfile.h \
    ../../include/dvdetect/dvdparse.h \
    ../../include/dvdetect/dvdpgc.h \
    ../../include/dvdetect/dvdprogram.h \
    ../../include/dvdetect/dvdpttvmg.h \
    ../../include/dvdetect/dvdpttvts.h \
    ../../include/dvdetect/dvdtitle.h \
    ../../include/dvdetect/dvdunit.h \
    ../../include/dvdetect/dvdutils.h \
    ../../include/dvdetect/types.h \
    ../../include/dvdetect/version.h \
    aboutdlg.h \
    dvdexplorerdlg.h \
    searchtextdlg.h \ 
    explorerutils.h \
    editdetailsdlg.h

DEFINES += HAVE_CONFIG_H
win32:DEFINES -= UNICODE

CONFIG(debug, debug|release) {
DEFINES += DEBUG
}
else {
DEFINES += NDEBUG
}

win32:LIBS += -lws2_32

LIBS += -L"$$PWD/../../lib/.libs/" -ldvdetect

FORMS    += \
    dvdexplorerdlg.ui \
    searchtextdlg.ui \
    aboutdlg.ui \
    editdetailsdlg.ui

# SPÄTER WEG!
DEFINES += BUILDING_STATIC