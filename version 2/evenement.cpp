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
#include <QSqlError>
#include <QDebug>
#include <QMap>
#include <QSslSocket>
#include <QMimeDatabase>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

// Liste des coordonnées des gouvernorats tunisiens
QMap<QString, QPair<double, double>> gouvernoratCoordonnees = {
    {"Tunis", {36.8065, 10.1815}}, {"Sfax", {34.7396, 10.7603}},
    {"Sousse", {35.8256, 10.636}}, {"Ariana", {36.8665, 10.1647}},
    {"Ben Arous", {36.7531, 10.2189}}, {"Bizerte", {37.2744, 9.8739}},
    {"Gabes", {33.8815, 10.0982}}, {"Gafsa", {34.425, 8.7842}},
    {"Jendouba", {36.5011, 8.7792}}, {"Kairouan", {35.6781, 10.0963}},
    {"Kasserine", {35.167, 8.836}}, {"Kébili", {33.705, 8.9734}},
    {"Le Kef", {36.174, 8.704}}, {"Mahdia", {35.5047, 11.0622}},
    {"Manouba", {36.8089, 10.1019}}, {"Medenine", {33.3549, 10.5053}},
    {"Monastir", {35.7643, 10.8113}}, {"Nabeul", {36.4561, 10.7376}},
    {"Sidi Bouzid", {35.0382, 9.4853}}, {"Siliana", {36.0849, 9.3748}},
    {"Tataouine", {32.929, 10.4518}}, {"Tozeur", {33.9205, 8.1339}},
    {"Zaghouan", {36.402, 10.1478}},{"Béja", {36.7256, 9.1817}},
    {"Djerba", {33.877, 10.848}}
};

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
            QString value;

            if (col == 3) { // DATE_HEURE est à la colonne 3
                QDateTime dateTime = QDateTime::fromString(query.value(col).toString(), Qt::ISODate);
                value = dateTime.toString("dd/MM/yyyy HH:mm"); // Format lisible
            } else {
                value = query.value(col).toString();
            }

            tabevent->setItem(row, col, new QTableWidgetItem(value));
        }
        row++;
    }

    tabevent->resizeRowsToContents();
}



