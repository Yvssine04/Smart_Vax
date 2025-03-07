#include "mainwindow.h"
#include "ui_mainwindow.h"
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
    ui->choixtri->addItem("date");
    ui->choixtri->addItem("participants");
    ui->choixtri->addItem("nom");
    delete_vac = ui->delete_vac;
    edit_vac = ui->edit_vac;
    save_vac_2 = ui->save_vac_2;
    cherche_vac = ui->cherche_vac;
    ui->choix_vac->addItem("prix");
    ui->choix_vac->addItem("date");
    ui->choix_vac->addItem("quantite");
    vaccinTab->setStyleSheet(
        "QTabWidget::pane { border: none; background: transparent; }"
        "QTabBar::tab { height: 0; width: 0; }"
        "QTabBar { height: 0; }"
        );
    connect(cherche_vac, &QLineEdit::textChanged, this, [this]() {
        vaccinManager->filterVaccinTable(ui->tabvaccin, cherche_vac->text());
    });
    connect(ui->choix_vac, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxIndexChanged);
    connect(ui->tabvaccin, &QTableWidget::itemSelectionChanged, this, &MainWindow::onVaccinTableSelectionChanged);
    connect(edit_vac, &QPushButton::clicked, this, &MainWindow::on_edit_vac_2_clicked);
    connect(save_vac_2, &QPushButton::clicked, this, &MainWindow::on_save_2_clicked);
    connect(ui->date_vac, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);
    connect(ui->date_vac_2, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);
    connect(ajoutvac, &QPushButton::clicked, this, &MainWindow::onajoutvacclicked);
    connect(vaccinB, &QPushButton::clicked, this, &MainWindow::onvaccinBclicked);
    connect(ajoutevent, &QPushButton::clicked, this, &MainWindow::onajouteventclicked);
    connect(eventB, &QPushButton::clicked, this, &MainWindow::oneventBclicked);
    connect(ui->cherchevent, &QLineEdit::textChanged, this, [=](const QString &searchTerm) {
        evenement->searchEventData(ui->tabevent, searchTerm);
    });
    connect(ui->trievent, &QPushButton::clicked, this, [=]() {
        QString critere = ui->choixtri->currentText();
        evenement->sortEventData(ui->tabevent, critere);
    });
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
        " background-color:rgb(63, 123, 134);"

        "}"
        "QHeaderView {"
        "    background-color: rgb(63, 123, 134);"
        "}"
        "QHeaderView::section {"
        "    background-color:#bae5e8;"
        "    border: 2px solid #9faaab;"
        "    color: black;"
        "    font-size: 16px;"
        "    padding: 6px;"
        " border-radius: 5px;"
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
    eventManager->loadEventData(ui->tabevent);
}

void MainWindow::on_Quit_3_clicked() {
    vaccinTab->setCurrentIndex(2);
}

