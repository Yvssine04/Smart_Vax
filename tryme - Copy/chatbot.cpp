#include "chatbot.h"

ChatBot::ChatBot(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
}

void ChatBot::sendMessageToChatbot(const QString &userMessage) {
    QString apiKey = "fe4429ae05mshe9b08fc10a6eaefp1e30c9jsn26fb0b6e7731"; // Replace with your actual API key
    QString endpoint = "https://chatgpt-api8.p.rapidapi.com/"; // Replace with the actual endpoint

    QNetworkRequest request;
    request.setUrl(QUrl(endpoint));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-RapidAPI-Key", apiKey.toUtf8());
    request.setRawHeader("X-RapidAPI-Host", "chatgpt-api8.p.rapidapi.com"); // Adjust if necessary

    QJsonArray messagesArray;
    QJsonObject userMessageObject;
    userMessageObject["content"] = userMessage;
    userMessageObject["role"] = "user";
    messagesArray.append(userMessageObject);
    QJsonDocument doc(messagesArray);
    QByteArray postData = doc.toJson();

    QNetworkReply *reply = manager->post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleResponse(reply);
    });
}

void ChatBot::handleResponse(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();

        if (responseObject.contains("text")) {
            QString botResponse = responseObject["text"].toString();
            emit responseReceived(botResponse);
        } else {
            emit errorOccurred("Invalid response format, missing 'text' field");
        }
    } else {
        emit errorOccurred("Error: " + reply->errorString());
    }
    reply->deleteLater();
}
