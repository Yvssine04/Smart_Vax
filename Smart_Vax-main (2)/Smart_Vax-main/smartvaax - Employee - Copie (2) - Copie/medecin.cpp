#include "medecin.h"
#include "qapplication.h"
#include "qelapsedtimer.h"
#include "qheaderview.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QCryptographicHash>
#include <QPdfWriter>
#include <QTextDocument>
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>
#include <QTableWidget>
#include <QPrinter>

medecin::medecin(QObject *parent) : QObject(parent) {}



void medecin::loadMedData(QTableWidget *tabMed) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID, PRENOM_E, NOM_E, ANNEE_EXP, SPECIALITE, NUM_TEL, MAIL_E, SEXE, ADRESSE_E, MDP_E FROM EMPLOYEES WHERE ID LIKE 'M%'");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    tabMed->setSortingEnabled(true);
    tabMed->horizontalHeader()->setSectionsClickable(true);
    tabMed->horizontalHeader()->setSortIndicatorShown(true);

    tabMed->setRowCount(0); // Clear the table
    int row = 0;
    while (query.next()) {
        tabMed->insertRow(row);
        for (int col = 0; col < 10; col++) { // 10 colonnes incluant MDP_E
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());

            // Store numeric values in UserRole for proper sorting
            if (col == 3 || col == 5) { // ANNEE_EXP or NUM_TEL columns
                bool ok;
                int value = query.value(col).toInt(&ok);
                item->setData(Qt::UserRole, ok ? value : 0);
            }

            if (col == 9) { // Password column
                item->setText("●●●●●●●●");
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }

            tabMed->setItem(row, col, item);
        }
        row++;
    }

    tabMed->resizeRowsToContents(); // Adjust row heights
}


