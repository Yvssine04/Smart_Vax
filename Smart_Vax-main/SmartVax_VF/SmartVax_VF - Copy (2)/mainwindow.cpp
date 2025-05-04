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
#include <QPrinter>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QPageLayout>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QAudioDevice>
#include <QMediaFormat>
#include <windows.h>
#include <sapi.h>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioInput>
#include <objbase.h>
#include <sphelper.h>
#include <windows.h>
#include <QProcess>
#include <QProcess>
#include <QStringList>
//#pragma comment(lib, "winmm.lib") // Link against the Windows multimedia library



Connection::Connection() {}
//////////////////cam
QProcess *yoloProcess = nullptr;
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
    , cameraTimer(new QTimer(this)) // Proper initializer list
{
ui->setupUi(this);

    process = new QProcess(this);

    alertTimer = new QTimer(this);
    connect(alertTimer, &QTimer::timeout, this, &MainWindow::checkEquipmentQuantity);
    alertTimer->start(2000); // Check every minute

    // Configure audio player
    alertPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    alertPlayer->setAudioOutput(audioOutput);

    // Configure audio player with debug output
    QString audioPath = QCoreApplication::applicationDirPath() + "/voice.mp3";
    qDebug() << "Audio file path:" << audioPath; // Debug path

    alertPlayer->setSource(QUrl::fromLocalFile(audioPath));

    // Verify file existence
    if(alertPlayer->source().isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Fichier audio non trouvé dans les ressources !");
    }


    audioOutput->setVolume(0.8f);

    connect(alertPlayer, &QMediaPlayer::playbackStateChanged, this, [](QMediaPlayer::PlaybackState state) {
        qDebug() << "Playback state changed:" << state;
    });
    //alertPlayer->play();

    // In MainWindow constructor (after initializing alertPlayer)
    connect(alertPlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
        qDebug() << "Media Player Error:" << errorString;
    });


    connect(ui->vocale, &QPushButton::clicked, this, &MainWindow::startSpeechRecognition);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessOutput);

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessFinished);

// Try multiple potential paths
QString classifierPath = "haarcascade_frontalface_default.xml";  // Local to executable

if(!medicalClassifier.load(classifierPath.toStdString())) {
    // Try different common locations
    QString classifierPath = "C:/opencv/build/etc/haarcascades/haarcascade_frontalface_default.xml";


    if(!medicalClassifier.load(classifierPath.toStdString())) {
        QMessageBox::critical(this, "Error",
                              "Failed to load classifier at:\n" + classifierPath +
                                  "\nPlease ensure:\n"
                                  "1. The file exists at this location\n"
                                  "2. You have read permissions\n"
                                  "3. The path uses correct slashes");
        qDebug() << "Current working directory:" << QDir::currentPath();
    }
}

connect(cameraTimer, &QTimer::timeout, this, &MainWindow::processFrame);







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

    /////////////
    /// // For quantity line edit (assuming it's named lineEdit_85)
    ui->lineEdit_85->setValidator(new QIntValidator(1, 999999, this)); // Quantity must be ≥1

    // For price line edit (assuming it's named lineEdit_84)
    ui->lineEdit_84->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this)); // Price ≥0 with 2 decimals
 ui->tabequi->setSortingEnabled(true);



 // For quantity line edit (assuming it's named lineEdit_85)
 ui->lineEdit_11116->setValidator(new QIntValidator(1, 999999, this)); // Quantity must be ≥1

 // For price line edit (assuming it's named lineEdit_84)
 ui->lineEdit_11117->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this)); // Price ≥0 with 2 decimals

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
////////////////////////////////////////////////////////////////
connect(ui->pdfButton, &QPushButton::clicked, this, &MainWindow::onPdfButtonClicked);
connect(ui->actionStatistics, &QAction::triggered, this, &MainWindow::onStatistiqueActionTriggered);

/////////////////
    //connect(ffff, &QPushButton::clicked, this, &MainWindow::on_edit_equi_clicked);

    connect(ui->supprimerevent_3, &QPushButton::clicked, this, &MainWindow::on_supprime_eq_clicked);
 connect(ui->choix_equi, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onEquipementComboBoxIndexChanged);
   // connect(ui->recherche_eq, &QLineEdit::returnPressed, this, &MainWindow::onRechercheEqReturnPressed);
 //////////////////
  //  connect(ui->actionequipements, &QAction::triggered, this, &MainWindow::onStatistiqueActionTriggered);
