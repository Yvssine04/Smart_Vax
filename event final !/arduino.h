#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class Arduino : public QObject
{
    Q_OBJECT

public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    QString connectToPort(const QString &portName = "COM4", int baudRate = QSerialPort::Baud9600); // Return error message
    int readHeartRate();
    QStringList getAvailablePorts();
void sendAverageBPM(int average);
    void writeData(const QString &data);
   QString readSerialLine();
private:
    QSerialPort *serial;
};

#endif // ARDUINO_H
