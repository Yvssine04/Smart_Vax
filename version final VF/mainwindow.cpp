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
#include <QVBoxLayout>
#include <QQuickWidget>
#include <QListWidget>
#include <QQuickItem>
#include <QQmlContext>
#include <QtCharts>
#include <QSslSocket>
#include <QMimeDatabase>


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
    ,rdvWindow (new rendez_vous(this))
{

    ui->setupUi(this);
    listevent = ui->listevent;
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
    ui->choix_vac->addItem("Prix");
    ui->choix_vac->addItem("Date");
    ui->choix_vac->addItem("Quantite");
    ui->tabequi->setSortingEnabled(true);
    ui->tabevent->setSortingEnabled(true);
    ui->tabvaccin->setSortingEnabled(true);
    connect(ui->tabvaccin->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onVaccinHeaderSectionClicked);
    vaccinTab->setStyleSheet(
        "QTabWidget::pane { border: none; background: transparent; }"
        "QTabBar::tab { height: 0; width: 0; }"
        "QTabBar { height: 0; }"
        );


    vaccinTab->setCurrentIndex(7);
    connect(ui->pdfvaccin, &QPushButton::clicked, this, &MainWindow::onPdfVaccinButtonClicked);
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

    connect(ui->pdfinvi, &QPushButton::clicked, this ,[=]() {
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



    //connect(ffff, &QPushButton::clicked, this, &MainWindow::on_edit_equi_clicked);

    connect(ui->supprimerevent_3, &QPushButton::clicked, this, &MainWindow::on_supprime_eq_clicked);
    connect(ui->choix_equi, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onEquipementComboBoxIndexChanged);
    connect(ui->recherche_eq, &QLineEdit::returnPressed, this, &MainWindow::onRechercheEqReturnPressed);
    connect(ui->tri_rdv, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::comboIndex_rdv);

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


    //////////////////////////////////////////////////////////////////////////////////////////

    // Récupérer l'onglet existant
    QWidget *carteTab = vaccinTab->widget(14);

    // Créer un QGridLayout
    QGridLayout *layout = new QGridLayout(carteTab);

    // Ajouter le QQuickWidget
    quickWidget = new QQuickWidget(this);
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quickWidget->setFixedSize(1375, 530);  // Fixer la taille du quickWidget

    // Ajouter le QListWidget dans un QScrollArea
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(listevent);  // Ajouter la liste des événements dans la zone défilable
    listevent->setFixedSize(1375, 100);  // Fixer la taille de listevent

    // Ajouter les widgets à la grille
    layout->addWidget(quickWidget, 0, 0);  // Ajouter le quickWidget
    layout->addWidget(scrollArea, 1, 0);  // Ajouter la zone défilable de la liste

    // Appliquer le layout à l'onglet
    carteTab->setLayout(layout);

    //////////////////////////////////////////////////////////////////////////////////////////
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_showstat_clicked() {
    vaccinTab->setCurrentIndex(15);

    // Récupérer les statistiques de type d'événement
    QMap<QString, int> stats = evenement->compterEvenementsParType();

    // Vérifier si la carte est vide
    if (stats.isEmpty()) {
        QMessageBox::warning(this, "Aucune donnée", "Aucun événement trouvé.");
        return;
    }

    // Supprimer le graphique précédent s'il existe
    if (chartView) {
        delete chartView;
        chartView = nullptr;
    }

    // Création du graphique pour la répartition des types d'événements
    QPieSeries *seriesType = new QPieSeries();
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        QPieSlice *slice = seriesType->append(it.key(), it.value());
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1: %2").arg(it.key()).arg(it.value()));
    }

    QChart *chartType = new QChart();
    chartType->addSeries(seriesType);
    chartType->setTitle("Répartition des types d'événements");

    chartView = new QChartView(chartType);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);
    chartView->setMaximumSize(700, 500);

    // Récupérer les statistiques de répartition par région
    QMap<QString, int> statsRegion = evenement->repartitionParRegion();

    // Vérifier si la map n'est pas vide avant de récupérer la valeur max
    int maxValue = statsRegion.isEmpty() ? 1 : *std::max_element(statsRegion.begin(), statsRegion.end());

    // Création du graphique en barres pour la répartition par région
    QBarSeries *seriesRegion = new QBarSeries();
    QBarSet *set = new QBarSet("Nombre d'événements");

    QStringList categories;
    for (auto it = statsRegion.begin(); it != statsRegion.end(); ++it) {
        *set << it.value();
        categories << it.key(); // Ajouter les noms des régions
    }

    seriesRegion->append(set);

    QChart *chartRegion = new QChart();
    chartRegion->addSeries(seriesRegion);
    chartRegion->setTitle("Répartition des événements par région");
    chartRegion->setAnimationOptions(QChart::SeriesAnimations);

    // Axe X (Catégories des régions)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chartRegion->addAxis(axisX, Qt::AlignBottom);
    seriesRegion->attachAxis(axisX);

    // Axe Y (Dynamique avec graduation de 1 en 1)
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, maxValue);  // Plage dynamique
    axisY->setTickCount(maxValue + 1);  // Graduation tous les 1
    axisY->setLabelFormat("%d");  // Affichage en entier
    chartRegion->addAxis(axisY, Qt::AlignLeft);
    seriesRegion->attachAxis(axisY);

    // Affichage du graphique
    QChartView *chartViewRegion = new QChartView(chartRegion);
    chartViewRegion->setRenderHint(QPainter::Antialiasing);
    chartViewRegion->setMinimumSize(400, 300);
    chartViewRegion->setMaximumSize(700, 500);



    // Créer le layout de la page
    QWidget *statWidget = findChild<QWidget*>("statWidget");
    if (statWidget) {
        QGridLayout *layout = qobject_cast<QGridLayout*>(statWidget->layout());
        if (!layout) {
            layout = new QGridLayout(statWidget);
            statWidget->setLayout(layout);
        }

        // Diviser la page en 4 coins
        layout->addWidget(chartView, 0, 0, 2, 2);  // Graphique des types d'événements dans le coin supérieur gauche (2x2)
        layout->setRowStretch(0, 2);  // Ajuster la taille des lignes
        layout->setRowStretch(1, 1);
        layout->setColumnStretch(0, 2);  // Ajuster la taille des colonnes
        layout->setColumnStretch(1, 1);

        // Ajouter le graphique de répartition par région dans la moitié droite
        layout->addWidget(chartViewRegion, 0, 2, 2, 2);  // Graphique des régions dans le coin supérieur droit (2x2)
        layout->setRowStretch(0, 2);
        layout->setRowStretch(1, 1);
        layout->setColumnStretch(2, 2);
        layout->setColumnStretch(3, 1);
    }
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
    vaccinTab->setCurrentIndex(13);
}

