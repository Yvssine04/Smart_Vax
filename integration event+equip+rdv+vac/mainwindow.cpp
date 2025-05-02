#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QGraphicsOpacityEffect>
#include "connection.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
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
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSqlTableModel>
/////mariem
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QMediaFormat>
#include <windows.h>
#include <sapi.h>
#include <QMediaDevices>
  // Core header for SAPI
#include <sphelper.h>  // Useful helper functions (optional but common)
#include "brasrobotique.h"

static bool g_alertShown = false;

    Connection::Connection() {}
//elloumi
//QProcess *yoloProcess = nullptr;
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
    , brasrobotique (new BrasRobotique(this))
    , chatbot(new ChatBot(this))
    , notificationWidget (new NotificationWidget(this))
    //elloumi
   // ,cameraTimer(new QTimer(this))
    ,timer(new QTimer(this)),
    totalHeartRate(0)
{

    ui->setupUi(this);
    this->showFullScreen();
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
    ui->nom_vac->addItem("Pfizer-BioNTech");
    ui->nom_vac->addItem("Moderna");
    ui->nom_vac->addItem("AstraZeneca");
    ui->nom_vac->addItem("Johnson & Johnson");
    ui->nom_vac->addItem("Sinopharm");
    ui->nom_vac->addItem("Sputnik V");
    ui->nom_vac->addItem("Novavax");
    ui->nom_vac->addItem("Sinovac");
    ui->nom_vac->addItem("Comirnaty");
    ui->nom_vac->addItem("Spikevax");
    ui->nom_vac->addItem("nouveau vaccin");
    ui->nom_vac->setEditable(true);

    // Same for nom_vac_2
    ui->nom_vac_2->addItem("Pfizer-BioNTech");
    ui->nom_vac_2->addItem("Moderna");
    ui->nom_vac_2->addItem("AstraZeneca");
    ui->nom_vac_2->addItem("Johnson & Johnson");
    ui->nom_vac_2->addItem("Sinopharm");
    ui->nom_vac_2->addItem("Sputnik V");
    ui->nom_vac_2->addItem("Novavax");
    ui->nom_vac_2->addItem("Sinovac");
    ui->nom_vac_2->addItem("Comirnaty");
    ui->nom_vac_2->addItem("Spikevax");
    ui->nom_vac_2->addItem("nouveau vaccin");
    ui->nom_vac_2->setEditable(true);

    ui->choix_vac->addItem("Prix");
    ui->choix_vac->addItem("Date");
    ui->choix_vac->addItem("Quantite");
    ui->tabequi->setSortingEnabled(true);
    ui->tabevent->setSortingEnabled(true);
    ui->tabvaccin->setSortingEnabled(true);
    ui->mode_vac->addItem("injection intradermique");
    ui->mode_vac->addItem("injection sous-cutanée");
    ui->mode_vac->addItem("injection intramusculaire");

    ui->type_vac->addItem("ARN");
    ui->type_vac->addItem("Vaccin à ARN messager");
    ui->type_vac->addItem("Vaccin à vecteur viral");
    ui->type_vac->addItem("Vaccin inactivé");
    ui->type_vac->addItem("Vaccin à sous-unité protéique");
    ui->type_vac->addItem("nouveau type");
    ui->type_vac->setEditable(true);

    ui->type_vac_2->addItem("ARN");
    ui->type_vac_2->addItem("Vaccin à ARN messager");
    ui->type_vac_2->addItem("Vaccin à vecteur viral");
    ui->type_vac_2->addItem("Vaccin inactivé");
    ui->type_vac_2->addItem("Vaccin à sous-unité protéique");
    ui->type_vac_2->addItem("nouveau type");
    ui->type_vac_2->setEditable(true);
    ui->mode_vac_2->addItem("injection intradermique");
    ui->mode_vac_2->addItem("injection sous-cutanée");
    ui->mode_vac_2->addItem("injection intramusculaire");






    ui->lineEdit_85->setValidator(new QIntValidator(1, 999999, this)); // Quantity must be ≥1

    // For price line edit (assuming it's named lineEdit_84)
    ui->lineEdit_84->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this)); // Price ≥0 with 2 decimals
    ui->tabequi->setSortingEnabled(true);



    // For quantity line edit (assuming it's named lineEdit_85)
    ui->lineEdit_1116->setValidator(new QIntValidator(1, 999999, this)); // Quantity must be ≥1

    // For price line edit (assuming it's named lineEdit_84)
    ui->lineEdit_1117->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));
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

    ui->boxlieu->addItems(QStringList()
                          << "Tunis" << "Sfax" << "Sousse" << "Ariana" << "Ben Arous"
                          << "Bizerte" << "Gabes" << "Gafsa" << "Jendouba" << "Kairouan"
                          << "Kasserine" << "Kébili" << "Le Kef" << "Mahdia" << "Manouba"
                          << "Medenine" << "Monastir" << "Nabeul" << "Sidi Bouzid"
                          << "Siliana" << "Tataouine" << "Tozeur" << "Zaghouan"
                          << "Béja" << "Djerba");

    connect(ui->boxlieu, &QComboBox::currentTextChanged, this, [=](const QString &selectedLieu){
        ui->lineEdit_lieu->setText(selectedLieu);
    });

    ui->boxlieu_2->addItems(QStringList()
                            << "Tunis" << "Sfax" << "Sousse" << "Ariana" << "Ben Arous"
                            << "Bizerte" << "Gabes" << "Gafsa" << "Jendouba" << "Kairouan"
                            << "Kasserine" << "Kébili" << "Le Kef" << "Mahdia" << "Manouba"
                            << "Medenine" << "Monastir" << "Nabeul" << "Sidi Bouzid"
                            << "Siliana" << "Tataouine" << "Tozeur" << "Zaghouan"
                            << "Béja" << "Djerba");

    connect(ui->boxlieu_2, &QComboBox::currentTextChanged, this, [=](const QString &selectedLieu){
        ui->lineEdit_mlieu->setText(selectedLieu);
    });
    averageTimer = new QTimer(this);
    connect(averageTimer, &QTimer::timeout, this, &MainWindow::readAverageHeartRate);
    averageTimer->start(1000); // check every 1 second

    vaccinTab->setCurrentIndex(7);

    connect(ajoutevent, &QPushButton::clicked, this, &MainWindow::onajouteventclicked);
    connect(eventB, &QPushButton::clicked, this, &MainWindow::oneventBclicked);
//mariem
connect(ui->vocale, &QPushButton::clicked, this, &MainWindow::startSpeechRecognition);
connect(ui->actionStatistics, &QAction::triggered, this, &MainWindow::onStatistiqueActionTriggered);
connect(ui->pdfButton, &QPushButton::clicked, this, &MainWindow::onPdfButtonClicked);
process = new QProcess(this);
connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessOutput);

connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &MainWindow::onProcessFinished);
//elloumi
//connect(cameraTimer, &QTimer::timeout, this, &MainWindow::processFrame);
//connect(ui->camButton, &QPushButton::clicked, this, &MainWindow::on_camButton_clicked);
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
////////////


connect(alertPlayer, &QMediaPlayer::playbackStateChanged, this, [](QMediaPlayer::PlaybackState state) {
    qDebug() << "Playback state changed:" << state;
});
//alertPlayer->play();

// In MainWindow constructor (after initializing alertPlayer)
connect(alertPlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
    qDebug() << "Media Player Error:" << errorString;
});



////////////


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




    //////////////////////// aafset azza
    QString currentYear = QString::number(QDate::currentDate().year());
    rdvWindow->fetchTunisianHolidays( QString::number(QDate::currentDate().year()))  ;

    ////////////////////Calendrier
    QMap<QDate, QStringList> appointmentTooltips;
    ui->calendrier_rdv->findChild<QTableView *>("qt_calendar_calendarview")->installEventFilter(this);

    /////////////////////////connect
    connect(rdv, &QPushButton::clicked, this, &MainWindow::on_rdv_clicked);
    connect(Quit, &QPushButton::clicked, this, &MainWindow::on_Quit_clicked);
    connect(Quit4, &QPushButton::clicked, this, &MainWindow::on_Quit_4_clicked);
    connect(ajoutrdv, &QPushButton::clicked, this, &MainWindow::on_ajoutrdv_clicked);
    connect(delete_vac, &QPushButton::clicked, this, &MainWindow::on_delete_2_clicked);
    connect(ui->rdv_rech, &QLineEdit::textChanged, this, &MainWindow::rdv_recherche);
    connect(ui->tri_rdv, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::tri);
    connect(ui->calendrier_rdv, &QCalendarWidget::clicked, this, &MainWindow::on_calendarWidget_clicked);
    connect(ui->CIN_rdv, &QLineEdit::textChanged, this, &MainWindow::autoFillPatientInfo);
    connect(ui->attente, &QCheckBox::stateChanged, this, &MainWindow::check_att);


    ////////////////////////////////notification

    QSoundEffect notif;

    QSoundEffect *soundEffect = new QSoundEffect(this);
    soundEffect->setSource(QUrl::fromLocalFile("C:/Users/MSI/Downloads/ding-101492.wav"));

    ui->bellN->hide();

    QVBoxLayout *centralLayout = new QVBoxLayout(ui->centralWidget);
    ui->centralWidget->setLayout(centralLayout);

    notificationWidget = new NotificationWidget(this);
    notificationWidget->hide();

    centralLayout->addWidget(notificationWidget);

    connect(ui->alert, &QPushButton::clicked, this, &MainWindow::showNotifications);
    connect(notificationWidget, &NotificationWidget::notificationClicked,
            this, &MainWindow::handleNotificationClicked);
    //////////////////////////////


    //connect(ffff, &QPushButton::clicked, this, &MainWindow::on_edit_equi_clicked);

    connect(ui->supprimerevent_3, &QPushButton::clicked, this, &MainWindow::on_supprime_eq_clicked);
    connect(ui->choix_equi, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onEquipementComboBoxIndexChanged);
    connect(ui->recherche_eq, &QLineEdit::returnPressed, this, &MainWindow::onRechercheEqReturnPressed);

