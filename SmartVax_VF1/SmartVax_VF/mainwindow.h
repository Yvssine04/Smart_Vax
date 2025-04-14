#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QtCharts>
#include <QSqlQueryModel>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QAudioDevice>
#include <QMediaFormat>
#include "vaccin.h"
#include "evenement.h"
#include "equipements.h"
#include <windows.h>
#include <QtGlobal> // Include this to use Qt type definitions
#include <opencv2/opencv.hpp>
// Add these includes
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <QProcess>
#include <QStringList>

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /////////////////////////////////////////////////////////
   void sortEquipementTable(QTableWidget *tableEqui, int column);
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
   // void onRechercheEqReturnPressed();

    void on_edit_equi_clicked(); // Corrected return type to void
    void on_modif_save_clicked(); // Corrected return type to void

    void on_supprime_eq_clicked();

    void onEquipementComboBoxIndexChanged(int index);
    //void onVocaleClicked();  // Slot for button click
   // void processSpeechResult();

///////////////////////////////////////////////
        void onPdfButtonClicked();
 void onStatistiqueActionTriggered();
           void createEquipmentChart(const QMap<QString, int>& stats);


           //void on_vocale_clicked();

          // void processAudioFile();
    void on_cameraButton_clicked();
              void on_camButton_clicked();

    void onRechercheEqReturnPressed();
private:
    Ui::MainWindow *ui;
    Vaccin *vaccinManager;
    QTabWidget *vaccinTab;
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
    QPushButton *edit_vac; // Declare the edit button
    QPushButton *save_vac_2; // Declare the save button for edited data
    QPushButton *edit_event; // Declare the edit button
    QPushButton *save_event;
    QPushButton *supprimerevent;
    QLineEdit *cherche_vac;
    QPushButton *tri_vac;
    int selectedRow = -1;

// les 4 pour audio
   // QMediaRecorder *m_recorder;
   // QMediaCaptureSession *m_captureSession;
   // QAudioInput *m_audioInput;
    //QString m_lastAudioFile;
    ////




    bool checkDatabaseConnection();

        // QPushButton *ffff;

    void loadEquipementsData();
    void loadEventData();
    void loadAppointments();


   // QMediaRecorder *audioRecorder;



    cv::VideoCapture cap;
    cv::CascadeClassifier medicalClassifier;
    QTimer* cameraTimer;
    void processFrame();
    void startSpeechRecognition();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
   QProcess *speechProcess;
    void onProcessOutput();
   // QString recognizeSpeech(const QString &filePath);
 QProcess *process;
  //  QString reconnaitreParole();

 void checkEquipmentQuantity();
 QTimer* alertTimer;
 QMediaPlayer* alertPlayer;
 QAudioOutput* audioOutput;
// QSet<QString> alertedEquipment;
 bool isFirstCheck = true;
    bool alertShown = false;
};

#endif // MAINWINDOW_H
