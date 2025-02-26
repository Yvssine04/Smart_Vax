#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QGraphicsOpacityEffect>
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

Connection::Connection() {}

bool Connection::createconnect() {
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projetc++"); // Insert the data source name
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

    vaccinTab->setStyleSheet(
        "QTabWidget::pane { border: none; background: transparent; }"
        "QTabBar::tab { height: 0; width: 0; }"
        "QTabBar { height: 0; }"
        );

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

    // Connect the dateChanged signal to the custom slot
    connect(ui->date_vac, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);

    QLabel *main = ui->main;
    QPixmap pixmap(":/logo.png");
    main->setPixmap(pixmap);
    main->setAlignment(Qt::AlignCenter);

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect();
    opacityEffect->setOpacity(0.5);
    main->setGraphicsEffect(opacityEffect);

    // Stylesheet for table widgets
    QTableWidget *tabvaccin = ui->tabvaccin;
    tabvaccin->setColumnWidth(0, 145);
    tabvaccin->setColumnWidth(1, 145);
    tabvaccin->setColumnWidth(2, 145);
    tabvaccin->setColumnWidth(3, 145);
    tabvaccin->setColumnWidth(4, 145);
    tabvaccin->setColumnWidth(5, 140);

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

    // Set the stylesheet for the table widget
    tabvaccin->setStyleSheet(
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
    loadAppointments();
}

void MainWindow::on_Quit_clicked() {
    vaccinTab->setCurrentIndex(7);
}

void MainWindow::on_Quit_4_clicked() {
    vaccinTab->setCurrentIndex(7);
}

void MainWindow::on_ajoutrdv_clicked() {
    vaccinTab->setCurrentIndex(8);
}

void MainWindow::on_save_rdv_clicked() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database is not connected.");
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
    query.prepare("INSERT INTO TABLE1 (ID_RDV, DATE_RDV, LIEU, DOC_ATT, INFIRMIER_ATT, SALLE_ATT, FACTURATION_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV) "
                  "VALUES (:cin, TO_DATE(:dispo, 'YYYY-MM-DD'),:lieu, :doc_att, :infirmier_att, :salle_att, :facturation, :nom_rdv, :prenom_rdv, :vaccin_rdv)");

    // Bind values
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
        QMessageBox::critical(this, "Error", "Failed to save data: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "The data was successfully uploaded!");
    }
}

void MainWindow::on_save_event_clicked() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database is not connected.");
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
        QMessageBox::critical(this, "Database Error", "Database is not connected.");
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
        QMessageBox::critical(this, "Database Error", "Database is not connected.");
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
        QMessageBox::critical(this, "Database Error", "Database is not connected.");
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

void MainWindow::loadAppointments() {
    ui->liste_att->clear();

    QSqlQuery query("SELECT ID_RDV, NOM_RDV, PRENOM_RDV, VACCIN_RDV FROM TABLE1");
    while (query.next()) {
        QString CIN = query.value(0).toString();
        QString nom = query.value(1).toString();
        QString prenom = query.value(2).toString();
        QString vaccin = query.value(3).toString();
        QString itemText = CIN + " " + nom + " " + prenom + "   :   " + vaccin;
        ui->liste_att->addItem(itemText);
    }
}

void MainWindow::onDateChanged() {
    QDate selectedDate = ui->date_vac->date();
    QDate currentDate = QDate::currentDate();

    if (selectedDate <= currentDate) {
        QMessageBox::warning(this, "Invalid Date", "The expiration date must be greater than the current date.");
        // Optionally, revert the date to the previous valid date or current date + 1 day
        ui->date_vac->setDate(currentDate.addDays(1));
        return;
    }

    // Assuming you have a way to identify which record to update, e.g., a reference or ID
    int reference = ui->reference->text().toInt(); // Example: getting reference from UI

    QSqlQuery query;
    query.prepare("UPDATE VACCIN SET DATE_EXP = :date_exp WHERE REFERENCE = :reference");
    query.bindValue(":date_exp", selectedDate);
    query.bindValue(":reference", reference);

    if (!query.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update date: " + query.lastError().text());
    } else {
        QMessageBox::information(this, "Success", "The expiration date was successfully updated!");
    }
}
