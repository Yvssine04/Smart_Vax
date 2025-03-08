#include "evenement.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QMessageBox>

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
                  "VALUES (:id_event, :nom_event, :type, TO_DATE(:date_heure, 'MM-DD-YYYY HH24:MI:SS'), :lieu, :nbr_participant, :nom_responsable)");

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

void Evenement::supprimerEvenement(int id_event) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM EVENTS WHERE ID_EVENT = :id_event");
    query.bindValue(":id_event", id_event);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression de l'événement : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "L'événement a été supprimé avec succès.");
    }
}


bool Evenement::recupererEvenement(int id, QString &nom, QString &responsable, QString &type, QString &lieu, int &participants, QDateTime &dateHeure) {
    QSqlQuery query;
    query.prepare("SELECT NOM_EVENT, NOM_RESPONSABLE, TYPE, LIEU, NBR_PARTICIPANT, DATE_HEURE FROM EVENTS WHERE ID_EVENT = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        nom = query.value(0).toString();
        responsable = query.value(1).toString();
        type = query.value(2).toString();
        lieu = query.value(3).toString();
        participants = query.value(4).toInt();
        dateHeure = query.value(5).toDateTime();
        return true;
    }
    return false;
}

bool Evenement::modifierEvenement(int id, const QString &nom, const QString &responsable, const QString &type, const QString &lieu, int participants, const QDateTime &dateHeure) {
    QSqlQuery query;
    query.prepare("UPDATE EVENTS SET NOM_EVENT = :nom, NOM_RESPONSABLE = :responsable, TYPE = :type, LIEU = :lieu, NBR_PARTICIPANT = :participants, DATE_HEURE = :dateHeure WHERE ID_EVENT = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":responsable", responsable);
    query.bindValue(":type", type);
    query.bindValue(":lieu", lieu);
    query.bindValue(":participants", participants);
    query.bindValue(":dateHeure", dateHeure);
    query.bindValue(":id", id);

    return query.exec();
}

void Evenement::searchEventData(QTableWidget *tabevent, const QString &searchTerm) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    // Ajout du critère de recherche pour NOM_RESPONSABLE
    QString queryString = "SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS "
                          "WHERE NOM_EVENT LIKE '%" + searchTerm + "%' OR "
                                         "TYPE LIKE '%" + searchTerm + "%' OR "
                                         "LIEU LIKE '%" + searchTerm + "%' OR "
                                         "NOM_RESPONSABLE LIKE '%" + searchTerm + "%'";  // Nouvelle condition

    query.prepare(queryString);
    qDebug() << "Requête exécutée : " << queryString;

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la recherche : " + query.lastError().text());
        return;
    }

    tabevent->setRowCount(0);
    int row = 0;
    while (query.next()) {
        qDebug() << "Résultat : " << query.value(1).toString();  // Debugging

        tabevent->insertRow(row);
        for (int col = 0; col < 7; col++) {
            tabevent->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    tabevent->resizeRowsToContents();
}

void Evenement::sortEventData(QTableWidget *tabevent, const QString &critere) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    QString queryString = "SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS ORDER BY ";

    if (critere == "date") {
        queryString += "DATE_HEURE ASC";
    } else if (critere == "participants") {
        queryString += "NBR_PARTICIPANT DESC";
    } else if (critere == "nom") {
        queryString += "NOM_EVENT ASC";
    } else {
        QMessageBox::warning(nullptr, "Critère invalide", "Veuillez choisir un critère valide.");
        return;
    }

    query.prepare(queryString);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec du tri des données : " + query.lastError().text());
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