//////////////////////////////////////////////////Vaccin
    newsFetcher = new NewsFetcher(this);
    connect(newsFetcher, &NewsFetcher::newSicknessDetected, this, &MainWindow::handleNewSicknessDetected);
    connect(ui->ordonnerButton, &QPushButton::clicked, this, &MainWindow::on_ordonner_clicked);
    connect(ui->tabvaccin->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onVaccinHeaderSectionClicked);
    connect(chatbot, &ChatBot::historyLoaded, this, &MainWindow::loadChatHistory);
    connect(ui->historyButton, &QPushButton::clicked, this, &MainWindow::showChatHistory);
    connect(ui->newChatButton, &QPushButton::clicked, this, &MainWindow::createNewChat);
    // Change this connection in the constructor
    connect(chatbot, &ChatBot::responseReceived, this, [this](const QString &response) {
        QString formattedResponse = QString("<div style='margin:10px; padding:12px; border-radius:12px; "
                                            "background-color:#ffffff; color:#2d3748;"
                                            "border: 1px solid #e2e8f0;'>"
                                            "<b style='color:#48bb78;'>SmartChat:</b> %1</div>").arg(response);
        ui->chatbot_display->append(formattedResponse);
    });
    connect(ui->send_button, &QPushButton::clicked, this, &MainWindow::sendMessageToChatbot);

    // Connect the returnPressed signal to sendMessageToChatbot slot
    connect(ui->chatbot_line, &QLineEdit::returnPressed, this, &MainWindow::sendMessageToChatbot);

    connect(ui->chatbot_line_2, &QLineEdit::returnPressed, this, &MainWindow::handleChatbotLine2ReturnPressed);

    connect(chatbot, &ChatBot::errorOccurred, this, [this](const QString &errorMessage) {
        ui->chatbot_display->append("<b style='color:red'>Error</b>: " + errorMessage);
    });
    connect(ui->nom_vac, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onVaccineNameChanged);
    connect(ui->nom_vac_2, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onVaccineName2Changed);
    connect(ui->type_vac, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onTypeVacCurrentIndexChanged);
    connect(ui->type_vac_2, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onTypeVac2CurrentIndexChanged);
    vaccinTab->setCurrentIndex(7);
    connect(ui->pdfvaccin, &QPushButton::clicked, this, &MainWindow::onPdfVaccinButtonClicked);
    connect(cherche_vac, &QLineEdit::textChanged, this, [this]() {
        vaccinManager->filterVaccinTable(ui->tabvaccin, cherche_vac->text());
    });
    connect(ui->choix_vac, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxIndexChanged);
    connect(ui->tri_rdv, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::tri);
    connect(ui->tabvaccin, &QTableWidget::itemSelectionChanged, this, &MainWindow::onVaccinTableSelectionChanged);
    connect(edit_vac, &QPushButton::clicked, this, &MainWindow::on_edit_vac_2_clicked);
    connect(save_vac_2, &QPushButton::clicked, this, &MainWindow::on_save_2_clicked);
    connect(ui->date_vac, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);
    connect(ui->date_vac_2, &QDateEdit::dateChanged, this, &MainWindow::onDateChanged);
    connect(vaccinB, &QPushButton::clicked, this, &MainWindow::onvaccinBclicked);
    connect(ajoutvac, &QPushButton::clicked, this, &MainWindow::onajoutvacclicked);
    connect(ui->actionVaccin, &QAction::triggered, this, &MainWindow::onActionVaccinTriggered);
    connect(ui->tabvaccin->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onVaccinHeaderSectionClicked);
    ui->tabvaccin->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabvaccin->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->chatbot_line_3, &QLineEdit::returnPressed, this, &MainWindow::handleChatbotLine3ReturnPressed);
////////////////////////////////////////////////////////////



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

    QTableWidget *tabpat = ui->tabpat;
    tabpat->setColumnWidth(0, 165);
    tabpat->setColumnWidth(1, 165);
    tabpat->setColumnWidth(2, 165);
    tabpat->setColumnWidth(3, 165);
    tabpat->setColumnWidth(4, 165);
    tabpat->setColumnWidth(5, 165);
    tabpat->setColumnWidth(6, 165);
    tabpat->setColumnWidth(7, 165);
    tabpat->setColumnWidth(8, 165);
    tabpat->setColumnWidth(9, 165);
    tabpat->setColumnWidth(10, 165);

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

    tabpat->setStyleSheet(
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
    tabpat->resizeRowsToContents();

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
    ui->chatbot_display->setStyleSheet(
        "QTextEdit {"
        "    background-color: #f5f7fa;"
        "    border: 1px solid #e2e8f0;"
        "    border-radius: 12px;"
        "    padding: 16px;"
        "    font-size: 15px;"
        "    color: #2d3748;"
        "    font-family: 'Segoe UI', system-ui, sans-serif;"
        "    selection-background-color: #4c51bf;"
        "    selection-color: #f7fafc;"
        "    line-height: 1.5;"
        "}"
        );
    ui->chatbot_line->setStyleSheet(
        "QLineEdit {"
        "    background-color: #ffffff;"      // Pure white only for input
        "    border: 2px solid #e2e8f0;"
        "    border-radius: 20px;"
        "    padding: 14px 20px;"
        "    font-size: 15px;"
        "    color: #2d3748;"
        "    font-family: 'Segoe UI', system-ui, sans-serif;"
        "    margin-right: 8px;"
        "    min-height: 20px;"
        "}"
        "QLineEdit:focus {"
        "    border-color: #667eea;"          // Soft blue accent
        "    background-color: #f8fafc;"
        "}"
        );
    ui->send_button->setStyleSheet(
        "QPushButton {"
        "    background-color: #4a90e2;"      // Original blue color
        "    color: white;"
        "    border: none;"
        "    border-radius: 20px;"            // Matches input field
        "    padding: 8px 16px;"              // Original padding
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    min-width: 0px;"                 // Ensures original size
        "    min-height: 0px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #3a7bc8;"      // Original hover color
        "}"
        "QPushButton:pressed {"
        "    background-color: #2a6bb4;"      // Original pressed color
        "}"
        );
    ui->newChatButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #48bb78;"      // Vibrant green
        "    color: #f7fafc;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 10px 18px;"
        "    font-size: 13px;"
        "    font-weight: 600;"
        "    font-family: 'Segoe UI', system-ui, sans-serif;"
        "    margin-right: 8px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #38a169;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #2f855a;"
        "}"
        );
    ui->historyButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #667eea;"
        "    color: #f7fafc;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 10px 18px;"
        "    font-size: 13px;"
        "    font-weight: 600;"
        "    font-family: 'Segoe UI', system-ui, sans-serif;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5a67d8;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #4c51bf;"
        "}"
        );
    ui->detail_label->setStyleSheet("QLabel { padding: 10px; background: #f8f8f8; border-radius: 5px; }");
    QTimer::singleShot(0, this, &MainWindow::checkVaccineExpiration);


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


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::readHeartRate);

    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &MainWindow::updateCountdown);

    //QObject::connect(arduino->getserial(), SIGNAL(readyRead()), this, SLOT(update_label()));


    arduino = new Arduino(this);
    // Get available ports and let user select
    QStringList ports = arduino->getAvailablePorts();
    if (ports.isEmpty()) {
        ///QMessageBox::critical(this, "Error", "No ports available!");
        return;
    }

    bool ok;
    QString port = QInputDialog::getItem(this, "Select Port", "Choose COM Port:", ports, 0, false, &ok);
    if (!ok || port.isEmpty()) return;


    QString error = arduino->connectToPort("COM7"); // Update COM7 if needed
    if (!error.isEmpty()) {
        QMessageBox::critical(this, "Erreur", "Échec de la connexion au port : " + error);
    }


}

MainWindow::~MainWindow() {
    if (serial->isOpen()) {
        serial->close();
    }
    delete ui;
}

void MainWindow::on_pushButton_4_clicked() {
    vaccinTab->setCurrentIndex(16);
    loadPatientData(ui->tabpat);

    //////////////mar
    // Highlight the updated row in the table and update the displayed cell



}

