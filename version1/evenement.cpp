#include "evenement.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextDocument>
#include <QDate>


Evenement::Evenement(QObject *parent) : QObject(parent) {}

void Evenement::loadEventData(QTableWidget *tabevent) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    tabevent->setRowCount(0);
    int row = 0;
    while (query.next()) {
        tabevent->insertRow(row);
        for (int col = 0; col < 7; col++) {
            tabevent->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    tabevent->resizeRowsToContents();
}

void Evenement::saveEventData(int id_event, const QString &nom_event, const QString &nom_responsable,
                              const QString &type, const QString &date_heure,
                              const QString &lieu, int nbr_participant) {
    // Vérifier si la base de données est connectée
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    // Vérification des champs vides
    if (nom_event.isEmpty() || nom_responsable.isEmpty() || type.isEmpty() ||
        date_heure.isEmpty() || lieu.isEmpty()) {
        QMessageBox::warning(nullptr, "Champs obligatoires", "Veuillez remplir tous les champs.");
        return;
    }

    // Vérification du nombre de participants (doit être >= 1)
    if (nbr_participant <= 0) {
        QMessageBox::warning(nullptr, "Nombre de participants invalide", "Le nombre de participants doit être supérieur à zéro.");
        return;
    }


    // Préparer la requête SQL
    QSqlQuery query;
    query.prepare("INSERT INTO EVENTS (ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE) "
                  "VALUES (:id_event, :nom_event, :type, TO_DATE(:date_heure, 'MM-DD-YYYY HH24:MI:SS'), :lieu, :nbr_participant, :nom_responsable)");

    query.bindValue(":id_event", id_event);
    query.bindValue(":nom_event", nom_event);
    query.bindValue(":type", type);
    query.bindValue(":date_heure", date_heure);
    query.bindValue(":lieu", lieu);
    query.bindValue(":nbr_participant", nbr_participant);
    query.bindValue(":nom_responsable", nom_responsable);

    // Exécuter la requête
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'ajout de l'événement : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "L'événement a été enregistré avec succès.");
    }
}


void Evenement::supprimerEvenement(int id_event) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM EVENTS WHERE ID_EVENT = :id_event");
    query.bindValue(":id_event", id_event);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression de l'événement : " + query.lastError().text());
    } else {
        QMessageBox::information(nullptr, "Succès", "L'événement a été supprimé avec succès.");
    }
}


bool Evenement::recupererEvenement(int id, QString &nom, QString &responsable, QString &type, QString &lieu, int &participants, QDateTime &dateHeure) {
    QSqlQuery query;
    query.prepare("SELECT NOM_EVENT, NOM_RESPONSABLE, TYPE, LIEU, NBR_PARTICIPANT, DATE_HEURE FROM EVENTS WHERE ID_EVENT = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        nom = query.value(0).toString();
        responsable = query.value(1).toString();
        type = query.value(2).toString();
        lieu = query.value(3).toString();
        participants = query.value(4).toInt();
        dateHeure = query.value(5).toDateTime();
        return true;
    }
    return false;
}

bool Evenement::modifierEvenement(int id, const QString &nom, const QString &responsable, const QString &type, const QString &lieu, int participants, const QDateTime &dateHeure) {
    // Vérifier si la date est valide
    if (!dateHeure.isValid()) {
        qDebug() << "Date invalide :" << dateHeure;
        return false;
    }

    // Vérifier si la date n'est pas dans le passé
    if (dateHeure < QDateTime::currentDateTime()) {
        qDebug() << "La date ne peut pas être dans le passé :" << dateHeure;
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE EVENTS SET NOM_EVENT = :nom, NOM_RESPONSABLE = :responsable, TYPE = :type, LIEU = :lieu, NBR_PARTICIPANT = :participants, DATE_HEURE = :dateHeure WHERE ID_EVENT = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":responsable", responsable);
    query.bindValue(":type", type);
    query.bindValue(":lieu", lieu);
    query.bindValue(":participants", participants);
    query.bindValue(":dateHeure", dateHeure);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la mise à jour de l'événement :" << query.lastError().text();
        return false;
    }

    return true;
}

