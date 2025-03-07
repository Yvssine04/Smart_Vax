#include "equipements.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>


/*

QSqlQueryModel* Equipements::afficherEtTrierParPrix() {
    QSqlQuery query;
    query.prepare("SELECT * FROM Equipements ORDER BY prix ASC");

    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        return nullptr;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(query); // Set the query to the model
    return model;
}
*/




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

    QSqlQuery query;
    query.prepare("UPDATE EQUIPEMENTS SET NOM_EQ = :name, TYPE_EQ = :type, DATE_LIMITE = TO_DATE(:date_limit, 'YYYY-MM-DD'), FOURNISSEUR = :supplier, QUANTITE_EQ = :quantity, PRIX = :price WHERE REFERNCE_EQ = :reference");

    query.bindValue(":reference", this->reference);
    query.bindValue(":name", this->nom);
    query.bindValue(":type", this->type);
    query.bindValue(":date_limit", this->date_limite);
    query.bindValue(":supplier", this->fournisseur);
    query.bindValue(":quantity", this->quantite);
    query.bindValue(":price", this->prix);

    // Debugging: Print the query and bound values
    qDebug() << "Executing query:" << query.executedQuery();
    qDebug() << "With values: reference =" << this->reference
             << "name =" << this->nom
             << "type =" << this->type
             << "date_limit =" << this->date_limite
             << "supplier =" << this->fournisseur
             << "quantity =" << this->quantite
             << "price =" << this->prix;

    if (!query.exec()) {
        qDebug() << "Query execution failed:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Échec de la modification de l'équipement : " + query.lastError().text());
        return false;
    } else {
        QMessageBox::information(nullptr, "Succès", "L'équipement a été modifié avec succès.");
        return true;
    }
}
