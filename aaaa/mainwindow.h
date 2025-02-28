#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include "vaccin.h"

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
    void onvaccinBclicked();
    void onajouteventclicked();
    void oneventBclicked();
    void on_equiB_clicked();
    void on_ajoutequi_clicked();
    void on_rdv_clicked();
    void on_Quit_clicked();
    void on_Quit_4_clicked();
    void on_ajoutrdv_clicked();
    void on_save_rdv_clicked();
    void on_save_vac_clicked();
    void on_save_event_clicked();
    void on_save_eq_clicked();
    void onDateChanged();
    void on_delete_2_clicked();
    void on_edit_vac_clicked(); // Slot for the edit button
    void on_save_vac_2_clicked(); // Slot for saving edited data

private:
    Ui::MainWindow *ui;
    Vaccin *vaccinManager;
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
    QPushButton *delete_vac;
    QPushButton *edit_vac; // Declare the edit button
    QPushButton *save_vac_2; // Declare the save button for edited data

    void loadEquipementsData();
    void loadEventData();
    void loadAppointments();
};

#endif // MAINWINDOW_H
