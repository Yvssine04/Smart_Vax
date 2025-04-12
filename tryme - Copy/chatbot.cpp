#include "chatbot.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>

ChatBot::ChatBot(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);

    // Set up history file path
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    historyFilePath = documentsPath + "/chatbot_history.txt";
}

void ChatBot::saveChatHistory(const QString &userMessage, const QString &botResponse) {
    QFile file(historyFilePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ")
            << "Utilisateur: " << userMessage << "\n"
            << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ")
            << "SmartChat: " << botResponse << "\n\n";
        file.close();
    }
}

void ChatBot::loadChatHistory() {
    QFile file(historyFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString history = in.readAll();
        emit historyLoaded(history);
        file.close();
    }
}

void ChatBot::clearChatHistory() {
    QFile file(historyFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        file.close();
    }
}

bool ChatBot::isMedicalQuestion(const QString &userMessage) {
    // Liste de mots-clés médicaux
    QStringList medicalKeywords = {
        "vaccin", "maladie", "santé", "médical", "médecin", "traitement",
        "symptôme", "diagnostic", "médicament", "hôpital", "grippe", "virus",
        "bactérie", "infection", "inflammation", "cancer", "diabète", "asthme",
        "cardiaque", "neurologique", "dermatologique", "gastro-entérologie"
    };

    // Vérifier si le message contient l'un des mots-clés
    for (const QString &keyword : medicalKeywords) {
        if (userMessage.contains(keyword, Qt::CaseInsensitive)) {
            return true;
        }
    }
    return false;
}

void ChatBot::sendMessageToChatbot(const QString &userMessage) {
    // Check for greeting
    if (userMessage.trimmed().toLower() == "bonjour") {
        QString botResponse = "Bonjour ! Comment puis-je vous aider aujourd'hui ?";
        saveChatHistory(userMessage, botResponse);
        emit responseReceived(botResponse);
        return;
    }

    if (!isMedicalQuestion(userMessage)) {
        emit errorOccurred("Désolé, je ne peux répondre qu'aux questions médicales.");
        return;
    }

    QString apiKey = "dc602c131amsh31b9145a0457e53p10d544jsnfe36726f607d";
    QString endpoint = "https://chatgpt-api8.p.rapidapi.com/";

    QNetworkRequest request;
    request.setUrl(QUrl(endpoint));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("X-RapidAPI-Key", apiKey.toUtf8());
    request.setRawHeader("X-RapidAPI-Host", "chatgpt-api8.p.rapidapi.com");

    QJsonArray messagesArray;
    QJsonObject userMessageObject;
    userMessageObject["content"] = userMessage;
    userMessageObject["role"] = "user";
    messagesArray.append(userMessageObject);
    QJsonDocument doc(messagesArray);
    QByteArray postData = doc.toJson();

    QNetworkReply *reply = manager->post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [this, reply, userMessage]() {
        handleResponse(reply, userMessage);
    });
}

void ChatBot::handleResponse(QNetworkReply *reply, const QString &userMessage) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject responseObject = jsonResponse.object();

        if (responseObject.contains("text")) {
            QString botResponse = responseObject["text"].toString();
            saveChatHistory(userMessage, botResponse);
            emit responseReceived(botResponse);
        } else {
            emit errorOccurred("Invalid response format, missing 'text' field");
        }
    } else {
        emit errorOccurred("Error: " + reply->errorString());
    }
    reply->deleteLater();
}
