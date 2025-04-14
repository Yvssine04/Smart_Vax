#include "equipements.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>


QMap<QString, int> Equipements::calculerStatistiques() {
    QMap<QString, int> stats;

    QSqlQuery query("SELECT TYPE_EQ, SUM(QUANTITE_EQ) FROM EQUIPEMENTS GROUP BY TYPE_EQ");

    if (query.lastError().isValid()) {
        qDebug() << "SQL Error:" << query.lastError().text();
        return stats;
    }

    while (query.next()) {
        QString type = query.value(0).toString();
        int total = query.value(1).toInt();
        stats.insert(type, total);
    }

    return stats;
}

void Equipements::sortEquipementTable(QTableWidget *tableEqui, int column) {
    if (tableEqui == nullptr || tableEqui->rowCount() == 0 || column < 0 || column >= tableEqui->columnCount()) {
        qDebug() << "Invalid table or column index!";
        return;
    }

    bool isNumericColumn = (column == 5 || column == 6); // Numeric columns: quantity (5), price (6)

    // Parse the values for sorting
    for (int row = 0; row < tableEqui->rowCount(); ++row) {
        QTableWidgetItem *item = tableEqui->item(row, column);
        if (item) {
            if (isNumericColumn) {
                bool ok;
                double numericValue = item->text().toDouble(&ok);  // Use double for more general numeric types

                if (ok) {
                    item->setData(Qt::UserRole, numericValue);  // Store the numeric value
                } else {
                    qDebug() << "Invalid numeric value: " << item->text() << " at row " << row;
                }
            } else {
                item->setData(Qt::UserRole, item->text());  // Store as text if not numeric
            }
        }
    }

    // Sort by the numeric data in UserRole
    tableEqui->sortItems(column, Qt::AscendingOrder);
    qDebug() << "Sorted column" << column << (isNumericColumn ? " numerically" : " alphabetically") << " (ascending).";
}





bool Equipements::supprimerEquipement(QString reference) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM EQUIPEMENTS WHERE REFERNCE_EQ = :reference");
    query.bindValue(":reference", reference);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression de l'équipement : " + query.lastError().text());
        return false;
    } else {
        QMessageBox::information(nullptr, "Succès", "L'équipement a été supprimé avec succès.");
        return true;
    }
}

Equipements::Equipements() {
    model = new QSqlQueryModel();
}







bool Equipements::ajouter() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return false;
    }

    // Contrôle de la date d'expiration
    QDate currentDate = QDate::currentDate();
    QDate minDate(2025, 3, 9); // Date minimale : 09/03/2025
    QDate expirationDate = QDate::fromString(this->date_limite, "yyyy-MM-dd");

    if (expirationDate <= currentDate || expirationDate <= minDate) {
        QMessageBox::warning(nullptr, "Erreur de date", "La date d'expiration doit être supérieure à la date actuelle! .");
        return false;
    }

    // Contrôle du nom (ne doit pas contenir de chiffres)
    QRegularExpression nameRegex("^[^0-9]*$"); // Expression régulière pour vérifier l'absence de chiffres
    if (!nameRegex.match(this->nom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de nom", "Le nom ne doit pas contenir de chiffres.");
        return false;
    }

    // Contrôle du type (ne doit pas contenir de chiffres)
    QRegularExpression typeRegex("^[^0-9]*$"); // Expression régulière pour vérifier l'absence de chiffres
    if (!typeRegex.match(this->type).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de type", "Le type ne doit pas contenir de chiffres.");
        return false;
    }

    // Contrôle du fournisseur (ne doit pas contenir de chiffres)
    if (!typeRegex.match(this->fournisseur).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de fournisseur", "Le fournisseur ne doit pas contenir de chiffres.");
        return false;
    }

    // Contrôle du prix (ne doit pas contenir de caractères alphabétiques)
    QRegularExpression priceRegex("^[^a-zA-Z]*$"); // Expression régulière pour vérifier l'absence de lettres
    QString prixStr = QString::number(this->prix); // Convertir le prix en QString pour vérifier
    if (!priceRegex.match(prixStr).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de prix", "Le prix ne doit pas contenir de caractères alphabétiques.");
        return false;
    }

    // Contrôle de la quantité (ne doit pas contenir de caractères alphabétiques)
    QRegularExpression quantityRegex("^[^a-zA-Z]*$"); // Expression régulière pour vérifier l'absence de lettres
    QString quantiteStr = QString::number(this->quantite); // Convertir la quantité en QString pour vérifier
    if (!quantityRegex.match(quantiteStr).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de quantité", "La quantité ne doit pas contenir de caractères alphabétiques.");
        return false;
    }

    // Si tous les contrôles sont passés, on procède à l'ajout dans la base de données
    QSqlQuery query;
    query.prepare("INSERT INTO EQUIPEMENTS (REFERNCE_EQ, NOM_EQ, TYPE_EQ, DATE_LIMITE, FOURNISSEUR, QUANTITE_EQ, PRIX) "
                  "VALUES (:reference, :name, :type, TO_DATE(:date_limit, 'YYYY-MM-DD'), :supplier, :quantity, :price)");

    query.bindValue(":reference", this->reference);
    query.bindValue(":name", this->nom);
    query.bindValue(":type", this->type);
    query.bindValue(":date_limit", this->date_limite);
    query.bindValue(":supplier", this->fournisseur);
    query.bindValue(":quantity", this->quantite);
    query.bindValue(":price", this->prix);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'ajout de l'équipement : " + query.lastError().text());
        return false;
    } else {
        QMessageBox::information(nullptr, "Succès", "L'équipement a été ajouté avec succès.");
        return true;
    }
}