void Evenement::searchEventData(QTableWidget *tabevent, const QString &searchTerm) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    // Ajout du critère de recherche pour NOM_RESPONSABLE
    QString queryString = "SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS "
                          "WHERE NOM_EVENT LIKE '%" + searchTerm + "%' OR "
                                         "TYPE LIKE '%" + searchTerm + "%' OR "
                                         "LIEU LIKE '%" + searchTerm + "%' OR "
                                         "NOM_RESPONSABLE LIKE '%" + searchTerm + "%'";  // Nouvelle condition

    query.prepare(queryString);
    qDebug() << "Requête exécutée : " << queryString;

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la recherche : " + query.lastError().text());
        return;
    }

    tabevent->setRowCount(0);
    int row = 0;
    while (query.next()) {
        qDebug() << "Résultat : " << query.value(1).toString();  // Debugging

        tabevent->insertRow(row);
        for (int col = 0; col < 7; col++) {
            tabevent->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    tabevent->resizeRowsToContents();
}

void Evenement::sortEventData(QTableWidget *tabevent, const QString &critere) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    QString queryString = "SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS ORDER BY ";

    if (critere == "Date") {
        queryString += "DATE_HEURE ASC";
    } else if (critere == "Participants") {
        queryString += "NBR_PARTICIPANT DESC";
    } else if (critere == "Nom") {
        queryString += "NOM_EVENT ASC";
    } else {
        QMessageBox::warning(nullptr, "Critère invalide", "Veuillez choisir un critère valide.");
        return;
    }

    query.prepare(queryString);

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec du tri des données : " + query.lastError().text());
        return;
    }

    tabevent->setRowCount(0);
    int row = 0;
    while (query.next()) {
        tabevent->insertRow(row);
        for (int col = 0; col < 7; col++) {
            tabevent->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    tabevent->resizeRowsToContents();
}

QString imageToBase64(const QString &imagePath) {
    QFile file(imagePath);
    if (!file.open(QIODevice::ReadOnly)) return "";
    QByteArray imageData = file.readAll();
    return "data:image/png;base64," + imageData.toBase64();
}

void Evenement::exporterInvitationPDF(QTableWidget *tabevent) {
    QModelIndexList selection = tabevent->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(nullptr, "Sélection requise", "Veuillez sélectionner un événement.");
        return;
    }

    bool ok;
    QString nomDestinataire = QInputDialog::getText(nullptr, "Nom du destinataire",
                                                    "Entrez le nom du destinataire :", QLineEdit::Normal, "", &ok);
    if (!ok || nomDestinataire.isEmpty()) {
        QMessageBox::warning(nullptr, "Nom requis", "Veuillez entrer un nom valide.");
        return;
    }

    int row = selection.first().row();
    QString nomEvent = tabevent->item(row, 1)->text();
    QString typeEvent = tabevent->item(row, 2)->text();
    QString dateHeure = tabevent->item(row, 3)->text();
    QString lieu = tabevent->item(row, 4)->text();
    QString responsable = tabevent->item(row, 6)->text();
    QString nbrParticipants = tabevent->item(row, 5)->text();
    QString dateAujourdhui = QDate::currentDate().toString("dd/MM/yyyy");

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Enregistrer l'invitation", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    writer.setTitle("Invitation - SmartVax");
    QTextDocument doc;

    QString logoBase64 = imageToBase64(":/logo1.png");
    QString signatureBase64 = imageToBase64(":/signature.png");

    QString html = "<!DOCTYPE html>"
                   "<html lang='fr'>"
                   "<head>"
                   "<meta charset='UTF-8'>"
                   "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                   "<title>Invitation Officielle - SmartVax</title>"
                   "<style>"
                   "body { font-family: Arial, sans-serif; padding: 40px; margin: 0; background-color: white; }"

                   "#container { border: 3px solid #4CAF50; padding: 20px; width: 100%; max-width: 700px; margin: auto; "
                   "background: white; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); border-radius: 10px; }"

                   "#header { display: flex; align-items: center; justify-content: space-between; "
                   "border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }"

                   "#header h1 { font-size: 24px; color: #4CAF50; margin: 0; font-weight: bold; }"

                   "#title { font-size: 22px; font-weight: bold; color: #4CAF50; margin: 20px 0; text-decoration: underline; text-align: center; }"

                   "#info { text-align: left; font-size: 16px; margin-top: 20px; line-height: 1.8; min-height: 400px; }"
                   "#info p { margin-bottom: 15px; white-space: pre-line; }"

                   "#signature-container { display: flex; justify-content: space-between; align-items: center; margin-top: 60px; }"
                   "#signature { text-align: right; font-style: italic; font-size: 16px; }"

                   "#spacer { height: 50px; }"

                   "#footer { font-size: 14px; text-align: center; color: black; padding: 15px; border-top: 2px solid #4CAF50; margin-top: auto; }"
                   "</style>"
                   "</head>"
                   "<body>"

                   "<div id='container'>"

                   "<div id='header'>"
                   "<img src='" + logoBase64 + "' alt='Logo SmartVax' width: 30px; height: 30px; position: absolute; top: 10px; left: 10px; >"

                                  "<h1>SmartVax</h1>"
                                  "</div>"

                                  "<div id='title'>Invitation Officielle</div>"

                                  "<div id='info'>"
                                  "<p>Cher(e) <strong>" + nomDestinataire + "</strong>,</p>"
                                       "<p>Nous avons le plaisir de vous inviter à l'événement suivant :</p>"
                                       "<p><strong>Événement :</strong> " + nomEvent + "</p>"
                                "<p><strong>Type :</strong> " + typeEvent + "</p>"
                                 "<p><strong>Date et Heure :</strong> " + dateHeure + "</p>"
                                 "<p><strong>Lieu :</strong> " + lieu + "</p>"
                            "<p><strong>Responsable :</strong> " + responsable + "</p>"
                                   "<p><strong>Nombre de participants :</strong> " + nbrParticipants + " personnes</p>"
                                       "</div>"

                                       "<div id='signature-container'>"
                                       "<div></div>"
                                       "<div id='signature'>"
                                       "Signature du centre :<br><br>"
                                       "<img src='" + signatureBase64 + "' alt='Signature du centre' width='150'>"
                                       "<div><strong>" + dateAujourdhui + " - Ariana</strong></div>"
                                      "</div>"
                                      "</div>"

                                      "<div id='spacer'></div>"

                                      "<div id='footer'>"
                                      "<p>Adresse : Ariana 2081</p>"
                                      "<p>Email : SmartVax@yahoo.com | Téléphone : +216 21 276 002</p>"
                                      "</div>"

                                      "</div>"

                                      "</body>"
                                      "</html>";




    doc.setHtml(html);
    doc.setPageSize(QSizeF(QPageSize(QPageSize::A4).rectPixels(96).size()));
    doc.print(&writer);





    QMessageBox::information(nullptr, "Exportation réussie", "L'invitation a été enregistrée avec succès !");
}

//"<img src=':/logo.png' alt='Logo SmartVax'>"
//"<img src=':/signature.png' alt=\"Signature du centre\">"
