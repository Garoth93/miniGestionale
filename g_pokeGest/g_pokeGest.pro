QT       += core gui

QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QT += network

win32:RC_ICONS += snorlax.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    anagraficaArticoli/screenmainartica.cpp \
    backEndUtility/databaseconnection.cpp \
    backEndUtility/dialogdatidatabase.cpp \
    backEndUtility/dialogerrori.cpp \
    backEndUtility/tabellaarticoli.cpp \
    backEndUtility/tabellabase.cpp \
    backEndUtility/tabelladeposito.cpp \
    backEndUtility/tabellarigadocumento.cpp \
    backEndUtility/tabellatabvab.cpp \
    backEndUtility/tabellatestatadocumento.cpp \
    documenti/screendettagliodocumenti.cpp \
    documenti/screenesploradocumenti.cpp \
    magazzino/screenmaindeposito.cpp \
    main.cpp \
    mainwindow.cpp \
    widgetsUtility/dialogcercaarticolo.cpp \
    widgetsUtility/dialogcercatipodoc.cpp \
    widgetsUtility/dialoginfoalert.cpp \
    widgetsUtility/dialogseparatore.cpp \
    widgetsUtility/screenbase.cpp \
    widgetsUtility/screentabvab.cpp

HEADERS += \
    anagraficaArticoli/screenmainartica.h \
    backEndUtility/databaseconnection.h \
    backEndUtility/dialogdatidatabase.h \
    backEndUtility/dialogerrori.h \
    backEndUtility/tabellaarticoli.h \
    backEndUtility/tabellabase.h \
    backEndUtility/tabelladeposito.h \
    backEndUtility/tabellarigadocumento.h \
    backEndUtility/tabellatabvab.h \
    backEndUtility/tabellatestatadocumento.h \
    documenti/screendettagliodocumenti.h \
    documenti/screenesploradocumenti.h \
    magazzino/screenmaindeposito.h \
    mainwindow.h \
    widgetsUtility/dialogcercaarticolo.h \
    widgetsUtility/dialogcercatipodoc.h \
    widgetsUtility/dialoginfoalert.h \
    widgetsUtility/dialogseparatore.h \
    widgetsUtility/screenbase.h \
    widgetsUtility/screentabvab.h

FORMS += \
    anagraficaArticoli/screenmainartica.ui \
    backEndUtility/dialogdatidatabase.ui \
    backEndUtility/dialogerrori.ui \
    documenti/screendettagliodocumenti.ui \
    documenti/screenesploradocumenti.ui \
    magazzino/screenmaindeposito.ui \
    mainwindow.ui \
    widgetsUtility/dialogcercaarticolo.ui \
    widgetsUtility/dialogcercatipodoc.ui \
    widgetsUtility/dialoginfoalert.ui \
    widgetsUtility/dialogseparatore.ui \
    widgetsUtility/screentabvab.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Risorse/iconeImmagini.qrc \
    icone.qrc

DISTFILES +=
