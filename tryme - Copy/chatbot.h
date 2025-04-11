// chatbot.h
#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>

class ChatBot : public QObject {
    Q_OBJECT

public:
    explicit ChatBot(QObject *parent = nullptr);
    void sendMessageToChatbot(const QString &userMessage);
    void saveChatHistory(const QString &userMessage, const QString &botResponse);
    void loadChatHistory();
    void clearChatHistory();
    void handleResponse(QNetworkReply *reply, const QString &userMessage);

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &errorMessage);
    void historyLoaded(const QString &history);

private:
    QNetworkAccessManager *manager;
    QString historyFilePath;
};

#endif // CHATBOT_H
