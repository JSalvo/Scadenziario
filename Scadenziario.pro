#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T18:49:44
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scadenziario
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    formperson.cpp \
    removerowfrommodel.cpp \
    formstatement.cpp \
    datedelegate.cpp \
    formfincancialplanner.cpp \
    formmanagecontacts.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h \
    formperson.h \
    removerowfrommodel.h \
    formstatement.h \
    datedelegate.h \
    formfincancialplanner.h \
    formmanagecontacts.h \
    qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui \
    formperson.ui \
    formstatement.ui \
    formfincancialplanner.ui \
    formmanagecontacts.ui

OTHER_FILES += \
    ../Database/anagrafica.db