//connect(ui->vocale, &QPushButton::clicked, this, &MainWindow::on_vocale_clicked);
    // In your main UI thread
connect(ui->camButton, &QPushButton::clicked, this, &MainWindow::on_camButton_clicked);
    speechProcess = new QProcess(this);
   // connect(speechProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::processSpeechResult);

/////////////////////////////////////////////////
   // connect(ui->vocale, &QPushButton::clicked, this, &MainWindow::onVocaleClicked);


connect(cameraTimer, &QTimer::timeout, this, &MainWindow::processFrame);


connect(ui->recherche_eq, &QLineEdit::returnPressed, this, &MainWindow::onRechercheEqReturnPressed);


connect(yoloProcess, &QProcess::readyReadStandardError, [=]() {
    QByteArray errorOutput = yoloProcess->readAllStandardError();
    qDebug() << "YOLO Error: " << errorOutput;
});




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

void MainWindow::onPdfButtonClicked() {
    // Open a file dialog to choose the save location
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return; // User canceled the dialog
    }

    // Fetch the list of equipment from the database
    Equipements equipement;
    QSqlQueryModel *model = equipement.afficher(); // Fetch data using your afficher method

    // Create an HTML string for the PDF content
    QString html = "<div style='border: 5px solid green; padding: 20px; font-family: Arial;'>"
                   "<h1 style='text-align: center; color: green;'>Facture des Equipments pour smartvax</h1>" // Title at the top
                   "<div style='margin: 0 auto; width: 90%;'>" // Center the table and adjust width
                   "<table border='1' cellpadding='10' cellspacing='0' style='width: 100%; border-collapse: collapse; font-size: 12px; margin: 0 auto;'>"
                   "<tr>"
                   "<th style='background-color: #f2f2f2; padding: 10px;'>Name</th>"
                   "<th style='background-color: #f2f2f2; padding: 10px;'>Price</th>"
                   "</tr>";

    double totalSum = 0.0; // Variable to store the total sum of prices

    for (int row = 0; row < model->rowCount(); ++row) {
        QString name = model->data(model->index(row, 1)).toString(); // Assuming "nom" is in column 1
        double price = model->data(model->index(row, 6)).toDouble(); // Assuming "prix" is in column 6
        totalSum += price; // Add the price to the total sum

        html += QString("<tr>"
                        "<td style='padding: 10px;'>%1</td>"
                        "<td style='padding: 10px; text-align: right;'>%2</td>"
                        "</tr>")
                    .arg(name)
                    .arg(QString::number(price, 'f', 2)); // Format price to 2 decimal places
    }

    // Add the total sum row to the table
    html += QString("<tr>"
                    "<td style='padding: 10px; font-weight: bold;'>Total</td>"
                    "<td style='padding: 10px; text-align: right; font-weight: bold;'>%1</td>"
                    "</tr>")
                .arg(QString::number(totalSum, 'f', 2)); // Format total sum to 2 decimal places

    html += "</table>"
            "</div>"; // Close the div for centering

    // Add the logo at the bottom of the page (smaller size)
    html += "<div style='text-align: center; margin-top: 10px;'>"
            "<img src=':/logo.png' style='transform: scale(0.2); transform-origin: center;' />"
            "</div>";

    // Add the address, email, telephone, and a description of the center at the bottom
    html += "<div style='text-align: center; font-family: Arial; font-size: 12px; margin-top: 20px;'>"
            "<p style='margin: 5px;'>Adresse: Ariana 2083</p>"
            "<p style='margin: 5px;'>Email: smartvax@yahoo.com</p>"
            "<p style='margin: 5px;'>Téléphone: +216 21 276 002</p>"
            "<p style='margin-top: 15px; font-style: italic;'>Le Centre de Vaccination et de Recherche Biologique offre des services de vaccination de qualité et mène des recherches avancées pour la santé publique.</p>"
            "</div>";

    html += "</div>"; // Close the outer div with the green border

    // Create a QTextDocument and set the HTML content
    QTextDocument document;
    document.setHtml(html);

    // Create a QPrinter to generate the PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    // Set page margins (optional)
    printer.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter); // 20mm margins on all sides

    // Print the document to the PDF file
    document.print(&printer);

    qDebug() << "PDF saved to:" << fileName;
}



