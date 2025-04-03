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
#include <QSqlDatabase>
#include <QTimer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QUrl>
#include <QAudioOutput>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QBarCategoryAxis>
#include <QToolTip>
#include <vector>
#include <iostream>

Connection::Connection() {}

bool Connection::createconnect() {
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projetc++");
    db.setUserName("Mariem");
    db.setPassword("2004");

    if (db.open())
        test = true;

    return test;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , vaccinManager(new Vaccin(this))
    , rdvWindow (new rendez_vous(this))
    , notificationWidget (new NotificationWidget(this))
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
    delete_vac = ui->delete_vac;
    edit_vac = ui->edit_vac;
    save_vac_2 = ui->save_vac_2;
    cherche_vac = ui->cherche_vac;
    ui->choix_vac->addItem("prix");
    ui->choix_vac->addItem("date");
    ui->choix_vac->addItem("quantite");
    ui->tabequi->setSortingEnabled(true);

    calendrier= ui->calendrier_rdv;

    int nbr_notif;


    ////////////////////Calendrier


    calendrier = ui->calendrier_rdv;
    QTextCharFormat format;
    format.setToolTip("Meeting at 3 PM");
    format.setBackground(QColor(255, 255, 150));
    format.setFontUnderline(true);
    QDate dateToModify = QDate(2025, 4, 15);
    ui->calendrier_rdv->setDateTextFormat(dateToModify, format);



    ////////////////////////////////notification
    ///
    ///
    QSoundEffect notif;


    QSoundEffect *soundEffect = new QSoundEffect(this);
    soundEffect->setSource(QUrl::fromLocalFile("C:/Users/MSI/Downloads/ding-101492.wav"));

    ui->bellN->hide();

    QVBoxLayout *centralLayout = new QVBoxLayout(ui->centralWidget);
    ui->centralWidget->setLayout(centralLayout);


    notificationWidget = new NotificationWidget(this);
    notificationWidget->hide();

    centralLayout->addWidget(notificationWidget);

    connect(ui->alert, &QPushButton::clicked, this, &MainWindow::showNotifications);



    connect(notificationWidget, &NotificationWidget::notificationClicked, this, &MainWindow::handleNotificationClicked);


    vaccinTab->setStyleSheet(
        "QTabWidget::pane { border: none; background: transparent; }"
        "QTabBar::tab { height: 0; width: 0; }"
        "QTabBar { height: 0; }"
        );
    connect(cherche_vac, &QLineEdit::textChanged, this, [this]() {
        vaccinManager->filterVaccinTable(ui->tabvaccin, cherche_vac->text());
    });
    connect(ui->choix_vac, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxIndexChanged);
    connect(ui->tri_rdv, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::comboIndex_rdv);
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
    connect(ui->choixtri, &QComboBox::currentTextChanged, this, [=](const QString &critere) {
        evenement->sortEventData(ui->tabevent, critere);
    });

    connect(ui->pdfinvi, &QPushButton::clicked, [=]() {
        evenement->exporterInvitationPDF(ui->tabevent);
    });
    connect(ajoutequi, &QPushButton::clicked, this, &MainWindow::on_ajoutequi_clicked);
    connect(equiB, &QPushButton::clicked, this, &MainWindow::on_equiB_clicked);
    connect(rdv, &QPushButton::clicked, this, &MainWindow::on_rdv_clicked);
    connect(Quit, &QPushButton::clicked, this, &MainWindow::on_Quit_clicked);
    connect(Quit4, &QPushButton::clicked, this, &MainWindow::on_Quit_4_clicked);
    connect(ajoutrdv, &QPushButton::clicked, this, &MainWindow::on_ajoutrdv_clicked);
    connect(delete_vac, &QPushButton::clicked, this, &MainWindow::on_delete_2_clicked);
    connect(ui->rdv_rech, &QLineEdit::textChanged, this, &MainWindow::rdv_recherche);
    connect(ui->CIN_rdv, &QLineEdit::textChanged, this, &MainWindow::autoFillPatientInfo);




    //connect(ffff, &QPushButton::clicked, this, &MainWindow::on_edit_equi_clicked);

    connect(ui->supprimerevent_3, &QPushButton::clicked, this, &MainWindow::on_supprime_eq_clicked);
    connect(ui->choix_equi, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onEquipementComboBoxIndexChanged);
    connect(ui->recherche_eq, &QLineEdit::returnPressed, this, &MainWindow::onRechercheEqReturnPressed);
    connect(ui->tri_rdv, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::comboIndex_rdv);
    connect(ui->calendrier_rdv, &QCalendarWidget::clicked, this, &MainWindow::on_calendarWidget_clicked);

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
        identifiant = ui->tabevent->model()->data(selection.first()).toInt();
        ok = true; // On a un identifiant valide
    } else {
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

void MainWindow::onComboBoxIndexChanged(int index)
{
    QTableWidget *table = ui->tabvaccin;
    if (!table || table->rowCount() == 0) return;

    // Map combo box index to column and sort type
    struct SortColumn {
        int column;
        bool isNumeric;
        bool isDate;
    };

    static const QVector<SortColumn> columnMapping = {
        {3, true, false},   // 0: "prix" (numeric)
        {4, false, true},   // 1: "date" (date)
        {5, true, false}    // 2: "quantite" (numeric)
    };

    if (index < 0 || index >= columnMapping.size()) return;

    // Configure sorting parameters
    const SortColumn& sortCol = columnMapping[index];
    table->setSortingEnabled(false);

    // Prepare items with proper sort data
    for (int row = 0; row < table->rowCount(); ++row) {
        QTableWidgetItem* item = table->item(row, sortCol.column);
        if (item) {
            if (sortCol.isNumeric) {
                bool ok;
                double value = item->text().toDouble(&ok);
                item->setData(Qt::UserRole, ok ? value : 0);
            }
            else if (sortCol.isDate) {
                QDate date = QDate::fromString(item->text(), "d/M/yyyy");
                item->setData(Qt::UserRole, date.isValid() ? date.toJulianDay() : 0);
            }
            else {
                item->setData(Qt::UserRole, item->text());
            }
        }
    }

    // Sort using the prepared data
    table->sortByColumn(sortCol.column, Qt::AscendingOrder);
    table->setSortingEnabled(true);
}



/////////////////////////////////////////////////////////////////////////////////

void MainWindow::sortEquipementTable(QTableWidget *tableEqui, int column) {
    if (tableEqui == nullptr) {
        qDebug() << "Table widget is null!";
        return;
    }

    // Sort the table by the specified column
    tableEqui->sortItems(column, Qt::AscendingOrder);
}
void MainWindow::onEquipementComboBoxIndexChanged(int index) {
    QTableWidget *table = ui->tabequi;
    int column = -1;

    // Determine which column to sort by based on the combo box selection
    switch (index) {

    case 0: // "name"
        column = 3; // Assuming "name" is the 2nd column (index 1)
        break;
    case 1: // "type"
        column = 5; // Assuming "type" is the 3rd column (index 2)
        break;
    case 2: // "date_limit"
        column = 6; // Assuming "date_limit" is the 4th column (index 3)
        break;

    default:
        qDebug() << "Invalid combo box index:" << index;
        return;
    }

    qDebug() << "Sorting by column:" << column;

    // Sort the table by the selected column
    if (column != -1) {
        Equipements equipement;
        equipement.sortEquipementTable(table, column);
    }
}

void MainWindow::onRechercheEqReturnPressed() {
    QString reference = ui->recherche_eq->text();

    // Debug: Print the entered reference
    qDebug() << "Entered reference:" << reference;

    bool found = false;

    // Iterate over the rows in the table to find matching references
    for (int row = 0; row < ui->tabequi->rowCount(); ++row) {
        QTableWidgetItem* item = ui->tabequi->item(row, 0); // Assuming the reference is in the first column
        if (item && item->text().contains(reference, Qt::CaseInsensitive)) {
            // Highlight the found row or perform any other action
            ui->tabequi->selectRow(row);
            found = true;
        } else {
            // Deselect the row if it doesn't match
            ui->tabequi->setRowHidden(row, true);
        }
    }

    if (!found) {
        QMessageBox::warning(this, "Non trouvé", "Aucune référence trouvée.");
    }
}



void MainWindow::on_ajoutequi_clicked() {
    vaccinTab->setCurrentIndex(5);
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

    Equipements equipement(reference, name, type, date_limit.toString("yyyy-MM-dd"), supplier, quantity, price);
    if (!equipement.ajouter()) {
        QMessageBox::critical(this, "Error", "Failed to save data.");
    } else {
        QMessageBox::information(this, "Success", "The equipment data was successfully uploaded!");
    }
}


void MainWindow::loadEquipementsData() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Database Error", "The database is not connected.");
        return;
    }

    Equipements equipement;
    QSqlQueryModel *model = equipement.afficher();

    // Clear the table
    ui->tabequi->setRowCount(0);

    // Populate the table with data from the model
    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tabequi->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            ui->tabequi->setItem(row, col, new QTableWidgetItem(model->data(index).toString()));



        }
    }

    ui->tabequi->resizeRowsToContents();
}




