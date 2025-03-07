#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rendez_vous.h"
#include <QPixmap>
#include <QGraphicsOpacityEffect>
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QInputDialog>
#include <QDebug>

Connection::Connection() {}

bool Connection::createconnect() {
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projetc++"); // Insérer le nom de la source de données
    db.setUserName("Mariem"); // Insérer le nom d'utilisateur
    db.setPassword("2004"); // Insérer le mot de passe pour cet utilisateur

    if (db.open())
        test = true;

    return test;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , vaccinManager(new Vaccin(this))
{
    ui->setupUi(this);
    rdvWindow = new rendez_vous(this);
    vaccinTab = ui->vaccin;
    ajoutvac = ui->ajoutvac;
    vaccinB = ui->vaccinB;
    ajoutevent = ui->ajoutevent;
    eventB = ui->eventB;
    rdv = ui->rdv;
    ajoutequi = ui->ajoutequi;
    equiB = ui->equiB;
    Quit = ui->Quit;
    Quit4 = ui->Quit_4;
    ajoutrdv = ui->ajoutrdv;
    delete_vac = ui->delete_vac;
    edit_vac = ui->edit_vac; // Initialiser le bouton de modification
    save_vac_2 = ui->save_vac_2; // Initialiser le bouton d'enregistrement pour les données modifiées

    vaccinTab->setStyleSheet(
        "QTabWidget::pane { border: none; background: transparent; }"
        "QTabBar::tab { height: 0; width: 0; }"
        "QTabBar { height: 0; }"
        );

    // Assurez-vous que cette connexion n'est faite qu'une seule fois
    connect(edit_vac, &QPushButton::clicked, this, &MainWindow::on_edit_vac_2_clicked);
    connect(save_vac_2, &QPushButton::clicked, this, &MainWindow::on_save_2_clicked);

    // Connecter le signal dateChanged pour les deux éditeurs de date
    connect(ui->date_vac, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);
    connect(ui->date_vac_2, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);

    // Autres connexions...
    connect(ajoutvac, &QPushButton::clicked, this, &MainWindow::onajoutvacclicked);
    connect(vaccinB, &QPushButton::clicked, this, &MainWindow::onvaccinBclicked);
    connect(ajoutevent, &QPushButton::clicked, this, &MainWindow::onajouteventclicked);
    connect(eventB, &QPushButton::clicked, this, &MainWindow::oneventBclicked);
    connect(ajoutequi, &QPushButton::clicked, this, &MainWindow::on_ajoutequi_clicked);
    connect(equiB, &QPushButton::clicked, this, &MainWindow::on_equiB_clicked);
    connect(rdv, &QPushButton::clicked, this, &MainWindow::on_rdv_clicked);
    connect(Quit, &QPushButton::clicked, this, &MainWindow::on_Quit_clicked);
    connect(Quit4, &QPushButton::clicked, this, &MainWindow::on_Quit_4_clicked);
    connect(ajoutrdv, &QPushButton::clicked, this, &MainWindow::on_ajoutrdv_clicked);
    connect(delete_vac, &QPushButton::clicked, this, &MainWindow::on_delete_2_clicked);

    QLabel *main = ui->main;
    QPixmap pixmap(":/logo.png");
    main->setPixmap(pixmap);
    main->setAlignment(Qt::AlignCenter);

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect();
    opacityEffect->setOpacity(0.5);
    main->setGraphicsEffect(opacityEffect);

    // Feuille de style pour les widgets de tableau
    QTableWidget *tabvaccin = ui->tabvaccin;
    tabvaccin->setColumnWidth(0, 185);
    tabvaccin->setColumnWidth(1, 185);
    tabvaccin->setColumnWidth(2, 185);
    tabvaccin->setColumnWidth(3, 185);
    tabvaccin->setColumnWidth(4, 185);
    tabvaccin->setColumnWidth(5, 185);

    QTableWidget *tabevent = ui->tabevent;
    tabevent->setColumnWidth(0, 165);
    tabevent->setColumnWidth(1, 165);
    tabevent->setColumnWidth(2, 165);
    tabevent->setColumnWidth(3, 165);
    tabevent->setColumnWidth(4, 165);
    tabevent->setColumnWidth(5, 165);
    tabevent->setColumnWidth(6, 165);
    tabevent->setColumnWidth(7, 165);

    QTableWidget *tabequi = ui->tabequi;
    tabequi->setColumnWidth(0, 165);
    tabequi->setColumnWidth(1, 165);
    tabequi->setColumnWidth(2, 165);
    tabequi->setColumnWidth(3, 165);

    // Définir la feuille de style pour les widgets de tableau
    tabvaccin->setStyleSheet(
        "QTableWidget {"
        " background-color:rgb(63, 123, 134);"
        "}"
        "QHeaderView {"
        "    background-color: rgb(63, 123, 134);"
        "}"
        "QHeaderView::section {"
        "    background-color:#bae5e8;"
        "    color: black;"
        "    font-size: 16px;"
        "    padding: 6px;"
        "}"
        "QTableWidget::item {"
        "    padding: 6px;"
        "}"
        );
    tabvaccin->resizeRowsToContents();

    tabevent->setStyleSheet(
        "QTableWidget {"
        "    background-color: rgb(63, 123, 134);"
        "    border: 2px solid #000000;"
        "    border-radius: 15px;"
        "    gridline-color: #d3d3d3;"
        "    font-size: 14px;"
        "    padding: 5px;"
        "}"
        "QHeaderView {"
        "    background-color: rgb(63, 123, 134);"
        "}"
        "QHeaderView::section {"
        "    background-color: #b0c4de;"
        "    border: 1px solid #d3d3d3;"
        "    color: black;"
        "    font-size: 16px;"
        "    padding: 6px;"
        "}"
        "QTableWidget::item {"
        "    padding: 6px;"
        "}"
        );
    tabevent->resizeRowsToContents();

    tabequi->setStyleSheet(
        "QTableWidget {"
        "    background-color: rgb(63, 123, 134);"
        "    border: 2px solid #000000;"
        "    border-radius: 15px;"
        "    gridline-color: #d3d3d3;"
        "    font-size: 14px;"
        "    padding: 5px;"
        "}"
        "QHeaderView {"
        "    background-color: rgb(63, 123, 134);"
        "}"
        "QHeaderView::section {"
        "    background-color: #b0c4de;"
        "    border: 1px solid #d3d3d3;"
        "    color: black;"
        "    font-size: 16px;"
        "    padding: 6px;"
        "}"
        "QTableWidget::item {"
        "    padding: 6px;"
        "}"
        );
    tabequi->resizeRowsToContents();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onajoutvacclicked() {
    vaccinTab->setCurrentIndex(1);
}

void MainWindow::onvaccinBclicked() {
    vaccinTab->setCurrentIndex(0);
    vaccinManager->loadVaccinData(ui->tabvaccin);
}

void MainWindow::on_save_vac_clicked() {
    int reference = ui->reference->text().toInt();
    QString nom = ui->nom_vac->text();
    QString type = ui->type_vac->text();
    int age_min = ui->age_vac->text().toInt();
    QString mode_admin = ui->mode_vac->text();
    QString dose = ui->dose_vac->text();
    QDate date_exp = ui->date_vac->date();
    double prix = ui->prix_vac->text().toDouble();
    int quantite = ui->quantite_vac->text().toInt();

    vaccinManager->saveVaccinData(reference, nom, type, age_min, mode_admin, dose, date_exp, prix, quantite);
    ui->reference->clear();
    ui->nom_vac->clear();
    ui->type_vac->clear();
    ui->age_vac->clear();
    ui->mode_vac->clear();
    ui->dose_vac->clear();
    ui->date_vac->clear();
    ui->prix_vac->clear();
    ui->quantite_vac->clear();
    QMessageBox::information(this, "Succès", "Les données ont été téléchargées avec succès !");
}

void MainWindow::onajouteventclicked() {
    vaccinTab->setCurrentIndex(3);
}

void MainWindow::oneventBclicked() {
    vaccinTab->setCurrentIndex(2);
    loadEventData();
}

void MainWindow::on_equiB_clicked() {
    vaccinTab->setCurrentIndex(4);
    loadEquipementsData();
}

void MainWindow::on_ajoutequi_clicked() {
    vaccinTab->setCurrentIndex(5);
}

void MainWindow::on_rdv_clicked() {
    vaccinTab->setCurrentIndex(6);
    rdvWindow->loadAppointments(ui->liste_att);
}

void MainWindow::on_Quit_clicked() {
    vaccinTab->setCurrentIndex(7);
}

void MainWindow::on_Quit_4_clicked() {
    vaccinTab->setCurrentIndex(7);
}

void MainWindow::on_ajoutrdv_clicked() {
    vaccinTab->setCurrentIndex(8);
    rdvWindow->loadVaccins(ui->vaccin_2);
}

<<<<<<< HEAD
void MainWindow::on_save_rdv_clicked() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QString CIN = ui->CIN_rdv->text();
    QString vaccin_rdv = ui->vaccin_des->text();
    QString adresse_rdv = ui->adresse->text();
    QString nom_rdv = ui->nom_rdv->text();
    QString prenom_rdv = ui->prenom_rdv->text();
    QString dispo = ui->dispo->text();
    QString medecin_rdv = ui->medecin_att->text();
    QString infirmier_rdv = ui->infirmier_att->text();
    QString salle_rdv = ui->salle_att->text();
    double facturation = ui->facturation->value();

    QSqlQuery query;
    query.prepare("INSERT INTO RENDEZ_VOUS (ID_RDV, DATE_RDV, LIEU, DOC_ATT, INFIRMIER_ATT, SALLE_ATT, FACTURATION_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV) "
                  "VALUES (:cin, TO_DATE(:dispo, 'YYYY-MM-DD'),:lieu, :doc_att, :infirmier_att, :salle_att, :facturation, :nom_rdv, :prenom_rdv, :vaccin_rdv)");

    // Lier les valeurs
    query.bindValue(":cin", CIN);
    query.bindValue(":dispo", dispo);
    query.bindValue(":lieu", adresse_rdv);
    query.bindValue(":doc_att", medecin_rdv);
    query.bindValue(":infirmier_att", infirmier_rdv);
    query.bindValue(":salle_att", salle_rdv);
    query.bindValue(":facturation", facturation);
    query.bindValue(":nom_rdv", nom_rdv);
    query.bindValue(":prenom_rdv", prenom_rdv);
    query.bindValue(":vaccin_rdv", vaccin_rdv);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'enregistrement des données : " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Succès", "Les données ont été téléchargées avec succès !");
    }
}
=======