/////////////////////////////////////////////////////////////////////////////////
void MainWindow::onEquipementComboBoxIndexChanged(int index) {
    QTableWidget *table = ui->tabequi;
    int column = -1;

    // Determine which column to sort by based on the combo box selection
    switch (index) {
    case 0: // "name"
        column = 3;
        break;
    case 1: // "type"
        column = 5;
        break;
    case 2:
        column = 6;
        break;
    default:
        qDebug() << "Invalid combo box index:" << index;
        return;
    }

    if (!table || table->rowCount() == 0 || column < 0 || column >= table->columnCount()) {
        qDebug() << "Invalid table or column index!";
        return;
    }

    bool isNumericColumn = (column == 5 || column == 6); // Quantité & Prix

    for (int row = 0; row < table->rowCount(); ++row) {
        QTableWidgetItem *item = table->item(row, column);
        if (!item) continue;

        if (isNumericColumn) {
            bool ok;
            // Store original text before converting to numeric value
            QString originalText = item->text().trimmed();

            double value = originalText.toDouble(&ok);
            if (ok) {
                item->setData(Qt::UserRole, value); // Store the numeric value for sorting
            } else {
                qDebug() << "Non-numeric value at row" << row << ":" << originalText;
                item->setData(Qt::UserRole, 0.0); // Fallback
            }
        } else {
            item->setData(Qt::UserRole, item->text()); // For string columns
        }
    }

    // Sort the table using the UserRole for sorting numeric values
    table->sortItems(column, Qt::AscendingOrder);

    qDebug() << "Sorted by column" << column << (isNumericColumn ? " numerically" : " alphabetically");
}



///////////////////
/*
void MainWindow::sortEquipementTable(QTableWidget *tableEqui, int column) {
    if (tableEqui == nullptr) {
        qDebug() << "Table widget is null!";
        return;
    }
    tableEqui->horizontalHeader()->setSortIndicatorShown(true);
    tableEqui->horizontalHeader()->setIconSize(QSize(32, 32)); // Adjust size as needed

    // Sort the table by the specified column
    tableEqui->sortItems(column, Qt::AscendingOrder);
}
*/


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

// In MainWindow::loadEquipementsData (mainwindow.cpp)
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
            QVariant data = model->data(index);
            QTableWidgetItem *item = nullptr;

            // Handle numeric columns differently
            if (col == 5 || col == 6) { // Quantity (5) and Price (6)
                bool ok;
                double numericValue = data.toDouble(&ok);

                if (ok) {
                    // Format quantity as integer without decimals
                    if (col == 5) {
                        item = new QTableWidgetItem(QString::number(numericValue, 'f', 0));
                    }
                    // Format price with conditional decimals
                    else {
                        if (numericValue == floor(numericValue)) {
                            item = new QTableWidgetItem(QString::number(numericValue, 'f', 0));
                        } else {
                            item = new QTableWidgetItem(QString::number(numericValue, 'f', 2));
                        }
                    }
                    // Store raw numeric value for sorting
                    item->setData(Qt::UserRole, numericValue);
                } else {
                    item = new QTableWidgetItem(data.toString());
                }
            } else {
                item = new QTableWidgetItem(data.toString());
            }
            ui->tabequi->setItem(row, col, item);
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

        ui->lineEdit_11111->setText(reference);
        ui->lineEdit_11114->setText(nom);
        ui->lineEdit_11112->setText(type);
        ui->dateEdit_11113->setDate(QDate::fromString(dateLimite, "yyyy-MM-dd"));
        ui->lineEdit_11115->setText(fournisseur);
        ui->lineEdit_11117->setText(quantite);
        ui->lineEdit_11116->setText(prix);

        vaccinTab->setCurrentIndex(11);
    } else {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner une ligne à éditer.");
    }
}