void MainWindow::on_edit_equi_clicked() {
    QList<QTableWidgetItem*> selectedItems = ui->tabequi->selectedItems();
    if (!selectedItems.isEmpty()) {
        int row = ui->tabequi->row(selectedItems.first());

        QString reference = ui->tabequi->item(row, 0)->text();
        QString nom = ui->tabequi->item(row, 1)->text();
        QString type = ui->tabequi->item(row, 2)->text();
        QString dateLimite = ui->tabequi->item(row, 3)->text();
        QString fournisseur = ui->tabequi->item(row, 4)->text();
        QString quantite = ui->tabequi->item(row, 5)->text();
        QString prix = ui->tabequi->item(row, 6)->text();

        ui->lineEdit_1111->setText(reference);
        ui->lineEdit_1114->setText(nom);
        ui->lineEdit_1112->setText(type);
        ui->dateEdit_1113->setDate(QDate::fromString(dateLimite, "yyyy-MM-dd"));
        ui->lineEdit_1115->setText(fournisseur);
        ui->lineEdit_1117->setText(quantite);
        ui->lineEdit_1116->setText(prix);

        vaccinTab->setCurrentIndex(11);
    } else {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner une ligne à éditer.");
    }
}





void MainWindow::on_modif_save_clicked() {
    QString reference = ui->lineEdit_1111->text();
    QString nom = ui->lineEdit_1114->text();
    QString type = ui->lineEdit_1112->text();
    QString dateLimite = ui->dateEdit_1113->date().toString("yyyy-MM-dd");
    QString fournisseur = ui->lineEdit_1115->text();
    int quantite = ui->lineEdit_1117->text().toInt();
    double prix = ui->lineEdit_1116->text().toDouble();

    // Debugging: Print the values being saved
    qDebug() << "Saving equipment with data: reference =" << reference
             << "nom =" << nom
             << "type =" << type
             << "dateLimite =" << dateLimite
             << "fournisseur =" << fournisseur
             << "quantite =" << quantite
             << "prix =" << prix;

    Equipements equipement(reference, nom, type, dateLimite, fournisseur, quantite, prix);
    if (!equipement.modifier()) {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de la base de données.");
    } else {
        QMessageBox::information(this, "Succès", "Les modifications ont été enregistrées dans la base de données.");
        vaccinTab->setCurrentIndex(11);
    }
}




