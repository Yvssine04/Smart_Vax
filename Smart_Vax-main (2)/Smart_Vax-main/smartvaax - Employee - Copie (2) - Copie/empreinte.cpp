#include "empreinte.h"







empreinte::empreinte(QObject *parent) : QObject(parent) {}



void empreinte::connectToArduino() {
    arduino = new QSerialPort(nullptr);

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        qDebug() << "Port name:" << serialPortInfo.portName();
    }

    arduino->setPortName("COM5");
    arduino->setBaudRate(QSerialPort::Baud9600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);

    if (arduino->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully.";
    } else {
        qDebug() << "Failed to open serial port:" << arduino->errorString();
    }

}

void empreinte::sendCommandToArduino(QString command) {
    if (arduino && arduino->isWritable()) {
        arduino->write(command.toStdString().c_str());
    } else {
        qDebug() << "Cannot write to Arduino.";
    }
}




empreinte::empreinte() {}
