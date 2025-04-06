#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class ChatBot : public QObject {
    Q_OBJECT

public:
    explicit ChatBot(QObject *parent = nullptr);
    void sendMessageToChatbot(const QString &userMessage);

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &errorMessage);

private:
    QNetworkAccessManager *manager;

private slots:
    void handleResponse(QNetworkReply *reply);
};

#endif // CHATBOT_H