void MainWindow::on_supprime_eq_clicked() {
    QList<QTableWidgetItem*> selectedItems = ui->tabequi->selectedItems();
    if (!selectedItems.isEmpty()) {
        int row = ui->tabequi->row(selectedItems.first());
        QString reference = ui->tabequi->item(row, 0)->text();

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette ligne?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            Equipements equipement;
            if (!equipement.supprimerEquipement(reference)) {
                QMessageBox::critical(this, "Erreur", "Échec de la suppression de la base de données.");
            } else {
                ui->tabequi->removeRow(row);
                QMessageBox::information(this, "Succès", "La ligne a été supprimée de la base de données.");
            }
        }
    } else {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner une ligne à supprimer.");
    }

}

//Equipements::calculateStatistics(model);






//RENDEZ_VOUS






void MainWindow::on_ajoutrdv_clicked() {
    vaccinTab->setCurrentIndex(8);
    rdvWindow->loadVaccins(ui->vaccin_2,ui->medecin_att,ui->infirmier_att);

}

void MainWindow::on_save_rdv_clicked() {
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


    int verif =rdvWindow->saveAppointment(CIN, vaccin, date_rdv, adresse, nom, prenom, dispo, medecin, infirmier, salle, facturation);

    if (verif==1){
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
        rdvWindow->loadAppointments(ui->liste_att);
        vaccinTab->setCurrentIndex(6);}
    if (verif==2){


        ajoutNotification("Un nouveau patient a été ajouté\n"
                          "Veuillez compléter le dossier médical.");
    }
}





void MainWindow::autoFillPatientInfo(const QString &cin) {
    if (cin.isEmpty()) {
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM FROM PATIENT WHERE CIN_PASSP = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche du patient:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        ui->nom_rdv->setText(query.value(0).toString());
        ui->prenom_rdv->setText(query.value(1).toString());
    } else {
        ui->nom_rdv->clear();
        ui->prenom_rdv->clear();
    }
}





void MainWindow::on_deleteRdv_clicked()
{
    QListWidgetItem *selectedItem = ui->liste_att->currentItem();

    if (!selectedItem) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
        return;
    }

    int idRdv = selectedItem->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer ce rendez-vous?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
       rdvWindow->supprimerRdv(idRdv);
            rdvWindow->loadAppointments(ui->liste_att);

    }
}