void MainWindow::on_Quit_13_clicked() {
    vaccinTab->setCurrentIndex(2);
    eventManager->loadEventData(ui->tabevent);
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
        QMessageBox::critical(this, "Database Error", "The database is not connected.");
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
    int id_event = ui->lineEdit_id->text().toInt();
    QString nom_event = ui->lineEdit_nom->text();
    QString nom_responsable = ui->lineEdit_nomR->text();
    QString type = ui->lineEdit_type->text();
    QString date_heure = ui->date_event->text();
    QString lieu = ui->lineEdit_lieu->text();
    int nbr_participant = ui->lineEdit_nbrp->text().toInt();

    eventManager->saveEventData(id_event, nom_event, nom_responsable, type, date_heure, lieu, nbr_participant);
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
void MainWindow::onVaccinTableSelectionChanged() {
    QList<QTableWidgetSelectionRange> selectedRanges = ui->tabvaccin->selectedRanges();
    if (!selectedRanges.isEmpty()) {
        QTableWidgetSelectionRange range = selectedRanges.at(0);
        selectedRow = range.topRow();
    } else {
        selectedRow = -1;
    }
}

void MainWindow::on_delete_2_clicked() {
    if (selectedRow != -1) {
        QTableWidgetItem* referenceItem = ui->tabvaccin->item(selectedRow, 0);
        int reference = referenceItem->text().toInt();
        vaccinManager->deleteVaccin(reference);
        vaccinManager->loadVaccinData(ui->tabvaccin);
        selectedRow = -1;
    } else {
        QMessageBox::warning(this, "No Selection", "veuillez selectionner un élément du tableau");
    }
}

void MainWindow::on_edit_vac_2_clicked() {
    if (selectedRow != -1) {
        QTableWidgetItem* referenceItem = ui->tabvaccin->item(selectedRow, 0);
        int reference = referenceItem->text().toInt();
        vaccinManager->fetchVaccinData(reference, ui->reference_2, ui->nom_vac_2, ui->type_vac_2, ui->age_vac_2, ui->mode_vac_2, ui->dose_vac_2, ui->date_vac_2, ui->prix_vac_2, ui->quantite_vac_2);
        ui->reference_2->setDisabled(true);

        // Switch to the edit tab
        vaccinTab->setCurrentIndex(9);
        selectedRow = -1;
    } else {
        QMessageBox::warning(this, "No Selection", "veuillez selectionner un élément du tableau");
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
    vaccinManager->updateVaccinData(reference, nom, type, age_min, mode_admin, dose, date_exp, prix, quantite);
    ui->reference_2->clear();
    ui->nom_vac_2->clear();
    ui->type_vac_2->clear();
    ui->age_vac_2->clear();
    ui->mode_vac_2->clear();
    ui->dose_vac_2->clear();
    ui->date_vac_2->clear();
    ui->prix_vac_2->clear();
    ui->quantite_vac_2->clear();
    ui->reference_2->setDisabled(false);
}
void MainWindow::on_supprimerevent_clicked() {
    int identifiant = -1;
    bool ok = false;

    // Vérifier si une ligne est sélectionnée
    QModelIndexList selection = ui->tabevent->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        // Récupérer l'identifiant depuis la première colonne de la ligne sélectionnée
        identifiant = ui->tabevent->model()->data(selection.first()).toInt();
        ok = true; // On a un identifiant valide
    } else {
        // Demander l'identifiant avec un QInputDialog si aucune ligne n'est sélectionnée
        identifiant = QInputDialog::getInt(this, "Supprimer un événement",
                                           "Entrez l'identifiant de l'événement :",
                                           0, 0, INT_MAX, 1, &ok);
    }

    if (ok) {
        eventManager->supprimerEvenement(identifiant);
        eventManager->loadEventData(ui->tabevent);
    }
}


void MainWindow::on_modifierevent_clicked() {
    int identifiant = -1;
    bool ok = false;

    // Vérifier si une ligne est sélectionnée
    QModelIndexList selection = ui->tabevent->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        // Récupérer l'identifiant depuis la première colonne de la ligne sélectionnée
        identifiant = ui->tabevent->model()->data(selection.first()).toInt();
        ok = true; // On a un identifiant valide
    } else {
        // Demander l'identifiant avec un QInputDialog si aucune ligne n'est sélectionnée
        identifiant = QInputDialog::getInt(this, "Modifier un événement",
                                           "Entrez l'identifiant de l'événement :",
                                           0, 0, INT_MAX, 1, &ok);
    }

    if (ok) {
        // Basculer vers l'onglet 10
        vaccinTab->setCurrentIndex(10);

        // Récupérer les données
        Evenement event;
        QString nom, responsable, type, lieu;
        int participants;
        QDateTime dateHeure;

        if (event.recupererEvenement(identifiant, nom, responsable, type, lieu, participants, dateHeure)) {
            // Remplir les champs
            ui->lineEdit_mid->setText(QString::number(identifiant));
            ui->lineEdit_mnom->setText(nom);
            ui->lineEdit_mnomR->setText(responsable);
            ui->lineEdit_mtype->setText(type);
            ui->lineEdit_mlieu->setText(lieu);
            ui->lineEdit_mnbrp->setText(QString::number(participants));
            ui->date_mevent->setDateTime(dateHeure);
        } else {
            QMessageBox::warning(this, "Erreur", "Aucun événement trouvé avec cet identifiant.");
        }
    }
}


void MainWindow::on_sauvegarderevent_clicked() {
    int identifiant = ui->lineEdit_mid->text().toInt();
    QString nom = ui->lineEdit_mnom->text();
    QString responsable = ui->lineEdit_mnomR->text();
    QString type = ui->lineEdit_mtype->text();
    QString lieu = ui->lineEdit_mlieu->text();
    int participants = ui->lineEdit_mnbrp->text().toInt();
    QDateTime dateHeure = ui->date_mevent->dateTime();

    Evenement event;
    if (event.modifierEvenement(identifiant, nom, responsable, type, lieu, participants, dateHeure)) {
        QMessageBox::information(this, "Succès", "L'événement a été modifié avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification de l'événement.");
    }
}

void MainWindow::onComboBoxIndexChanged(int index) {
    QTableWidget *table = ui->tabvaccin;
    int column = -1;

    // Determine which column to sort by based on the combo box selection
    switch (index) {
    case 0: // "prix"
        column = 3; // Assuming "prix" is the 4th column (index 3)
        break;
    case 1: // "date"
        column = 4; // Assuming "date" is the 5th column (index 4)
        break;
    case 2: // "quantite"
        column = 5; // Assuming "quantite" is the 6th column (index 5)
        break;
    default:
        return;
    }

    // Sort the table by the selected column
    if (column != -1) {
        vaccinManager->sortVaccinTable(table, column);
    }
}