void MainWindow::on_modif_save_clicked() {
    QString reference = ui->lineEdit_11111->text();
    QString nom = ui->lineEdit_11114->text();
    QString type = ui->lineEdit_11112->text();
    QString dateLimite = ui->dateEdit_11113->date().toString("yyyy-MM-dd");
    QString fournisseur = ui->lineEdit_11115->text();
    int quantite = ui->lineEdit_11117->text().toInt();
    double prix = ui->lineEdit_11116->text().toDouble();

    Equipements equipement(reference, nom, type, dateLimite, fournisseur, quantite, prix);
    if (!equipement.modifier()) {
        QMessageBox::critical(this, "Erreur", "Échec de la mise à jour de la base de données.");
    } else {
        // Update the row in the table directly after saving
        int row = ui->tabequi->currentRow();
        ui->tabequi->item(row, 0)->setText(reference);
        ui->tabequi->item(row, 1)->setText(nom);
        ui->tabequi->item(row, 2)->setText(type);
        ui->tabequi->item(row, 3)->setText(dateLimite);
        ui->tabequi->item(row, 4)->setText(fournisseur);
        ui->tabequi->item(row, 5)->setText(QString::number(quantite));
        ui->tabequi->item(row, 6)->setText(QString::number(prix));

        QMessageBox::information(this, "Succès", "Les modifications ont été enregistrées dans la base de données.");
        vaccinTab->setCurrentIndex(11);
    }
    // Vérification directe de la quantité après modification
    if (quantite < 5) {
        // Réinitialiser et jouer l’audio
        alertPlayer->stop();
        alertPlayer->setPosition(0);
        alertPlayer->play();

        QMessageBox::warning(this, "Alerte Stock",
                             tr("Attention! Quantité critique pour %1 (Réf: %2)\nQuantité restante: %3\nCommander rapidement!")
                                 .arg(nom).arg(reference).arg(quantite));
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


void MainWindow::onStatistiqueActionTriggered() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database not connected!");
        return;
    }

    vaccinTab->setCurrentIndex(12); // Switch to the statistics tab

    // Clear previous chart widgets from the layout
    QLayoutItem* child;
    while ((child = ui->chartLayout->takeAt(0))) { // Remove all items from the layout
        if (child->widget()) {
            delete child->widget(); // Delete the widget (QChartView)
        }
        delete child; // Delete the layout item
    }

    // Fetch updated statistics data
    Equipements equipement;
    QMap<QString, int> stats = equipement.calculerStatistiques();

    // Create and display the new chart
    createEquipmentChart(stats);
}


void MainWindow::createEquipmentChart(const QMap<QString, int>& stats) {
    QPieSeries *series = new QPieSeries();

    // Add data to series
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        QPieSlice *slice = series->append(it.key() + " (" + QString::number(it.value()) + ")", it.value());
        slice->setLabelVisible();
        slice->setLabelColor(Qt::white);  // Set label text color to white
    }

    // Configure chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Equipment Distribution by Type");

    // Set background color
    chart->setBackgroundBrush(QBrush(QColor(63, 123, 134)));  // Your specified color
    chart->setTitleBrush(QBrush(Qt::white));  // Set title color to white
    chart->legend()->setLabelColor(Qt::white);  // Set legend text color to white
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);

    // Configure chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(800, 600);

    // Add to layout
    ui->chartLayout->addWidget(chartView);
}


void MainWindow::on_cameraButton_clicked()
{
    if (!cap.isOpened()) {
        cap.open(0);
        if (cap.isOpened()) {
            // Start the camera feed
            cameraTimer->start(10);
            ui->cameraButton->setText("Stop");
        }
    } else {
        // Properly release resources
        cap.release();
        cameraTimer->stop();

        // Clear the display using an empty pixmap
        ui->cameraFeed->setPixmap(QPixmap());

        // Optional: Force UI update
        qApp->processEvents();

        ui->cameraButton->setText("Start");
    }
}