void MainWindow::loadPatientData(QTableWidget *tabpat) {
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.isValid() || !db.isOpen()) {
        db.close();
        if (!db.open()) {

            return;
        }
    }

    QSqlQuery query;
    query.prepare("SELECT CIN_PASSP, NOM, PRENOM, TO_CHAR(DATE_NAISS, 'DD/MM/YYYY'), HEART_RATE, NUM_DOSSIER, TEL, MAIL, SEXE, ADRESSE, CNAM FROM MARIEM.PATIENT");

    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la récupération : " + query.lastError().text());
        return;
    }

    tabpat->setRowCount(0);
    tabpat->setColumnCount(11);
    tabpat->setHorizontalHeaderLabels({"CIN", "Nom", "Prenom", "Date Naissance", "Heart Rate", "Num Dossier", "Tel", "Mail", "Sexe", "Adresse", "Cnam"});

    int row = 0;
    while (query.next()) {
        tabpat->insertRow(row);
        for (int col = 0; col < 11; col++) {
            QString value = query.value(col).toString();
            tabpat->setItem(row, col, new QTableWidgetItem(value));
        }
        row++;
    }

    tabpat->resizeColumnsToContents();
    tabpat->resizeRowsToContents();




}

void MainWindow::on_hearte_rate_clicked() {
    identifiant = -1;
    bool ok = false;
    QAbstractItemModel *model = ui->tabpat->model();

    QModelIndexList selection = ui->tabpat->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        identifiant = model->data(selection.first().siblingAtColumn(0)).toInt();
        ok = true;
    } else {
        identifiant = QInputDialog::getInt(this,
                                           "Mesure du rythme cardiaque",
                                           "Entrez l'identifiant du patient :",
                                           0, 0, INT_MAX, 1, &ok);
    }

    if (!ok || identifiant < 0) return;

    heartRates.clear();
    totalHeartRate = 0;

    countdown = 15;
    ui->ratelab->setText("Prochaine mesure dans : " + QString::number(countdown) + "s");

    countdownTimer->start(1000); // Lancer le décompte 1 fois
}


void MainWindow::on_hearte_rate2_clicked() {
    identifiant = -1;
    bool ok = false;
    QAbstractItemModel *model = ui->tabpat->model();

    QModelIndexList selection = ui->tabpat->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        identifiant = model->data(selection.first().siblingAtColumn(0)).toInt();
        ok = true;
    } else {
        identifiant = QInputDialog::getInt(this,
                                           "Mesure du rythme cardiaque",
                                           "Entrez l'identifiant du patient :",
                                           0, 0, INT_MAX, 1, &ok);
    }

    if (!ok || identifiant < 0) return;

    // Nouvelle partie : Chercher BPM et l'envoyer à Arduino
    QSqlQuery query;
    query.prepare("SELECT HEART_RATE FROM MARIEM.PATIENT WHERE CIN_PASSP = :id");
    query.bindValue(":id", identifiant);

    if (query.exec() && query.next()) {
        int heartRate = query.value(0).toInt(); // Lire la valeur du BPM

        qDebug() << "Heart Rate trouvé pour CIN" << identifiant << ":" << heartRate;

        if (heartRate > 0) {
            // Envoyer la valeur au Arduino via Serial
            QString command = QString("A%1\n").arg(heartRate);
            arduino->writeData(command); // ✨ Déjà disponible dans ton Arduino.cpp
        } else {
            QMessageBox::warning(this, "BPM Non valide", "BPM invalide pour ce patient.");
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer le BPM du patient !");
    }



}

void MainWindow::updateCountdown() {
    countdown--;

    // Lire la valeur du capteur à chaque seconde (optionnel)
    int heartRate = arduino->readHeartRate();
    if (heartRate != -1) {
        heartRates.append(heartRate);
        totalHeartRate += heartRate;
        ui->ratelab->setText("Mesure (" + QString::number(countdown) + "s restantes) : " + QString::number(heartRate));
    } else {
        ui->ratelab->setText("Aucune donnée valide (" + QString::number(countdown) + "s restantes)");
    }

    if (countdown == 0) {
        countdownTimer->stop();

        int finalHeartRate = heartRates.isEmpty() ? -1 : totalHeartRate / heartRates.size();

        if (finalHeartRate != -1) {

            QSqlDatabase db = QSqlDatabase::database();
            if (!db.transaction()) {
                qWarning() << "Failed to start transaction:" << db.lastError().text();
            }

            // 2) Execute exactly one UPDATE…
            QSqlQuery query(db);
            query.prepare(R"(
    UPDATE MARIEM.PATIENT
       SET HEART_RATE = :rate
     WHERE CIN_PASSP  = :id
)");
            query.bindValue(":rate", finalHeartRate);
            query.bindValue(":id",   identifiant);

            if (!query.exec()) {
                db.rollback();
                QMessageBox::warning(this, "Erreur BDD",
                                     "Échec de la mise à jour : " + query.lastError().text());
                return;
            }

            // 3) Commit so the change is visible to **all** sessions (including your SQL Developer)
            if (!db.commit()) {
                QMessageBox::warning(this, "Erreur BDD",
                                     "Échec du commit : " + db.lastError().text());
                return;
            }

            // 4) Now reload the table from the database
            loadPatientData(ui->tabpat);

            // 5) (Optional) re-select the updated row
            for (int row = 0; row < ui->tabpat->rowCount(); ++row) {
                if (ui->tabpat->item(row, 0)->text().toInt() == identifiant) {
                    ui->tabpat->selectRow(row);
                    break;
                }
            }


            QMessageBox::information(this, "Succès", "Rythme cardiaque moyen enregistré : " + QString::number(finalHeartRate));

            // Nouveau : afficher le résultat final dans le label
            ui->ratelab->setText("Mesure terminée, BPM moyen : " + QString::number(finalHeartRate));
            arduino->sendAverageBPM(finalHeartRate);

        } else {
            // Nouveau : si aucune donnée, afficher ça aussi dans le label
            ui->ratelab->setText("Mesure terminée, aucune donnée valide.");
        }
    }
}



void MainWindow::readHeartRate() {
    int heartRate = arduino->readHeartRate();

    if (heartRate == -1) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée valide reçue du capteur !");
        return;
    }

    heartRates.append(heartRate);
    totalHeartRate += heartRate;

    ui->ratelab->setText("Valeur mesurée : " + QString::number(heartRate));

    // Mise à jour dans la base de données
    QSqlQuery query;
    query.prepare("UPDATE PATIENT SET HEART_RATE = :rate WHERE CIN_PASSP = :id");
    query.bindValue(":rate", heartRate);
    query.bindValue(":id", identifiant);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur BDD", "Échec de la mise à jour : " + query.lastError().text());
        return;
    }

    // Rechargement des données dans la table
    loadPatientData(ui->tabpat);

    // Message de succès
    QMessageBox::information(this, "Succès", "Rythme cardiaque enregistré : " + QString::number(heartRate));
}

void MainWindow::readAverageHeartRate() {
    if (!arduino) return;

    QString message = arduino->readSerialLine();

    if (message.startsWith("A")) {
        int avgBPM = message.mid(1).toInt();
        qDebug() << "Average BPM received:" << avgBPM;

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmer", "Enregistrer le BPM moyen : " + QString::number(avgBPM) + " ?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // Save to database
            QSqlQuery query;
            query.prepare("UPDATE PATIENT SET HEART_RATE = :rate WHERE CIN_PASSP = :id");
            query.bindValue(":rate", avgBPM);
            query.bindValue(":id", identifiant);  // 'identifiant' doit être bien défini
            if (query.exec()) {
                QMessageBox::information(this, "Succès", "BPM moyen enregistré !");
                   loadPatientData(ui->tabpat);
            } else {
                QMessageBox::critical(this, "Erreur", "Échec d'enregistrement : " + query.lastError().text());
            }

            // Send BPM back to Arduino to display on LCD
            arduino->writeData(QString("A%1\n").arg(avgBPM));
        }
    }
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


void MainWindow::on_Quit_clicked() {
    vaccinTab->setCurrentIndex(0);
}

void MainWindow::on_Quit_4_clicked() {
    vaccinTab->setCurrentIndex(7);
}

void MainWindow::onvaccinBclicked() {
    vaccinTab->setCurrentIndex(0);
    vaccinManager->loadVaccinData(ui->tabvaccin);
}

void MainWindow::on_chatbot_page_clicked()
{
    vaccinTab->setCurrentIndex(22);
}

void MainWindow::on_chatbot_page_2_clicked()
{
    vaccinTab->setCurrentIndex(22);
}


