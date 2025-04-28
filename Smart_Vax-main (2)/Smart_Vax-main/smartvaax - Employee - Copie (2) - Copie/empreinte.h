#ifndef EMPREINTE_H
#define EMPREINTE_H
#include "qtextedit.h"
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>

class empreinte:public QObject{
    Q_OBJECT

public:

public:
    empreinte();
    empreinte(QObject *parent = nullptr);

    QSerialPort *arduino;


    void connectToArduino();
    void sendCommandToArduino(QString command);

    bool isArduinoReadable() {
        return arduino && arduino->isReadable() && arduino->bytesAvailable() > 0;
    };

    QString readArduinoData() {
        if (isArduinoReadable()) {
            QByteArray data = arduino->readAll();
            return QString::fromUtf8(data);
        }
        return "";
    }



};

#endif // EMPREINTE_H
