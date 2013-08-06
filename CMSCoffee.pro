#-------------------------------------------------
#
# Project created by QtCreator 2013-08-01T13:43:42
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++0x

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CMSCoffee
TEMPLATE = app


SOURCES += main.cpp\
        cmscoffeemainwindow.cpp \
    cmscoffeeusermodel.cpp \
    cmscoffeeuser.cpp \
    cmscoffeetickentry.cpp \
    cmscoffeetickmodel.cpp \
    cmscoffeeaccountentry.cpp \
    cmscoffeeaccountmodel.cpp \
    cmscoffeeusertableview.cpp \
    cmscoffeeactiveusertableview.cpp \
    cmscoffeeaccounttableview.cpp \
    cmscoffeeaccountentrywidget.cpp \
    cmscoffeeaccountentrycompletionmodel.cpp \
    cmscoffeetableviewdelegates.cpp

HEADERS  += cmscoffeemainwindow.h \
    cmscoffeeusermodel.h \
    cmscoffeeuser.h \
    cmscoffeetickentry.h \
    cmscoffeetickmodel.h \
    cmscoffeeaccountentry.h \
    cmscoffeeaccountmodel.h \
    cmscoffeeusertableview.h \
    cmscoffeeactiveusertableview.h \
    cmscoffeeaccounttableview.h \
    cmscoffeeaccountentrywidget.h \
    cmscoffeeaccountentrycompletionmodel.h \
    cmscoffeetableviewdelegates.h