void MainWindow::processFrame()
{
    cv::Mat frame;
    cap >> frame;  // Capture frame

    if (frame.empty()) {
        qDebug() << "Empty frame captured!";
        return;
    }

    // Pass the frame to the YOLOv5 model through a Python script or subprocess
    // Here you would call Python detect.py and pass the image data

    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    ui->cameraFeed->setPixmap(QPixmap::fromImage(img));  // Display camera feed
}




void MainWindow::on_camButton_clicked()
{
    if (!cap.isOpened()) {
        if (!cap.open(0)) {
            qDebug() << "Failed to open camera!";
            return;
        }


        ui->camButton->setText("Stop");

        // Start YOLOv5 without blocking UI
        QString pythonPath = "C:/Users/marie/Documents/yolov5/yolov5env/Scripts/python.exe";
        QString scriptPath = "C:/Users/marie/Documents/yolov5/detect.py";

        if (!QProcess::startDetached(pythonPath, QStringList() << scriptPath << "--weights" << "yolov5s.pt" << "--source" << "0")) {
            qDebug() << "Failed to start YOLOv5!";
        }

    } else {
        cap.release();
        cameraTimer->stop();
        ui->camButton->setText("Start");
        ui->cameraFeed->clear();
    }
}
/*

void MainWindow::on_vocale_clicked() {
    QString text = reconnaitreParole();
    qDebug() << "Texte reconnu :" << text; // Ajoute ceci pour voir ce qui est capté

    if (!text.isEmpty()) {
        ui->recherche_eq->setText(text.trimmed());
        onRechercheEqReturnPressed();
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune parole détectée.");
    }
}


void MainWindow::processAudioFile()
{
    if(QFile::exists(m_lastAudioFile)) {
        QString recognizedText = this->recognizeSpeech(m_lastAudioFile);
        if(!recognizedText.isEmpty()) {
            ui->recherche_eq->setText(recognizedText.trimmed());
            this->onRechercheEqReturnPressed();
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'enregistrement audio!");
    }
}
void MainWindow::onRechercheEqReturnPressed() {
    QString reference = ui->recherche_eq->text().trimmed();
    qDebug() << "Recherche référence:" << reference;

    bool found = false;

    for (int row = 0; row < ui->tabequi->rowCount(); ++row) {
        QTableWidgetItem *item = ui->tabequi->item(row, 0);
        bool match = (item && item->text().trimmed().compare(reference, Qt::CaseInsensitive) == 0);
        ui->tabequi->setRowHidden(row, !match);

        if (match) {
            qDebug() << "Équipement trouvé à la ligne :" << row;
            ui->tabequi->selectRow(row);
            ui->tabequi->scrollToItem(item);
            found = true;
        }
    }

    if (!found) {
        qDebug() << "Aucun équipement trouvé.";
        QMessageBox::warning(this, "Introuvable", "Aucun équipement trouvé pour : " + reference);
    }
}


// Basic speech recognition (implement proper speech recognition here)
QString MainWindow::recognizeSpeech(const QString &filePath)
{
    // TEMPORARY IMPLEMENTATION - REPLACE WITH ACTUAL SPEECH RECOGNITION
    QFile file(filePath);
    if(file.exists()) {
        QMessageBox::information(this, "Debug", "Fichier audio enregistré avec succès!");
        // Here you would implement actual speech-to-text conversion
        // For testing purposes, return a dummy text
        return "TEST123"; // Replace with actual recognized text
    }
    return "";
}

QString MainWindow::reconnaitreParole() {
    qDebug() << "Début de la reconnaissance vocale...";
    ISpRecognizer *pRecognizer = nullptr;
    ISpRecoContext *pContext = nullptr;
    ISpRecoGrammar *pGrammar = nullptr;
    ISpPhrase *pPhrase = nullptr;
    HRESULT hr = CoInitialize(NULL);

    QString recognizedText;

    if (SUCCEEDED(hr)) {
        qDebug() << "Recognizer initialisé...";
        hr = CoCreateInstance(CLSID_SpSharedRecognizer, NULL, CLSCTX_ALL, IID_ISpRecognizer, (void**)&pRecognizer);
    }

    if (SUCCEEDED(hr)) {
        qDebug() << "Contexte de reconnaissance créé...";
        hr = pRecognizer->CreateRecoContext(&pContext);
    }

    if (SUCCEEDED(hr)) {
        qDebug() << "Grammaire chargée...";
        hr = pContext->CreateGrammar(1, &pGrammar);
    }

    if (SUCCEEDED(hr)) {
        hr = pGrammar->LoadDictation(NULL, SPLO_STATIC);
    }

    if (SUCCEEDED(hr)) {
        hr = pGrammar->SetDictationState(SPRS_ACTIVE);
    }

    if (SUCCEEDED(hr)) {
        qDebug() << "Début de l'écoute...";
        while (true) {
            SPEVENT event;
            ULONG eventCount;
            hr = pContext->GetEvents(1, &event, &eventCount);

            if (SUCCEEDED(hr) && eventCount > 0 && event.eEventId == SPEI_RECOGNITION) {
                pPhrase = (ISpPhrase*)event.lParam;
                SPPHRASE *pElements;
                hr = pPhrase->GetPhrase(&pElements);

                if (SUCCEEDED(hr)) {
                    recognizedText = QString::fromWCharArray(pElements->pElements->pszDisplayText);
                    qDebug() << "Texte détecté :" << recognizedText;
                    CoTaskMemFree(pElements);
                    break;
                }
            }
        }
    } else {
        qDebug() << "Erreur lors de la reconnaissance !";
    }

    if (pGrammar) pGrammar->Release();
    if (pContext) pContext->Release();
    if (pRecognizer) pRecognizer->Release();
    CoUninitialize();

    return recognizedText;
}
*/


