#ifndef SPEECHWORKER_H
#define SPEECHWORKER_H
#include <QObject>
#include <sapi.h>


class SpeechWorker : public QObject
{
    Q_OBJECT
public:
    explicit SpeechWorker(QObject *parent = nullptr);

public slots:
    void startRecognition();
    void stopRecognition();

signals:
    void recognized(QString text);
    void error(QString message);

private:
    bool m_running = false;
};

#endif // SPEECHWORKER_H