>>>>>>> ee88b6b07fe7b3be8728f30dde7239bb425c3694

void MainWindow::on_save_event_clicked() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    int id_event = ui->lineEdit_16->text().toInt();
    QString name = ui->lineEdit_18->text();
    QString nom_responsable = ui->lineEdit_19->text();
    QString type = ui->lineEdit_20->text();
    QString date_event = ui->lineEdit_21->text();
    QString heure_event = ui->lineEdit_22->text();
    QString lieu = ui->lineEdit_23->text();
    int nbr_participant = ui->lineEdit_24->text().toInt();

    QSqlQuery query;
    query.prepare("INSERT INTO EVENTS (ID_EVENT, NOM_EVENT, NOM_RESPONSABLE, TYPE, DATE_EVENT, HEURE_EVENT, LIEU, NBR_PARTICIPANT) "
                  "VALUES (:id_event, :name, :nom_responsable, :type, :date_event, :heure_event, :lieu, :nbr_participant)");

    // Lier les valeurs
    query.bindValue(":id_event", id_event);
    query.bindValue(":name", name);
    query.bindValue(":nom_responsable", nom_responsable);
    query.bindValue(":type", type);
    query.bindValue(":date_event", date_event);
    query.bindValue(":heure_event", heure_event);
    query.bindValue(":lieu", lieu);
    query.bindValue(":nbr_participant", nbr_participant);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'enregistrement des données : " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Succès", "L'événement a été ajouté avec succès !");
    }
}

