#include "vaccin.h"
#include "qpagesize.h"
#include "qpdfwriter.h"
#include <QSqlError>
#include <QDate>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QPdfWriter>
#include <QTextDocument>
#include <QSqlQueryModel>


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

    if (isReferenceExists(reference)) {
        updateVaccinData(reference, nom, type, age_min, mode_admin, dose, date_exp, prix, quantite);
    } else {
        insertVaccinData(reference, nom, type, age_min, mode_admin, dose, date_exp, prix, quantite);
    }
}

void Vaccin::insertVaccinData(int reference, QString nom, QString type, int age_min, QString mode_admin, QString dose, QDate date_exp, double prix, int quantite) {
    QSqlQuery query;
    query.prepare("INSERT INTO VACCIN (REFERENCE, NOM, TYPE, AGE_MIN, MODE_ADMIN, DOSE, DATE_EXP, PRIX, QUANTITE) "
                  "VALUES (:reference, :nom, :type, :age_min, :mode_admin, :dose, :date_exp, :prix, :quantite)");
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
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'insertion des données : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Les données ont été insérées avec succès !");
    }
}

void Vaccin::updateVaccinData(int reference, QString nom, QString type, int age_min, QString mode_admin, QString dose, QDate date_exp, double prix, int quantite) {
    QSqlQuery query;
    query.prepare("UPDATE VACCIN SET NOM = :nom, TYPE = :type, AGE_MIN = :age_min, MODE_ADMIN = :mode_admin, DOSE = :dose, DATE_EXP = :date_exp, PRIX = :prix, QUANTITE = :quantite WHERE REFERENCE = :reference");
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
        QMessageBox::critical(nullptr, "Erreur", "Échec de la mise à jour des données : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "Les données ont été mises à jour avec succès !");
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

void Vaccin::fetchVaccinData(int reference, QLineEdit *referenceEdit, QComboBox *nomEdit, QComboBox *typeEdit, QLineEdit *ageEdit, QComboBox *modeEdit, QLineEdit *doseEdit, QDateEdit *dateEdit, QLineEdit *prixEdit, QLineEdit *quantiteEdit) {
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
        nomEdit->setCurrentText(query.value(0).toString());
        typeEdit->setCurrentText(query.value(1).toString());
        ageEdit->setText(query.value(2).toString());
        modeEdit->setCurrentText(query.value(3).toString());
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

void Vaccin::filterVaccinTable(QTableWidget *table, const QString &searchText) {
    bool firstMatchFound = false;
    int firstMatchRow = -1;

    for (int row = 0; row < table->rowCount(); ++row) {
        bool matchFound = false;
        QTableWidgetItem *itemRef = table->item(row, 0);
        if (itemRef && itemRef->text().contains(searchText, Qt::CaseInsensitive)) {
            matchFound = true;
        }
        QTableWidgetItem *itemName = table->item(row, 1);
        if (itemName && itemName->text().contains(searchText, Qt::CaseInsensitive)) {
            matchFound = true;
        }
        table->setRowHidden(row, !matchFound);
        if (matchFound && !firstMatchFound) {
            firstMatchFound = true;
            firstMatchRow = row;
        }
    }
    if (firstMatchRow != -1) {
        table->clearSelection();
        table->selectionModel()->select(table->model()->index(firstMatchRow, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
    }
}

void Vaccin::sortVaccinTable(QTableWidget *tableVaccin, int column, Qt::SortOrder order) {
    if (tableVaccin == nullptr || tableVaccin->rowCount() == 0 || column < 0 || column >= tableVaccin->columnCount()) {
        qDebug() << "Invalid table or column index!";
        return;
    }
    // Check if the column is for quantity or price
    bool isNumericColumn = (column == 3 || column == 5); // Assuming prix is column 3 and quantite is column 5

    if (isNumericColumn) {
        // Sort numerically
        tableVaccin->sortItems(column, order);
        qDebug() << "Sorted column" << column << "numerically (" << (order == Qt::AscendingOrder ? "ascending" : "descending") << ").";
    } else {
        // Sort alphabetically
        tableVaccin->sortItems(column, order);
        qDebug() << "Sorted column" << column << "alphabetically (" << (order == Qt::AscendingOrder ? "ascending" : "descending") << ").";
    }
}
void Vaccin::exporterVaccinPDF(QTableWidget *tabvaccin) {
    QModelIndexList selection = tabvaccin->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(nullptr, "Sélection requise", "Veuillez sélectionner un vaccin.");
        return;
    }
    int row = selection.first().row();
    int reference = tabvaccin->item(row, 0)->text().toInt();
    QString nomVaccin = tabvaccin->item(row, 1)->text();
    QString dose = tabvaccin->item(row, 2)->text();
    QString prix = tabvaccin->item(row, 3)->text();
    QString dateExp = tabvaccin->item(row, 4)->text();
    QString quantite = tabvaccin->item(row, 5)->text();
    QString dateAujourdhui = QDate::currentDate().toString("dd/MM/yyyy");
    QString typeVaccin, modeAdmin;
    QSqlQuery query;
    query.prepare("SELECT TYPE, MODE_ADMIN FROM VACCIN WHERE REFERENCE = :reference");
    query.bindValue(":reference", reference);
    if (query.exec() && query.next()) {
        typeVaccin = query.value(0).toString();
        modeAdmin = query.value(1).toString();
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération des données du vaccin : " + query.lastError().text());
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Détail le rapport de vaccin", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    writer.setTitle("Détail de Vaccin - SmartVax");
    QTextDocument doc;

    QString html = "<!DOCTYPE html>"
                   "<html lang=\"fr\">"
                   "<head>"
                   "<meta charset=\"UTF-8\">"
                   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                   "<title>Détail de Vaccin - SmartVax</title>"
                   "<style>"
                   "body { font-family: Arial, sans-serif; background-color: white; padding: 40px; margin: 0; }"
                   "#container { border: 3px solid #4CAF50; padding: 30px; width: 90%; max-width: 700px; margin: auto; background: white; text-align: center; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); border-radius: 10px; }"
                   "#header { display: flex; justify-content: space-between; align-items: center; padding-bottom: 20px; border-bottom: 3px solid #4CAF50; }"
                   "#header div { font-size: 18px; font-weight: bold; color: #4CAF50; }"
                   "#title { font-size: 24px; font-weight: bold; color: #4CAF50; margin: 20px 0; }"
                   "#info { text-align: left; font-size: 16px; margin-top: 20px; line-height: 1.6; }"
                   "#info p { margin: 10px 0; }"
                   "#footer { margin-top: 30px; font-size: 14px; text-align: center; background-color: white; color: black; padding: 10px; border-radius: 0 0 10px 10px; }"
                   "#consignes { margin-top: 30px; font-size: 16px; text-align: left; background-color: #f9f9f9; padding: 20px; border-radius: 10px; }"
                   "#consignes h3 { font-size: 20px; color: #4CAF50; margin-bottom: 10px; }"
                   "</style>"
                   "</head>"
                   "<body>"
                   "<div id=\"container\">"
                   "<div id=\"header\">"
                   "<div><strong>" + dateAujourdhui + " - Ariana</strong></div>"
                                      "</div>"
                                      "<div id=\"title\">Détail de Vaccin - SmartVax</div>"
                                      "<div id=\"info\">"
                                      "<p>Voici les détails du vaccin sélectionné :</p>"
                                      "<p><strong>Nom du vaccin :</strong> " + nomVaccin + "</p>"
                                 "<p><strong>Type du vaccin :</strong> " + typeVaccin + "</p>"
                                  "<p><strong>Mode d'administration :</strong> " + modeAdmin + "</p>"
                                 "<p><strong>Dose :</strong> " + dose + "</p>"
                            "<p><strong>Prix :</strong> " + prix + " DT</p>"
                            "<p><strong>Date d'expiration :</strong> " + dateExp + "</p>"
                               "<p><strong>Quantité :</strong> " + quantite + "</p>"
                                "</div>"
                                "<div id=\"consignes\">"
                                "<h3>Consignes pour la bonne pratique des vaccins</h3>"
                                "<p>1. <strong>Hygiène des mains :</strong> Lavez-vous les mains avant et après chaque administration de vaccin.</p>"
                                "<p>2. <strong>Préparation du vaccin :</strong> Suivez les instructions spécifiques pour chaque vaccin concernant la reconstitution et la conservation.</p>"
                                "<p>3. <strong>Identification du patient :</strong> Vérifiez toujours l'identité du patient avant d'administrer le vaccin.</p>"
                                "<p>4. <strong>Stockage :</strong> Conservez les vaccins à la température recommandée pour garantir leur efficacité.</p>"
                                "<p>5. <strong>Gestion des déchets :</strong> Éliminez les aiguilles et les seringues usagées dans des conteneurs appropriés.</p>"
                                "</div>"
                                "<div id=\"footer\">"
                                "<p>Adresse : Ariana 2081</p>"
                                "<p>Email : SmartVax@yahoo.com | Téléphone : +216 21 276 002</p>"
                                "</div>"
                                "</div>"
                                "</body>"
                                "</html>";

    doc.setHtml(html);
    doc.setPageSize(QSizeF(QPageSize(QPageSize::A4).rectPixels(96).size()));
    doc.print(&writer);
    QMessageBox::information(nullptr, "Exportation réussie", "Le rapport de vaccin a été enregistré avec succès !");
}
QSqlQueryModel* Vaccin::fetchVaccineStatistics() {
    // Create a new QSqlQueryModel to hold the query results
    QSqlQueryModel *model = new QSqlQueryModel();

    // Execute the query to get the vaccine names and quantities
    model->setQuery("SELECT NOM, QUANTITE FROM VACCIN");

    // Check if the query was successful
    if (model->lastError().isValid()) {
        qDebug() << "Error executing query:" << model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}

