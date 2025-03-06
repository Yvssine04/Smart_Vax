#include "rendez_vous.h"
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

rendez_vous::rendez_vous(QObject *parent)
    : QObject(parent) {

}

rendez_vous::~rendez_vous() {

}

void rendez_vous::loadVaccins(QComboBox *comboBox, QComboBox *BoxMed, QComboBox *BoxInf) {
    if (!comboBox || !BoxMed || !BoxInf) {
        qDebug() << "One or more combo boxes are null!";
        return;
    }

    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database is not connected.");
        return;
    }

    QSqlQuery query;

    // Load Vaccines
    query.prepare("SELECT NOM FROM VACCIN");
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "Query failed: " + query.lastError().text());
        return;
    }
    comboBox->clear();
    while (query.next()) {
        QString nom = query.value(0).toString();
        qDebug() << "Vaccin ajouté: " << nom;
        comboBox->addItem(nom);
    }

    // Load Doctors
    query.prepare("SELECT NOM_E, PRENOM_E FROM EMPLOYEES WHERE SPECIALITE = 'docteur'");
    if (!query.exec()) {
        qDebug() << "Doctor Query failed:" << query.lastError().text();
        return;
    }

    BoxMed->clear();

    while (query.next()) {
        QString nom_e = query.value(0).toString();
        QString prenom_e = query.value(1).toString();
        BoxMed->addItem(nom_e + " " + prenom_e);
    }


    query.prepare("SELECT NOM_E, PRENOM_E FROM EMPLOYEES WHERE SPECIALITE = 'infirmier'");
    if (!query.exec()) {
        qDebug() << "Nurse Query failed:" << query.lastError().text();
        return;
    }

    BoxInf->clear();
    while (query.next()) {
        QString nom_e1 = query.value(0).toString();
        QString prenom_e2 = query.value(1).toString();
        BoxInf->addItem(nom_e1 + " " + prenom_e2);
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

    QString trimmedDispo = dispo.trimmed();
    QString trimmedDateNaiss = date_rdvNaiss.trimmed();

    qDebug() << "Formatted DateTime:" << trimmedDispo;
    qDebug() << "Formatted Birth Date:" << trimmedDateNaiss;

    QSqlQuery query;
    int CINi = CIN.toInt();
    if (CIN.length()!=7){
        QMessageBox::warning(nullptr, "Erreur", "CIN doit avoir 7 chiffres",QMessageBox::Ok);

    }else{

        query.prepare("INSERT INTO RENDEZ_VOUS (ID_RDV, DATE_RDV, LIEU, DOC_ATT, INFIRMIER_ATT, SALLE_ATT, FACTURATION_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV, DATENAISS_RDV) "
                      "VALUES (:cin, TO_DATE(:dispo, 'YYYY-MM-DD HH24:MI:SS'), :lieu, :doc_att, :infirmier_att, :salle_att, :facturation, :nom_rdv, :prenom_rdv, :vaccin_rdv, TO_DATE(:date_rdvNaiss, 'YYYY-MM-DD'))");


    query.bindValue(":cin", CINi);
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


    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "j'ai pas pu le faire :(( : " + query.lastError().text());
        qDebug() << "SQL Error: " << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery();
    } else {
        QMessageBox::information(nullptr, "Success", "Les informations ont été sauvegrdés");
    }
    }}
void rendez_vous::modifier_rdv(int CINi, const QString &CIN, const QString &vaccin, const QString &date_rdvNaiss, const QString &adresse,
                                              const QString &nom, const QString &prenom, const QString &dispo,
                                              const QString &medecin, const QString &infirmier, const QString &salle,
                                              double facturation){


    QSqlQuery query;


    query.prepare("UPDATE RENDEZ_VOUS SET "
                  "ID_RDV = :CIN, "
                  "DATE_RDV = TO_DATE(:dispo, 'YYYY-MM-DD HH24:MI:SS'), "
                  "LIEU = :lieu, "
                  "DOC_ATT = :doc_att, "
                  "INFIRMIER_ATT = :infirmier_att, "
                  "SALLE_ATT = :salle_att, "
                  "FACTURATION_RDV = :facturation, "
                  "NOM_RDV = :nom_rdv, "
                  "PRENOM_RDV = :prenom_rdv, "
                  "VACCIN_RDV = :vaccin_rdv, "
                  "DATENAISS_RDV = TO_DATE(:date_rdvNaiss, 'YYYY-MM-DD') "
                  "WHERE ID_RDV = :CIN");
    query.bindValue(":CIN", CIN);
    query.bindValue(":dispo", dispo);
    query.bindValue(":lieu", adresse);
    query.bindValue(":doc_att", medecin);
    query.bindValue(":infirmier_att", infirmier);
    query.bindValue(":salle_att", salle);
    query.bindValue(":facturation", facturation);
    query.bindValue(":nom_rdv", nom);
    query.bindValue(":prenom_rdv", prenom);
    query.bindValue(":vaccin_rdv", vaccin);
    query.bindValue(":date_rdvNaiss", date_rdvNaiss);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "j'ai pas pu le faire :(( : " + query.lastError().text());
        qDebug() << "SQL Error: " << query.lastError().text();
        qDebug() << "Query: " << query.lastQuery();
    } else {
        QMessageBox::information(nullptr, "Success", "The data was successfully updated!");
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
void rendez_vous::infoEdit(int CIN, QLineEdit *CIN_rdv_2, QDateEdit *daterdv_2, QComboBox *vaccin_3, QLineEdit *adresse_2, QLineEdit *nom_rdv_2, QLineEdit *prenom_rdv_2, QDateTimeEdit *dispo_rdv_2, QComboBox *medecin_att_2, QComboBox *infirmier_att_2, QLineEdit *salle_att_2, QDoubleSpinBox *facturation_2) {
    QSqlQuery query;
    query.prepare("SELECT ID_RDV, DATE_RDV, LIEU, DOC_ATT, INFIRMIER_ATT, SALLE_ATT, FACTURATION_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV, DATENAISS_RDV FROM RENDEZ_VOUS WHERE ID_RDV = :CIN");
    query.bindValue(":CIN", CIN);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors d'importation de données: " + query.lastError().text());
        qDebug() << "Query execution failed:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        qDebug() << "Data found for ID_RDV:" << CIN;
        qDebug() << "Data found :" << vaccin_3;
        CIN_rdv_2->setText(QString::number(CIN));
        daterdv_2->setDate(query.value(1).toDate());
        adresse_2->setText(query.value(2).toString());
        medecin_att_2->setCurrentText(query.value(3).toString());
        infirmier_att_2->setCurrentText(query.value(4).toString());
        salle_att_2->setText(query.value(5).toString());
        facturation_2->setValue(query.value(6).toDouble());
        nom_rdv_2->setText(query.value(7).toString());
        prenom_rdv_2->setText(query.value(8).toString());
        vaccin_3->setCurrentText(query.value(9).toString());
        dispo_rdv_2->setDate(query.value(10).toDate());


    } else {
        QMessageBox::warning(nullptr, "Introuvable", "Rendez_vous n'existe pas.");
        qDebug() << "No data found for ID_RDV:" << CIN;
    }
}

bool rendez_vous::rdv_existe(int ID_RDV){
    QSqlQuery query;
    query.prepare("SELECT 1 FROM RENDEZ_VOUS WHERE ID_RDV = :ID_RDV");
    query.bindValue(":ID_RDV", ID_RDV);
    query.exec();
    return query.next();
}



