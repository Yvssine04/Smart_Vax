#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QKeyEvent>
#include <QCryptographicHash> // For password hashing


loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::loginWindow), mainWindow(nullptr)
{
    ui->setupUi(this);

    // Initialize database connection
    if (!conn.createconnect()) {
        QMessageBox::critical(this, "Error", "Failed to connect to database!");
    }

    faceProcess = new QProcess(this);
    connect(faceProcess, &QProcess::readyReadStandardOutput, [=]() {
        QString result = QString::fromUtf8(faceProcess->readAllStandardOutput()).trimmed();
        qDebug() << "Face ID result:" << result;


        if (result != "NO_MATCH" && result != "NO_FACE") {
            ui->lineEdit_id->setText(result); // Fill ID field
        } else {
            QMessageBox::warning(this, "Face Recognition", "Face not recognized.");
        }
    });


    // Window styling
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    ui->lineEdit_id->setFocus();

    // Tab order
    setTabOrder(ui->lineEdit_id, ui->lineEdit_mdp);
    setTabOrder(ui->lineEdit_mdp, ui->pushButton_conn);

    // Signals
    connect(ui->lineEdit_mdp, &QLineEdit::returnPressed,
            ui->pushButton_conn, &QPushButton::click);
}

loginWindow::~loginWindow()
{
    delete ui;
}

void loginWindow::on_pushButton_conn_clicked()
{
    QString id = ui->lineEdit_id->text().trimmed();
    QString password = ui->lineEdit_mdp->text().trimmed();

    // Validate inputs
    if (id.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter both ID and password");
        return;
    }

    // Check database connection
    if (!QSqlDatabase::database().isOpen()) {
        QMessageBox::critical(this, "Error", "Database connection lost!");
        return;
    }

    // Query database (assuming plaintext passwords)
    QSqlQuery query;
    query.prepare("SELECT ID, MDP_E FROM EMPLOYEES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error",
                              "Query failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value("MDP_E").toString().trimmed();

        // Compare plaintext passwords (temporarily for testing)
        if (password == storedPassword) {
            // Login successful
            mainWindow = new MainWindow(id); // send the ID to MainWindow
            mainWindow->show();
            this->close();
        }
         else {
            QMessageBox::warning(this, "Login Failed",
                                 "Incorrect password");
            qDebug() << "Entered:" << password << "Stored:" << storedPassword;
        }
    } else {
        QMessageBox::warning(this, "Login Failed",
                             "Employee ID does not exist");
    }
}
void loginWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void loginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragStartPosition);
        event->accept();
    }
}

void loginWindow::on_pushButton_close_clicked()
{
    qApp->quit(); // Close the entire application
}

void loginWindow::on_pushButton_Face_clicked() {
    qDebug() << "Face recognition button clicked";

    if (faceProcess->state() == QProcess::NotRunning) {
        qDebug() << "Starting Python face recognition script...";

        // Clear any previous output
        faceProcess->setProcessChannelMode(QProcess::MergedChannels);

        // Start the process
        QString pythonPath = "C:/Users/ahmed/AppData/Local/Programs/Python/Python39/python.exe";
        QString scriptPath = "C:/PythonSmartVax/face_recognition_login.py";

        faceProcess->start(pythonPath, QStringList() << scriptPath);

        // Wait for process to start (with timeout)
        if (!faceProcess->waitForStarted(5000)) {
            QMessageBox::warning(this, "Error", "Failed to start face recognition process");
            return;
        }
    } else {
        qDebug() << "Face process already running";
    }
}