void MainWindow::on_geventg_clicked()
{
    vaccinTab->setCurrentIndex(2);
    eventManager->loadEventData(ui->tabevent);
}

void MainWindow::on_Quit_3_clicked() {
    vaccinTab->setCurrentIndex(2);
}

void MainWindow::on_Quit_7_clicked(){
    vaccinTab->setCurrentIndex(13);
}


void MainWindow::on_Quit_13_clicked() {
    vaccinTab->setCurrentIndex(2);
    eventManager->loadEventData(ui->tabevent);
}

void MainWindow::on_equiB_clicked() {
    vaccinTab->setCurrentIndex(4);
    loadEquipementsData();
}


void MainWindow::on_Quit_5_clicked()
{
    vaccinTab->setCurrentIndex(0);
}

void MainWindow::on_rdv_clicked() {
    vaccinTab->setCurrentIndex(6);
    rdvWindow->loadAppointments(ui->liste_att);
}

void MainWindow::on_Quit_clicked() {
    vaccinTab->setCurrentIndex(0);
}

void MainWindow::on_Quit_4_clicked() {
    vaccinTab->setCurrentIndex(7);
}

//////////////////////////////////////////////////////////////////////////////////////////

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
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmer la suppression", "Êtes-vous sûr de vouloir supprimer cet élément?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            QTableWidgetItem* referenceItem = ui->tabvaccin->item(selectedRow, 0);
            int reference = referenceItem->text().toInt();
            vaccinManager->deleteVaccin(reference);
            vaccinManager->loadVaccinData(ui->tabvaccin);
            selectedRow = -1;
        } else {
            QMessageBox::information(this, "Opération annulée", "La suppression a été annulée.");
        }
    } else {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un élément du tableau.");
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

        // Refresh the table to show updated data
        vaccinManager->loadVaccinData(ui->tabvaccin);
        showAllRows();
    } else {
        QMessageBox::warning(this, "No Selection", "Veuillez sélectionner un élément du tableau.");
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

    // Clear the input fields
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

    // Refresh the table to show updated data
    vaccinManager->loadVaccinData(ui->tabvaccin);
    showAllRows();
}

