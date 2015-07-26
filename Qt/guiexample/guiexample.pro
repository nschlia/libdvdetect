#-------------------------------------------------
#
# Project created by QtCreator 2013-05-15T21:02:58
#
#-------------------------------------------------

# Target: 32 or 64 bit (Windoze default is 32 bit; *nix default is 64 bit)
!32bit:!64bit {
    win32 {
        message( "Setting default output to 32bit" )
        CONFIG += "32bit"
    }
    else {
        message( "Setting default output to 64bit" )
        CONFIG += "64bit"
    }
}

32bit {
    MODE = 32-bit
    BITCOUNT=32
}
64bit {
    MODE = 64-bit
    BITCOUNT=64
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = guiexample
TEMPLATE = app

SOURCES += main.cpp \
    aboutdlg.cpp \
    dvdexplorerdlg.cpp \
    searchtextdlg.cpp \ 
    explorerutils.cpp \
    editdetailsdlg.cpp \
    editoptionsdlg.cpp \
    openfromwebdlg.cpp \
    dvdlanguage.cpp
    
INCLUDEPATH += \
    ../.. \
    ../../lib \
    ../../include \
    ../../include/dvdetect \
    ../../$${MODE}/ \
    ../../$${MODE}/include \
    ../../$${MODE}/include/dvdetect

HEADERS  += \
    aboutdlg.h \
    dvdexplorerdlg.h \
    searchtextdlg.h \ 
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
    explorerutils.h \
    editdetailsdlg.h \
    editoptionsdlg.h \
    openfromwebdlg.h \
    dvdlanguage.h

DEFINES += HAVE_CONFIG_H
win32:DEFINES -= UNICODE

CONFIG(debug, debug|release) {
DEFINES += DEBUG
}
else {
DEFINES += NDEBUG
}

win32:LIBS += -lws2_32

LIBS += -L"$$PWD/../../$${MODE}/lib/.libs/" -L"$$PWD/../../lib/.libs/" -ldvdetect

FORMS    += \
    dvdexplorerdlg.ui \
    searchtextdlg.ui \
    aboutdlg.ui \
    editdetailsdlg.ui \
    editoptionsdlg.ui \
    openfromwebdlg.ui

# SPÄTER WEG!
DEFINES += BUILDING_STATIC

