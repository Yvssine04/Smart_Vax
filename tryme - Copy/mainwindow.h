#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>

#include "vaccin.h"
#include "evenement.h"
#include "equipements.h"
#include "rendez_vous.h"
#include <QSoundEffect>
#include <QTimer>
#include <QtMultimedia>
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
    void on_Quit_3_clicked();
    void on_Quit_13_clicked();
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
    void on_edit_vac_2_clicked();
    void on_save_2_clicked();
    void on_supprimerevent_clicked();
    void on_modifierevent_clicked();
    void on_sauvegarderevent_clicked();
    void onVaccinTableSelectionChanged();
    void onComboBoxIndexChanged(int index);
    void comboIndex_rdv (int index);
    void onRechercheEqReturnPressed();
    void on_edit_equi_clicked();
    void on_modif_save_clicked();
    void on_supprime_eq_clicked();
    void onEquipementComboBoxIndexChanged(int index);
    void sortEquipementTable(QTableWidget *tableEqui, int column);
    void on_deleteRdv_clicked();
    void on_edit_rdv_clicked();
    void on_save_rdv_2_clicked();
    void on_rdv_pdf_clicked();
    void rdv_recherche(const QString &text);
    void on_calendarWidget_clicked(const QDate &date);
    void showNotifications();
    void ajoutNotification(const QString message);
    void handleNotificationClicked(const QString &message);
    void autoFillPatientInfo(const QString &cin);






    void on_actiontaux_des_rendez_vous_triggered();

private:
    Ui::MainWindow *ui;
    Vaccin *vaccinManager;
    rendez_vous *rdvWindow;
    QTabWidget *vaccinTab;
    Evenement *eventManager;
    Evenement *evenement;
    QMediaPlayer *M_Player;
    QAudioOutput *audioOutput;
    QCalendarWidget *calendrier;

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
    QPushButton *edit_vac;
    QPushButton *save_vac_2;
    QPushButton *edit_event;
    QPushButton *save_event;
    QPushButton *supprimerevent;
    QLineEdit *cherche_vac;
    QPushButton *tri_vac;
    int selectedRow = -1;
    bool checkDatabaseConnection();
    void loadEquipementsData();
    void loadEventData();
    void loadAppointments();
    NotificationWidget *notificationWidget;
    int currentIdRdv;



};




#endif
