#include "rendez_vous.h"
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

rendez_vous::rendez_vous(QObject *parent)
    : QObject(parent) {

}

rendez_vous::~rendez_vous() {

}

void rendez_vous::loadVaccins(QComboBox *comboBox) {
    if (!comboBox) {
        qDebug() << "Combo box is null!";
        return;
    }

    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database is not connected.");
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT NOM FROM VACCIN");


    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "Query failed: " + query.lastError().text());
        qDebug() << "SQL Error: " << query.lastError().text();
        return;
    }
    comboBox->clear();

    while (query.next()) {
        QString nom = query.value(0).toString();
        qDebug() << "Vaccin ajouté: " << nom;
        comboBox->addItem(nom);

    }
}

void rendez_vous::saveAppointment(const QString &CIN, const QString &vaccin, const QString &date_rdvNaiss, const QString &adresse,
                                  const QString &nom, const QString &prenom, const QString &dispo,
                                  const QString &medecin, const QString &infirmier, const QString &salle,
                                  double facturation) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database is not connected.");
        return;
    }

    // Trim and validate input strings
    QString trimmedDispo = dispo.trimmed();
    QString trimmedDateNaiss = date_rdvNaiss.trimmed();

    // Debug: Print formatted strings
    qDebug() << "Formatted DateTime:" << trimmedDispo;
    qDebug() << "Formatted Birth Date:" << trimmedDateNaiss;

    QSqlQuery query;
    query.prepare("INSERT INTO RENDEZ_VOUS (ID_RDV, DATE_RDV, LIEU, DOC_ATT, INFIRMIER_ATT, SALLE_ATT, FACTURATION_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV, DATENAISS_RDV) "
                  "VALUES (:cin, TO_DATE(:dispo, 'YYYY-MM-DD HH24:MI:SS'), :lieu, :doc_att, :infirmier_att, :salle_att, :facturation, :nom_rdv, :prenom_rdv, :vaccin_rdv, TO_DATE(:date_rdvNaiss, 'YYYY-MM-DD'))");

    // Bind values
    query.bindValue(":cin", CIN);
    query.bindValue(":dispo", trimmedDispo);
    query.bindValue(":lieu", adresse);
    query.bindValue(":doc_att", medecin);
    query.bindValue(":infirmier_att", infirmier);
    query.bindValue(":salle_att", salle);
    query.bindValue(":facturation", facturation);
    query.bindValue(":nom_rdv", nom);
    query.bindValue(":prenom_rdv", prenom);
    query.bindValue(":vaccin_rdv", vaccin);
    query.bindValue(":date_rdvNaiss", trimmedDateNaiss);

    // Execute the query
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "Failed to save data: " + query.lastError().text());
        qDebug() << "SQL Error: " << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery();
    } else {
        QMessageBox::information(nullptr, "Success", "The data was successfully uploaded!");
    }
}
void rendez_vous::loadAppointments(QListWidget *liste_att)
{

    liste_att->clear();
    liste_att->addItem("CIN           Nom   Prenom        Vaccin");
    qDebug() << "Loading appointments...";

    QSqlQuery query("SELECT ID_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV FROM RENDEZ_VOUS");
    while (query.next()) {
        QString CIN = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString vaccin = query.value(3).toString();
        QString itemText = CIN + "    " + nom + "     " + prenom + "         " + vaccin;
        liste_att->addItem(itemText);



    }
}

void rendez_vous::supprimerRdv(int CIN){

    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database n'est pas connecté.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM RENDEZ_VOUS WHERE ID_RDV = :CIN");
    query.bindValue(":CIN", CIN);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "echec de suppression: " + query.lastError().text());
    } else if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Not Found", "Rendez_vous n'existe pas.");
    } else {
        QMessageBox::information(nullptr, "Success", "Rendez_vous supprimé avec succés!");
    }



}