bool medecin::saveMedData(QString id_e, const QString &prenom_e, const QString &nom_e,
                          const QString &annee_exp_e, const QString &specialite_e,
                          const QString &num_tel_e, const QString &mail_e, const QString sexe_e,
                          const QString &adresse_e, const QString &mdp_e) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return false;
    }

    // Remove the empty field check (already done in UI)

    // Keep all other validations but return false instead of just returning
    QRegularExpression idRegex("^[AMIT]\\d{6}$");
    if (!idRegex.match(id_e).hasMatch()) {
        QMessageBox::warning(nullptr, "ID invalide", "L'ID doit commencer par A, M, I ou T suivi de 6 chiffres (ex: A123456).");
        return false;
    }

    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");
    if (!nameRegex.match(prenom_e).hasMatch() || !nameRegex.match(nom_e).hasMatch()) {
        QMessageBox::warning(nullptr, "Nom ou prénom invalide", "Le prénom et le nom doivent contenir uniquement des lettres.");
        return false;
    }

    bool expOk;
    int annee_exp = annee_exp_e.toInt(&expOk);
    if (!expOk || annee_exp < 0) {
        QMessageBox::warning(nullptr, "Année d'expérience invalide", "L'année d'expérience doit être un entier positif.");
        return false;
    }

    QRegularExpression phoneRegex("^\\d{8,15}$");
    if (!phoneRegex.match(num_tel_e).hasMatch()) {
        QMessageBox::warning(nullptr, "Numéro de téléphone invalide", "Le numéro de téléphone doit contenir entre 8 et 15 chiffres.");
        return false;
    }

    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(mail_e).hasMatch()) {
        QMessageBox::warning(nullptr, "Email invalide", "Veuillez entrer une adresse email valide.");
        return false;
    }

    if (sexe_e != "M" && sexe_e != "F") {
        QMessageBox::warning(nullptr, "Sexe invalide", "Le sexe doit être 'M' pour Masculin ou 'F' pour Féminin.");
        return false;
    }

    if (mdp_e.length() < 6) {
        QMessageBox::warning(nullptr, "Mot de passe trop court", "Le mot de passe doit contenir au moins 6 caractères.");
        return false;
    }


     // Ask for fingerprint
        QMessageBox::information(nullptr, "Enregistrement de l'empreinte", "Veuillez placer votre pouce...");
    emp->sendCommandToArduino("2");
        QMessageBox::information(nullptr, "Enregistrement de l'empreinte", "Veuillez placer votre pouce un 2 eme fois...");

    QString fingerprintId;
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < 10000) {
        if (emp->isArduinoReadable()) {
            QString data = emp->readArduinoData();
            qDebug() << "Received from Arduino: " << data;

            QRegularExpression regex("Fingerprint ID:\\s*(\\d+)");
            QRegularExpressionMatch match = regex.match(data);
            if (match.hasMatch()) {
                fingerprintId = match.captured(1);
                break;
            }
        }
        QApplication::processEvents();
    }

    if (fingerprintId.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur d'empreinte", "Échec de la capture d'empreinte digitale.");
        return false;
    }

    // ✅ Now save everything including fingerprint ID







    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYEES (ID, PRENOM_E, NOM_E, ANNEE_EXP, SPECIALITE, NUM_TEL, MAIL_E, SEXE, ADRESSE_E, MDP_E, FINGER) "
                  "VALUES (:id_e, :prenom_e, :nom_e, :annee_exp_e, :specialite_e, :num_tel_e, :mail_e, :sexe_e, :adresse_e, :mdp_e, :fingerprint_id)");

    query.bindValue(":id_e", id_e);
    query.bindValue(":prenom_e", prenom_e);
    query.bindValue(":nom_e", nom_e);
    query.bindValue(":annee_exp_e", annee_exp_e);
    query.bindValue(":specialite_e", specialite_e);
    query.bindValue(":num_tel_e", num_tel_e);
    query.bindValue(":mail_e", mail_e);
    query.bindValue(":sexe_e", sexe_e);
    query.bindValue(":adresse_e", adresse_e);
    query.bindValue(":mdp_e", mdp_e);
    query.bindValue(":fingerprint_id", fingerprintId);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'ajout du médecin : " + query.lastError().text());
        return false;
    }



    QMessageBox::information(nullptr, "Succès", "Le Médecin a été enregistré avec succès.");
    return true;
}







bool medecin::deleteMed(QString id_e) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYEES WHERE ID = :id_e"); // Ensure the column name is correct
    query.bindValue(":id_e", id_e);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression du medecin : " + query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Le medecin a été supprimé avec succès.");
    return true;
}






bool medecin::updateMed(QString id_e, const QString &prenom_e, const QString &nom_e,
                        const QString &annee_exp_e, const QString &specialite_e,
                        const QString &num_tel_e, const QString &mail_e, const QString sexe_e,
                        const QString &adresse_e, const QString &mdp_e)
{
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Error", "Database not connected!");
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE EMPLOYEES SET "
                  "PRENOM_E = :prenom, "
                  "NOM_E = :nom, "
                  "ANNEE_EXP = :exp, "
                  "SPECIALITE = :spec, "
                  "NUM_TEL = :tel, "
                  "MAIL_E = :mail, "
                  "SEXE = :sexe, "
                  "ADRESSE_E = :addr, "
                  "MDP_E = :mdp "
                  "WHERE ID = :id");

    query.bindValue(":id", id_e);
    query.bindValue(":prenom", prenom_e);
    query.bindValue(":nom", nom_e);
    query.bindValue(":exp", annee_exp_e);
    query.bindValue(":spec", specialite_e);
    query.bindValue(":tel", num_tel_e);
    query.bindValue(":mail", mail_e);
    query.bindValue(":sexe", sexe_e);
    query.bindValue(":addr", adresse_e);
    query.bindValue(":mdp", mdp_e);

    return query.exec();
}


