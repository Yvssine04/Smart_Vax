#include "evenement.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>



Evenement::Evenement(QObject *parent) : QObject(parent) {}

void Evenement::loadEventData(QTableWidget *tabevent) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    tabevent->setRowCount(0);
    int row = 0;
    while (query.next()) {
        tabevent->insertRow(row);
        for (int col = 0; col < 7; col++) {
            tabevent->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    tabevent->resizeRowsToContents();
}

void Evenement::saveEventData(int id_event, const QString &nom_event, const QString &nom_responsable,
                              const QString &type, const QString &date_heure,
                              const QString &lieu, int nbr_participant) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO EVENTS (ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE) "
                  "VALUES (:id_event, :nom_event, :type, TO_DATE(:date_heure, 'YYYY-MM-DD HH24:MI:SS'), :lieu, :nbr_participant, :nom_responsable)");

    query.bindValue(":id_event", id_event);
    query.bindValue(":nom_event", nom_event);
    query.bindValue(":type", type);
    query.bindValue(":date_heure", date_heure);
    query.bindValue(":lieu", lieu);
    query.bindValue(":nbr_participant", nbr_participant);
    query.bindValue(":nom_responsable", nom_responsable);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'ajout de l'événement : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "L'événement a été enregistré avec succès.");
    }
}

bool Evenement::deleteEvent(int id_event) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM EVENTS WHERE ID_EVENT = :id_event");
    query.bindValue(":id_event", id_event);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression de l'événement : " + query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "L'événement a été supprimé avec succès.");
    return true;
}

bool Evenement::updateEvent(int id_event, const QString &nom_event, const QString &nom_responsable,
                            const QString &type, const QString &date_heure,
                            const QString &lieu, int nbr_participant) {
    QSqlQuery query;
    query.prepare("UPDATE evenement SET NOM_EVENT = :nom_event, NOM_RESPONSABLE = :nom_responsable, "
                  "TYPE = :type, DATE_HEURE = TO_DATE(:date_heure, 'YYYY-MM-DD HH24:MI:SS'), "
                  "LIEU = :lieu, NBR_PARTICIPANT = :nbr_participant WHERE ID_EVENT = :id_event");

    query.bindValue(":id_event", id_event);
    query.bindValue(":nom_event", nom_event);
    query.bindValue(":nom_responsable", nom_responsable);
    query.bindValue(":type", type);
    query.bindValue(":date_heure", date_heure);
    query.bindValue(":lieu", lieu);
    query.bindValue(":nbr_participant", nbr_participant);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la mise à jour de l'événement:" << query.lastError().text();
        return false;
    }
    return true;
}
