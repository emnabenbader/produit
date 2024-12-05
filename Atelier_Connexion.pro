
QT       += core gui sql widgets
QT += serialport

TARGET = Atelier_Connexion
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated.
DEFINES += QT_DEPRECATED_WARNINGS

# Enable C++11 standard
CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        connection.cpp \
        produit.cpp

HEADERS += \
        StatistiquesGraphWidget.h \
        mainwindow.h \
        connection.h \
        produit.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
