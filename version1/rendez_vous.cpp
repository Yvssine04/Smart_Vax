
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


    query.prepare("SELECT NOM_E, PRENOM_E FROM EMPLOYEES WHERE SPECIALITE = 'docteur'");
    if (!query.exec()) {
        qDebug() << "Problème avec liste des docteurs:" << query.lastError().text();
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
        qDebug() << "Problème avec liste des infirmiers:" << query.lastError().text();
        return;
    }

    BoxInf->clear();
    while (query.next()) {
        QString nom_e1 = query.value(0).toString();
        QString prenom_e2 = query.value(1).toString();
        BoxInf->addItem(nom_e1 + " " + prenom_e2);
    }
}

bool rendez_vous::saveAppointment(const QString &CIN, const QString &vaccin, const QString &dateVaccination,
                                  const QString &adresse, const QString &nom, const QString &prenom,
                                  const QString &dispo, const QString &medecin, const QString &infirmier,
                                  const QString &salle, double facturation)
{
    // Trim all input strings
    QString trimmedCIN = CIN.trimmed();
    QString trimmedNom = nom.trimmed();
    QString trimmedPrenom = prenom.trimmed();
    QString trimmedAdresse = adresse.trimmed();
    QString trimmedSalle = salle.trimmed();
    QString trimmedVaccin = vaccin.trimmed();
    QString trimmedMedecin = medecin.trimmed();
    QString trimmedInfirmier = infirmier.trimmed();
    QString trimmedDateVaccination = dateVaccination.trimmed();
    QString trimmedDispo = dispo.trimmed();

    // Validate required fields
    if (trimmedCIN.isEmpty() || trimmedNom.isEmpty() || trimmedPrenom.isEmpty() ||
        trimmedAdresse.isEmpty() || trimmedSalle.isEmpty() || trimmedVaccin.isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Tous les champs obligatoires doivent être remplis.", QMessageBox::Ok);
        return false;
    }

    // Validate CIN format (exactly 7 digits)
    QRegularExpression cinRegex("^\\d{7}$");
    if (!cinRegex.match(trimmedCIN).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le CIN doit contenir exactement 7 chiffres.", QMessageBox::Ok);
        return false;
    }

    // Validate names (letters, accents, and hyphens only)
    QRegularExpression nameRegex("^[A-Za-zÀ-ÖØ-öø-ÿ\\s-]+$");
    if (!nameRegex.match(trimmedNom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom ne doit contenir que des lettres.", QMessageBox::Ok);
        return false;
    }
    if (!nameRegex.match(trimmedPrenom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le prénom ne doit contenir que des lettres.", QMessageBox::Ok);
        return false;
    }

    // Validate dates
    QDate today = QDate::currentDate();
    QDate vaccinDate = QDate::fromString(trimmedDateVaccination, "yyyy-MM-dd");
    QDateTime dispoDateTime = QDateTime::fromString(trimmedDispo, "yyyy-MM-dd HH:mm:ss");

    if (!vaccinDate.isValid()) {
        QMessageBox::warning(nullptr, "Erreur", "Format de date de vaccination invalide (utilisez AAAA-MM-JJ).", QMessageBox::Ok);
        return false;
    }

    if (vaccinDate > today) {
        QMessageBox::warning(nullptr, "Erreur", "La date de vaccination ne peut pas être dans le futur.", QMessageBox::Ok);
        return false;
    }

    if (!dispoDateTime.isValid()) {
        QMessageBox::warning(nullptr, "Erreur", "Format de date/heure de rendez-vous invalide.", QMessageBox::Ok);
        return false;
    }

    if (dispoDateTime <= QDateTime::currentDateTime()) {
        QMessageBox::warning(nullptr, "Erreur", "Le rendez-vous doit être programmé dans le futur.", QMessageBox::Ok);
        return false;
    }

    // Validate facturation (must be positive)
    if (facturation < 0) {
        QMessageBox::warning(nullptr, "Erreur", "La facturation ne peut pas être négative.", QMessageBox::Ok);
        return false;
    }

    // Start database transaction
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.transaction()) {
        QMessageBox::warning(nullptr, "Erreur", "Impossible de démarrer la transaction.", QMessageBox::Ok);
        return false;
    }

    // Convert CIN to int (leading zeros will be lost, ensure your database can handle this)
    bool conversionOk;
    int CINi = trimmedCIN.toInt(&conversionOk);
    if (!conversionOk) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur", "CIN invalide.", QMessageBox::Ok);
        return false;
    }
     int idRdv = 2;
    // Insert into RENDEZ_VOUS

     QSqlQuery checkPatientQuery;
     checkPatientQuery.prepare("SELECT 1 FROM PATIENT WHERE CIN_PASSP = :cin");
     checkPatientQuery.bindValue(":cin", CINi); // CINi is your converted integer CIN

     if (!checkPatientQuery.exec() || !checkPatientQuery.next()) {
         // Patient doesn't exist - ask if we should create one
         QMessageBox::StandardButton reply;
         reply = QMessageBox::question(nullptr, "Il s'agit d'un nouveau patient",
                                       "Le CIN correspondant n'est pas trouvé. Voulez-vous créer un nouveau patient par la suite?",
                                       QMessageBox::Yes|QMessageBox::No);

     }



    QSqlQuery query;
    query.prepare("INSERT INTO RENDEZ_VOUS (CIN_RDV, DATE_RDV, LIEU, DOC_ATT, INFIRMIER_ATT, SALLE_ATT, FACTURATION_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV, DATENAISS_RDV) "
                  "VALUES (:cin, TO_DATE(:dispo, 'YYYY-MM-DD HH24:MI:SS'), :lieu, :doc_att, :infirmier_att, :salle_att, :facturation, :nom_rdv, :prenom_rdv, :vaccin_rdv, TO_DATE(:date_rdvNaiss, 'YYYY-MM-DD'))"
                  "RETURNING ID_RDV INTO :new_id");


    query.bindValue(":id_rdv", idRdv);
    query.bindValue(":cin", CINi);
    query.bindValue(":dispo", trimmedDispo);
    query.bindValue(":lieu", trimmedAdresse);
    query.bindValue(":doc_att", medecin);
    query.bindValue(":infirmier_att", infirmier);
    query.bindValue(":salle_att", trimmedSalle);
    query.bindValue(":facturation", facturation);
    query.bindValue(":nom_rdv", trimmedNom);
    query.bindValue(":prenom_rdv", trimmedPrenom);
    query.bindValue(":vaccin_rdv", trimmedVaccin);
    query.bindValue(":date_rdvNaiss", trimmedDateVaccination);

    int newRdvId = -1;
    query.bindValue(":new_id", newRdvId,QSql::Out);
    if (!query.exec()) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             "Échec de l'enregistrement du rendez-vous:\n" + query.lastError().text(),
                             QMessageBox::Ok);
        return false;
    }
    newRdvId = query.boundValue(":new_id").toInt();

    if (newRdvId <= 0) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur", "Échec de récupération de l'ID du rendez-vous.", QMessageBox::Ok);
        return false;
    }

    qDebug() << "ID_RDV " << newRdvId;
    // Insert into RESERVER
    QSqlQuery insertReserverQuery;
    insertReserverQuery.prepare("INSERT INTO RESERVER (CIN_PASSP,ID_RDV,VACCIN_RSV,DATE_RSV) "
                                "VALUES (:cin, :id_rdv, :vaccin_rdv,  TO_DATE(:dispo, 'YYYY-MM-DD HH24:MI:SS'))");
    insertReserverQuery.bindValue(":id_rdv", newRdvId);
    insertReserverQuery.bindValue(":vaccin_rdv", trimmedVaccin);
    insertReserverQuery.bindValue(":cin", CINi);
    insertReserverQuery.bindValue(":dispo", trimmedDispo);

    if (!insertReserverQuery.exec()) {
        db.rollback();
        QMessageBox::warning(nullptr, "Erreur",
                             "Échec de l'enregistrement de la réservation:\n" + insertReserverQuery.lastError().text(),
                             QMessageBox::Ok);
        return false;
    }

    // Commit transaction if everything succeeded
    if (!db.commit()) {
        QMessageBox::warning(nullptr, "Erreur", "Échec de la validation des modifications.", QMessageBox::Ok);
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Le rendez-vous a été enregistré avec succès.");
    return true;
}
void rendez_vous::modifier_rdv(int CINi, const QString &vaccin, const QString &date_rdvNaiss, const QString &adresse,
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
    query.bindValue(":CIN", CINi);
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
    liste_att->clear(); // Clear the list before loading new items
    liste_att->addItem("CIN           Nom   Prenom        Vaccin"); // Add header
    qDebug() << "Loading appointments...";

    QSqlQuery query("SELECT ID_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV FROM RENDEZ_VOUS");
    while (query.next()) {
        QString CIN = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString vaccin = query.value(3).toString();

        // Create a new QListWidgetItem
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(CIN + "    " + nom + "     " + prenom + "         " + vaccin); // Display text

        // Store additional data using setData()
        item->setData(Qt::UserRole, CIN);          // Store CIN (ID_RDV)
        item->setData(Qt::UserRole + 1, nom);      // Store Nom
        item->setData(Qt::UserRole + 2, prenom);  // Store Prenom
        item->setData(Qt::UserRole + 3, vaccin);  // Store Vaccin

        // Add the item to the list
        liste_att->addItem(item);
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

void rendez_vous::generateCertificate(int id_rdv)
{
    QSqlQuery query;


    query.prepare("SELECT NOM_RDV,PRENOM_RDV, VACCIN_RDV, DATE_RDV, DATENAISS_RDV FROM RENDEZ_VOUS WHERE ID_RDV = :id_rdv");
    query.bindValue(":id_rdv", id_rdv);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de récupérer les informations du rendez-vous.");
        qDebug() << "SQL Error: " << query.lastError().text();
        return;
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    QString vaccin_ref = query.value(2).toString();

    QString date_naissp = query.value(4).toDate().toString("yyyy-MM-dd");

    QDateTime dateTimeRdv = query.value(3).toDateTime();
    QString date_rdv = dateTimeRdv.toString("yyyy-MM-dd HH:mm");



    query.prepare("SELECT CIN_PASSP FROM PATIENT WHERE NOM = :nom");
    query.bindValue(":nom", nom);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(nullptr, "Erreur", "récupération des données a échoué !");
        qDebug() << "SQL Error: " << query.lastError().text();
        return;
    }

    QString CIN = query.value(0).toString();



    query.prepare("SELECT DOSE, TYPE FROM VACCIN WHERE NOM = :vaccin_ref");
    query.bindValue(":vaccin_ref", vaccin_ref);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(nullptr, "Erreur", "récupération des données a échoué !");
        qDebug() << "SQL Error: " << query.lastError().text();
        return;
    }


    QString dose_vaccin = query.value(0).toString();
    QString type_vaccin = query.value(1).toString();

    // Generate the HTML certificate
    QString html = QString(R"(
    <!DOCTYPE html>
    <html lang="fr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Certificat de Confirmation de Vaccination</title>
        <style>
            body { font-family: Arial, sans-serif; margin: 20px; padding: 20px; border: 2px solid #000; max-width: 600px; text-align: center; position: relative; }
            h1 { color: #333; margin-bottom: 20px; }
            .section { margin-bottom: 15px; text-align: left; }
            .section p { margin: 5px 0; }
            .footer { margin-top: 20px; font-size: 0.9em; color: #666; }
            .consent { font-style: italic; margin-top: 15px; padding: 10px; background-color: #f9f9f9; border: 1px solid #ddd; }
            .important-info { margin-top: 15px; padding: 10px; border: 1px solid #ffeeba; border-radius: 5px; }
        </style>
    </head>
    <body>

        <h1>Certificat de Confirmation de Vaccination</h1>
        <div class="section">
            <h2>Informations du Patient</h2>
            <p><strong>Nom :</strong> %1</p>
            <p><strong>Prénom :</strong> %2</p>
            <p><strong>Date de naissance :</strong> %3</p>
            <p><strong>Identifiant unique (CIN/Passeport) :</strong> %4</p>
        </div>

        <div class="section">
            <h2>Détails de la Vaccination</h2>
            <p><strong>Nom du vaccin :</strong> %5</p>
            <p><strong>Date d'administration :</strong> %6</p>
            <p><strong>Dose :</strong> %7</p>
            <p><strong>Centre de vaccination :</strong> SmartVax Center</p>
        </div>

        <div class="section">
            <h2>Informations du Professionnel de Santé</h2>
            <p><strong>Nom :</strong> -----------------</p>
            <p><strong>Signature :</strong> _______________</p>
        </div>

        <div class="consent">
            <p>
                Le patient éligible, ou son représentant légal, a accepté d’entrer dans le processus de vaccination après avoir reçu toutes les informations nécessaires à son choix éclairé.
            </p>
        </div>

        <div class="important-info">
            <p><strong>En cas d'effets indésirables, veuillez nous informer immédiatement.</strong></p>
        </div>

        <div class="footer">
            <h3>Informations Importantes</h3>
            <p>Ce certificat est une preuve officielle de vaccination. Veuillez le conserver précieusement.</p>
            <p>Il est recommandé de garder une copie électronique ou papier de ce certificat pour référence future.
            En cas de perte, veuillez contacter le centre de vaccination pour obtenir une copie.</p>
            <p>Pour toute question, contactez-nous à :</p>
            <p><strong>Email :</strong> SmartVax@yahoo.com | <strong>Téléphone :</strong> +216 21 276 002</p>
        </div>
    </body>
    </html>
    )").arg(nom, prenom, date_naissp, CIN, vaccin_ref, date_rdv, dose_vaccin);
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    QTextDocument doc;
    doc.setHtml(html);
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Enregistrer le certificat",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/certificat_vaccination.pdf",
                                                    "PDF Files (*.pdf)");

    if (!filePath.isEmpty()) {
        printer.setOutputFileName(filePath);
    }




    doc.print(&printer);

    QMessageBox::information(nullptr, "Succès", "Le certificat de vaccination a été généré avec succès !");
}