//////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_ordonner_clicked()
{
    QListWidgetItem *selected = ui->liste->currentItem();
    if (!selected) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
        return;
    }

    vaccinTab->setCurrentIndex(21);
    int idRdvl = selected->data(Qt::UserRole).toInt();
    QSqlQuery prep;
    prep.prepare("SELECT VACCIN_RDV FROM RENDEZ_VOUS WHERE ID_RDV = :idRdvl");
    prep.bindValue(":idRdvl", idRdvl);
    QString Nom;

    if (prep.exec() && prep.next()) {
        Nom = prep.value(0).toString();
        ui->Nomvac->setText(Nom);

        prep.prepare("SELECT REFERENCE, QUANTITE FROM VACCIN WHERE NOM = :Nom");
        prep.bindValue(":Nom", Nom);

        if (prep.exec() && prep.next()) {
            QString fullReference = prep.value(0).toString();
            int quantite = prep.value(1).toInt();
            ui->Referencevac->setText(fullReference);
            ui->quantitevac->setText(QString::number(quantite));

            // Extraire les 4 premiers chiffres
            QString movementReference = fullReference.left(4);
            if (movementReference.length() != 4) {
                QMessageBox::warning(this, "Attention", "La référence doit contenir au moins 4 chiffres");
                return;
            }

            // Envoyer à l'BrasRobotique
            if (brasrobotique != nullptr) {
                if (!brasrobotique->isSerialOpen()) {
                    if (!brasrobotique->initSerialPort("COM4", 9600)) {
                        QMessageBox::critical(this, "Erreur", "Port série non ouvert");
                        return;
                    }
                }

                // 1. Envoyer la quantité
                QString quantityCommand = QString("Q:%1\n").arg(quantite);
                brasrobotique->sendCommand(quantityCommand);
                qDebug() << "Quantité envoyée:" << quantityCommand;

                // 2. Envoyer les 4 premiers chiffres de la référence
                brasrobotique->sendCommand(movementReference + "\n");
                qDebug() << "Référence de mouvement envoyée:" << movementReference;

                // Demande de confirmation
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Confirmation",
                                              "Voulez-vous confirmer cette opération?",
                                              QMessageBox::Yes|QMessageBox::No);

                if (reply == QMessageBox::Yes) {
                    // Si confirmé, le bras reste en position
                    qDebug() << "Opération confirmée - Bras reste en position";

                    // Envoyer confirmation après délai
                    QTimer::singleShot(3000, this, [this]() {
                        if (brasrobotique && brasrobotique->isSerialOpen()) {
                            brasrobotique->sendCommand("C\n");  // Commande de confirmation
                            qDebug() << "Commande de confirmation envoyée";
                        }
                    });
                } else {
                    // Si annulé, retour à la position initiale
                    qDebug() << "Opération annulée - Retour à la position initiale";

                    // Envoyer commande de retour
                    if (brasrobotique && brasrobotique->isSerialOpen()) {
                        brasrobotique->sendCommand("H\n");  // Commande 'Home'
                        qDebug() << "Commande 'Home' envoyée";
                    }

                    // Réinitialiser l'interface
                    ui->Nomvac->clear();
                    ui->Referencevac->clear();
                    ui->quantitevac->clear();
                    vaccinTab->setCurrentIndex(6);  // Modification ici: retour à l'onglet 6 au lieu de 0
                }
            }
        }
    }
}

void MainWindow::on_confirmer_clicked()
{
    QString nomVaccin = ui->Nomvac->text();
    int currentQuantity = ui->quantitevac->text().toInt();

    if (nomVaccin.isEmpty() || currentQuantity <= 0) {
        QMessageBox::warning(this, "Erreur", "Quantité invalide");
        return;
    }

    // Demande de confirmation finale
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation finale",
                                  "Êtes-vous sûr de vouloir valider cette opération?",
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        // Si l'utilisateur annule
        if (brasrobotique != nullptr && brasrobotique->isSerialOpen()) {
            brasrobotique->sendCommand("H\n");  // Retour à la position initiale
            qDebug() << "Opération annulée - Retour à la position initiale";
        }
        return;
    }

    // Si confirmé, procéder à la mise à jour
    int newQuantity = currentQuantity - 1;
    ui->quantitevac->setText(QString::number(newQuantity));

    // Mise à jour de la base de données
    QSqlQuery query;
    query.prepare("UPDATE VACCIN SET QUANTITE = :newQuantity WHERE NOM = :nomVaccin");
    query.bindValue(":newQuantity", newQuantity);
    query.bindValue(":nomVaccin", nomVaccin);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
        return;
    }

    // Envoyer la quantité mise à jour à l'BrasRobotique
    if (brasrobotique != nullptr && brasrobotique->isSerialOpen()) {
        QString quantityCommand = QString("Q:%1\n").arg(newQuantity);
        brasrobotique->sendCommand(quantityCommand);
        qDebug() << "Quantité mise à jour envoyée:" << quantityCommand;
    }

    // Séquence d'opération du bras
    if (brasrobotique != nullptr && brasrobotique->isSerialOpen()) {
        // 1. Ouvrir la pince
        brasrobotique->sendCommand("1\n");
        qDebug() << "Commande 'ouvrir' envoyée";

        // 2. Fermer la pince après délai
        QTimer::singleShot(1000, this, [this]() {
            if (brasrobotique && brasrobotique->isSerialOpen()) {
                brasrobotique->sendCommand("2\n");
                qDebug() << "Commande 'fermer' envoyée";
            }
        });

        // 3. Retour à la position initiale après opération
        QTimer::singleShot(2000, this, [this]() {
            if (brasrobotique && brasrobotique->isSerialOpen()) {
                brasrobotique->sendCommand("H\n"); // Commande 'Home'
                qDebug() << "Commande 'Home' envoyée";
            }
        });
    }
}

void MainWindow::checkVaccineExpiration() {
    QSqlQuery query;
    query.prepare("SELECT NOM, DATE_EXP FROM VACCIN WHERE DATE_EXP < SYSDATE");

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }
    while (query.next()) {
        QString nom = query.value(0).toString();
        QDate dateExp = query.value(1).toDate();
        if (!notifiedVaccines.contains(nom)) {
            notifiedVaccines.insert(nom);
            triggerExpiredVaccineNotification(nom, dateExp);
        }
    }
}


void MainWindow::triggerExpiredVaccineNotification(const QString &nom, const QDate &dateExp) {
    QString message = QString("Le vaccin '%1' a expiré le %2.")
                          .arg(nom, dateExp.toString("dd-MM-yyyy"));

    ajoutNotification(message);
}


void MainWindow::loadChatHistory(const QString &history) {
    ui->chatbot_display->clear();
    ui->chatbot_display->append(history);
}

void MainWindow::showChatHistory() {
    chatbot->loadChatHistory();
        ui->chatbot_line->setEnabled(false);
    ui->chatbot_line_2->setEnabled(false);
    ui->chatbot_line_3->setEnabled(false);
}



void MainWindow::createNewChat() {
    ui->chatbot_display->clear();
    ui->chatbot_line->setEnabled(true);
    ui->chatbot_line_2->setEnabled(true);
    ui->chatbot_line_3->setEnabled(true);
}



void MainWindow::sendMessageToChatbot() {
    QString userMessage = ui->chatbot_line->text();
    if (!userMessage.isEmpty()) {
        QString formattedUserMessage = QString("<div style='margin:5px; padding:12px; border-radius:12px; "
                                               "background-color:#ffffff; color:#2d3748;"
                                               "border: 1px solid #e2e8f0;'>"
                                               "<b style='color:#4a90e2;'>Utilisateur:</b> %1</div>").arg(userMessage);
        ui->chatbot_display->append(formattedUserMessage);
        chatbot->sendMessageToChatbot(userMessage);
        ui->chatbot_line->clear();
    }
}


void MainWindow::handleChatbotLine2ReturnPressed()
{
    QString text = ui->chatbot_line_2->text();
    ui->vaccin->setCurrentIndex(22);
    ui->chatbot_line->setText(text);
    ui->chatbot_line_2->clear();
    sendMessageToChatbot();
}

void MainWindow::handleChatbotLine3ReturnPressed()
{
    QString text = ui->chatbot_line_3->text();
    ui->vaccin->setCurrentIndex(22);
    ui->chatbot_line->setText(text);
    ui->chatbot_line_3->clear();
    sendMessageToChatbot();
}

void MainWindow::handleNewSicknessDetected(const QString &sicknessName,
                                           const QString &date,
                                           const QString &location,
                                           const QString &vaccineInfo,
                                           const QString &detail,
                                           const QString &source) {
    qDebug() << "handleNewSicknessDetected called with:" << sicknessName << date << location << vaccineInfo << detail << source;

    QString notificationMessage = QString("Nouvelle : %1\n"
                                          "Date : %2\n"
                                          "Lieu : %3\n"
                                          "Vaccin : %4\n"
                                          "Source : %5")
                                      .arg(sicknessName, date, location, vaccineInfo, source);

    ajoutNotification(notificationMessage);
    static int uniqueNumero = 1;
    saveNotificationToFile(uniqueNumero++,
                           detail,
                           vaccineInfo,
                           QDate::fromString(date, "yyyy-MM-dd"),
                           source,
                           sicknessName,
                           location);
}

