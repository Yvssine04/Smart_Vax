#include "NewsFetcher.h"

NewsFetcher::NewsFetcher(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NewsFetcher::onNewsFetched);

    // Fetch news every hour
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &NewsFetcher::fetchNews);
    timer->start(3600000); // 1 hour in milliseconds

    // Initial fetch
    fetchNews();
}

void NewsFetcher::fetchNews() {
    QString apiKey = "2df7707941d4407cb99b1c8e607e6b59"; // Replace with your actual NewsAPI key
    QString url = QString("https://newsapi.org/v2/everything?q=nouvelle+maladie&apiKey=%1").arg(apiKey);

    QNetworkRequest request(url);
    manager->get(request);
}

void NewsFetcher::onNewsFetched(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray articles = jsonObj["articles"].toArray();

        foreach (const QJsonValue &value, articles) {
            QJsonObject article = value.toObject();
            QString title = article["title"].toString();
            QString date = article["publishedAt"].toString();

            // Check for keywords related to new sicknesses
            if (title.contains("nouvelle maladie", Qt::CaseInsensitive)) {
                emit newSicknessDetected(title, date);
            }
        }
    } else {
        qDebug() << "Erreur lors de la récupération des nouvelles:" << reply->errorString();
    }
    reply->deleteLater();
}