void MainWindow::on_edit_rdv_clicked()
{
    QListWidgetItem *selectedItem = ui->liste_att->currentItem();

    if (!selectedItem) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
        return;
    }

    int idRdv = selectedItem->data(Qt::UserRole).toInt();
    currentIdRdv = idRdv;



        rdvWindow->infoEdit(idRdv,ui->CIN_rdv_2,ui->date_rdv_2,ui->vaccin_3,ui->adresse_2,ui->nom_rdv_2,ui->prenom_rdv_2,ui->dispo_2,ui->medecin_att_2,ui->infirmier_att_2,ui->salle_att_2,ui->facturation_2);

        vaccinTab->setCurrentIndex(12);


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


     int verif=rdvWindow->modifier_rdv(currentIdRdv, CIN2, vaccin2, date_rdv2, adresse2, nom2, prenom2, dispo2, medecin2, infirmier2, salle2, facturation2);
    if (verif==1){
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
    rdvWindow->loadAppointments(ui->liste_att);
    vaccinTab->setCurrentIndex(6);}
    if (verif==2){


        ajoutNotification("Un nouveau patient avec le cin %1 a été ajouté\n"
                          "Veuillez compléter le dossier médical.");
    }

}

void MainWindow::on_rdv_pdf_clicked()
{
    QListWidgetItem *selectedItem = ui->liste_att->currentItem();
    if (selectedItem) {

        int idRdv = selectedItem->data(Qt::UserRole).toInt();
        currentIdRdv = idRdv;
        rdvWindow->generateCertificate(currentIdRdv);
    } else {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
    }
}

void MainWindow::rdv_recherche(const QString &text){

    for (int i = 0; i < ui->liste_att->count(); ++i) {
        QListWidgetItem *item = ui->liste_att->item(i);
        bool match = item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!match);
    }

}


void MainWindow::comboIndex_rdv(int index){


    QList<QListWidgetItem*> items;

    while (ui->liste_att->count() > 0) {
        items.append(ui->liste_att->takeItem(0));
    }

    std::sort(items.begin(), items.end(), [index](QListWidgetItem *a, QListWidgetItem *b) {
        int cinA = a->data(Qt::UserRole).toInt();
        int cinB = b->data(Qt::UserRole).toInt();
        QString nomA = a->data(Qt::UserRole + 1).toString().toLower();
        QString nomB = b->data(Qt::UserRole + 1).toString().toLower();
        QString prenomA = a->data(Qt::UserRole + 2).toString().toLower();
        QString prenomB = b->data(Qt::UserRole + 2).toString().toLower();
        QString vaccinA = a->data(Qt::UserRole + 3).toString().toLower();
        QString vaccinB = b->data(Qt::UserRole + 3).toString().toLower();

        if (index == 0) {
            if (cinA != cinB) return cinA < cinB;
            if (nomA != nomB) return nomA < nomB;
            if (prenomA != prenomB) return prenomA < prenomB;
            return vaccinA < vaccinB;
        }
        else if (index == 1) {
            if (nomA != nomB) return nomA < nomB;
            if (prenomA != prenomB) return prenomA < prenomB;
            if (cinA != cinB) return cinA < cinB;
            return vaccinA < vaccinB;
        }
        else if (index == 2) {
            if (prenomA != prenomB) return prenomA < prenomB;
            if (nomA != nomB) return nomA < nomB;
            if (cinA != cinB) return cinA < cinB;
            return vaccinA < vaccinB;
        }
        else if (index == 3) {
            if (vaccinA != vaccinB) return vaccinA < vaccinB;
            if (nomA != nomB) return nomA < nomB;
            if (prenomA != prenomB) return prenomA < prenomB;
            return cinA < cinB;
        }
        return false;
    });


    for (QListWidgetItem *item : items) {
        ui->liste_att->addItem(item);
    }
}
void MainWindow::on_calendarWidget_clicked(const QDate &date) {
    qDebug() << "Date clicked:" << date;

    ui->rendez_v->setText("RENDEZ_VOUS");




}


//////////////////////////////////////////notification



void MainWindow::showNotifications() {
    if (notificationWidget->isVisible()) {
        notificationWidget->hide();
    } else {
        notificationWidget->show();
    }
}