void MainWindow::saveNotificationToFile(int numero,
                                        const QString &detail,
                                        const QString &status,
                                        const QDate &date,
                                        const QString &source,
                                        const QString &maladie,
                                        const QString &lieu) {
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/notifications.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();

        // Check for existing notification with the same disease name
        bool isDuplicate = content.contains("Maladie: " + maladie, Qt::CaseInsensitive);

        if (!isDuplicate) {
            // Append new notification if it doesn't already exist
            file.open(QIODevice::Append | QIODevice::Text);
            QTextStream out(&file);
            out << QString("[%1] Notification Numero: %2\nDetail: %3\nStatus: %4\nDate: %5\nSource: %6\nMaladie: %7\nLieu: %8\n\n")
                       .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                       .arg(numero)
                       .arg(detail)
                       .arg(status)
                       .arg(date.toString("yyyy-MM-dd"))
                       .arg(source)
                       .arg(maladie)
                       .arg(lieu);
            file.close();
        } else {
            qDebug() << "Duplicate notification detected for disease:" << maladie;
        }
    } else {
        qDebug() << "Failed to open file for reading notifications.";
    }
}


void MainWindow::onActionVaccinTriggered() {
    vaccinTab->setCurrentIndex(23);

    QSqlQueryModel* model = vaccinManager->fetchVaccineStatistics();
    if (!model) return;

    QPieSeries *series = new QPieSeries();
    int totalQuantite = 0;

    int rows = model->rowCount();
    for (int i = 0; i < rows; ++i) {
        totalQuantite += model->data(model->index(i, 1)).toInt();
    }

    QList<QColor> colors = {
        QColor(0, 120, 150), QColor(0, 160, 180), QColor(0, 190, 200),
        QColor(0, 220, 230), QColor(50, 130, 150), QColor(30, 160, 180),
        QColor(20, 190, 210), QColor(0, 100, 120)
    };

    for (int i = 0; i < rows; ++i) {
        QString nom = model->data(model->index(i, 0)).toString();
        int quantite = model->data(model->index(i, 1)).toInt();

        double percentage = (double(quantite) / totalQuantite) * 100;
        QPieSlice *slice = series->append(nom, quantite);
        slice->setLabel(QString("%1: %2% (%3 unités)").arg(nom).arg(percentage, 0, 'f', 1).arg(quantite));
        slice->setLabelVisible(true);
        slice->setBrush(colors[i % colors.size()]);

        if (quantite == model->data(model->index(0, 1)).toInt()) {
            slice->setExploded(true);
            slice->setPen(QPen(Qt::black, 2));
        }
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Vaccins (en fonction des quantités)");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);

    // Modifier la couleur de fond pour correspondre aux couleurs du tableau
    chart->setBackgroundBrush(QBrush(QColor(240, 230, 220))); // Couleur douce et neutre
    chart->setTitleBrush(QBrush(Qt::black));
    chart->legend()->setLabelBrush(QBrush(Qt::black));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(1450, 750);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Modifier la couleur du cadre pour qu'elle corresponde à l'onglet
    chartView->setStyleSheet("background: rgb(240, 230, 220); border: 2px solid rgb(50, 130, 150);");

    QWidget *currentTab = vaccinTab->widget(23);
    QLayout *oldLayout = currentTab->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QHBoxLayout *mainLayout = new QHBoxLayout(currentTab);
    currentTab->setLayout(mainLayout);

    mainLayout->addWidget(chartView, 1, Qt::AlignCenter);
}

void MainWindow::onVaccineNameChanged()
{
    QString vaccine = ui->nom_vac->currentText();
    if (vaccine == "nouveau vaccin") {
        ui->nom_vac->setCurrentText("");
        ui->nom_vac->setFocus();
        return;
    }
    setVaccineTypeBasedOnName(vaccine, ui->type_vac);
    setVaccineModeBasedOnName(vaccine, ui->mode_vac); // This line was missing
}



void MainWindow::onVaccineName2Changed()
{
    QString vaccine = ui->nom_vac_2->currentText();
    if (vaccine == "nouveau vaccin") {
        ui->nom_vac_2->setCurrentText("");
        ui->nom_vac_2->setFocus();
        return;
    }
    setVaccineTypeBasedOnName(vaccine, ui->type_vac_2);
    setVaccineModeBasedOnName(vaccine, ui->mode_vac_2);
}


void MainWindow::setVaccineTypeBasedOnName(const QString &vaccineName, QComboBox *typeCombo)
{
    static const QMap<QString, QString> typeMap = {
        {"Pfizer-BioNTech", "Vaccin à ARN messager"},
        {"Moderna", "Vaccin à ARN messager"},
        {"AstraZeneca", "Vaccin à vecteur viral"},
        {"Johnson & Johnson", "Vaccin à vecteur viral"},
        {"Sinopharm", "Vaccin inactivé"},
        {"Sputnik V", "Vaccin à vecteur viral"},
        {"Novavax", "Vaccin à sous-unité protéique"},
        {"Sinovac", "Vaccin inactivé"},
        {"Comirnaty", "Vaccin à ARN messager"},
        {"Spikevax", "Vaccin à ARN messager"}
    };

    QString type = typeMap.value(vaccineName, "");
    if (!type.isEmpty()) {
        int idx = typeCombo->findText(type);
        typeCombo->setCurrentIndex(idx != -1 ? idx : 0);
    }
}

void MainWindow::setVaccineModeBasedOnName(const QString &vaccineName, QComboBox *modeCombo)
{
    static const QMap<QString, QString> modeMap = {
                                                   {"Pfizer-BioNTech", "Injection intramusculaire"},
                                                   {"Moderna", "Injection intramusculaire"},
                                                   {"AstraZeneca", "Injection intramusculaire"},
                                                   {"Johnson & Johnson", "Injection intramusculaire"},
                                                   {"Sinopharm", "Injection intramusculaire"},
                                                   {"Sputnik V", "Injection intramusculaire"},
                                                   {"Novavax", "Injection intramusculaire"},
                                                   {"Sinovac", "Injection intramusculaire"},
                                                   {"Comirnaty", "Injection intramusculaire"},
                                                   {"Spikevax", "Injection intramusculaire"},
                                                   };

    if (vaccineName == "nouveau vaccin") {
        modeCombo->setCurrentIndex(0);
        return;
    }

    QString mode = modeMap.value(vaccineName, "Injection intramusculaire");
    int modeIndex = modeCombo->findText(mode);
    if (modeIndex != -1) {
        modeCombo->setCurrentIndex(modeIndex);
    } else {
        modeCombo->setCurrentText(mode);
    }
}

void MainWindow::onTypeVacCurrentIndexChanged()
{
    if (ui->type_vac->currentText() == "nouveau type") {
        ui->type_vac->setCurrentText("");
        ui->type_vac->setFocus();
    }
}

void MainWindow::onTypeVac2CurrentIndexChanged()
{
    if (ui->type_vac_2->currentText() == "nouveau type") {
        ui->type_vac_2->setCurrentText("");
        ui->type_vac_2->setFocus();
    }
}

void MainWindow::on_save_vac_clicked() {
    bool isValid = true;
    QString referenceStr = ui->reference->text();

    // Check if the reference is numeric and has exactly 7 digits
    bool isNumeric = false;
    referenceStr.toInt(&isNumeric);
    if (!isNumeric || referenceStr.length() != 7) {
        QMessageBox::warning(this, "Erreur", "La référence doit être composée de 7 chiffres.");
        return;
    }

    int reference = referenceStr.toInt(&isValid);
    if (!isValid || reference <= 0) {
        QMessageBox::warning(this, "Erreur", "Référence invalide");
        return;
    }

    // Vérification de l'unicité de la référence
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM VACCIN WHERE REFERENCE = :reference");
    checkQuery.bindValue(":reference", reference);
    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la vérification de la référence : " + checkQuery.lastError().text());
        return;
    }
    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count > 0) {
        QMessageBox::warning(this, "Erreur", "La référence doit être unique.");
        return;
    }

    QString nom = ui->nom_vac->currentText();
    if (nom.isEmpty() || nom == "nouveau vaccin") {
        QMessageBox::warning(this, "Erreur", "Nom de vaccin invalide");
        return;
    }
    QString type = ui->type_vac->currentText();
    if (type.isEmpty() || type == "nouveau type") {
        QMessageBox::warning(this, "Erreur", "Type de vaccin invalide");
        return;
    }
    int age_min = ui->age_vac->text().toInt(&isValid);
    if (!isValid || age_min < 1 || age_min > 120) {
        QMessageBox::warning(this, "Erreur", "Âge minimum invalide (1-120)");
        return;
    }
    QString mode_admin = ui->mode_vac->currentText();
    if (mode_admin.isEmpty() || mode_admin == "nouveau mode") {
        QMessageBox::warning(this, "Erreur", "Mode d'administration invalide");
        return;
    }
    QString dose = ui->dose_vac->text();
    if (dose.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Dose invalide");
        return;
    }
    QDate date_exp = ui->date_vac->date();
    if (date_exp <= QDate::currentDate()) {
        QMessageBox::warning(this, "Erreur", "Date d'expiration doit être future");
        return;
    }
    double prix = ui->prix_vac->text().toDouble(&isValid);
    if (!isValid || prix < 0) {
        QMessageBox::warning(this, "Erreur", "Prix invalide");
        return;
    }
    int quantite = ui->quantite_vac->text().toInt(&isValid);
    if (!isValid || quantite <= 0) {
        QMessageBox::warning(this, "Erreur", "Quantité invalide");
        return;
    }
    vaccinManager->saveVaccinData(reference, nom, type, age_min, mode_admin, dose, date_exp, prix, quantite);

    ui->reference->clear();
    ui->nom_vac->setCurrentIndex(0);
    ui->type_vac->setCurrentIndex(0);
    ui->age_vac->clear();
    ui->mode_vac->setCurrentIndex(0);
    ui->dose_vac->clear();
    ui->date_vac->setDate(QDate::currentDate().addDays(1));
    ui->prix_vac->clear();
    ui->quantite_vac->clear();
    vaccinManager->loadVaccinData(ui->tabvaccin);
    showAllRows();
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
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmer la suppression", "Êtes-vous sûr de vouloir supprimer cet élément?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            QTableWidgetItem* referenceItem = ui->tabvaccin->item(selectedRow, 0);
            int reference = referenceItem->text().toInt();
            vaccinManager->deleteVaccin(reference);
            vaccinManager->loadVaccinData(ui->tabvaccin);
            selectedRow = -1;
            showAllRows();
        } else {
            QMessageBox::information(this, "Opération annulée", "La suppression a été annulée.");
        }
    } else {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un élément du tableau.");
        showAllRows();
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
    bool isValid = true;
    int reference = ui->reference_2->text().toInt(&isValid);
    if (!isValid || reference <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le champ Référence doit être un nombre positif.");
        return;
    }

    QString nom = ui->nom_vac_2->currentText();
    QString type = ui->type_vac_2->currentText();

    int age_min = ui->age_vac_2->text().toInt(&isValid);
    if (!isValid || age_min <= 0 || age_min < 1 || age_min >= 120) {
        QMessageBox::warning(this, "Erreur de saisie", "Le champ Âge minimum doit être un nombre entre 1 et 120");
        return;
    }

    QString mode_admin = ui->mode_vac_2->currentText(); // Get selected text from combo box
    QString dose = ui->dose_vac_2->text();

    QDate date_exp = ui->date_vac_2->date();
    QDate currentDate = QDate::currentDate();
    if (date_exp <= currentDate) {
        QMessageBox::warning(this, "Date invalide", "La date d'expiration doit être postérieure à la date actuelle.");
        return;
    }

    double prix = ui->prix_vac_2->text().toDouble(&isValid);
    if (!isValid || prix < 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le champ Prix doit être un nombre positif ou 0.");
        return;
    }

    int quantite = ui->quantite_vac_2->text().toInt(&isValid);
    if (!isValid || quantite <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le champ Quantité doit être un nombre positif.");
        return;
    }

    vaccinManager->updateVaccinData(reference, nom, type, age_min, mode_admin, dose, date_exp, prix, quantite);

    // Clear the input fields
    ui->reference_2->clear();
    ui->nom_vac_2->setCurrentIndex(0); // Reset to the first item
    ui->type_vac_2->setCurrentIndex(0); // Reset to the first item
    ui->age_vac_2->clear();
    ui->mode_vac_2->setCurrentIndex(0); // Reset to the first item
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

//Equipements::calculateStatistics(model);


void MainWindow::showNotificationHistory() {
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/notifications.txt";
    QFile file(filePath);
    QString historyContent;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        historyContent = in.readAll();
        file.close();
    } else {
        qDebug() << "Failed to open file for reading notification history.";
        historyContent = "<i>Historique non disponible</i>";
    }

    ui->detail_label->setHtml("<html><body style='font-family:Arial; font-size:12pt;'>"
                              + historyContent.replace("\n", "<br>")
                              + "</body></html>");
    ui->detail_label->setOpenExternalLinks(true);

    if (vaccinTab) {
        vaccinTab->setCurrentIndex(20);
        QTimer::singleShot(100, [this]() {
            ui->detail_label->moveCursor(QTextCursor::Start);
            ui->detail_label->ensureCursorVisible();
        });
    }
}



