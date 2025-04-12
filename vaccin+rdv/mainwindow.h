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
#include <QComboBox>
#include <QMap>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include "NewsFetcher.h"  // Include the NewsFetcher header
#include "chatbot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString selectedDays;
    QString periode;
    QDate Date;
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

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
    ////////////////////////////////////////////////////////////////////////////////
    void onVaccinHeaderSectionClicked(int logicalIndex);
    void onPdfVaccinButtonClicked();
    void showAllRows();
    void onTypeVacCurrentIndexChanged();
    void onTypeVac2CurrentIndexChanged();
    void onVaccineNameChanged();
    void onVaccineName2Changed();
    void setVaccineModeBasedOnName(const QString &vaccineName, QComboBox *modeCombo);
    void onActionVaccinTriggered();

    void handleNewSicknessDetected(const QString &sicknessName,
                                   const QString &date,
                                   const QString &location,
                                   const QString &vaccineInfo,
                                   const QString &detail,
                                   const QString &source);




    void onCheckboxChanged();
    void onCheckboxChangedP();
    void on_actiontaux_des_rendez_vous_triggered();
    void on_chatbot_page_clicked();
    void sendMessageToChatbot();
    void handleChatbotLine2ReturnPressed();
    void on_Quit_5_clicked();
    void loadChatHistory(const QString &history);
    void showChatHistory();
    void createNewChat();
    void showNotificationHistory();
    void clearNotificationHistory();
    void on_drag_clicked();
    void tri(int index);



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
    NewsFetcher *newsFetcher;  // Add a NewsFetcher member
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
    QPushButton *tri_vac;
    int selectedRow = -1;
    QLineEdit *cherche_vac;
    QPushButton *pdfvaccin;
    ChatBot *chatbot;
    void saveNotificationToFile(int numero,
                                const QString &detail,
                                const QString &status,
                                const QDate &date,
                                const QString &source,
                                const QString &maladie,
                                const QString &lieu);
    void setVaccineTypeBasedOnName(const QString &vaccineName, QComboBox *typeCombo);
    bool checkDatabaseConnection();
    void loadEquipementsData();
    void loadEventData();
    void loadAppointments();
    NotificationWidget *notificationWidget;
    int currentIdRdv;
    QMap<QDate, QStringList> appointmentTooltips;
    QMap<QDate, QList<QDateTime>> appointmentTimes;

    QMap<QString, QString> holidayTranslation = {
        {"New Year", "Jour de l'An"},
        {"Ramadan Start", "Début du Ramadan"},
        {"Independence Day", "Fête de l'Indépendance"},
        {"March Equinox", "Équinoxe de mars"},
        {"Eid al-Fitr", "Aïd el-Fitr"},
        {"Eid al-Fitr Holiday", "Jour férié de l'Aïd el-Fitr"},
        {"Martyrs' Day", "Journée des Martyrs"},
        {"Labour Day", "Fête du Travail"},
        {"Eid al-Adha", "Aïd el-Adha"},
        {"Eid al-Adha Holiday", "Jour férié de l'Aïd el-Adha"},
        {"June Solstice", "Solstice de juin"},
        {"Muharram", "Mouharram"},
        {"Republic Day", "Fête de la République"},
        {"Women’s Day", "Journée de la Femme"},
        {"The Prophet's Birthday", "Mawlid"},
        {"September Equinox", "Équinoxe de septembre"},
        {"Evacuation Day", "Journée de l'Évacuation"},
        {"Revolution and Youth Day", "Fête de la Révolution et de la Jeunesse"},
        {"December Solstice", "Solstice de décembre"}
    };
    QCheckBox *lundiCheckBox;
    QCheckBox *mardiCheckBox;
    QCheckBox *mercrediCheckBox;
    QCheckBox *jeudiCheckBox;
    QCheckBox *vendrediCheckBox;
    QCheckBox *samediCheckBox;
    void handleNotificationClicked1();





};




#endif
