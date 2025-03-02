#include "vaccin.h"
#include <QSqlError>
#include <QDate>

Vaccin::Vaccin(QObject *parent) : QObject(parent) {}

void Vaccin::loadVaccinData(QTableWidget *tabvaccin) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT REFERENCE, NOM, DOSE, PRIX, DATE_EXP, QUANTITE FROM VACCIN");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    tabvaccin->setRowCount(0);

    int row = 0;
    while (query.next()) {
        tabvaccin->insertRow(row);
        tabvaccin->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // REFERENCE
        tabvaccin->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // NOM
        tabvaccin->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // DOSE
        tabvaccin->setItem(row, 3, new QTableWidgetItem(query.value(3).toString())); // PRIX
        QDate dateExp = query.value(4).toDate();
        if (dateExp.isValid()) {
            QTableWidgetItem *dateItem = new QTableWidgetItem(dateExp.toString(Qt::ISODate));
            dateItem->setData(Qt::DisplayRole, dateExp);
            dateItem->setData(Qt::EditRole, dateExp);
            tabvaccin->setItem(row, 4, dateItem);
        } else {
            tabvaccin->setItem(row, 4, new QTableWidgetItem("Date invalide"));
        }

        tabvaccin->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // QUANTITE

        row++;
    }

    tabvaccin->resizeRowsToContents();
}

void Vaccin::saveVaccinData(int reference, QString nom, QString type, int age_min, QString mode_admin, QString dose, QDate date_exp, double prix, int quantite) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    if (isReferenceExists(reference)) {
        query.prepare("UPDATE VACCIN SET NOM = :nom, TYPE = :type, AGE_MIN = :age_min, MODE_ADMIN = :mode_admin, DOSE = :dose, DATE_EXP = :date_exp, PRIX = :prix, QUANTITE = :quantite WHERE REFERENCE = :reference");
    } else {
        query.prepare("INSERT INTO VACCIN (REFERENCE, NOM, TYPE, AGE_MIN, MODE_ADMIN, DOSE, DATE_EXP, PRIX, QUANTITE) "
                      "VALUES (:reference, :nom, :type, :age_min, :mode_admin, :dose, :date_exp, :prix, :quantite)");
    }

    // Lier les valeurs
    query.bindValue(":reference", reference);
    query.bindValue(":nom", nom);
    query.bindValue(":type", type);
    query.bindValue(":age_min", age_min);
    query.bindValue(":mode_admin", mode_admin);
    query.bindValue(":dose", dose);
    query.bindValue(":date_exp", date_exp);
    query.bindValue(":prix", prix);
    query.bindValue(":quantite", quantite);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'enregistrement des données : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Les données ont été téléchargées avec succès !");
    }
}

void Vaccin::deleteVaccin(int reference) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM VACCIN WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression des données : " + query.lastError().text());
    } else if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Introuvable", "La référence n'existe pas.");
    } else {
        QMessageBox::information(nullptr, "Succès", "Les données ont été supprimées avec succès !");
    }
}

void Vaccin::fetchVaccinData(int reference, QLineEdit *referenceEdit, QLineEdit *nomEdit, QLineEdit *typeEdit, QLineEdit *ageEdit, QLineEdit *modeEdit, QLineEdit *doseEdit, QDateEdit *dateEdit, QLineEdit *prixEdit, QLineEdit *quantiteEdit) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM, TYPE, AGE_MIN, MODE_ADMIN, DOSE, DATE_EXP, PRIX, QUANTITE FROM VACCIN WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    if (query.next()) {
        referenceEdit->setText(QString::number(reference));
        nomEdit->setText(query.value(0).toString());
        typeEdit->setText(query.value(1).toString());
        ageEdit->setText(query.value(2).toString());
        modeEdit->setText(query.value(3).toString());
        doseEdit->setText(query.value(4).toString());
        QDate dateExp = query.value(5).toDate();
        if (dateExp.isValid()) {
            dateEdit->setDate(dateExp);
        }
        prixEdit->setText(query.value(6).toString());
        quantiteEdit->setText(query.value(7).toString());
    } else {
        QMessageBox::warning(nullptr, "Introuvable", "La référence n'existe pas.");
    }
}

bool Vaccin::isReferenceExists(int reference) {
    QSqlQuery query;
    query.prepare("SELECT 1 FROM VACCIN WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);
    query.exec();
    return query.next();
}