void Evenement::saveEventData(int id_event, const QString &nom_event, const QString &nom_responsable,
                              const QString &type, const QString &date_heure,
                              const QString &lieu, int nbr_participant) {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    // Vérification des champs obligatoires (chaînes de caractères)
    if (nom_event.trimmed().isEmpty() || nom_responsable.trimmed().isEmpty() ||
        type.trimmed().isEmpty() || date_heure.trimmed().isEmpty() || lieu.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Champs vides", "Tous les champs doivent être remplis.");
        return;
    }

    // Vérification que l'ID est un nombre positif
    if (id_event <= 0) {
        QMessageBox::warning(nullptr, "ID invalide", "L'ID de l'événement doit être un nombre positif.");
        return;
    }

    // Vérifier que le nombre de participants est un nombre valide et positif
    if (nbr_participant <= 0) {
        QMessageBox::warning(nullptr, "Nombre de participants invalide", "Le nombre de participants doit être un nombre positif.");
        return;
    }

    // Vérifier que les chaînes de caractères contiennent seulement des lettres et espaces
    QRegularExpression regexNom("^[A-Za-zÀ-ÿ '-]+$"); // Accepte lettres, accents, espaces, apostrophes et tirets

    if (!regexNom.match(nom_event).hasMatch() || !regexNom.match(nom_responsable).hasMatch() ||
        !regexNom.match(type).hasMatch() || !regexNom.match(lieu).hasMatch()) {
        QMessageBox::warning(nullptr, "Saisie invalide", "Les champs texte ne doivent contenir que des lettres.");
        return;
    }

    // Convertir QString en QDateTime
    QDateTime date_mevent = QDateTime::fromString(date_heure, "yyyy-MM-dd HH:mm:ss");

    if (!date_mevent.isValid()) {
        QMessageBox::warning(nullptr, "Format de date invalide", "Veuillez saisir une date valide.");
        return;
    }

    // Vérification que la date est dans le futur
    if (date_mevent <= QDateTime::currentDateTime()) {
        QMessageBox::warning(nullptr, "Date invalide", "La date doit être dans le futur.");
        return;
    }

    // Préparer la requête SQL
    QSqlQuery query;
    query.prepare("INSERT INTO EVENTS (ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE) "
                  "VALUES (:id_event, :nom_event, :type, TO_DATE(:date_heure, 'YYYY-MM-DD HH24:MI:SS'), :lieu, :nbr_participant, :nom_responsable)");

    query.bindValue(":id_event", id_event);
    query.bindValue(":nom_event", nom_event);
    query.bindValue(":type", type);
    query.bindValue(":date_heure", date_mevent.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":lieu", lieu);
    query.bindValue(":nbr_participant", nbr_participant);  // Il est déjà un int, pas besoin de le convertir
    query.bindValue(":nom_responsable", nom_responsable);

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

    // Étape 1 : Récupérer les informations de l'événement à supprimer
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_HEURE, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS WHERE ID_EVENT = :id_event");
    selectQuery.bindValue(":id_event", id_event);

    if (!selectQuery.exec() || !selectQuery.next()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération de l'événement à supprimer : " + selectQuery.lastError().text());
        return;
    }

    // Extraire les données de l'événement
    QString idEvent = selectQuery.value(0).toString();
    QString nomEvent = selectQuery.value(1).toString();
    QString type = selectQuery.value(2).toString();
    QString dateHeure = selectQuery.value(3).toString();
    QString lieu = selectQuery.value(4).toString();
    QString nbrParticipant = selectQuery.value(5).toString();
    QString nomResponsable = selectQuery.value(6).toString();

    // Étape 2 : Envoyer l'email avec les informations de l'événement
    sendEmail2(idEvent, nomEvent, type, dateHeure, lieu, nbrParticipant, nomResponsable);

    // Étape 3 : Supprimer l'événement
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM EVENTS WHERE ID_EVENT = :id_event");
    deleteQuery.bindValue(":id_event", id_event);

    if (!deleteQuery.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression de l'événement : " + deleteQuery.lastError().text());
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

QList<EvenementData> Evenement::chargerEvenements() {
    QList<EvenementData> evenements;

    // Vérification de la connexion
    if (!QSqlDatabase::database().isOpen()) {
        qWarning() << "La base de données n'est pas connectée.";
        return evenements;
    }
    QSqlQuery query;
    query.prepare("SELECT ID_EVENT, NOM_EVENT, LIEU, NOM_RESPONSABLE, DATE_HEURE FROM EVENTS");
    // Vérifier si la requête a été exécutée correctement
    if (!query.exec()) {
        qWarning() << "Erreur SQL : " << query.lastError().text();
        return evenements;  // Retourner une liste vide en cas d'erreur
    }
    // Vérifier si des résultats sont renvoyés par la requête
    if (!query.first()) {
        qWarning() << "Aucun événement trouvé dans la base de données.";
        return evenements;  // Retourner une liste vide si aucun événement n'est trouvé
    }
    // Traiter les résultats
    do {
        EvenementData event;
        event.id = query.value(0).toInt();
        event.nom = query.value(1).toString();
        event.lieu = query.value(2).toString();
        event.resp = query.value(3).toString();
        event.date_heure = QDateTime::fromString(query.value(4).toString(), Qt::ISODate)
                               .toString("dd/MM/yyyy HH:mm");


        // Vérifier si le lieu existe dans la liste des gouvernorats
        if (gouvernoratCoordonnees.contains(event.lieu)) {
            event.latitude = gouvernoratCoordonnees[event.lieu].first;
            event.longitude = gouvernoratCoordonnees[event.lieu].second;
        } else {
            event.latitude = 0;
            event.longitude = 0;
            qWarning() << "Aucun gouvernorat trouvé pour :" << event.lieu;
        }

        evenements.append(event);  // Ajouter l'événement à la liste
    } while (query.next());  // Passer à l'événement suivant

    return evenements;  // Retourner la liste des événements
}

QMap<QString, int> Evenement::compterEvenementsParType() {
    QMap<QString, int> stats;
    QSqlQuery query("SELECT TYPE, COUNT(*) FROM EVENTS GROUP BY TYPE");

    while (query.next()) {
        QString type = query.value(0).toString();
        int count = query.value(1).toInt();
        stats[type] = count;
    }

    return stats;
}

QMap<QString, int> Evenement::repartitionParRegion() {
    QMap<QString, int> stats;  // QMap pour stocker les régions et le nombre d'événements

    // Dictionnaire de mappage des villes aux sous-régions géographiques
    QMap<QString, QString> lieuVersRegion;

    // **Nord-Est**
    lieuVersRegion["Tunis"] = "Nord-Est";
    lieuVersRegion["Ariana"] = "Nord-Est";
    lieuVersRegion["Nabeul"] = "Nord-Est";
    lieuVersRegion["Zaghouan"] = "Nord-Est";
    lieuVersRegion["Manouba"] = "Nord-Est";
    lieuVersRegion["Ben Arous"] = "Nord-Est";

    // **Nord-Ouest**
    lieuVersRegion["Bizerte"] = "Nord-Ouest";
    lieuVersRegion["Béja"] = "Nord-Ouest";
    lieuVersRegion["Jendouba"] = "Nord-Ouest";
    lieuVersRegion["Le Kef"] = "Nord-Ouest";
    lieuVersRegion["Siliana"] = "Nord-Ouest";

    // **Centre Est**
    lieuVersRegion["Sousse"] = "Centre Est";
    lieuVersRegion["Mahdia"] = "Centre Est";
    lieuVersRegion["Monastir"] = "Centre Est";
    lieuVersRegion["Sfax"] = "Centre Est";
    // **Centre Ouest**
    lieuVersRegion["Kairouan"] = "Centre Ouest";
    lieuVersRegion["Sidi Bouzid"] = "Centre Ouest";
    lieuVersRegion["Kasserine"] = "Centre Ouest";
    lieuVersRegion["Gafsa"] = "Centre Ouest";

    // **Sud Est**
    lieuVersRegion["Gabes"] = "Sud Est";
    lieuVersRegion["Médenine"] = "Sud Est";
    lieuVersRegion["Tataouine"] = "Sud Est";
    lieuVersRegion["Djerba"] = "Sud Est";

    // **Sud Ouest**
    lieuVersRegion["Kebili"] = "Sud Ouest";
    lieuVersRegion["Tozeur"] = "Sud Ouest";  // Tozeur peut être dans Sud Ouest, mais déjà dans Sud Est

    // Récupérer les lieux depuis la base de données
    QSqlQuery query("SELECT LIEU FROM EVENTS");

    while (query.next()) {
        QString lieu = query.value(0).toString();

        // Déterminer la région du lieu
        QString region = lieuVersRegion.value(lieu, "Autres");  // Par défaut, "Autres" si lieu non mappé

        // Ajouter la région au compteur
        stats[region] = stats.value(region, 0) + 1;
    }

    return stats;
}

void Evenement::sendEmail2(const QString &eventId, const QString &eventName, const QString &eventType,
                           const QString &eventDateTime, const QString &eventLocation,
                           const QString &participantCount, const QString &responsibleName) {
    // Configuration du serveur SMTP (Gmail)
    QString smtpServer = "smtp.gmail.com";
    int smtpPort = 465;  // Connexion SSL
    QString senderEmail = "smartvax6@gmail.com";
    QString senderPassword = "vgxl glgw xxzg jwup";  // Mot de passe d'application
    QString receiverEmail = "yassinebenaissa2004@gmail.com";
    QString subject = "Annulation de l'événement";
    QString body = "Objet : Annulation de l'événement \"" + eventName + "\"\n\n"
                                                                        "Bonjour,\n\n"
                                                                        "Nous vous informons par la présente que l'événement suivant a été annulé. Veuillez trouver ci-dessous les détails :\n\n"
                                                                        "Identifiant de l'événement : " + eventId + "\n"
                               "Nom de l'événement : " + eventName + "\n"
                                 "Type : " + eventType + "\n"
                                 "Date et heure prévues : " + eventDateTime + "\n"
                                     "Lieu : " + eventLocation + "\n"
                                     "Nombre de participants : " + participantCount + "\n"
                                        "Responsable  : " + responsibleName + "\n\n"
                                       "Nous vous prions de bien vouloir nous excuser pour la gêne occasionnée par cette annulation.\n\n"
                                       "Nous restons à votre disposition pour toute information complémentaire.\n\n"
                                       "Cordialement,\n"
                                       "L’équipe Smart Vax";



    // Connexion sécurisée au serveur SMTP
    QSslSocket* socket = new QSslSocket();
    socket->connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket->waitForConnected(5000)) {
        QMessageBox::warning(nullptr, "Échec de connexion", "Impossible de se connecter au serveur SMTP.");
        delete socket;
        return;
    }

    // Authentification et dialogue SMTP
    QByteArray emailContent;
    emailContent.append("EHLO localhost\r\n");
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    QByteArray serverResponse = socket->readAll();
    qDebug() << "EHLO Response: " << serverResponse;

    emailContent = "AUTH LOGIN\r\n";
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();

    emailContent = QByteArray(senderEmail.toUtf8()).toBase64() + "\r\n";
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();

    emailContent = QByteArray(senderPassword.toUtf8()).toBase64() + "\r\n";
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();

    emailContent = "MAIL FROM: <" + senderEmail.toUtf8() + ">\r\n";
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();

    emailContent = "RCPT TO: <" + receiverEmail.toUtf8() + ">\r\n";
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();

    emailContent = "DATA\r\n";
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();

    emailContent.clear();
    emailContent.append("Subject: " + subject.toUtf8() + "\r\n");
    emailContent.append("To: " + receiverEmail.toUtf8() + "\r\n");
    emailContent.append("MIME-Version: 1.0\r\n");
    emailContent.append("Content-Type: text/plain; charset=\"UTF-8\"\r\n\r\n");
    emailContent.append(body.toUtf8() + "\r\n.\r\n");

    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();

    socket->write("QUIT\r\n");
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();

    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
    }

    QMessageBox::information(nullptr, "E-mail envoyé", "L'e-mail a été envoyé avec succès.");
    delete socket;
}
