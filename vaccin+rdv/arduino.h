// arduino.h
#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    bool initSerialPort(const QString &portName, qint32 baudRate);
    void sendCommand(const QString &command);
    bool isSerialOpen() const {
        return serial && serial->isOpen();
    }

signals:
    void serialDataReceived(const QByteArray &data);

private slots:
    void readSerialData();

private:
    QSerialPort *serial;
};

#endif // ARDUINO_H