void MainWindow::showAllRows() {
    for (int row = 0; row < ui->tabvaccin->rowCount(); ++row) {
        ui->tabvaccin->setRowHidden(row, false);
    }
}



void MainWindow::onVaccinHeaderSectionClicked(int logicalIndex) {
    static Qt::SortOrder sortOrder = Qt::AscendingOrder;
    sortOrder = (sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    vaccinManager->sortVaccinTable(ui->tabvaccin, logicalIndex, sortOrder);
}

void MainWindow::onComboBoxIndexChanged(int index) {
    static Qt::SortOrder sortOrder = Qt::AscendingOrder; // Static variable to keep track of sort order
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
        qDebug() << "Invalid combo box index:" << index;
        return;
    }

    qDebug() << "Sorting by column:" << column;

    // Sort the table by the selected column
    if (column != -1) {
        vaccinManager->sortVaccinTable(table, column, sortOrder);
        sortOrder = (sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder; // Toggle sort order
    }
}

void MainWindow::onPdfVaccinButtonClicked() {
    QModelIndexList selection = ui->tabvaccin->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un vaccin.");
        return;
    }
    vaccinManager->exporterVaccinPDF(ui->tabvaccin);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_save_event_clicked() {
    int id_event = ui->lineEdit_id->text().toInt();
    QString nom_event = ui->lineEdit_nom->text();
    QString nom_responsable = ui->lineEdit_nomR->text();
    QString type = ui->lineEdit_type->text();
    QDateTime date_heure = ui->date_event->dateTime();
    QString lieu = ui->lineEdit_lieu->text();
    int nbr_participant = ui->lineEdit_nbrp->text().toInt();

    eventManager->saveEventData(id_event, nom_event, nom_responsable, type, date_heure.toString("yyyy-MM-dd HH:mm:ss"), lieu, nbr_participant);
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
        eventManager->loadEventData(ui->tabevent);;
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


void MainWindow::updateEventList(QString ville)
{
    listevent->clear();

    if (ville.isEmpty()) {
        listevent->addItem("Aucune ville sélectionnée");
        return;
    }

    Evenement evenement;
    QList<EvenementData> evenements = evenement.chargerEvenements();

    bool found = false;
    for (const EvenementData &event : evenements) {
        if (event.lieu == ville) {
            QString eventText = QString("%1 - %2 - %3 - %4 - %5").arg(event.id).arg(event.nom).arg(event.resp).arg(event.date_heure).arg(event.lieu);
            listevent->addItem(eventText);
            found = true;
        }
    }

    if (!found) {
        listevent->addItem("Aucun événement trouvé pour cette ville");
    }
}


void MainWindow::on_showmap_clicked()
{
    vaccinTab->setCurrentIndex(14);

    if (vaccinTab->currentIndex() == 14) {
        if (!quickWidget) {
            quickWidget = new QQuickWidget(this);
            quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        }

        listevent = ui->listevent;
        listevent->clear();
        listevent->addItem("Aucune ville sélectionnée");

        Evenement evenement;
        QList<EvenementData> evenements = evenement.chargerEvenements();

        // Compter les événements par ville
        QMap<QString, int> eventCountByCity;
        for (const EvenementData &event : evenements) {
            eventCountByCity[event.lieu]++;
        }

        QVariantList eventsData;
        for (const EvenementData &event : evenements) {
            QVariantMap eventMap;
            eventMap["nom"] = event.nom;
            eventMap["latitude"] = event.latitude;
            eventMap["longitude"] = event.longitude;
            eventMap["ville"] = event.lieu;
            eventMap["count"] = eventCountByCity[event.lieu];
            eventsData.append(eventMap);
        }

        QQmlContext *context = quickWidget->rootContext();
        context->setContextProperty("eventHandler", this);
        context->setContextProperty("eventsModel", QVariant::fromValue(eventsData));
        quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));

        QWidget *carteTab = vaccinTab->widget(14);
        if (!carteTab->layout()) {
            QVBoxLayout *layout = new QVBoxLayout();
            layout->addWidget(quickWidget);
            layout->addWidget(listevent);
            carteTab->setLayout(layout);
        }
    }
}


