#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QGraphicsOpacityEffect>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vaccin = ui->vaccin;
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

    isModifying = false;
    modifyingCinPass = -1;

    vaccin->setStyleSheet(
        "QTabWidget::pane { border: none; background: transparent; }"
        "QTabBar::tab { height: 0; width: 0; }"
        "QTabBar { height: 0; }"
        );

    // Connexions existantes
    connect(ajoutvac, &QPushButton::clicked, this, &MainWindow::onajoutvacclicked);
    connect(vaccinB, &QPushButton::clicked, this, &MainWindow::onvaccinBclicked);
    connect(ajoutevent, &QPushButton::clicked, this, &MainWindow::onajouteventclicked);
    connect(eventB, &QPushButton::clicked, this, &MainWindow::oneventBclicked);
    connect(ajoutequi, &QPushButton::clicked, this, &MainWindow::on_ajoutequi_clicked);
    connect(equiB, &QPushButton::clicked, this, &MainWindow::on_equiB_clicked);
    connect(rdv, &QPushButton::clicked, this, &MainWindow::on_rdv_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);
    connect(ui->dossierpatient, &QPushButton::clicked, this, &MainWindow::on_dossierpatient_clicked);
    connect(Quit, &QPushButton::clicked, this, &MainWindow::on_Quit_clicked);
    connect(Quit4, &QPushButton::clicked, this, &MainWindow::on_Quit_4_clicked);
    connect(ajoutrdv, &QPushButton::clicked, this, &MainWindow::on_ajoutrdv_clicked);
    connect(ui->ajoutequi_2, &QPushButton::clicked, this, &MainWindow::on_ajoutequi_2_clicked);
    connect(ui->pushButton_192, &QPushButton::clicked, this, &MainWindow::on_pushButton_192_clicked);
    connect(ui->pushButton_189, &QPushButton::clicked, this, &MainWindow::on_pushButton_189_clicked);

    // Nouvelles connexions
    connect(ui->label_97, &QLabel::linkActivated, this, &MainWindow::on_label_97_clicked); // Si c'est un QLabel cliquable
    connect(ui->lineEdit_88, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_88_textChanged);
    connect(ui->pushButton_stat, &QPushButton::clicked, this, &MainWindow::on_pushButton_stat_clicked);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_clicked);

    // Initialisation de l'interface (comme dans ton code original)
    QLabel *main = ui->main;
    QPixmap pixmap(":/logo.png");
    main->setPixmap(pixmap);
    main->setAlignment(Qt::AlignCenter);

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect();
    opacityEffect->setOpacity(0.5);
    main->setGraphicsEffect(opacityEffect);

    QTableWidget *tabPatient = ui->tabPatient;
    tabPatient->setColumnCount(11);
    tabPatient->setHorizontalHeaderLabels({
        "CIN/Passeport", "Num. Dossier", "Nom", "Prénom", "Date Naissance",
        "Téléphone", "E-mail", "Code Postal", "Sexe", "Adresse", "CNAM"
    });
    tabPatient->setStyleSheet(
        "QTableWidget { background-color: rgb(63, 123, 134); border: 2px solid #000000; border-radius: 15px; gridline-color: #d3d3d3; font-size: 14px; padding: 5px; }"
        "QHeaderView { background-color: rgb(63, 123, 134); }"
        "QHeaderView::section { background-color: #b0c4de; border: 1px solid #d3d3d3; color: black; font-size: 16px; padding: 6px; }"
        "QTableWidget::item { padding: 6px; }"
        );
    tabPatient->resizeRowsToContents();

    loadPatientData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Fonctions existantes (je ne les réécris pas toutes pour brevité, garde ton code original ici)
// ...

void MainWindow::loadPatientData()
{
    QSqlQuery query("SELECT * FROM PATIENT");
    ui->tabPatient->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tabPatient->insertRow(row);
        ui->tabPatient->setItem(row, 0, new QTableWidgetItem(query.value("CIN_PASSP").toString()));
        ui->tabPatient->setItem(row, 1, new QTableWidgetItem(query.value("NUM_DOSSIER").toString()));
        ui->tabPatient->setItem(row, 2, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tabPatient->setItem(row, 3, new QTableWidgetItem(query.value("PRÉNOM").toString()));
        ui->tabPatient->setItem(row, 4, new QTableWidgetItem(query.value("DATE_NAISS").toDate().toString("yyyy-MM-dd")));
        ui->tabPatient->setItem(row, 5, new QTableWidgetItem(query.value("TEL").toString()));
        ui->tabPatient->setItem(row, 6, new QTableWidgetItem(query.value("MAIL").toString()));
        ui->tabPatient->setItem(row, 7, new QTableWidgetItem(query.value("CODE_P").toString()));
        ui->tabPatient->setItem(row, 8, new QTableWidgetItem(query.value("SEXE").toString()));
        ui->tabPatient->setItem(row, 9, new QTableWidgetItem(query.value("ADRESSE").toString()));
        ui->tabPatient->setItem(row, 10, new QTableWidgetItem(query.value("CNAM").toString()));
        row++;
    }
}