void MainWindow::ajoutNotification(const QString message) {

    QSoundEffect *soundEffect = new QSoundEffect(this);
    soundEffect->setSource(QUrl::fromLocalFile("C:/Users/MSI/Downloads/ding-101492.wav"));




    if (soundEffect->status() == QSoundEffect::Ready) {
        soundEffect->play();
    } else {
        qDebug() << "Error: Sound not ready. Status:" << soundEffect->status();
    }

    ui->bellN->show();
    notificationWidget->setStyleSheet(
        "background: rgb(70, 148, 156);"
        "color: white;"
        "text-align: left;"
        "border-radius: 8px;"
        "padding: 10px;"
        "QLabel {"
        "   min-width: 200px;"
        "   max-width: 250px;"
        "   word-wrap: break-word;"
        "}");
    notificationWidget->addNotification(message);
    notificationWidget->show();
    QTimer::singleShot(5000, notificationWidget, &QWidget::hide);
}

void MainWindow::handleNotificationClicked(const QString &message) {
    QMessageBox::information(this, "Notification", message);
}




void MainWindow::on_actiontaux_des_rendez_vous_triggered()
{   using namespace std;

    vector<int> tabCin;
    QSqlQuery query;
    query.prepare("SELECT CIN_RDV FROM RENDEZ_VOUS");
    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    while (query.next()) {

        int cin = query.value(0).toInt();
        qDebug() << "kaabet:"<<cin ;
        tabCin.push_back(cin);
    }


    unordered_map<int, int> cinCounts;
    for (int cin : tabCin) {
        cinCounts[cin]++;
    }


    int nv_patient = 0;
    int patient = 0;

    for (const auto& pair : cinCounts) {
        if (pair.second == 1) {
            nv_patient++;

        } else {
            nv_patient++;
            patient++;
        }
    }
    int totalPatients = nv_patient + patient;

    float pourcentageNouveaux = (static_cast<float>(nv_patient) / totalPatients) * 100;
    float pourcentageRecurrents = (static_cast<float>(patient) / totalPatients) * 100;

    qDebug() << "New patients (first visit):" << nv_patient;
    qDebug() << "Recurrent patients (returning):" << patient;

     vaccinTab->setCurrentIndex(13);




QBarSet *patientSet = new QBarSet("Patients");
*patientSet << pourcentageNouveaux << pourcentageRecurrents;


QColor barColor("#8bc1c7");
QColor textColor("#ffffff");


patientSet->setColor(barColor);

QBarSeries *series = new QBarSeries();
series->append(patientSet);

QChart *chart = new QChart();
chart->addSeries(series);
chart->setTitle("Nouveaux patients vs Patients récurrents");
chart->setAnimationOptions(QChart::SeriesAnimations);

chart->setBackgroundBrush(Qt::transparent);
chart->setTitleBrush(QBrush(textColor));
QFont titleFont;
titleFont.setFamily("Arial");
titleFont.setPointSize(18);
titleFont.setBold(true);
titleFont.setItalic(false);
titleFont.setUnderline(true);

chart->setTitleFont(titleFont);

QStringList categories = { "Nouveaux", "Récurrents" };
QBarCategoryAxis *axisX = new QBarCategoryAxis();
axisX->append(categories);
axisX->setLabelsBrush(QBrush(textColor));

QFont axisFont;
axisFont.setPointSize(10);
axisFont.setFamily("Arial");
axisFont.setBold(true);

axisX->setLabelsFont(axisFont);
chart->addAxis(axisX, Qt::AlignBottom);
series->attachAxis(axisX);

QValueAxis *axisY = new QValueAxis();
axisY->setRange(0, 100);
axisY->setLabelFormat("%d%%");
axisY->setLabelsBrush(QBrush(textColor));
chart->addAxis(axisY, Qt::AlignLeft);
series->attachAxis(axisY);


chart->legend()->setVisible(true);
chart->legend()->setLabelColor(textColor);


QChartView *chartView = new QChartView(chart);
chartView->setRenderHint(QPainter::Antialiasing, true);
chartView->setStyleSheet("background: transparent; border: none;");


if (ui->stat->layout()) {
    QLayoutItem *item;
    while ((item = ui->stat->layout()->takeAt(0))) {
        delete item->widget();
        delete item;
    }
    delete ui->stat->layout();
}



QObject::connect(series, &QBarSeries::hovered, [](bool state, int index, QBarSet* set) {
    if (state) {
        QString tooltipText = QString("%1: %2%").arg(set->label()).arg(set->at(index));
        QToolTip::showText(QCursor::pos(), tooltipText);
    } else {
        QToolTip::hideText();
    }
});




QVBoxLayout *layout = new QVBoxLayout(ui->stat);
layout->setContentsMargins(0, 0, 0, 0);
layout->addWidget(chartView);
ui->stat->setLayout(layout);
}