QString MainWindow::generateEmailBody(const QString& prompt)
{
    QEventLoop loop;
    QNetworkAccessManager manager;

    // Préparer la requête
    QNetworkRequest request(QUrl("https://deepseek-all-in-one.p.rapidapi.com/chat"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("x-rapidapi-key", "8f2a186f9dmsh94a0061ada0abb6p1d7c82jsnbf680cae3132");
    request.setRawHeader("x-rapidapi-host", "deepseek-all-in-one.p.rapidapi.com");

    // Corps de la requête JSON
    QJsonObject message1{{"role", "system"}, {"content", "You are a helpful assistant."}};
    QJsonObject message2{{"role", "user"}, {"content", prompt}};
    QJsonArray messages;
    messages.append(message1);
    messages.append(message2);

    QJsonObject bodyJson;
    bodyJson["messages"] = messages;

    QNetworkReply* reply = manager.post(request, QJsonDocument(bodyJson).toJson());

    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        loop.quit();
    });

    loop.exec(); // Bloque jusqu’à la réponse

    // Lire la réponse
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Erreur HTTP:" << reply->errorString();
        reply->deleteLater();
        return "Erreur lors de la génération du contenu.";
    }

    QByteArray responseBytes = reply->readAll();
    reply->deleteLater();

    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseBytes);
    if (!jsonResponse.isObject()) {
        return "Réponse invalide de l'API.";
    }

    // Extraire le contenu de la réponse
    QJsonObject responseObj = jsonResponse.object();
    QJsonArray choices = responseObj["choices"].toArray();
    if (choices.isEmpty()) return "Pas de contenu retourné.";

    QString emailBody = choices[0].toObject()["message"].toObject()["content"].toString();
    return emailBody;
}


