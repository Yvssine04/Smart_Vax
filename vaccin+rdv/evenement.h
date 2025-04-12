#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <QObject>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateTimeEdit>

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

};

#endif // EVENEMENT_H
