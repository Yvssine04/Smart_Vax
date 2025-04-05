#ifndef NEWSFETCHER_H
#define NEWSFETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class NewsFetcher : public QObject {
    Q_OBJECT

public:
    NewsFetcher(QObject *parent = nullptr);

private slots:
    void fetchNews();
    void onNewsFetched(QNetworkReply *reply);

signals:
    void newSicknessDetected(const QString &sicknessName, const QString &date);

private:
    QNetworkAccessManager *manager;
};

#endif // NEWSFETCHER_H