void MainWindow::on_save_eq_clicked() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QString reference = ui->lineEdit_78->text().trimmed();
    QString name = ui->lineEdit_79->text().trimmed();
    QString type = ui->lineEdit_80->text().trimmed();
    QDate date_limit = ui->dateEdit_2->date();
    QString supplier = ui->lineEdit_83->text().trimmed();
    QString quantityStr = ui->lineEdit_85->text().trimmed();
    QString priceStr = ui->lineEdit_84->text().trimmed();

    bool quantityOk, priceOk;
    int quantity = quantityStr.toInt(&quantityOk);
    double price = priceStr.toDouble(&priceOk);

    if (reference.isEmpty() || name.isEmpty() || type.isEmpty() || supplier.isEmpty() ||
        !quantityOk || quantity <= 0 || !priceOk || price < 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Tous les champs doivent être remplis avec des données valides.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO EQUIPEMENTS (REFERNCE_EQ, NOM_EQ, TYPE_EQ, DATE_LIMITE, FOURNISSEUR, QUANTITE_EQ, PRIX) "
                  "VALUES (:reference, :name, :type, TO_DATE(:date_limit, 'YYYY-MM-DD'), :supplier, :quantity, :price)");

    // Lier les valeurs
    query.bindValue(":reference", reference);
    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":date_limit", date_limit.toString("yyyy-MM-dd"));
    query.bindValue(":supplier", supplier);
    query.bindValue(":quantity", quantity);
    query.bindValue(":price", price);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'enregistrement des données : " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Succès", "Les données de l'équipement ont été téléchargées avec succès !");
    }
}

