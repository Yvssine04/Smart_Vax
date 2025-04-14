#ifndef SPEECH_H
#define SPEECH_H

#include <QObject>
#include <sapi.h>  // Required for Speech API

class Speech : public QObject {
    Q_OBJECT

public:
    explicit Speech(QObject *parent = nullptr);
    void recognizeSpeech();

signals:
    void recognitionCompleted(QString text);
    void errorOccurred(QString error);

private:
    ISpRecognizer *pRecognizer = nullptr;
    ISpRecoContext *pContext = nullptr;
};

#endif // SPEECH_H
