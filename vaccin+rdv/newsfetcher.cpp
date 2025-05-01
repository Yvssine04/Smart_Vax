#include "NewsFetcher.h"
#include "qdir.h"
#include "qstandardpaths.h"
#include <QRegularExpression>
#include <QDateTime>

NewsFetcher::NewsFetcher(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NewsFetcher::onNewsFetched);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NewsFetcher::fetchNews);
    timer->start(3600000); // 1 hour
    fetchNews();
}

void NewsFetcher::fetchNews() {
    QString apiKey = "62da6ce7cded4bdfa7d1e23db76dd4cc";
    QString url = QString("https://newsapi.org/v2/everything?q=nouvelle+maladie&apiKey=%1").arg(apiKey);
    QNetworkRequest request(url);
    manager->get(request);
}

void NewsFetcher::onNewsFetched(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "Response:" << response;

        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonArray articles = jsonDoc.object()["articles"].toArray();

        QDate currentDate = QDate::currentDate();
        QDate weekAgo = currentDate.addMonths(-1);



        QStringList keywords = {"vaccin ", "nouvelle maladie", "nouvelle pandémie", "nouveau vaccin "};

        foreach (const QJsonValue &value, articles) {
            QJsonObject article = value.toObject();
            QString title = article["title"].toString();
            QString dateStr = article["publishedAt"].toString();
            QString content = article["content"].toString();
            QString source = article["source"].toObject()["name"].toString();

            QDateTime dateTime = QDateTime::fromString(dateStr, Qt::ISODate);
            QString formattedDate = dateTime.date().toString("yyyy-MM-dd");
            QString location = extractLocationFromContent(content);
            QString diseaseName = title;

            qDebug() << "Article Title:" << title;
            qDebug() << "Article Date:" << formattedDate;


            if (dateTime.date() >= weekAgo) {
                bool containsKeyword = false;
                foreach (const QString &keyword, keywords) {
                    if (title.contains(keyword, Qt::CaseInsensitive) ||
                        content.contains(keyword, Qt::CaseInsensitive)) {
                        containsKeyword = true;
                        break;
                    }
                }

                if (containsKeyword) {
                    int colonIndex = diseaseName.indexOf(":");
                    if (colonIndex != -1) {
                        diseaseName = diseaseName.left(colonIndex).trimmed();
                    }
                    diseaseName = diseaseName.replace("Nouvelle maladie détectée :", "").trimmed();

                    QString vaccineInfo = "Non disponible";
                    QRegularExpression vaccineRegex(
                        "\\b(vaccin|vaccine|vaccination)\\s+(?:contre|pour|against)?\\s*"
                        "([A-ZÀ-ÖØ-ß][a-zà-öø-ÿ]+(?:\\s+[A-ZÀ-ÖØ-ß][a-zà-öø-ÿ]+)*)\\b",
                        QRegularExpression::CaseInsensitiveOption);

                    QRegularExpressionMatch vaccineMatch = vaccineRegex.match(content);
                    if (vaccineMatch.hasMatch()) {
                        vaccineInfo = QString("Vaccin disponible: %1").arg(vaccineMatch.captured(2).trimmed());
                    } else if (content.contains("vaccin", Qt::CaseInsensitive) ||
                               content.contains("vaccine", Qt::CaseInsensitive)) {
                        vaccineInfo = "Vaccin disponible (non spécifié)";
                    }

                    QString detail = QString("%1\nSource: %2\n%3")
                                         .arg(title)
                                         .arg(source)
                                         .arg(content.left(200));

                    emit newSicknessDetected(diseaseName, formattedDate, location, vaccineInfo, detail, source);
                }
            }
        }

    } else {
        qDebug() << "Erreur lors de la récupération des nouvelles:" << reply->errorString();
    }

    reply->deleteLater();
}


QString NewsFetcher::extractLocationFromContent(const QString &content) {
    QRegularExpression locationRegex(
        "\\b(?:in|at|from|of|à|au|en|dans)\\s+"
        "([A-ZÀ-ÖØ-ß][a-zà-öø-ÿ]+(?:\\s+[A-ZÀ-ÖØ-ß][a-zà-öø-ÿ]+)*)\\b",
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatch match = locationRegex.match(content);
    return match.hasMatch() ? match.captured(1).trimmed() : "Inconnu";
}