void medecin::searchMedData(QTableWidget *tabMed, const QString &searchText) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database not connected.");
        return;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT ID, PRENOM_E, NOM_E, ANNEE_EXP, SPECIALITE, NUM_TEL, MAIL_E, SEXE, ADRESSE_E, MDP_E "
        "FROM EMPLOYEES "
        "WHERE LOWER(ID) LIKE LOWER(:search) "
        "OR LOWER(PRENOM_E) LIKE LOWER(:search) "
        "OR LOWER(NOM_E) LIKE LOWER(:search) "
        "OR LOWER(SPECIALITE) LIKE LOWER(:search)"
        );
    query.bindValue(":search", "%" + searchText + "%");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Error", "Search failed: " + query.lastError().text());
        return;
    }

    tabMed->setRowCount(0); // Clear table
    int row = 0;
    while (query.next()) {
        tabMed->insertRow(row);
        for (int col = 0; col < 10; col++) {
            if (col == 9) { // Password column
                QTableWidgetItem *passwordItem = new QTableWidgetItem("●●●●●●●●");
                passwordItem->setFlags(passwordItem->flags() & ~Qt::ItemIsEditable);
                tabMed->setItem(row, col, passwordItem);
            } else {
                tabMed->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
        row++;
    }
    tabMed->resizeRowsToContents();
}


void medecin::exporterCertificatTravailPDF(QTableWidget *tabEmployees) {
    QModelIndexList selection = tabEmployees->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(nullptr, "Sélection requise", "Veuillez sélectionner un employé.");
        return;
    }

    int row = selection.first().row();
    QString id = tabEmployees->item(row, 0)->text();
    QString prenom = tabEmployees->item(row, 1)->text();
    QString nom = tabEmployees->item(row, 2)->text();
    QString specialite = tabEmployees->item(row, 4)->text();
    QString telephone = tabEmployees->item(row, 5)->text();
    QString email = tabEmployees->item(row, 6)->text();
    QString adresse = tabEmployees->item(row, 8)->text();
    QString dateAujourdhui = QDate::currentDate().toString("dd/MM/yyyy");

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Exporter le certificat de travail", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    writer.setTitle("Certificat d'Inscription au Travail");
    QTextDocument doc;

    QString html = "<!DOCTYPE html>"
                   "<html lang=\"fr\">"
                   "<head>"
                   "<meta charset=\"UTF-8\">"
                   "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                   "<title>Certificat d'Inscription au Travail</title>"
                   "<style>"
                   "body { font-family: Arial, sans-serif; background-color: white; padding: 40px; margin: 0; }"
                   "#container { border: 3px solid #4CAF50; padding: 30px; width: 90%; max-width: 700px; margin: auto; background: white; text-align: center; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); border-radius: 10px; }"
                   "#header { display: flex; justify-content: space-between; align-items: center; padding-bottom: 20px; border-bottom: 3px solid #4CAF50; }"
                   "#header div { font-size: 18px; font-weight: bold; color: #4CAF50; }"
                   "#title { font-size: 24px; font-weight: bold; color: #4CAF50; margin: 20px 0; }"
                   "#info { text-align: left; font-size: 16px; margin-top: 20px; line-height: 1.6; }"
                   "#info p { margin: 10px 0; }"
                   "#footer { margin-top: 30px; font-size: 14px; text-align: center; background-color: white; color: black; padding: 10px; border-radius: 0 0 10px 10px; }"
                   "</style>"
                   "</head>"
                   "<body>"
                   "<div id=\"container\">"
                   "<div id=\"header\">"
                   "<div><strong>" + dateAujourdhui + " - Ariana</strong></div>"
                                      "</div>"
                                      "<div id=\"title\">Certificat d'Inscription au Travail</div>"
                                      "<div id=\"info\">"
                                      "<p>Ce certificat confirme l'inscription de l'employé suivant :</p>"
                                      "<p><strong>ID :</strong> " + id + "</p>"
                          "<p><strong>Prénom :</strong> " + prenom + "</p>"
                              "<p><strong>Nom :</strong> " + nom + "</p>"
                           "<p><strong>Spécialité :</strong> " + specialite + "</p>"
                                  "<p><strong>Téléphone :</strong> " + telephone + "</p>"
                                 "<p><strong>Email :</strong> " + email + "</p>"
                             "<p><strong>Adresse :</strong> " + adresse + "</p>"
                               "</div>"
                               "<div id=\"footer\">"
                               "<p>Adresse : Ariana 2081</p>"
                               "<p>Email : entreprise@exemple.com | Téléphone : +216 21 276 002</p>"
                               "</div>"
                               "</div>"
                               "</body>"
                               "</html>";

    doc.setHtml(html);
    doc.setPageSize(QSizeF(QPageSize(QPageSize::A4).rectPixels(96).size()));
    doc.print(&writer);
    QMessageBox::information(nullptr, "Exportation réussie", "Le certificat d'inscription au travail a été enregistré avec succès !");
}