// 1. Tri par date de naissance
void MainWindow::on_label_97_clicked()
{
    QSqlQuery query("SELECT * FROM PATIENT ORDER BY DATE_NAISS ASC");
    ui->tabPatient->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tabPatient->insertRow(row);
        ui->tabPatient->setItem(row, 0, new QTableWidgetItem(query.value("CIN_PASSP").toString()));
        ui->tabPatient->setItem(row, 1, new QTableWidgetItem(query.value("NUM_DOSSIER").toString()));
        ui->tabPatient->setItem(row, 2, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tabPatient->setItem(row, 3, new QTableWidgetItem(query.value("PRÉNOM").toString()));
        ui->tabPatient->setItem(row, 4, new QTableWidgetItem(query.value("DATE_NAISS").toDate().toString("yyyy-MM-dd")));
        ui->tabPatient->setItem(row, 5, new QTableWidgetItem(query.value("TEL").toString()));
        ui->tabPatient->setItem(row, 6, new QTableWidgetItem(query.value("MAIL").toString()));
        ui->tabPatient->setItem(row, 7, new QTableWidgetItem(query.value("CODE_P").toString()));
        ui->tabPatient->setItem(row, 8, new QTableWidgetItem(query.value("SEXE").toString()));
        ui->tabPatient->setItem(row, 9, new QTableWidgetItem(query.value("ADRESSE").toString()));
        ui->tabPatient->setItem(row, 10, new QTableWidgetItem(query.value("CNAM").toString()));
        row++;
    }
    QMessageBox::information(this, "Tri", "Patients triés par date de naissance.");
}

// 2. Recherche par nom
void MainWindow::on_lineEdit_88_textChanged(const QString &text)
{
    QString searchText = text.trimmed();
    if (searchText.isEmpty()) {
        loadPatientData(); // Réafficher tous les patients si le champ est vide
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM PATIENT WHERE NOM LIKE :nom");
    query.bindValue(":nom", "%" + searchText + "%");
    query.exec();

    ui->tabPatient->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tabPatient->insertRow(row);
        ui->tabPatient->setItem(row, 0, new QTableWidgetItem(query.value("CIN_PASSP").toString()));
        ui->tabPatient->setItem(row, 1, new QTableWidgetItem(query.value("NUM_DOSSIER").toString()));
        ui->tabPatient->setItem(row, 2, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tabPatient->setItem(row, 3, new QTableWidgetItem(query.value("PRÉNOM").toString()));
        ui->tabPatient->setItem(row, 4, new QTableWidgetItem(query.value("DATE_NAISS").toDate().toString("yyyy-MM-dd")));
        ui->tabPatient->setItem(row, 5, new QTableWidgetItem(query.value("TEL").toString()));
        ui->tabPatient->setItem(row, 6, new QTableWidgetItem(query.value("MAIL").toString()));
        ui->tabPatient->setItem(row, 7, new QTableWidgetItem(query.value("CODE_P").toString()));
        ui->tabPatient->setItem(row, 8, new QTableWidgetItem(query.value("SEXE").toString()));
        ui->tabPatient->setItem(row, 9, new QTableWidgetItem(query.value("ADRESSE").toString()));
        ui->tabPatient->setItem(row, 10, new QTableWidgetItem(query.value("CNAM").toString()));
        row++;
    }
}

// 3. Nombre total de patients inscrits
void MainWindow::on_pushButton_stat_clicked()
{
    QSqlQuery query("SELECT COUNT(*) FROM PATIENT");
    if (query.next()) {
        int total = query.value(0).toInt();
        QMessageBox::information(this, "Statistiques", QString("Nombre total de patients inscrits : %1").arg(total));
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les statistiques : " + query.lastError().text());
    }
}

// 4. Générer un PDF pour un patient sélectionné
void MainWindow::on_pushButton_pdf_clicked()
{
    int row = ui->tabPatient->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un patient dans le tableau.");
        return;
    }

    // Récupérer les données du patient sélectionné
    QString cinPass = ui->tabPatient->item(row, 0)->text();
    QString numDossier = ui->tabPatient->item(row, 1)->text();
    QString nom = ui->tabPatient->item(row, 2)->text();
    QString prenom = ui->tabPatient->item(row, 3)->text();
    QString dateNaiss = ui->tabPatient->item(row, 4)->text();
    QString tel = ui->tabPatient->item(row, 5)->text();
    QString mail = ui->tabPatient->item(row, 6)->text();
    QString codePostal = ui->tabPatient->item(row, 7)->text();
    QString sexe = ui->tabPatient->item(row, 8)->text();
    QString adresse = ui->tabPatient->item(row, 9)->text();
    QString cnam = ui->tabPatient->item(row, 10)->text();

    // Demander à l'utilisateur où sauvegarder le PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Sauvegarder le rapport", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    // Créer le PDF
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    QPainter painter(&pdfWriter);

    painter.setFont(QFont("Arial", 12));
    int y = 100;
    painter.drawText(100, y, "Rapport Patient - Centre de Vaccination");
    y += 50;
    painter.drawText(100, y, QString("CIN/Passeport : %1").arg(cinPass));
    y += 30;
    painter.drawText(100, y, QString("Numéro Dossier : %1").arg(numDossier));
    y += 30;
    painter.drawText(100, y, QString("Nom : %1").arg(nom));
    y += 30;
    painter.drawText(100, y, QString("Prénom : %1").arg(prenom));
    y += 30;
    painter.drawText(100, y, QString("Date de Naissance : %1").arg(dateNaiss));
    y += 30;
    painter.drawText(100, y, QString("Téléphone : %1").arg(tel));
    y += 30;
    painter.drawText(100, y, QString("E-mail : %1").arg(mail));
    y += 30;
    painter.drawText(100, y, QString("Code Postal : %1").arg(codePostal));
    y += 30;
    painter.drawText(100, y, QString("Sexe : %1").arg(sexe));
    y += 30;
    painter.drawText(100, y, QString("Adresse : %1").arg(adresse));
    y += 30;
    painter.drawText(100, y, QString("CNAM : %1").arg(cnam));
    y += 50;
    painter.drawText(100, y, QString("Généré le : %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss")));

    painter.end();
    QMessageBox::information(this, "Succès", "Rapport PDF généré avec succès !");
}

// Garde tes autres fonctions existantes ici (onajoutvacclicked, etc.)
// ...
