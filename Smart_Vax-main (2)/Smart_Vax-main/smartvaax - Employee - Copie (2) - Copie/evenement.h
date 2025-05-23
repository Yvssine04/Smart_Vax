#ifndef EVENEMENT_H
#define EVENEMENT_H

#include <QObject>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMessageBox>

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

    // Supprimer un événement sélectionné
    bool deleteEvent(int id_event);

    // Modifier un événement sélectionné
    bool updateEvent(int id_event, const QString &nom_event, const QString &nom_responsable,
                     const QString &type, const QString &date_heure,
                     const QString &lieu, int nbr_participant);
};

#endif // EVENEMENT_H