/*
void MainWindow::onVocaleClicked() {
    speechProcess->start("python", QStringList() << "speech_to_text.py");
}

void MainWindow::processSpeechResult() {
    QString output = speechProcess->readAllStandardOutput().trimmed();
    ui->recherche_eq->setText(output);  // Set text in QLineEdit
}
*/
void MainWindow::startSpeechRecognition() {
    // Check if process is already running
    if (process->state() == QProcess::NotRunning) {
        qDebug() << "Starting speech recognition process...";

        // Start the Python script
        process->start("python", QStringList() << "C:/Users/marie/Downloads/Smart_Vax-main/Smart_Vax-main/SmartVax_VF/speech_to_text.py");
    } else {
        qDebug() << "Process is already running.";
    }
}

void MainWindow::onProcessOutput() {
    QByteArray output = process->readAllStandardOutput();
    QString recognizedText = QString::fromUtf8(output);
    ui->recherche_eq->setText(recognizedText);  // Display recognized text in QLineEdit
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitStatus == QProcess::NormalExit) {
        qDebug() << "Process finished successfully with exit code" << exitCode;
    } else {
        qDebug() << "Process crashed with exit code" << exitCode;
    }
}
void MainWindow::onRechercheEqReturnPressed() {
    QString searchTerm = ui->recherche_eq->text().trimmed(); // Get input search term, and remove any leading/trailing spaces

    // Debug: Print the entered search term
    qDebug() << "Entered search term:" << searchTerm;

    bool found = false;

    // Iterate over the rows in the table and hide those that do not match the entered reference or name
    for (int row = 0; row < ui->tabequi->rowCount(); ++row) {
        QTableWidgetItem* referenceItem = ui->tabequi->item(row, 0); // Assuming the reference is in the first column
        QTableWidgetItem* nameItem = ui->tabequi->item(row, 2); // Assuming the name is in the second column

        bool matchesReference = false;
        bool matchesName = false;

        if (referenceItem) {
            matchesReference = referenceItem->text().toLower().contains(searchTerm.toLower());
        }

        if (nameItem) {
            matchesName = nameItem->text().toLower().contains(searchTerm.toLower());
        }

        // If either the reference or the name matches the search term, show the row
        if (matchesReference || matchesName) {
            ui->tabequi->showRow(row);
            found = true;
        } else {
            ui->tabequi->hideRow(row);
        }
    }

    // If no rows are found after the filtering, show a warning
    if (!found) {
        QMessageBox::warning(this, "Non trouvé", "Aucune référence ou nom trouvé.");
    }
}



