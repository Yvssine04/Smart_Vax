#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtCharts>
#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QString>
#include <QDebug>

#include "vaccin.h"
#include "evenement.h"
#include "equipements.h"
#include "rendez_vous.h"

//////notif
#include <QSoundEffect>
#include <QTimer>
#include <QtMultimedia>

#include <QComboBox>

#include <QSerialPort>
#include "arduino.h"
#include <QProcess>
#include <QStringList>
///////mariem
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioInput>
#include <QtGlobal>
//elloumi
//#include <opencv2/core.hpp>
//#include <opencv2/objdetect.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <QAudioDevice>
//#include <opencv2/opencv.hpp>
/////////////////////////camera_Elloumi ////////////
///
///



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int attente=0;


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

///// save
    void on_save_rdv_clicked();
    void on_save_vac_clicked();
    void on_save_event_clicked();
    void on_save_eq_clicked();


//////// dahen
    void showNotificationHistory();
    void clearNotificationHistory();


    void handleNotificationClicked(const QString &message);
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
    void onVaccinHeaderSectionClicked(int logicalIndex);
    void onPdfVaccinButtonClicked();
    void showAllRows();
    void on_Quit_5_clicked();

    void on_geventg_clicked();

    void on_Quit_7_clicked();

    void on_showmap_clicked();

    void on_showstat_clicked();

    void on_invimail_clicked();
    QString generateEmailBody(const QString& prompt);
    void on_pushButton_4_clicked();
    void on_hearte_rate_clicked();
     void on_hearte_rate2_clicked();
    void readHeartRate();
    void updateCountdown();

    void on_calendarWidget_clicked(const QDate &date);
    void showNotifications();
    void ajoutNotification(const QString message);
    void autoFillPatientInfo(const QString &cin);
    void on_actionEmploy_s_triggered();
    void on_actionQuitter_triggered();
    void on_drag_clicked();
    void tri(int index);
    void check_att();



////////////////////mariem
 void onStatistiqueActionTriggered();
 void createEquipmentChart(const QMap<QString, int>& stats);
 //elloumi_CAm
// void on_camButton_clicked();
     void onPdfButtonClicked();
 void on_actionStats_Fr_quentation_triggered();

 public slots:
    void updateEventList(QString ville);

private:
    Ui::MainWindow *ui;
    Vaccin *vaccinManager;
    rendez_vous *rdvWindow;
    QTabWidget *vaccinTab;
    QChartView *chartView = nullptr;
    QQuickWidget *quickWidget;
    QListWidget *listevent;
    Evenement *eventManager;
    Evenement *evenement;
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
    QPushButton *pdfvaccin;
    int selectedRow = -1;
    bool checkDatabaseConnection();
    void loadEquipementsData();
    void loadEventData();
    void loadAppointments();
    void loadPatientData(QTableWidget *tabpat);
    NotificationWidget *notificationWidget;
    int currentIdRdv;

    QMap<QDate, QStringList> appointmentTooltips;
    QMap<QDate, QList<QDateTime>> appointmentTimes;
    QList<QDateTime> dateProp;

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
    QCheckBox *attenteCheck;


    QSerialPort *serial;
    int heartRateColumn = 3;

    Arduino *arduino;
    QTimer *timer;
    QList<int> heartRates;
    int identifiant;
    int totalHeartRate;   // Pour lire la fréquence cardiaque
    QTimer *countdownTimer; // Pour afficher le décompte
    int countdown;         // Compteur pour les secondes restantes
    void readAverageHeartRate();


    QTimer *averageTimer; //


    QAudioOutput* audioOutput;
    /// mariem
    QTimer* alertTimer;

     QMediaPlayer* alertPlayer;
     void checkEquipmentQuantity();
    bool isFirstCheck = true;
    bool alertShown = false;
    QProcess* process;
    bool g_alertShown = false;

    void processFrame();
    void startSpeechRecognition();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    QProcess *speechProcess;
    void onProcessOutput();
    // QString recognizeSpeech(const QString &filePath);
//elloumi_cam
    //cv::VideoCapture cap;
    //cv::CascadeClassifier medicalClassifier;

   // QTimer* cameraTimer;


};



class AppointmentDialog : public QDialog {
    Q_OBJECT
public:
    AppointmentDialog(const QList<QDateTime>& options, QWidget* parent = nullptr)
        : QDialog(parent) {
        setWindowTitle("Choisir une date de rendez-vous");
        resize(400, 250);

        QVBoxLayout* layout = new QVBoxLayout(this);

        QLabel* label = new QLabel("Veuillez choisir l'une des dates proposées :", this);
        layout->addWidget(label);

        listWidget = new QListWidget(this);
        for (const QDateTime& dt : options) {
            listWidget->addItem(dt.toString("dddd dd MMMM yyyy - hh:mm"));
        }
        layout->addWidget(listWidget);

        QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        connect(buttons, &QDialogButtonBox::accepted, this, &AppointmentDialog::accept);
        connect(buttons, &QDialogButtonBox::rejected, this, &AppointmentDialog::reject);
        layout->addWidget(buttons);

        setStyleSheet(
            "QDialog {"
            "    background-color: #ffffff;"
            "}"
            "QLabel {"
            "    font-size: 20px;"
            "    color: rgb(63, 123, 134);"
            "}"
            "QListWidget {"
            "   background-color: rgb(121, 167, 173);"
            "   color: rgb(255, 255, 255);"
            "    font-size: 15px;"
            "}"
            "QListWidget::item {"
            "    padding: 5px;"
            "    font-size: 20px"
            "}"
            "QListWidget::item:selected {"
            "    background-color: #007acc;"
            "    color: #fff;"
            "}"
            "QPushButton {"
            "    background-color: rgb(63, 123, 134);"
            "    color: rgb(0, 0, 0);"

            "    border: none;"
            "    padding: 10px 20px;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "    background-color: rgb(63, 123, 134);"
            "}"
            );
    }

    int selectedIndex() const {
        return listWidget->currentRow();
    }

    QDateTime selectedDateTime(const QList<QDateTime>& options) const {
        int index = selectedIndex();
        if (index >= 0 && index < options.size()) {
            return options[index];
        }
        return QDateTime();
    }

private:
    QListWidget* listWidget;
};



#endif