void MainWindow::on_invimail_clicked()
{
    // Sélectionner le fichier à envoyer
    QString filePath = QFileDialog::getOpenFileName(this, "Sélectionner un fichier à joindre");
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Aucun fichier sélectionné", "Veuillez sélectionner un fichier à envoyer.");
        return;
    }

    // Lire le fichier et l'encoder en Base64
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Erreur de fichier", "Impossible d'ouvrir le fichier sélectionné.");
        return;
    }
    QByteArray fileData = file.readAll();
    QString base64FileData = fileData.toBase64();
    file.close();

    // Configuration du serveur SMTP (Gmail)
    QString smtpServer = "smtp.gmail.com";
    int smtpPort = 465;  // Pour une connexion cryptée SSL
    QString senderEmail = "smartvax6@gmail.com";
    QString senderPassword = "vgxl glgw xxzg jwup";  // Remplacez par le mot de passe d'application Gmail
    QString receiverEmail = "yassinebenaissa2004@gmail.com";
    QString subject = "Invitation à notre événement";
    QString prompt = "Rédige un email poli et professionnel pour inviter une personne à un événement médical sur la vaccination. Le contenu doit rester général, sans inclure de détails spécifiques comme la date, l’heure ou le lieu. Commence par 'Cher patient' ou 'Chère patiente'. Mentionne que le fichier joint est l'invitation officielle. À la fin de l’email, ajoute que nous sommes SmartVax.";
    QString body = generateEmailBody(prompt);


    // Configurer la connexion cryptée via QSslSocket
    QSslSocket* socket = new QSslSocket(this);
    socket->connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket->waitForConnected(5000)) {
        QMessageBox::warning(this, "Échec de la connexion", "Impossible de se connecter au serveur SMTP.");
        delete socket;
        return;
    }

    // Préparer le contenu de l'email
    QByteArray emailContent;
    emailContent.append("EHLO localhost\r\n");
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    QByteArray serverResponse = socket->readAll();
    qDebug() << "Réponse EHLO: " << serverResponse;

    emailContent.clear();
    emailContent.append("AUTH LOGIN\r\n");
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();
    qDebug() << "Réponse AUTH LOGIN: " << serverResponse;

    // Envoyer les identifiants de l'email (encodés en Base64)
    emailContent.clear();
    emailContent.append(QByteArray(senderEmail.toUtf8()).toBase64() + "\r\n");
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();
    qDebug() << "Réponse Nom d'utilisateur: " << serverResponse;

    emailContent.clear();
    emailContent.append(QByteArray(senderPassword.toUtf8()).toBase64() + "\r\n");
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();
    qDebug() << "Réponse Mot de passe: " << serverResponse;

    // Envoyer l'email
    emailContent.clear();
    emailContent.append("MAIL FROM: <" + senderEmail.toUtf8() + ">\r\n");
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();
    qDebug() << "Réponse MAIL FROM: " << serverResponse;

    emailContent.clear();
    emailContent.append("RCPT TO: <" + receiverEmail.toUtf8() + ">\r\n");
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();
    qDebug() << "Réponse RCPT TO: " << serverResponse;

    emailContent.clear();
    emailContent.append("DATA\r\n");
    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();
    qDebug() << "Réponse DATA: " << serverResponse;

    // Construire le contenu de l'email avec le corps et la pièce jointe
    emailContent.clear();
    emailContent.append("Subject: " + subject.toUtf8() + "\r\n");
    emailContent.append("To: " + receiverEmail.toUtf8() + "\r\n");
    emailContent.append("MIME-Version: 1.0\r\n");
    emailContent.append("Content-Type: multipart/mixed; boundary=boundary42\r\n\r\n");

    // Corps de l'email
    emailContent.append("--boundary42\r\n");
    emailContent.append("Content-Type: text/plain\r\n\r\n");
    emailContent.append(body.toUtf8() + "\r\n\r\n");

    // Pièce jointe
    emailContent.append("--boundary42\r\n");
    emailContent.append("Content-Type: application/octet-stream\r\n");
    emailContent.append("Content-Transfer-Encoding: base64\r\n");
    emailContent.append("Content-Disposition: attachment; filename=\"" + QFileInfo(filePath).fileName().toUtf8() + "\"\r\n\r\n");
    emailContent.append(base64FileData.toUtf8() + "\r\n");
    emailContent.append("--boundary42--\r\n.\r\n");

    socket->write(emailContent);
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();
    qDebug() << "Réponse Contenu de l'email envoyé: " << serverResponse;

    // Envoyer la commande QUIT
    socket->write("QUIT\r\n");
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    serverResponse = socket->readAll();
    qDebug() << "Réponse QUIT: " << serverResponse;

    // Déconnexion
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        socket->waitForDisconnected();
    }

    QMessageBox::information(this, "Email envoyé", "L'email a été envoyé avec succès avec la pièce jointe.");
    delete socket;

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


    bool verif =rdvWindow->saveAppointment(CIN, vaccin, date_rdv, adresse, nom, prenom, dispo, medecin, infirmier, salle, facturation);

    if (verif){
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
}


void MainWindow::on_deleteRdv_clicked()
{
    QListWidgetItem *selectedItem = ui->liste_att->currentItem();
    if (selectedItem) {
        QString fullText = selectedItem->text();

        QString cini = fullText.split(" ").first();

        int cin=cini.toInt();

        rdvWindow->supprimerRdv(cin);
        rdvWindow->loadAppointments(ui->liste_att);
    }        else {
        QMessageBox::warning(this, "Attentez", "Veuillez sélectionner un élément de la liste.");
    }
}