/*
void MainWindow::checkEquipmentQuantity()
{
    QSqlQuery query("SELECT REFERNCE_EQ, NOM_EQ, QUANTITE_EQ FROM EQUIPEMENTS WHERE QUANTITE_EQ < 5");

    bool alertTriggered = false;

    while (query.next()) {
        QString reference = query.value(0).toString();
        QString name = query.value(1).toString();
        int quantity = query.value(2).toInt();

        if(!alertedEquipment.contains(reference)) {
            alertTriggered = true;
            alertedEquipment.insert(reference);

            // Show visual alert first
            QMessageBox::warning(this, "Alerte Stock",
                                 tr("Attention! Quantité critique pour %1 (Réf: %2)\nQuantité restante: %3\nCommander rapidement!")
                                     .arg(name).arg(reference).arg(quantity));

            // Play voice alert after dialog is shown
            if(alertPlayer->playbackState() != QMediaPlayer::PlayingState) {
                alertPlayer->play();
            }
        }
    }

    // Clear alerts for items that are now above threshold
    QSqlQuery validQuery("SELECT REFERNCE_EQ FROM EQUIPEMENTS WHERE QUANTITE_EQ >= 5");
    while (validQuery.next()) {
        alertedEquipment.remove(validQuery.value(0).toString());
    }

    // Stop audio if no alerts were triggered
    if(!alertTriggered && alertPlayer->playbackState() == QMediaPlayer::PlayingState) {
        alertPlayer->stop();
    }
}
*/
/*void MainWindow::checkEquipmentQuantity()
{
    QSqlQuery query("SELECT REFERNCE_EQ, NOM_EQ, QUANTITE_EQ FROM EQUIPEMENTS WHERE QUANTITE_EQ < 5");
    bool alertTriggered = false;

    while (query.next()) {
        QString reference = query.value(0).toString();
        QString name = query.value(1).toString();
        int quantity = query.value(2).toInt();
        alertTriggered = true;

        // Réinitialiser le lecteur audio
        alertPlayer->stop();
        alertPlayer->setPosition(0); // Retour au début
        alertPlayer->play();

        QMessageBox::warning(this, "Alerte Stock",
                             tr("Attention! Quantité critique pour %1 (Réf: %2)\nQuantité restante: %3\nCommander rapidement!")
                                 .arg(name).arg(reference).arg(quantity));
    }

    if(!alertTriggered && alertPlayer->playbackState() == QMediaPlayer::PlayingState) {
        alertPlayer->stop();
    }
}

*/

void MainWindow::checkEquipmentQuantity()
{
    QSqlQuery query("SELECT REFERNCE_EQ, NOM_EQ, QUANTITE_EQ FROM EQUIPEMENTS WHERE QUANTITE_EQ < 5");
    bool alertTriggered = false;
    QString alertText;

    while (query.next()) {
        QString reference = query.value(0).toString();
        QString name = query.value(1).toString();
        int quantity = query.value(2).toInt();
        alertTriggered = true;

        alertText += tr("Attention! Quantité critique pour %1 (Réf: %2)\nQuantité restante: %3\n\n")
                         .arg(name).arg(reference).arg(quantity);
    }

    if (alertTriggered && !alertShown) {
        alertShown = true;

        // Jouer l'audio une seule fois
        alertPlayer->stop();
        alertPlayer->setPosition(0);
        alertPlayer->play();

        // Afficher l'alerte
        QMessageBox::warning(this, "Alerte Stock", alertText);
    }
    else if (!alertTriggered) {
        alertShown = false;  // Réinitialiser si tout est OK
    }
}





/*
void MainWindow::checkEquipmentQuantity() {
    QSqlQuery query("SELECT nom, quantite FROM equipements");

    while (query.next()) {
        QString equipName = query.value(0).toString();
        int quantite = query.value(1).toInt();

        if (quantite < 5) {
            qDebug() << "Low quantity detected for" << equipName << "- Quantity:" << quantite;

            if (alertPlayer->playbackState() != QMediaPlayer::PlayingState) {
                alertPlayer->play(); // ✅ Only play if not already playing
            }

            // Optional: show warning
            QMessageBox::warning(this, "Alerte", "La quantité de " + equipName + " est inférieure à 5 !");
        }
    }
}
*/