void MainWindow::handleNotificationClicked(const QString &message) {
    bool isExpirationNotification = message.contains("a expiré le", Qt::CaseInsensitive);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Détails de la notification");
    msgBox.setText(message);

    QPushButton *okButton = msgBox.addButton("Ok", QMessageBox::ActionRole);
    okButton->setStyleSheet("background-color: #46949c; color: white;");

    QPushButton *historiqueButton = nullptr;
    QPushButton *clearHistoryButton = nullptr;

    if (!isExpirationNotification) {
        historiqueButton = msgBox.addButton("Afficher l'historique", QMessageBox::ActionRole);
        historiqueButton->setStyleSheet("background-color: #46949c; color: white;");

        clearHistoryButton = msgBox.addButton("Effacer l'historique", QMessageBox::ActionRole);
        clearHistoryButton->setStyleSheet("background-color: #46949c; color: white;");
    }

    msgBox.exec();

    if (msgBox.clickedButton() == okButton) {
        // Handle OK button click if needed
    } else if (msgBox.clickedButton() == historiqueButton) {
        showNotificationHistory();
    } else if (msgBox.clickedButton() == clearHistoryButton) {
        clearNotificationHistory();
    }
}

void MainWindow::clearNotificationHistory() {
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/notifications.txt";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        // No need to write anything, just open and close to clear the file
        file.close();
        qDebug() << "Notification history cleared.";
    } else {
        qDebug() << "Failed to open file for clearing notifications.";
    }
}



//RENDEZ_VOUS




void MainWindow::on_rdv_clicked() {
    vaccinTab->setCurrentIndex(6);

    rdvWindow->loadAppointmentsL(ui->liste);
    rdvWindow->loadAppointments(ui->liste_att);
    QCalendarWidget *calendrier = ui->calendrier_rdv;

    QTextCharFormat format;
    QTextCharFormat format1;
    QTextCharFormat format2;
    format.setBackground(QColor(255, 255, 150));
    format1.setBackground(QColor(255, 146, 37));
    format2.setBackground(QColor(255, 32, 24));

    format.setFontUnderline(true);
    format1.setFontUnderline(true);
    format2.setFontUnderline(true);

    rdvWindow->loadHolidaysFromFile(ui->calendrier_rdv);

    appointmentTooltips.clear();
    appointmentTimes.clear();


    QTextCharFormat defaultFormat;




    QSqlQuery qry;
    qry.prepare("SELECT DATE_RDV, NOM_RDV, PRENOM_RDV FROM RENDEZ_VOUS");

    if (qry.exec()) {
        while (qry.next()) {
            QDateTime heure = qry.value(0).toDateTime();
            QDate dateToModify = heure.date();
            QString patientName = qry.value(1).toString() + " " + qry.value(2).toString();

            if (dateToModify.isValid()) {
                appointmentTooltips[dateToModify].append(patientName);
                appointmentTimes[dateToModify].append(heure);

                int count = appointmentTooltips[dateToModify].size();
                if (count == 0){
                    calendrier->setDateTextFormat(dateToModify, defaultFormat);

                }
                else if (count == 1) {
                    calendrier->setDateTextFormat(dateToModify, defaultFormat);
                    calendrier->setDateTextFormat(dateToModify, format);
                } else if (count == 2) {
                    calendrier->setDateTextFormat(dateToModify, defaultFormat);
                    calendrier->setDateTextFormat(dateToModify, format1);
                } else {
                    calendrier->setDateTextFormat(dateToModify, defaultFormat);
                    calendrier->setDateTextFormat(dateToModify, format2);
                }
            } else {
                qDebug() << "Query failed:" << qry.lastError().text();
            }
        }
    }

    QTableView *calendarView = ui->calendrier_rdv->findChild<QTableView *>();
    if (calendarView) {
        calendarView->viewport()->installEventFilter(this);
        qDebug() << "Event filter installed on calendar view.";
    } else {
        qDebug() << "Calendar view not found!";
    }
}



void MainWindow::on_ajoutrdv_clicked() {
    rdvWindow->loadVaccins(ui->vaccin_2,ui->medecin_att,ui->infirmier_att);
    vaccinTab->setCurrentIndex(8);


    qDebug()<<Date;
    ui->dispo->setDateTime(QDateTime(Date, QTime::currentTime()));

    rdvWindow->loadVaccins(ui->vaccin_2,ui->medecin_att,ui->infirmier_att);
}

void MainWindow::check_att(){
    attente = 0;
    if(ui->attente->isChecked()) attente=1;
    else attente=0;
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




    int verif =rdvWindow->saveAppointment(CIN, vaccin, date_rdv, adresse, nom, prenom, dispo, medecin, infirmier, salle, facturation,attente);

    if (verif==1){
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

        on_rdv_clicked();}
    if (verif==2){

        QString message = QString("Un nouveau patient au cin %1 a été ajouté\n "
                                  "Veuillez compléter le dossier médical.").arg(CIN);
        ajoutNotification(message);

    }
}





