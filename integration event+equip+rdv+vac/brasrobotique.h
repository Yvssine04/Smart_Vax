#ifndef BRASROBOTIQUE_H
#define BRASROBOTIQUE_H

#include <QObject>
#include <QSerialPort>

class BrasRobotique : public QObject
{
    Q_OBJECT

public:
    explicit BrasRobotique(QObject *parent = nullptr);
    ~BrasRobotique();

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

#endif // BRASROBOTIQUE_H
