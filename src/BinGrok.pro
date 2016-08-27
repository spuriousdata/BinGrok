#-------------------------------------------------
#
# Project created by QtCreator 2011-08-24T19:21:10
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = BinGrok
TEMPLATE = app

SOURCES += \
    bingrokwindow.cc \
    main.cc \
    hexwidget.cc \
    trtable.cc \
    preferences.cc \
    selection.cc \
    txtdisplaywidget.cc \
    structeditor.cc \
    syntaxhighlighter.cc \
    datahighlighter.cc \
    structtypes.cc \
    rd_parser.cc \
    datavisualizer.cc

HEADERS  += \
    bingrokwindow.h \
    hexwidget.h \
    trtable.h \
    preferences.h \
    selection.h \
    txtdisplaywidget.h \
    structeditor.h \
    syntaxhighlighter.h \
    datahighlighter.h \
    structtypes.h \
    rd_parser.h \
    datavisualizer.h

FORMS    += \
    bingrokwindow.ui \
    preferences.ui \
    structeditor.ui \
    datavisualizer.ui
