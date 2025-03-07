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
    db.setDatabaseName("projetc++"); // Insert the name of the data source
    db.setUserName("Mariem"); // Insert the username
    db.setPassword("2004"); // Insert the password for this user

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
    edit_vac = ui->edit_vac; // Initialize the edit button
    save_vac_2 = ui->save_vac_2; // Initialize the save button for edited data

    vaccinTab->setStyleSheet(
        "QTabWidget::pane { border: none; background: transparent; }"
        "QTabBar::tab { height: 0; width: 0; }"
        "QTabBar { height: 0; }"
        );

    // Ensure this connection is only made once
    connect(edit_vac, &QPushButton::clicked, this, &MainWindow::on_edit_vac_2_clicked);
    connect(save_vac_2, &QPushButton::clicked, this, &MainWindow::on_save_2_clicked);
    // Other connections...
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
    connect(ui->date_vac_2, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);

    QLabel *main = ui->main;
    QPixmap pixmap(":/logo.png");
    main->setPixmap(pixmap);
    main->setAlignment(Qt::AlignCenter);

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect();
    opacityEffect->setOpacity(0.5);
    main->setGraphicsEffect(opacityEffect);

    // Style sheet for table widgets
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

    // Set style sheet for table widgets
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
    rdvWindow->loadVaccins(ui->vaccin_2,ui->medecin_att,ui->infirmier_att);
}



void MainWindow::on_save_event_clicked() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Database Error", "The database is not connected.");
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

    // Bind values
    query.bindValue(":id_event", id_event);
    query.bindValue(":name", name);
    query.bindValue(":nom_responsable", nom_responsable);
    query.bindValue(":type", type);
    query.bindValue(":date_event", date_event);
    query.bindValue(":heure_event", heure_event);
    query.bindValue(":lieu", lieu);
    query.bindValue(":nbr_participant", nbr_participant);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to save data: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "The event was successfully added!");
    }
}

void MainWindow::on_save_eq_clicked() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Database Error", "The database is not connected.");
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
        QMessageBox::warning(this, "Input Error", "All fields must be filled with valid data.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO EQUIPEMENTS (REFERNCE_EQ, NOM_EQ, TYPE_EQ, DATE_LIMITE, FOURNISSEUR, QUANTITE_EQ, PRIX) "
                  "VALUES (:reference, :name, :type, TO_DATE(:date_limit, 'YYYY-MM-DD'), :supplier, :quantity, :price)");

    // Bind values
    query.bindValue(":reference", reference);
    query.bindValue(":name", name);
    query.bindValue(":type", type);
    query.bindValue(":date_limit", date_limit.toString("yyyy-MM-dd"));
    query.bindValue(":supplier", supplier);
    query.bindValue(":quantity", quantity);
    query.bindValue(":price", price);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to save data: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "The equipment data was successfully uploaded!");
    }
}

void MainWindow::loadEquipementsData() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Database Error", "The database is not connected.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT REFERNCE_EQ, NOM_EQ, TYPE_EQ, DATE_LIMITE, FOURNISSEUR, QUANTITE_EQ, PRIX FROM EQUIPEMENTS");

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to retrieve data: " + query.lastError().text());
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
        QMessageBox::critical(this, "Database Error", "The database is not connected.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_EVENT, NOM_EVENT, TYPE, DATE_EVENT, HEURE_EVENT, LIEU, NBR_PARTICIPANT, NOM_RESPONSABLE FROM EVENTS");

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to retrieve data: " + query.lastError().text());
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
    QDate selectedDate = ui->date_vac_2->date();
    QDate currentDate = QDate::currentDate();

    if (selectedDate <= currentDate) {
        QMessageBox::warning(this, "Invalid Date", "The expiration date must be greater than the current date.");
        // Optionally, revert to the previous valid date or the current date + 1 day
        ui->date_vac_2->setDate(currentDate.addDays(1));
        return;
    }

    // Assume there is a way to identify which record to update, e.g., a reference or ID
    int reference = ui->reference_2->text().toInt(); // Example: get the reference from the UI

    QSqlQuery query;
    query.prepare("UPDATE VACCIN SET DATE_EXP = :date_exp WHERE REFERENCE = :reference");
    query.bindValue(":date_exp", selectedDate);
    query.bindValue(":reference", reference);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update the date: " + query.lastError().text());
    }
}

void MainWindow::on_delete_2_clicked() {
    bool ok;
    int reference = QInputDialog::getInt(this, "Delete Record", "Enter the reference number:", 0, 0, INT_MAX, 1, &ok);

    if (ok) {
        vaccinManager->deleteVaccin(reference);
        vaccinManager->loadVaccinData(ui->tabvaccin);
    }
}

void MainWindow::on_edit_vac_2_clicked() {

    bool ok;
    int reference = QInputDialog::getInt(this, "Edit Record", "Enter the reference number:", 0, 0, INT_MAX, 1, &ok);

    if (ok) {
        vaccinManager->fetchVaccinData(reference, ui->reference_2, ui->nom_vac_2, ui->type_vac_2, ui->age_vac_2, ui->mode_vac_2, ui->dose_vac_2, ui->date_vac_2, ui->prix_vac_2, ui->quantite_vac_2);
        vaccinTab->setCurrentIndex(9);
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
    QString medecin = ui->medecin_att->currentText();
    QString infirmier = ui->infirmier_att->currentText();
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
        rdvWindow->loadVaccins(ui->vaccin_3,ui->medecin_att_2,ui->infirmier_att_2);
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
    QString medecin2 = ui->medecin_att_2->currentText();
    QString infirmier2 = ui->infirmier_att_2->currentText();
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



void MainWindow::on_rdv_pdf_clicked()
{
    QListWidgetItem *selectedItem = ui->liste_att->currentItem();
    if (selectedItem) {
        QString fullText = selectedItem->text();

        QString cini = fullText.split(" ").first();

        qDebug() << "Selected CIN:" << cini;
        int cin=cini.toInt();
        rdvWindow->generateCertificate(cin);
    } else {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
    }


}




