#include "arduino.h"
#include <QDebug>

Arduino::Arduino(QObject *parent) : QObject(parent), serial(new QSerialPort(this)) {}

Arduino::~Arduino() {
    if (serial->isOpen()) {
        serial->close();
    }
     delete serial;
}


QString Arduino::connectToPort(const QString &portName, int baudRate) {
    if (serial->isOpen()) {
        serial->close(); // Close port if already open
    }
    serial->setPortName(portName);
    serial->setBaudRate(baudRate);

    if (!serial->open(QIODevice::ReadWrite)) {
        QString error = serial->errorString();
        qDebug() << "Erreur ouverture port:" << error;
        return error;
    }

    qDebug() << "Connexion au port série réussie.";
    return "";
}


int Arduino::readHeartRate() {
    if (!serial->isOpen()) return -1;

    if (serial->waitForReadyRead(2000)) {
        QByteArray data = serial->readAll();
        while (serial->waitForReadyRead(100))
            data += serial->readAll();

        qDebug() << "Données reçues :" << data;

        QList<QByteArray> lines = data.split('\n');
        for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
            bool ok;
            int value = it->trimmed().toInt(&ok);
            if (ok && value > 0) return value;
        }
    }

    return -1;
}


void Arduino::sendAverageBPM(int average) {
    if (serial && serial->isOpen()) {
        QString command = QString("A%1\n").arg(average);
        serial->write(command.toUtf8());
        serial->flush();
    }
}

QStringList Arduino::getAvailablePorts() {
    QStringList ports;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ports << info.portName() + " - " + info.description();
    }
    return ports;
}
void Arduino::writeData(const QString &data) {
    if (serial && serial->isOpen()) {
        serial->write(data.toUtf8());
        serial->flush(); // Assure que les données partent
    }
}
QString Arduino::readSerialLine() {
    if (!serial->isOpen())
        return "";

    if (serial->waitForReadyRead(500)) {
        QByteArray data = serial->readAll();
        while (serial->waitForReadyRead(100)) {
            data += serial->readAll();
        }
        return QString::fromUtf8(data).trimmed();
    }
    return "";
}
