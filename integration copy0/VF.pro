QT       += sql
QT += printsupport
QT += charts

QT += core gui location qml quickwidgets network
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    equipements.cpp \
    evenement.cpp \
    main.cpp \
    mainwindow.cpp \
    rendez_vous.cpp \
    vaccin.cpp

HEADERS += \
    arduino.h \
    connection.h \
    equipements.h \
    evenement.h \
    mainwindow.h \
    rendez_vous.h \
    vaccin.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    projet_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc \
    resources.qrc

DISTFILES += \
    ../projet/save.png \
    216658.png \
    7398464.png \
    icon.png \
    icon2.png \
    icon4.png \
    icon5.png \
    icon6.png \
    icon7.png \
    icon8.png \
    logo.png \
    mail.png \
    pin.png \
    pin2.png \
    rate.png \
    signature.png \
    stat.png
