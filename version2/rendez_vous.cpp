
#include "rendez_vous.h"
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include "qpainter.h"
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


bool rendez_vous::saveAppointment(const QString &CIN, const QString &vaccin, const QString &date_rdvNaiss, const QString &adresse,
                                  const QString &nom, const QString &prenom, const QString &dispo,
                                  const QString &medecin, const QString &infirmier, const QString &salle,
                                  double facturation) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "Database is not connected.");

    }

    QString trimmedDispo = dispo.trimmed();
    QString trimmedDateNaiss = date_rdvNaiss.trimmed();

    qDebug() << "Formatted DateTime:" << trimmedDispo;
    qDebug() << "Formatted Birth Date:" << trimmedDateNaiss;

    QSqlQuery query;
    int CINi = CIN.toInt();
    if (CIN.length()!=7){
        QMessageBox::warning(nullptr, "Erreur", "CIN doit avoir 7 chiffres",QMessageBox::Ok);
        return false;
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
            return false;
        } else {
            QMessageBox::information(nullptr, "Success", "Les informations ont été sauvegrdés");
            return true;
        }
    }}
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
CustomCalendar::CustomCalendar(QWidget *parent) : QCalendarWidget(parent) {}

void CustomCalendar::addEvent(QDate date, QString text) {
    if (date.isValid()) {

        for (const SpecialDate &sd : specialDates) {
            if (sd.date == date) return;
        }
        specialDates.append({date, text});
        updateCell(date);
    }
}

void CustomCalendar::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const {
    QCalendarWidget::paintCell(painter, rect, date);

    for (const SpecialDate &sd : specialDates) {
        if (sd.date == date) {
            painter->setPen(Qt::red);
            painter->drawText(rect, Qt::AlignBottom | Qt::AlignHCenter, sd.text);
            break;
        }
    }
}