void MainWindow::autoFillPatientInfo(const QString &cin) {
    if (cin.isEmpty()) {
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM, PRÉNOM, ADRESSE, DATE_NAISS FROM PATIENT WHERE CIN_PASSP = :cin");
    query.bindValue(":cin", cin);
    if (!query.exec()) {
        qDebug() << "SQL Query Error:" << query.lastError().text();
        return;
    }


    if (query.next()) {
        ui->nom_rdv->setText(query.value(0).toString());
        ui->prenom_rdv->setText(query.value(1).toString());
        ui->adresse->setText(query.value(2).toString());
        ui->date_rdv->setDate(query.value(3).toDate());
    } else {
        ui->nom_rdv->clear();
        ui->prenom_rdv->clear();
        ui->adresse->clear();
    }
}





void MainWindow::on_deleteRdv_clicked()
{
    QListWidgetItem *selectedItem = ui->liste_att->currentItem();
    QListWidgetItem *selected = ui->liste->currentItem();

    if (!selectedItem && !selected) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer ce(s) rendez-vous ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (selectedItem) {
            int idRdv = selectedItem->data(Qt::UserRole).toInt();
            rdvWindow->supprimerRdv(idRdv,ui->calendrier_rdv);

        }

        if (selected) {
            int idRdvl = selected->data(Qt::UserRole).toInt();
            rdvWindow->supprimerRdv(idRdvl,ui->calendrier_rdv);

        }
    }
    on_rdv_clicked();
}


void MainWindow::on_edit_rdv_clicked()
{
    QListWidgetItem *selectedItem = ui->liste_att->currentItem();
    QListWidgetItem *selectedItemL = ui->liste->currentItem();
    int idRdv;
    if (!selectedItem && !selectedItemL) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
        return;
    }
    if (selectedItem){
        idRdv = selectedItem->data(Qt::UserRole).toInt();
        currentIdRdv = idRdv;}

    if(selectedItemL)  {
        idRdv = selectedItemL->data(Qt::UserRole).toInt();
        currentIdRdv = idRdv; }




    rdvWindow->infoEdit(idRdv,ui->CIN_rdv_2,ui->date_rdv_2,ui->vaccin_3,ui->adresse_2,ui->nom_rdv_2,ui->prenom_rdv_2,ui->dispo_2,ui->medecin_att_2,ui->infirmier_att_2,ui->salle_att_2,ui->facturation_2);

    vaccinTab->setCurrentIndex(12);


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


    int verif=rdvWindow->modifier_rdv(currentIdRdv, CIN2, vaccin2, date_rdv2, adresse2, nom2, prenom2, dispo2, medecin2, infirmier2, salle2, facturation2);
    if (verif==1){
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
        on_rdv_clicked();
        vaccinTab->setCurrentIndex(6);}
    if (verif==2){


        QString message = QString("Un nouveau patient au cin %1 a été ajouté\n "
                                  "Veuillez compléter le dossier médical.").arg(CIN2);
        ajoutNotification(message);

    }

}

void MainWindow::on_rdv_pdf_clicked()
{
    QListWidgetItem *selectedItem = ui->liste->currentItem();
    if (selectedItem) {

        int idRdv = selectedItem->data(Qt::UserRole).toInt();
        currentIdRdv = idRdv;
        rdvWindow->generateCertificate(currentIdRdv);
    } else {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
    }
}

void MainWindow::rdv_recherche(const QString &text){

    rdvWindow->recherche(text,ui->liste);
    rdvWindow->recherche(text,ui->liste_att);

}


void MainWindow::tri(int index){

    rdvWindow->comboIndex_rdv(index,ui->liste);
    rdvWindow->comboIndex_rdv(index,ui->liste_att);



}
void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    Date = date;


    QFont font;
    font.setFamily("Arial");
    font.setPointSize(12);
    font.setBold(true);

    QStringList displayList;

    QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFile file(QDir(path).filePath("feries.txt"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" - ");
            if (parts.size() == 2) {
                QString englishName = parts[0];
                QString frenchName = holidayTranslation.value(englishName, englishName);
                QString dateStr = parts[1];
                QDate holidayDate = QDate::fromString(dateStr.trimmed(), Qt::ISODate);

                if (holidayDate == date) {

                    ui->feries->setFont(font);
                    ui->feries->setText("Jour férié : " + frenchName);
                    return;
                }
            }
        }

        file.close();
    } else {
        qWarning() << "Erreur lors de l'ouverture de feries.txt.";
    }

    if (appointmentTooltips.contains(date)) {
        QStringList patientList = appointmentTooltips[date];
        QList<QDateTime> timeList = appointmentTimes[date];

        int count = qMin(patientList.size(), timeList.size());
        ui->rendez_v->clear();
        for (int i = 0; i < count; ++i) {
            QString timeString = timeList.at(i).toString("hh:mm");
            QString rdv = patientList.at(i) + " - " + timeString;
            ui->rendez_v->setFont(font);
            ui->rendez_v->addItem(rdv);
        }
    } else {
        ui->rendez_v->clear();
        ui->rendez_v->setFont(font);
        ui->rendez_v->addItem("Aucun rendez-vous prévus ce jour.");
    }

}



////////////////////////////////////////

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched->objectName() == "qt_calendar_calendarview") {
        if (event->type() == QEvent::ToolTip) {
            QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
            QDate hoveredDate = ui->calendrier_rdv->selectedDate();

            if (appointmentTooltips.contains(hoveredDate)) {
                QStringList patientList = appointmentTooltips[hoveredDate];
                QString tooltipText = "Patients:\n" + patientList.join("\n");
                QToolTip::showText(helpEvent->globalPos(), tooltipText);
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

//////////////////////////////////////////notification



void MainWindow::showNotifications() {
    if (notificationWidget->isVisible()) {
        notificationWidget->hide();
    } else {
        notificationWidget->show();
    }
}


void MainWindow::ajoutNotification(const QString message) {

    QSoundEffect *soundEffect = new QSoundEffect(this);
    soundEffect->setSource(QUrl::fromLocalFile("C:/Users/MSI/Downloads/ding-101492.wav"));




    if (soundEffect->status() == QSoundEffect::Ready) {
        soundEffect->play();
    } else {
        qDebug() << "Error: Sound not ready. Status:" << soundEffect->status();
    }

    ui->bellN->show();
    notificationWidget->setStyleSheet(
        "background: rgb(70, 148, 156);"
        "color: white;"
        "text-align: left;"
        "border-radius: 8px;"
        "padding: 10px;"
        "QLabel {"
        "   min-width: 200px;"
        "   max-width: 250px;"
        "   word-wrap: break-word;"
        "}");
    QDateTime currentDateTime = QDateTime::currentDateTime();

    QSqlQuery query;
    query.prepare("INSERT INTO NOTIFICATIONS (DETAIL_NOTIF, DATE_NOTIF) "
                  "VALUES (:message, TO_TIMESTAMP(:date, 'YYYY-MM-DD HH24:MI:SS'))");
    query.bindValue(":message", message);
    query.bindValue(":date", currentDateTime.toString("yyyy-MM-dd hh:mm:ss"));

    if (!query.exec()) {
        qDebug() << "Failed query:" << query.lastQuery();
        qDebug() << "Error:" << query.lastError().text();
    }
    else {
        qDebug() << "Notification inserted successfully!";
    }


    notificationWidget->addNotification(message);
    notificationWidget->show();
    QTimer::singleShot(5000, notificationWidget, &QWidget::hide);
}



void MainWindow::on_actionStats_Fr_quentation_triggered()
{
    using namespace std;

    vector<int> tabCin;
    QSqlQuery query;
    query.prepare("SELECT CIN_RDV FROM RENDEZ_VOUS");
    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        return;
    }

    while (query.next()) {

        int cin = query.value(0).toInt();
        tabCin.push_back(cin);
    }


    unordered_map<int, int> cinCounts;
    for (int cin : tabCin) {
        cinCounts[cin]++;
    }


    int nv_patient = 0;
    int patient = 0;

    for (const auto& pair : cinCounts) {
        if (pair.second == 1) {
            nv_patient++;

        } else {
            nv_patient++;
            patient++;
        }
    }
    int totalPatients = nv_patient + patient;

    float pourcentageNouveaux = (static_cast<float>(nv_patient) / totalPatients) * 100;
    float pourcentageRecurrents = (static_cast<float>(patient) / totalPatients) * 100;

    qDebug() << "New patients (first visit):" << nv_patient;
    qDebug() << "Recurrent patients (returning):" << patient;

    vaccinTab->setCurrentIndex(19);




    QBarSet *patientSet = new QBarSet("Patients");
    *patientSet << pourcentageNouveaux << pourcentageRecurrents;


    QColor barColor("#8bc1c7");
    QColor textColor("#ffffff");


    patientSet->setColor(barColor);

    QBarSeries *series = new QBarSeries();
    series->append(patientSet);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Nouveaux patients vs Patients récurrents");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->setBackgroundBrush(Qt::transparent);
    chart->setTitleBrush(QBrush(textColor));
    QFont titleFont;
    titleFont.setFamily("Arial");
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleFont.setItalic(false);
    titleFont.setUnderline(true);

    chart->setTitleFont(titleFont);

    QStringList categories = { "Nouveaux", "Récurrents" };
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsBrush(QBrush(textColor));

    QFont axisFont;
    axisFont.setPointSize(10);
    axisFont.setFamily("Arial");
    axisFont.setBold(true);

    axisX->setLabelsFont(axisFont);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setLabelFormat("%d%%");
    axisY->setLabelsBrush(QBrush(textColor));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    chart->legend()->setVisible(true);
    chart->legend()->setLabelColor(textColor);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing, true);
    chartView->setStyleSheet("background: transparent; border: none;");


    if (ui->stat->layout()) {
        QLayoutItem *item;
        while ((item = ui->stat->layout()->takeAt(0))) {
            delete item->widget();
            delete item;
        }
        delete ui->stat->layout();
    }



    QObject::connect(series, &QBarSeries::hovered, [](bool state, int index, QBarSet* set) {
        if (state) {
            QString tooltipText = QString("%1: %2%").arg(set->label()).arg(set->at(index));
            QToolTip::showText(QCursor::pos(), tooltipText);
        } else {
            QToolTip::hideText();
        }
    });




    QVBoxLayout *layout = new QVBoxLayout(ui->stat);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
    ui->stat->setLayout(layout);
}




/////////////////liste d'attente





void MainWindow::on_drag_clicked()
{
    QListWidgetItem *selectedItem = ui->liste_att->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un élément de la liste.");
        return;
    }
    int idRdvl;
    if (selectedItem) {
        idRdvl = selectedItem->data(Qt::UserRole).toInt();}
    dateProp = rdvWindow->QDatefindBestAppointmentDate();

    AppointmentDialog dialog(dateProp, this);
    if (dialog.exec() == QDialog::Accepted) {
        QDateTime dateTime = dialog.selectedDateTime(dateProp);
        if (dateTime.isValid()) {
            qDebug()<<"date"<<dateTime;
            qDebug() << "ID du rendez-vous :" << idRdvl;

            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Confirmation",
                                          "Désirez-vous fixer la date " + dateTime.toString("dd/MM/yyyy hh:mm") + " comme rendez-vous ?",
                                          QMessageBox::Yes | QMessageBox::No);

            if (reply == QMessageBox::Yes) {
                QDate date = dateTime.date();
                appointmentTimes[date].append(dateTime);

                QSqlQuery pr;
                pr.prepare("UPDATE RENDEZ_VOUS SET DATE_RDV = :date, ATTENTE = 0 WHERE ID_RDV = :idRdv");
                pr.bindValue(":date", dateTime);
                pr.bindValue(":idRdv", idRdvl);
                if (!pr.exec()) {
                    QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour du rendez-vous : " + pr.lastError().text());
                } else {
                    QMessageBox::information(this, "Succès", "Le rendez-vous a été fixé avec succès.");
                }
            }
        } else {
            QMessageBox::warning(this, "Avertissement", "La date et l'heure sélectionnées ne sont pas valides.");
        }
    }
    on_rdv_clicked();
}





