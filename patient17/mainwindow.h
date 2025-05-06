#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qpushbutton.h"
#include <QMainWindow>
#include <QLabel>
#include <QTabWidget>
#include <QLineEdit>
#include <QPdfWriter> // Pour générer des PDF
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onajoutvacclicked();
    void on_pushButton_4_clicked();
    void onvaccinBclicked();
    void onajouteventclicked();
    void oneventBclicked();
    void on_modifier1_clicked();
    void on_equiB_clicked();
    void on_ajoutequi_clicked();
    void on_pushButton_5_clicked();
    void on_rdv_clicked();
    void on_Quit_clicked();
    void on_Quit_4_clicked();
    void on_ajoutrdv_clicked();
    void on_ajoutequi_10_clicked();
    void on_dossierpatient_clicked();
    void on_pushButton_151_clicked();
    void loadPatientData();

    // Slots existants pour ajout/suppression/modification
    void on_ajoutequi_2_clicked();
    void on_pushButton_192_clicked();
    void on_pushButton_189_clicked();

    // Nouveaux slots pour les fonctionnalités demandées
    void on_label_97_clicked();         // 1. Tri par date de naissance
    void on_lineEdit_88_textChanged(const QString &text); // 2. Recherche par nom
    void on_pushButton_stat_clicked();  // 3. Nombre total de patients
    void on_pushButton_pdf_clicked();   // 4. Générer un PDF

private:
    Ui::MainWindow *ui;
    QTabWidget *vaccin;
    QPushButton *ajoutvac;
    QPushButton *vaccinB;
    QPushButton *ajoutevent;
    QPushButton *eventB;
    QPushButton *rdv;
    QPushButton *ajoutequi;
    QPushButton *equiB;
    QPushButton *Quit;
    QPushButton *Quit4;
    QPushButton *ajoutrdv;

    bool isModifying;
    int modifyingCinPass;
};

#endif // MAINWINDOW_H
