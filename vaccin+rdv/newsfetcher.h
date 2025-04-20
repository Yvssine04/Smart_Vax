#ifndef NEWSFETCHER_H
#define NEWSFETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QTimer>

class NewsFetcher : public QObject
{
    Q_OBJECT
public:
    explicit NewsFetcher(QObject *parent = nullptr);

signals:
    void newSicknessDetected(const QString &sicknessName,
                             const QString &date,
                             const QString &location,
                             const QString &vaccineInfo,
                             const QString &detail,
                             const QString &source);

public slots:
    void fetchNews();

private slots:
    void onNewsFetched(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QString extractLocationFromContent(const QString &content);
};

#endif // NEWSFETCHER_H
