#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include "vaccin.h"
#include "evenement.h"
#include "medecin.h"
#include <QProcess>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &userId, QWidget *parent = nullptr);
     ~MainWindow();
    void showStatistics();  // No bool checked argument


private slots:
    void onajoutvacclicked();
    void onvaccinBclicked();
    void onajouteventclicked();
    void oneventBclicked();
    void supprimerEvenement();
    void modifierEvenement();
    void on_equiB_clicked();
    void on_ajoutequi_clicked();
    void on_rdv_clicked();
    void on_Quit_clicked();
    void on_Quit_4_clicked();
    void on_Quit_3_clicked();
    void on_ajoutrdv_clicked();
    void on_save_rdv_clicked();
    void on_save_vac_clicked();
    void on_save_event_clicked();
    void on_save_eq_clicked();
    void onDateChanged(); // Slot for handling date changes


    //MEDECIN

    void on_pushButton_Med_clicked();
    void on_pushButton_AjoutMed_clicked();
    void on_pushButton_Inf_clicked();
    void on_actionGestion_d_acc_s_triggered();
    void on_save_AjMed_clicked();
    void on_btnSupprimer_Med_clicked();
    void on_btnModifier_Med_clicked();
    void on_save_EditMed_clicked();
    void on_searchMed_textChanged(const QString &text);
    void onPdfButtonClicked();

    void on_pushButton_SortMed_clicked();




    void on_actionshowStatsButton_triggered();

    void on_actionChanger_d_utilisateur_triggered();


    void on_pushButton_Talk_clicked();

    void onProcessOutput();


private:
    Ui::MainWindow *ui;
    Vaccin *vaccinManager;
    Evenement *eventManager;
    Evenement *evenement;
    QTabWidget *vaccinTab;
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
    void loadEquipementsData();
    void loadAppointments();

    QAction *GesAcc;

    QPushButton *listMed;
    QPushButton *listInf;

    QString selectedEmployeeId;
    medecin *Emp;

    void createExperienceGraph(const QMap<QString, int>& experienceGroups);  // Function to create the graph

    QProcess* process;


};

#endif // MAINWINDOW_H
