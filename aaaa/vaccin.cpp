#include "vaccin.h"
#include <QSqlError>
#include <QDate>

Vaccin::Vaccin(QObject *parent) : QObject(parent) {}

void Vaccin::loadVaccinData(QTableWidget *tabvaccin) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database is not connected.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT REFERENCE, NOM, DOSE, PRIX, DATE_EXP, QUANTITE FROM VACCIN");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "Failed to retrieve data: " + query.lastError().text());
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
            tabvaccin->setItem(row, 4, new QTableWidgetItem("Invalid Date"));
        }

        tabvaccin->setItem(row, 5, new QTableWidgetItem(query.value(5).toString())); // QUANTITE

        row++;
    }

    tabvaccin->resizeRowsToContents();
}

void Vaccin::saveVaccinData(int reference, QString nom, QString type, int age_min, QString mode_admin, QString dose, QDate date_exp, double prix, int quantite) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database is not connected.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO VACCIN (REFERENCE, NOM, TYPE, AGE_MIN, MODE_ADMIN, DOSE, DATE_EXP, PRIX, QUANTITE) "
                  "VALUES (:reference, :nom, :type, :age_min, :mode_admin, :dose, :date_exp, :prix, :quantite)");

    // Bind values
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
        QMessageBox::critical(nullptr, "Error", "Failed to save data: " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Success", "The data was successfully uploaded!");
    }
}

void Vaccin::deleteVaccin(int reference) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database is not connected.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM VACCIN WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "Failed to delete data: " + query.lastError().text());
    } else if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Not Found", "Reference does not exist.");
    } else {
        QMessageBox::information(nullptr, "Success", "The data was successfully deleted!");
    }
}