void MainWindow::loadEquipementsData() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT REFERNCE_EQ, NOM_EQ, TYPE_EQ, DATE_LIMITE, FOURNISSEUR, QUANTITE_EQ, PRIX FROM EQUIPEMENTS");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    QTableWidget *tabequi = ui->tabequi;
    tabequi->setRowCount(0);

    int row = 0;
    while (query.next()) {
        tabequi->insertRow(row);

        for (int col = 0; col < 7; col++) {
            tabequi->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }

        row++;
    }

    tabequi->resizeRowsToContents();
}

void MainWindow::loadEventData() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Erreur de base de données", "La base de données n'est pas connectée.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_EVENT, HEURE_EVENT, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de la récupération des données : " + query.lastError().text());
        return;
    }

    QTableWidget *tabevent = ui->tabevent;
    tabevent->setRowCount(0);

    int row = 0;
    while (query.next()) {
        tabevent->insertRow(row);

        for (int col = 0; col < 8; col++) {
            tabevent->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }

        row++;
    }

    tabevent->resizeRowsToContents();
}



void MainWindow::onDateChanged() {
    QDate selectedDate;
    QDateEdit *senderDateEdit = qobject_cast<QDateEdit*>(sender());

    if (senderDateEdit == ui->date_vac) {
        selectedDate = ui->date_vac->date();
    } else if (senderDateEdit == ui->date_vac_2) {
        selectedDate = ui->date_vac_2->date();
    } else {
        return;
    }

    QDate currentDate = QDate::currentDate();

    if (selectedDate <= currentDate) {
        QMessageBox::warning(this, "Date invalide", "La date d'expiration doit être postérieure à la date actuelle.");
        senderDateEdit->setDate(currentDate.addDays(1));
        return;
    }

    int reference = senderDateEdit == ui->date_vac ? ui->reference->text().toInt() : ui->reference_2->text().toInt();

    QSqlQuery query;
    query.prepare("UPDATE VACCIN SET DATE_EXP = :date_exp WHERE REFERENCE = :reference");
    query.bindValue(":date_exp", selectedDate);
    query.bindValue(":reference", reference);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de la date : " + query.lastError().text());
    }
}

void MainWindow::on_delete_2_clicked() {
    bool ok;
    int reference = QInputDialog::getInt(this, "Supprimer l'enregistrement", "Entrez le numéro de référence :", 0, 0, INT_MAX, 1, &ok);

    if (ok) {
        vaccinManager->deleteVaccin(reference);
        vaccinManager->loadVaccinData(ui->tabvaccin);
    }
}

void MainWindow::on_edit_vac_2_clicked() {
<<<<<<< HEAD
    qDebug() << "Bouton de modification cliqué";
=======

>>>>>>> ee88b6b07fe7b3be8728f30dde7239bb425c3694
    bool ok;
    int reference = QInputDialog::getInt(this, "Modifier l'enregistrement", "Entrez le numéro de référence :", 0, 0, INT_MAX, 1, &ok);

    if (ok) {
        vaccinManager->fetchVaccinData(reference, ui->reference_2, ui->nom_vac_2, ui->type_vac_2, ui->age_vac_2, ui->mode_vac_2, ui->dose_vac_2, ui->date_vac_2, ui->prix_vac_2, ui->quantite_vac_2);
        vaccinTab->setCurrentIndex(9); // Naviguer vers l'onglet de modification
    }
}


