#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T18:49:44
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

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
    qcustomplot/qcustomplot.cpp \
    formbankbalance.cpp \
    formpayments.cpp \
    dialogtest.cpp \
    testitemdelegate.cpp \
    currencydelegate.cpp

HEADERS  += mainwindow.h \
    formperson.h \
    removerowfrommodel.h \
    formstatement.h \
    datedelegate.h \
    formfincancialplanner.h \
    formmanagecontacts.h \
    qcustomplot/qcustomplot.h \
    formbankbalance.h \
    formpayments.h \
    dialogtest.h \
    testitemdelegate.h \
    currencydelegate.h

FORMS    += mainwindow.ui \
    formperson.ui \
    formstatement.ui \
    formfincancialplanner.ui \
    formmanagecontacts.ui \
    formbankbalance.ui \
    formpayments.ui \
    dialogtest.ui

OTHER_FILES += \
    ../Database/anagrafica.db
