#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <QObject>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateTimeEdit>
#include <QList>
#include <QString>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QString>

// Structure pour stocker les données d'un événement
struct EvenementData {
    int id;
    QString nom;
    QString lieu;
    QString resp;
    QString date_heure;
    double latitude;
    double longitude;
};

class Evenement : public QObject
{
    Q_OBJECT
public:
    explicit Evenement(QObject *parent = nullptr);

    // Charger les événements dans un QTableWidget
    void loadEventData(QTableWidget *tabevent);

    // Ajouter un nouvel événement dans la base de données
    void saveEventData(int id_event, const QString &nom_event, const QString &nom_responsable,
                       const QString &type, const QString &date_heure,
                       const QString &lieu, int nbr_participant);

    void supprimerEvenement(int id_event);
    bool recupererEvenement(int id, QString &nom, QString &responsable, QString &type, QString &lieu, int &participants, QDateTime &dateHeure);
    bool modifierEvenement(int id, const QString &nom, const QString &responsable, const QString &type, const QString &lieu, int participants, const QDateTime &dateHeure);
    void searchEventData(QTableWidget *tabevent, const QString &searchTerm);
    void sortEventData(QTableWidget *tabevent, const QString &critere);
    void exporterInvitationPDF(QTableWidget *tabevent);
    QList<EvenementData> chargerEvenements();
    QMap<QString, int> compterEvenementsParType();
    QMap<QString, int> repartitionParRegion();
    void sendEmail2(const QString &eventId, const QString &eventName, const QString &eventType,
                    const QString &eventDateTime, const QString &eventLocation,
                    const QString &participantCount, const QString &responsibleName);

private:
    QString generateEmailSuppresionBody(const QString &prompt);
};

#endif // EVENEMENT_H