void MainWindow::on_save_2_clicked() {
    int reference = ui->reference_2->text().toInt();
    QString nom = ui->nom_vac_2->text();
    QString type = ui->type_vac_2->text();
    int age_min = ui->age_vac_2->text().toInt();
    QString mode_admin = ui->mode_vac_2->text();
    QString dose = ui->dose_vac_2->text();
    QDate date_exp = ui->date_vac_2->date();
    double prix = ui->prix_vac_2->text().toDouble();
    int quantite = ui->quantite_vac_2->text().toInt();

    vaccinManager->saveVaccinData(reference, nom, type, age_min, mode_admin, dose, date_exp, prix, quantite);
    ui->reference_2->clear();
    ui->nom_vac_2->clear();
    ui->type_vac_2->clear();
    ui->age_vac_2->clear();
    ui->mode_vac_2->clear();
    ui->dose_vac_2->clear();
    ui->date_vac_2->clear();
    ui->prix_vac_2->clear();
    ui->quantite_vac_2->clear();


}

void MainWindow::on_save_rdv_clicked()
{
    QString CIN = ui->CIN_rdv->text();
    QString vaccin = ui->vaccin_2->currentText();
    QString date_rdv = ui->date_rdv->text();
    QString adresse = ui->adresse->text();
    QString nom = ui->nom_rdv->text();
    QString prenom = ui->prenom_rdv->text();
    QString dispo = ui->dispo->text();
    QString medecin = ui->medecin_att->text();
    QString infirmier = ui->infirmier_att->text();
    QString salle = ui->salle_att->text();
    double facturation = ui->facturation->value();


    rdvWindow->saveAppointment(CIN, vaccin, date_rdv, adresse, nom, prenom, dispo, medecin, infirmier, salle, facturation);


    ui->CIN_rdv->clear();
    ui->adresse->clear();
    ui->date_rdv->clear();
    ui->nom_rdv->clear();
    ui->prenom_rdv->clear();
    ui->medecin_att->clear();
    ui->infirmier_att->clear();
    ui->salle_att->clear();
    ui->reference->clear();
    ui->vaccin_2->clear();
    ui->dispo->clear();
    vaccinTab->setCurrentIndex(6);
}


void MainWindow::on_deleteRdv_clicked()
{
    bool ok;

    int CIN = QInputDialog::getInt(this, "Delete Record", "Enter the reference number:", 0, 0, INT_MAX, 1, &ok);

    if (ok) {
        rdvWindow->supprimerRdv(CIN);
        rdvWindow->loadAppointments(ui->liste_att);
    }

}


void MainWindow::on_edit_rdv_clicked()
{
    qDebug() << "Edit button clicked";
    bool ok;
    int id_rdv = QInputDialog::getInt(this, "Edit Record", "Enter the reference number:", 0, 0, INT_MAX, 1, &ok);

    if (ok) {



        rdvWindow->infoEdit(id_rdv,ui->CIN_rdv_2,ui->date_rdv_2,ui->vaccin_3,ui->adresse_2,ui->nom_rdv_2,ui->prenom_rdv_2,ui->dispo_2,ui->medecin_att_2,ui->infirmier_att_2,ui->salle_att_2,ui->facturation_2);
        rdvWindow->loadVaccins(ui->vaccin_3);
        rdvWindow->loadAppointments(ui->liste_att);
        vaccinTab->setCurrentIndex(10);

    }
}


void MainWindow::on_save_rdv_2_clicked()
{
    QString CIN2 = ui->CIN_rdv_2->text();
    QString vaccin2 = ui->vaccin_3->currentText();
    QString date_rdv2 = ui->date_rdv_2->text();
    QString adresse2 = ui->adresse_2->text();
    QString nom2 = ui->nom_rdv_2->text();
    QString prenom2 = ui->prenom_rdv_2->text();
    QString dispo2 = ui->dispo_2->text();
    QString medecin2 = ui->medecin_att_2->text();
    QString infirmier2 = ui->infirmier_att_2->text();
    QString salle2 = ui->salle_att_2->text();
    double facturation2 = ui->facturation_2->value();
    int CINi = CIN2.toInt();

    rdvWindow->modifier_rdv(CINi,CIN2, vaccin2,date_rdv2,adresse2,nom2, prenom2, dispo2,medecin2, infirmier2, salle2,facturation2);


    ui->CIN_rdv->clear();
    ui->adresse->clear();
    ui->date_rdv->clear();
    ui->nom_rdv->clear();
    ui->prenom_rdv->clear();
    ui->medecin_att->clear();
    ui->infirmier_att->clear();
    ui->salle_att->clear();
    ui->reference->clear();
    ui->vaccin_2->clear();
    ui->dispo->clear();
    vaccinTab->setCurrentIndex(6);

}