void MainWindow::on_actionEmploy_s_triggered()
{

    QSqlQuery query("SELECT PRENOM_E, NOM_E FROM EMPLOYEES");
    ui->tabPres->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tabPres->insertRow(row);
        ui->tabPres->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tabPres->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        row++;
    }


    vaccinTab->setCurrentIndex(18);

}


void MainWindow::on_actionQuitter_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmer", "Êtes-vous sûr de vouloir quitter l'application ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QApplication::quit();}
}



///////////////////////////////////////////////mariem
void MainWindow::onStatistiqueActionTriggered() {
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database not connected!");
        return;
    }

    vaccinTab->setCurrentIndex(17); // Switch to the statistics tab

    // Clear previous chart widgets from the layout
    QLayoutItem* child;
    while ((child = ui->chartLayout_3->takeAt(0))) { // Remove all items from the layout
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
    ui->chartLayout_3->addWidget(chartView);
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




LRESULT CALLBACK CustomAlertProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(63, 123, 134));
    static HFONT hFont = nullptr;

    switch (msg)
    {
    case WM_CREATE:
    {
        hFont = CreateFont(25, 0, 0, 0, FW_NORMAL,
                           FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                           OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");

        HWND hButton = CreateWindow(L"BUTTON", L"OK",
                                    WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                    150, 100, 80, 30,
                                    hWnd, (HMENU)1, NULL, NULL);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

        RECT rc;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
        int width = 600;
        int height = 200;
        SetWindowPos(hWnd, NULL,
                     rc.right - width - 10,
                     rc.bottom - height - 10,
                     width, height, SWP_NOZORDER);
        break;
    }

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(255, 255, 255));
        SetBkColor(hdcStatic, RGB(63, 123, 134));
        return (LRESULT)hBackgroundBrush;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, hBackgroundBrush);

        HICON hIcon = LoadIcon(NULL, IDI_WARNING);
        DrawIconEx(hdc, 20, 20, hIcon, 32, 32, 0, NULL, DI_NORMAL);

        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
        SetTextColor(hdc, RGB(255, 255, 255));
        SetBkMode(hdc, TRANSPARENT);

        RECT textRect = {70, 20, 1200, 160};
        wchar_t windowText[1024];
        GetWindowTextW(hWnd, windowText, 1024);
        DrawTextW(hdc, windowText, -1, &textRect, DT_LEFT | DT_WORDBREAK);

        SelectObject(hdc, hOldFont);
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) // OK button
        {
            DestroyWindow(hWnd);
        }
        break;

    case WM_DESTROY:
        DeleteObject(hBackgroundBrush);
        DeleteObject(hFont);
        g_alertShown = false; // Reset flag when closed
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}


void MainWindow::checkEquipmentQuantity()
{
    QSqlQuery query("SELECT REFERNCE_EQ, NOM_EQ, QUANTITE_EQ FROM EQUIPEMENTS WHERE QUANTITE_EQ < 5");
    QString alertText;

    while(query.next()) {
        alertText += tr("Quantité critique pour %1 (Réf: %2)\nQuantité restante: %3\n\n")
                         .arg(query.value(1).toString())
                         .arg(query.value(0).toString())
                         .arg(query.value(2).toInt());
    }

    if(!alertText.isEmpty() && !alertShown) {
        alertShown = true;

        // Play audio alert
        alertPlayer->stop();
        alertPlayer->setPosition(0);
        alertPlayer->play();

        // Register window class
        WNDCLASSW wc = {0};
        wc.lpfnWndProc = CustomAlertProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = L"CustomAlert";
        wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(63, 123, 134));
        RegisterClassW(&wc);

        // Create window
        HWND hWnd = CreateWindowW(L"CustomAlert", L"Alerte Stock",
                                  WS_POPUP | WS_CAPTION | WS_SYSMENU,
                                  0, 0, 400, 150,
                                  NULL, NULL, GetModuleHandle(NULL), NULL);

        // Set window text
        SetWindowTextW(hWnd, alertText.toStdWString().c_str());

        // Show window
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);

        // Message loop
        MSG msg;
        while(GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        alertShown = false;
    }

}


void MainWindow::startSpeechRecognition() {
    // Check if process is already running
    if (process->state() == QProcess::NotRunning) {
        qDebug() << "Starting speech recognition process...";

        // Start the Python script
        process->start("python", QStringList() << "C:/Users/MSI/OneDrive/Desktop/integration eqeui+event final!/speech_to_text.py");
    } else {
        qDebug() << "Process is already running.";
    }
}


//This function reads the output
void MainWindow::onProcessOutput() {
    QByteArray output = process->readAllStandardOutput();
    QString recognizedText = QString::fromUtf8(output);
    ui->recherche_eq->setText(recognizedText);  // Display recognized text in QLineEdit
}

void MainWindow::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    //This function is called automatically when the Python process finishes.
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

///////////elloumi
/*
void MainWindow::on_camButton_clicked()
{
    if (!cap.isOpened()) {
        if (!cap.open(0)) {
            qDebug() << "Failed to open camera!";
            return;
        }


        ui->camButton->setText("Stop");

        // Start YOLOv5 without blocking UI
        QString pythonPath = "C:/Users/marie/Downloads/Smart_Vax-main/Smart_Vax-main/SmartVax_VF/yolov5/yolov5env/Scripts/python.exe";
        QString scriptPath = "C:/Users/marie/Downloads/Smart_Vax-main/Smart_Vax-main/SmartVax_VF/yolov5/detect.py";
        //Starts the YOLOv5 script using QProcess::startDetached (non-blocking).
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

void MainWindow::processFrame()
{
    cv::Mat frame;
    cap >> frame;  // Capture frame

    if (frame.empty()) {
        qDebug() << "Empty frame captured!";
        return;
    }



    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
    ui->cameraFeed->setPixmap(QPixmap::fromImage(img));  // Display camera feed
}
*/