void MainWindow::on_edit_rdv_clicked()
{

    QListWidgetItem *selectedItem = ui->liste_att->currentItem();
    if (selectedItem) {
        QString fullText = selectedItem->text();

        QString cini = fullText.split(" ").first();

        int cin=cini.toInt();



        rdvWindow->infoEdit(cin,ui->CIN_rdv_2,ui->date_rdv_2,ui->vaccin_3,ui->adresse_2,ui->nom_rdv_2,ui->prenom_rdv_2,ui->dispo_2,ui->medecin_att_2,ui->infirmier_att_2,ui->salle_att_2,ui->facturation_2);
        rdvWindow->loadVaccins(ui->vaccin_3,ui->medecin_att_2,ui->infirmier_att_2);

        vaccinTab->setCurrentIndex(12);}

    else {
        QMessageBox::warning(this, "Attentez", "Veuillez sélectionner un élément de la liste.");
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

    rdvWindow->modifier_rdv(CINi, vaccin2,date_rdv2,adresse2,nom2, prenom2, dispo2,medecin2, infirmier2, salle2,facturation2);


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

void MainWindow::rdv_recherche(const QString &text){

    for (int i = 0; i < ui->liste_att->count(); ++i) {
        QListWidgetItem *item = ui->liste_att->item(i);
        bool match = item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!match);
    }

}


void MainWindow::comboIndex_rdv(int index){


    QList<QListWidgetItem*> items;

    // Remove all items from the list and store them temporarily
    while (ui->liste_att->count() > 0) {
        items.append(ui->liste_att->takeItem(0));
    }

    // Sort items based on the selected criteria (CIN, Nom, Prénom, Vaccin)
    std::sort(items.begin(), items.end(), [index](QListWidgetItem *a, QListWidgetItem *b) {
        int cinA = a->data(Qt::UserRole).toInt();
        int cinB = b->data(Qt::UserRole).toInt();
        QString nomA = a->data(Qt::UserRole + 1).toString().toLower();
        QString nomB = b->data(Qt::UserRole + 1).toString().toLower();
        QString prenomA = a->data(Qt::UserRole + 2).toString().toLower();
        QString prenomB = b->data(Qt::UserRole + 2).toString().toLower();
        QString vaccinA = a->data(Qt::UserRole + 3).toString().toLower();
        QString vaccinB = b->data(Qt::UserRole + 3).toString().toLower();

        if (index == 0) {  // Sort by CIN, then Nom, then Prénom, then Vaccin
            if (cinA != cinB) return cinA < cinB;
            if (nomA != nomB) return nomA < nomB;
            if (prenomA != prenomB) return prenomA < prenomB;
            return vaccinA < vaccinB;
        }
        else if (index == 1) {  // Sort by Nom, then Prénom, then CIN, then Vaccin
            if (nomA != nomB) return nomA < nomB;
            if (prenomA != prenomB) return prenomA < prenomB;
            if (cinA != cinB) return cinA < cinB;
            return vaccinA < vaccinB;
        }
        else if (index == 2) {  // Sort by Prénom, then Nom, then CIN, then Vaccin
            if (prenomA != prenomB) return prenomA < prenomB;
            if (nomA != nomB) return nomA < nomB;
            if (cinA != cinB) return cinA < cinB;
            return vaccinA < vaccinB;
        }
        else if (index == 3) {  // Sort by Vaccin, then Nom, then Prénom, then CIN
            if (vaccinA != vaccinB) return vaccinA < vaccinB;
            if (nomA != nomB) return nomA < nomB;
            if (prenomA != prenomB) return prenomA < prenomB;
            return cinA < cinB;
        }
        return false;
    });

    // Reinsert sorted items into the list
    for (QListWidgetItem *item : items) {
        ui->liste_att->addItem(item);
    }
}