void medecin::TriMedData(QTableWidget *tabMed, int column)
{
    // Don't sort the password column (column 9)
    if (column == 9) {
        QMessageBox::information(nullptr, "Information", "Password column cannot be sorted.");
        return;
    }

    // Get current sort order and toggle it
    Qt::SortOrder currentOrder = tabMed->horizontalHeader()->sortIndicatorOrder();
    Qt::SortOrder newOrder = (currentOrder == Qt::AscendingOrder)
                                 ? Qt::DescendingOrder
                                 : Qt::AscendingOrder;

    // For numeric columns (3=ANNEE_EXP, 5=NUM_TEL), use custom sorting
    if (column == 3 || column == 5) {
        // Disable sorting while we modify items
        tabMed->setSortingEnabled(false);

        // Convert all items in the column to integers for proper sorting
        for (int row = 0; row < tabMed->rowCount(); ++row) {
            QTableWidgetItem *item = tabMed->item(row, column);
            bool ok;
            int value = item->text().toInt(&ok);
            if (ok) {
                item->setData(Qt::UserRole, value);
            } else {
                item->setData(Qt::UserRole, 0); // Default value if conversion fails
            }
        }

        // Enable sorting and sort by UserRole data
        tabMed->setSortingEnabled(true);
        tabMed->sortByColumn(column, newOrder);
    } else {
        // Regular text sorting for other columns
        tabMed->sortByColumn(column, newOrder);
    }

    // Re-apply password masking after sorting
    for (int row = 0; row < tabMed->rowCount(); ++row) {
        QTableWidgetItem *passwordItem = new QTableWidgetItem("●●●●●●●●");
        passwordItem->setFlags(passwordItem->flags() & ~Qt::ItemIsEditable);
        tabMed->setItem(row, 9, passwordItem); // Column 9 = MDP_E
    }
}




//INF
void medecin::loadInfData(QTableWidget *tabInf) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID, PRENOM_E, NOM_E, ANNEE_EXP, SPECIALITE, NUM_TEL, MAIL_E, SEXE, ADRESSE_E, MDP_E FROM EMPLOYEES WHERE ID LIKE 'I%'");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    tabInf->setRowCount(0); // Clear the table
    int row = 0;
    while (query.next()) {
        tabInf->insertRow(row);
        for (int col = 0; col < 10; col++) { // 10 colonnes incluant MDP_E
            if (col == 9) { // Si c'est la colonne du mot de passe (MDP_E)
                QTableWidgetItem *passwordItem = new QTableWidgetItem("●●●●●●●●"); // Affichage en points
                passwordItem->setFlags(passwordItem->flags() & ~Qt::ItemIsEditable); // Empêche la modification
                tabInf->setItem(row, col, passwordItem);
            } else {
                tabInf->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
            }
        }
        row++;
    }

    tabInf->resizeRowsToContents(); // Adjust row heights
}



