#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "connection.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Translation setup (if needed)
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "projet_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // Create and initialize the connection
    Connection c;
    bool test = c.createconnect();

    if (test) {
        // If connection is successful
        QMessageBox::information(nullptr, QObject::tr("Database Connection"),
                                 QObject::tr("Connection to the database was successful."),
                                 QMessageBox::Ok);
    } else {
        // If connection fails
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Failed to connect to the database."),
                              QMessageBox::Cancel);
        return -1; // Exit if connection fails
    }

    // Show the main window if connected
    MainWindow w;
    w.show();

    return a.exec();
}
