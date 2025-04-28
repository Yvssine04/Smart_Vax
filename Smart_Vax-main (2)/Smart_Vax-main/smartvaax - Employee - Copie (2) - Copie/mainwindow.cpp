#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QGraphicsOpacityEffect>
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>
#include <QSqlTableModel>
#include <QtCharts>


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





MainWindow::MainWindow(const QString &userId, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    vaccinManager(new Vaccin(this)),
    fing(new empreinte(this)),
    Emp(new medecin(this))

{
    ui->setupUi(this);

    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessOutput);
    fing->connectToArduino();

    vaccinTab = ui->vaccin;
    vaccinTab->setCurrentIndex(7);

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

    if (userId.startsWith('A')) {
        ui->btnSupprimer_Med->show();
        ui->btnModifier_Med->show();
        ui->pushButton_AjoutMed->show();
    } else {
        ui->btnSupprimer_Med->hide();
        ui->btnModifier_Med->hide();
        ui->pushButton_AjoutMed->hide();
    }

    listMed = ui->pushButton_Med;
    listInf = ui->pushButton_Inf;
    GesAcc = ui->actionGestion_d_acc_s;
    Emp = new medecin(this);
    Emp->loadMedData(ui->tabMed);
    ui->tabMed->setSelectionMode(QAbstractItemView::SingleSelection); // Allow single row selection
    ui->tabMed->setSelectionBehavior(QAbstractItemView::SelectRows);

    Emp->loadInfData(ui->tabInf);
    ui->tabInf->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tabInf->setSelectionBehavior(QAbstractItemView::SelectRows);

    ////
    QSqlTableModel *model = new QSqlTableModel(this); // Create a new model
    model->setTable("EMPLOYEES"); // Tell the model to use the "EMPLOYEES" table
    model->select(); // Fetch the data from the table

    // Step 3: Connect the model to the table view
    ui->tableView->setModel(model); // Tell the table view to use this model

    ui->tabMed->setSortingEnabled(true);  // Enable built-in sorting
    ui->tabMed->horizontalHeader()->setSectionsClickable(true);  // Allow clicking headers to sort
    ui->tabMed->horizontalHeader()->setSortIndicatorShown(true); // Show sort arrows


    vaccinTab->setStyleSheet(
        "QTabWidget::pane { border: none; background: transparent; }"
        "QTabBar::tab { height: 0; width: 0; }"
        "QTabBar { height: 0; }"
        );


    connect(ajoutvac, &QPushButton::clicked, this, &MainWindow::onajoutvacclicked);
    connect(vaccinB, &QPushButton::clicked, this, &MainWindow::onvaccinBclicked);
    connect(ajoutevent, &QPushButton::clicked, this, &MainWindow::onajouteventclicked);
    connect(eventB, &QPushButton::clicked, this, &MainWindow::oneventBclicked);
    connect(ui->btnSupprimer, &QPushButton::clicked, this, &MainWindow::supprimerEvenement);
    connect(ui->btnModifier, &QPushButton::clicked, this, &MainWindow::modifierEvenement);
    connect(ajoutequi, &QPushButton::clicked, this, &MainWindow::on_ajoutequi_clicked);
    connect(equiB, &QPushButton::clicked, this, &MainWindow::on_equiB_clicked);
    connect(rdv, &QPushButton::clicked, this, &MainWindow::on_rdv_clicked);
    connect(Quit, &QPushButton::clicked, this, &MainWindow::on_Quit_clicked);
    connect(Quit4, &QPushButton::clicked, this, &MainWindow::on_Quit_4_clicked);
    connect(ajoutrdv, &QPushButton::clicked, this, &MainWindow::on_ajoutrdv_clicked);

    //MEDECIN
    connect(ajoutrdv, &QPushButton::clicked, this, &MainWindow::on_pushButton_Med_clicked);
    connect(ajoutrdv, &QPushButton::clicked, this, &MainWindow::on_pushButton_Inf_clicked);
    connect(ui->save_EditMed, &QPushButton::clicked, this, &MainWindow::on_save_EditMed_clicked);
    connect(ui->lineEdit_ChrMed, &QLineEdit::textChanged, this, &MainWindow::on_searchMed_textChanged);
    connect(ui->PdfButton, &QPushButton::clicked, this, &MainWindow::onPdfButtonClicked);
    connect(ui->pushButton_SortMed, &QPushButton::clicked, this, &MainWindow::on_pushButton_SortMed_clicked);
    connect(ui->actionshowStatsButton, &QAction::triggered, this, &MainWindow::on_actionshowStatsButton_triggered);
    ///connect(ui->actionShowCategoryStatsButton, &QAction::triggered, this, &MainWindow::showCategoryStatistics);
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        QByteArray stderrOutput = process->readAllStandardError();
        qDebug() << "STDERR:" << QString::fromUtf8(stderrOutput);
    });

    connect(process, &QProcess::errorOccurred, this, [](QProcess::ProcessError error){
        qDebug() << "Process error occurred:" << error;
    });






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

    QTableWidget *tabMed = ui->tabMed;
    tabMed->setColumnWidth(0, 165);
    tabMed->setColumnWidth(1, 165);
    tabMed->setColumnWidth(2, 165);
    tabMed->setColumnWidth(3, 165);
    tabMed->setColumnWidth(4, 165);
    tabMed->setColumnWidth(5, 165);
    tabMed->setColumnWidth(6, 165);
    tabMed->setColumnWidth(7, 165);


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

    tabMed->setStyleSheet(
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
    tabMed->resizeRowsToContents();

    ui->tabInf->setStyleSheet(
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
    ui->tabInf->resizeRowsToContents();


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

void MainWindow::supprimerEvenement() {
    int row = ui->tabevent->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Erreur", "Aucune ligne sélectionnée !");
        return;
    }

    int id = ui->tabevent->item(row, 0)->text().toInt();
    if (evenement->deleteEvent(id)) {
        ui->tabevent->removeRow(row);
    }
}

void MainWindow::modifierEvenement() {
    int row = ui->tabevent->currentRow();
    if (row == -1) {
        QMessageBox::warning(this, "Erreur", "Aucune ligne sélectionnée !");
        return;
    }

    int id = ui->tabevent->item(row, 0)->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString type = ui->lineEdit_type->text();
    QString date = ui->date_event->text();
    QString lieu = ui->lineEdit_lieu->text();
    int participants = ui->lineEdit_nbrp->text().toInt();
    QString responsable = ui->lineEdit_nomR->text();

    if (evenement->updateEvent(id, nom, responsable, type, date, lieu, participants)) {
        evenement->loadEventData(ui->tabevent);
    }
}

void MainWindow::onajouteventclicked() {
    vaccinTab->setCurrentIndex(3);
}

void MainWindow::oneventBclicked() {
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

void MainWindow::on_Quit_3_clicked() {
    vaccinTab->setCurrentIndex(2);
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


///MEDECIN?
//MEDECIN


void MainWindow::on_pushButton_Med_clicked()
{
    vaccinTab->setCurrentIndex(9);
    Emp->loadMedData(ui->tabMed);

}

void MainWindow::on_pushButton_Inf_clicked() {
    vaccinTab->setCurrentIndex(10);
    Emp->loadInfData(ui->tabInf);  // Load data when switching to the tab
}


void MainWindow::on_actionGestion_d_acc_s_triggered()
{
    vaccinTab->setCurrentIndex(11);
}

void MainWindow::on_pushButton_AjoutMed_clicked()
{
    vaccinTab->setCurrentIndex(12);
}


void MainWindow::on_save_AjMed_clicked()
{
    // Get all field values first
    QString id_e = ui->lineEdit_id_e->text();
    QString prenom_e = ui->lineEdit_prenom_e->text();
    QString nom_e = ui->lineEdit_nom_e->text();
    QString annee_exp_e = ui->lineEdit_annexp_e->text();
    QString specialite_e = ui->lineEdit_specialite_e->text();
    QString num_tel_e = ui->lineEdit_num_e->text();
    QString mail_e = ui->lineEdit_mail_e->text();
    QString adresse_e = ui->lineEdit_adresse_e->text();
    QString mdp_e = ui->lineEdit_mdp_e->text();

    // 1. First check all required fields are filled (except sexe)
    if (id_e.isEmpty() || prenom_e.isEmpty() || nom_e.isEmpty() ||
        annee_exp_e.isEmpty() || specialite_e.isEmpty() ||
        num_tel_e.isEmpty() || mail_e.isEmpty() ||
        adresse_e.isEmpty() || mdp_e.isEmpty()) {
        QMessageBox::warning(this, "Champs obligatoires", "Veuillez remplir tous les champs.");
        return; // This preserves all entered data
    }

    // 2. Then check gender selection
    QString sexe_e;
    if (ui->radioButton_M->isChecked()) {
        sexe_e = "M";
    } else if (ui->radioButton_F->isChecked()) {
        sexe_e = "F";
    } else {
        QMessageBox::warning(this, "Sexe manquant", "Veuillez sélectionner un sexe.");
        return; // Preserves all other data
    }

    // 3. If all checks pass, attempt to save
    if (Emp->saveMedData(id_e, prenom_e, nom_e,
                         annee_exp_e, specialite_e,
                         num_tel_e, mail_e, sexe_e,
                         adresse_e, mdp_e))
    {
        // Only clear fields if save was successful
        ui->lineEdit_id_e->clear();
        ui->lineEdit_prenom_e->clear();
        ui->lineEdit_nom_e->clear();
        ui->lineEdit_annexp_e->clear();
        ui->lineEdit_specialite_e->clear();
        ui->lineEdit_num_e->clear();
        ui->lineEdit_mail_e->clear();
        ui->lineEdit_adresse_e->clear();
        ui->lineEdit_mdp_e->clear();
        ui->radioButton_M->setAutoExclusive(false);
        ui->radioButton_M->setChecked(false);
        ui->radioButton_F->setAutoExclusive(false);
        ui->radioButton_F->setChecked(false);
        ui->radioButton_M->setAutoExclusive(true);
        ui->radioButton_F->setAutoExclusive(true);
    }
}

void MainWindow::on_btnSupprimer_Med_clicked() {
    // Step 1: Check if a row is selected in tabMed
    QList<QTableWidgetItem*> selectedItems = ui->tabMed->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un médecin à supprimer.");
        return;
    }

    // Step 2: Get the selected row
    int selectedRow = ui->tabMed->currentRow(); // Get the selected row index

    // Step 3: Retrieve the ID from the first column of the selected row
    QString id_e = ui->tabMed->item(selectedRow, 0)->text(); // Assuming ID is in the first column

    // Step 4: Debugging - Print the selected ID to verify
    qDebug() << "Selected ID:" << id_e;

    // Step 5: Call the deleteMed function using the existing Emp instance
    if (Emp->deleteMed(id_e)) {
        QMessageBox::information(this, "Succès", "Médecin supprimé !");
        Emp->loadMedData(ui->tabMed); // Refresh the table after deletion
    } else {
        QMessageBox::warning(this, "Échec", "Impossible de supprimer le médecin.");
    }
}




void MainWindow::on_btnModifier_Med_clicked()
{

    // 1. Check selection
    if (ui->tabMed->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a doctor first!");
        return;
    }

    // 2. Get selected row data
    int row = ui->tabMed->currentRow();
    selectedEmployeeId = ui->tabMed->item(row, 0)->text(); // ID

    // 3. Switch to edit tab (index 14)
    vaccinTab->setCurrentIndex(13);

    // 4. Fill the form
    ui->lineEdit_id_e_edit->setText(selectedEmployeeId);
    ui->lineEdit_prenom_e_edit->setText(ui->tabMed->item(row, 1)->text());
    ui->lineEdit_nom_e_edit->setText(ui->tabMed->item(row, 2)->text());
    ui->lineEdit_annexp_e_edit->setText(ui->tabMed->item(row, 3)->text());
    ui->lineEdit_specialite_e_edit->setText(ui->tabMed->item(row, 4)->text());
    ui->lineEdit_num_e_edit->setText(ui->tabMed->item(row, 5)->text());
    ui->lineEdit_mail_e_edit->setText(ui->tabMed->item(row, 6)->text());

    // Set gender radio button
    QString sexe = ui->tabMed->item(row, 7)->text();
    ui->radioButton_M_edit->setChecked(sexe == "M");
    ui->radioButton_F_edit->setChecked(sexe == "F");

    ui->lineEdit_adresse_e_edit->setText(ui->tabMed->item(row, 8)->text());
    // Leave password blank for security
}




void MainWindow::on_save_EditMed_clicked()
{
    // 1. Get all field values
    QString id_e = ui->lineEdit_id_e_edit->text();
    QString prenom_e = ui->lineEdit_prenom_e_edit->text();
    QString nom_e = ui->lineEdit_nom_e_edit->text();
    QString annee_exp_e = ui->lineEdit_annexp_e_edit->text();
    QString specialite_e = ui->lineEdit_specialite_e_edit->text();
    QString num_tel_e = ui->lineEdit_num_e_edit->text();
    QString mail_e = ui->lineEdit_mail_e_edit->text();
    QString adresse_e = ui->lineEdit_adresse_e_edit->text();
    QString mdp_e = ui->lineEdit_mdp_e_edit->text(); // Optional

    // 2. Get gender
    QString sexe_e = ui->radioButton_M_edit->isChecked() ? "M" : "F";

    // 3. Call update function
    if (Emp->updateMed(id_e, prenom_e, nom_e, annee_exp_e, specialite_e,
                       num_tel_e, mail_e, sexe_e, adresse_e, mdp_e))
    {
        QMessageBox::information(this, "Success", "Doctor updated!");
        Emp->loadMedData(ui->tabMed); // Refresh table
        vaccinTab->setCurrentIndex(9); // Return to list
    }
    else
    {
        QMessageBox::critical(this, "Error", "Update failed!");
    }
}


void MainWindow::on_searchMed_textChanged(const QString &text) {
    if (text.isEmpty()) {
        Emp->loadMedData(ui->tabMed); // Reload all data if search is empty
    } else {
        Emp->searchMedData(ui->tabMed, text); // Filter based on search text
    }
}

void MainWindow::onPdfButtonClicked() {
    QModelIndexList selection = ui->tabMed->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un employé.");
        return;
    }
    Emp->exporterCertificatTravailPDF(ui->tabMed);
}

void MainWindow::on_pushButton_SortMed_clicked()
{
    // Enable sorting if not already enabled
    ui->tabMed->setSortingEnabled(true);

    // Get the currently selected column (default to 0 if none selected)
    int column = ui->tabMed->currentColumn();
    if (column < 0) column = 0;

    // Toggle between ascending/descending order
    Qt::SortOrder currentOrder = ui->tabMed->horizontalHeader()->sortIndicatorOrder();
    Qt::SortOrder newOrder = (currentOrder == Qt::AscendingOrder)
                                 ? Qt::DescendingOrder
                                 : Qt::AscendingOrder;

    // Perform the sort
    ui->tabMed->sortByColumn(column, newOrder);

    // Re-apply password masking (column 9)
    for (int row = 0; row < ui->tabMed->rowCount(); ++row) {
        QTableWidgetItem *passwordItem = new QTableWidgetItem("●●●●●●●●");
        passwordItem->setFlags(passwordItem->flags() & ~Qt::ItemIsEditable);
        ui->tabMed->setItem(row, 9, passwordItem);
    }
}













// In mainwindow.cpp
void MainWindow::showStatistics() {
    vaccinTab->setCurrentIndex(14);  // Make sure this tab index exists


    // Create a QSqlQuery to fetch the years of experience from the database
    QSqlQuery query;
    query.prepare("SELECT ANNEE_EXP FROM EMPLOYEES");
    query.exec();

    // Create a map to store the counts for each experience group
    QMap<QString, int> experienceGroups;
    experienceGroups["<5"] = 0;
    experienceGroups["5-10"] = 0;
    experienceGroups["10-20"] = 0;
    experienceGroups[">20"] = 0;

    // Categorize employees based on years of experience
    while (query.next()) {
        int experience = query.value("ANNEE_EXP").toInt();

        if (experience < 5) {
            experienceGroups["<5"]++;
        } else if (experience >= 5 && experience < 10) {
            experienceGroups["5-10"]++;
        } else if (experience >= 10 && experience < 20) {
            experienceGroups["10-20"]++;
        } else {
            experienceGroups[">20"]++;
        }
    }

    // Call createExperienceGraph to show the graph based on categorized data
    createExperienceGraph(experienceGroups);
}


void MainWindow::createExperienceGraph(const QMap<QString, int>& experienceGroups) {
    // Create the chart
    QBarSet *set = new QBarSet("Employees");

    // Populate the bar set with the data from experienceGroups
    *set << experienceGroups["<5"] << experienceGroups["5-10"] << experienceGroups["10-20"] << experienceGroups[">20"];

    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Create the chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Employee Experience by Group");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create a chart view and set it in the widget_stat
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Assuming widget_stat is a QWidget (or layout container)
    if (ui->widget_stat->layout() == nullptr) {
        ui->widget_stat->setLayout(new QVBoxLayout());
    }    ui->widget_stat->layout()->addWidget(chartView);
}



void MainWindow::on_actionshowStatsButton_triggered()
{
    qDebug() << "Stats action triggered!";
    showStatistics();
}



/*
// Show category statistics (new function)
void MainWindow::showCategoryStatistics() {
    vaccinTab->setCurrentIndex(16);  // Make sure this tab index exists

    // Fetch employee IDs and categorize them
    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYE FROM EMPLOYEES");
    query.exec();

    QMap<QString, int> categoryCounts;
    categoryCounts["Admins"] = 0;
    categoryCounts["Medics"] = 0;
    categoryCounts["Nurses"] = 0;
    categoryCounts["Technicians"] = 0;

    while (query.next()) {
        QString employeeId = query.value("ID_EMPLOYE").toString();
        if (employeeId.startsWith("A")) categoryCounts["Admins"]++;
        else if (employeeId.startsWith("M")) categoryCounts["Medics"]++;
        else if (employeeId.startsWith("N")) categoryCounts["Nurses"]++;
        else if (employeeId.startsWith("T")) categoryCounts["Technicians"]++;
    }

    createCategoryGraph(categoryCounts);  // Create the pie chart
}

// Create the pie chart
void MainWindow::createCategoryGraph(const QMap<QString, int>& categoryCounts) {
    QPieSeries *series = new QPieSeries();
    for (auto category : categoryCounts.keys()) {
        series->append(category, categoryCounts[category]);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Employee Category Distribution");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (ui->widget_stat->layout() == nullptr) {
        ui->widget_stat->setLayout(new QVBoxLayout());
    }
    ui->widget_stat->layout()->addWidget(chartView);
}

// Connecting the new action or button
// Or for a QPushButton:
// connect(ui->showCategoryStatsButton, &QPushButton::clicked, this, &MainWindow::showCategoryStatistics);
*/
/*
void MainWindow::on_actionChanger_d_utilisateur_triggered()
{
    // Create a confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Changer d'utilisateur",
                                  "Êtes-vous sûr de vouloir changer d'utilisateur?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Close the current main window
        this->close();

        // Show the login window again
        loginWindow *login = new loginWindow();
        login->show();
    }
}
*/

void MainWindow::on_pushButton_Talk_clicked() {
    qDebug() << "Button clicked. Checking process state...";
    if (process->state() == QProcess::NotRunning) {
        QString pythonScriptPath = "C:/speech_to_text.py";  // Make sure this path is still valid
        qDebug() << "Starting Python script at:" << pythonScriptPath;

        process->start("C:/Users/ahmed/AppData/Local/Programs/Python/Python39/python.exe",
                       QStringList() << pythonScriptPath);
    } else {
        qDebug() << "Process is already running.";
    }
}



void MainWindow::onProcessOutput() {
    QByteArray output = process->readAllStandardOutput();
    QString recognizedText = QString::fromUtf8(output).trimmed();

    qDebug() << "Speech recognition output:" << recognizedText;

    ui->lineEdit_ChrMed->setText(recognizedText);
}