//////////////////////////////////////////////////////////////////////////////////////////
QSqlQueryModel* Equipements::afficher() {
    model->setQuery("SELECT * FROM EQUIPEMENTS");
    return model;
}

QSqlQueryModel* Equipements::rechercherEquipement(const QString& reference) {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EQUIPEMENTS WHERE REFERNCE_EQ = '" + reference + "'");
    return model;
}



bool Equipements::modifier() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return false;
    }

    // Contrôle de la date d'expiration
    QDate currentDate = QDate::currentDate();
    QDate minDate(2025, 3, 9); // Date minimale : 09/03/2025
    QDate expirationDate = QDate::fromString(this->date_limite, "yyyy-MM-dd");

    if (expirationDate <= currentDate || expirationDate <= minDate) {
        QMessageBox::warning(nullptr, "Erreur de date", "La date d'expiration doit être supérieure à la date actuelle.");
        return false;
    }

    // Contrôle du nom (ne doit pas contenir de chiffres)
    QRegularExpression nameRegex("^[^0-9]*$"); // Expression régulière pour vérifier l'absence de chiffres
    if (!nameRegex.match(this->nom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de nom", "Le nom ne doit pas contenir de chiffres.");
        return false;
    }

    // Contrôle du type (ne doit pas contenir de chiffres)
    QRegularExpression typeRegex("^[^0-9]*$"); // Expression régulière pour vérifier l'absence de chiffres
    if (!typeRegex.match(this->type).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de type", "Le type ne doit pas contenir de chiffres.");
        return false;
    }

    // Contrôle du fournisseur (ne doit pas contenir de chiffres)
    if (!typeRegex.match(this->fournisseur).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de fournisseur", "Le fournisseur ne doit pas contenir de chiffres.");
        return false;
    }

    // Contrôle du prix (ne doit pas contenir de caractères alphabétiques)
    QString prixStr = QString::number(this->prix); // Convertir le prix en QString pour vérifier
    if (prixStr.contains(QRegularExpression("[qawsedrftgyhujikolpmnbvcxz]"))) {
        QMessageBox::warning(nullptr, "Erreur de prix", "Le prix ne doit pas contenir de caractères alphabétiques.");
        return false;
    }

    // Contrôle de la quantité (ne doit pas contenir de caractères alphabétiques)
    QString quantiteStr = QString::number(this->quantite); // Convertir la quantité en QString pour vérifier
    if (quantiteStr.contains(QRegularExpression("[qawsedrftgyhujikolpmnbvcxz]"))) {
        QMessageBox::warning(nullptr, "Erreur de quantité", "La quantité ne doit pas contenir de caractères alphabétiques.");
        return false;
    }

    // Si tous les contrôles sont passés, on procède à la modification dans la base de données
    QSqlQuery query;
    query.prepare("UPDATE EQUIPEMENTS SET NOM_EQ = :name, TYPE_EQ = :type, DATE_LIMITE = TO_DATE(:date_limit, 'YYYY-MM-DD'), FOURNISSEUR = :supplier, QUANTITE_EQ = :quantity, PRIX = :price WHERE REFERNCE_EQ = :reference");

    query.bindValue(":reference", this->reference);
    query.bindValue(":name", this->nom);
    query.bindValue(":type", this->type);
    query.bindValue(":date_limit", this->date_limite);
    query.bindValue(":supplier", this->fournisseur);
    query.bindValue(":quantity", this->quantite);
    query.bindValue(":price", this->prix);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la modification de l'équipement : " + query.lastError().text());
        return false;
    } else {
        QMessageBox::information(nullptr, "Succès", "L'équipement a été modifié avec succès.");
        return true;
    }
}





