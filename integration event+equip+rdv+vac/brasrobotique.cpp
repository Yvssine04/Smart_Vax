#include "brasrobotique.h"
#include <QDebug>

BrasRobotique::BrasRobotique(QObject *parent)
    : QObject(parent), serial(new QSerialPort(this)) {}

BrasRobotique::~BrasRobotique() {
    if (serial->isOpen()) {
        serial->close();
    }
}

bool BrasRobotique::initSerialPort(const QString &portName, qint32 baudRate) {
    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully.";
        connect(serial, &QSerialPort::readyRead, this, &BrasRobotique::readSerialData);
        return true;
    } else {
        qDebug() << "Failed to open serial port:" << serial->errorString();
        return false;
    }
}

void BrasRobotique::sendCommand(const QString &command) {
    if (!serial->isOpen()) {
        qDebug() << "Serial port is not open.";
        return;
    }

    if (command.isEmpty()) {
        qDebug() << "Cannot send empty command.";
        return;
    }

    serial->write(command.toLocal8Bit());
    qDebug() << "Sending command:" << command;
}

void BrasRobotique::readSerialData()
{
    static QByteArray buffer;

    if (serial->bytesAvailable()) {
        buffer.append(serial->readAll());

        // Check if we received a full line (ending with "\r\n")
        while (buffer.contains("\r\n")) {
            int endIndex = buffer.indexOf("\r\n");
            QByteArray line = buffer.left(endIndex);
            buffer.remove(0, endIndex + 2); // Remove the processed line + "\r\n"

            qDebug() << "Full line received:" << line;
            emit serialDataReceived(line);
        }
    }
}
