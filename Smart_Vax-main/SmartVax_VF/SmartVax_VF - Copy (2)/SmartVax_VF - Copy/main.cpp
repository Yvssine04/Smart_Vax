#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include "connection.h"
#include <QSqlDatabase>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    // Configuration de la traduction (si nécessaire)
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "projet_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.show();
    bool test = c.createconnect();
    if (test)
    {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("Base de données ouverte"),
                                 QObject::tr("Connexion réussie.\n"
                                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Base de données non ouverte"),
                              QObject::tr("Échec de la connexion.\n"
                                          "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);

    return a.exec();
}
