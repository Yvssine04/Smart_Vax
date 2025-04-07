#ifndef RENDEZ_VOUS_H
#define RENDEZ_VOUS_H

#include "qcalendarwidget.h"
#include "qlistwidget.h"
#include <QObject>
#include <QSqlQuery>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QFileDialog>
#include <QStandardPaths>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

class NotificationWidget : public QWidget {
    Q_OBJECT

public:
    NotificationWidget(QWidget *parent = nullptr);
    void addNotification(const QString &message);
    QVBoxLayout *layout;
    void addWidgetToLayout(QWidget *widget);
signals:
    void notificationClicked(const QString &message);

private:

    QMap<QString, QString> notifications;


};

class rendez_vous : public QObject {
    Q_OBJECT

public:
    explicit rendez_vous(QObject *parent = nullptr);
    virtual ~rendez_vous();

    void loadVaccins(QComboBox *comboBox, QComboBox *BoxMed, QComboBox *BoxInf);
    int saveAppointment(const QString &CIN, const QString &vaccin, const QString &date_rdvNaiss, const QString &adresse,
                         const QString &nom, const QString &prenom, const QString &dispo,
                         const QString &medecin, const QString &infirmier, const QString &salle,
                         double facturation, const QString &SelectedDays, const QString &periode);
    void loadAppointments(QListWidget *liste_att);
    void loadAppointmentsL(QListWidget *liste);
    void infoEdit(int idRDV, QLineEdit *CIN_rdv_2, QDateEdit *daterdv_2, QComboBox *vaccin_3, QLineEdit *adresse_2, QLineEdit *nom_rdv_2, QLineEdit *prenom_rdv_2, QDateTimeEdit *dispo_rdv_2, QComboBox *medecin_att_2, QComboBox *infirmier_att_2, QLineEdit *salle_att_2, QDoubleSpinBox *facturation_2);
    void supprimerRdv(int idRdv);
    int modifier_rdv(int idRdv, const QString &CINi, const QString &vaccin, const QString &date_rdvNaiss, const QString &adresse,
                     const QString &nom, const QString &prenom, const QString &dispo,
                     const QString &medecin, const QString &infirmier, const QString &salle,
                     double facturation );
    void generateCertificate(int id_rdv);
    void recherche(const QString &text,QListWidget *liste);
    void comboIndex_rdv(int index,QListWidget *liste);
    NotificationWidget *notificationWidget;
    QList<QPair<QString, QString>> tunisianHolidays;
    void fetchTunisianHolidays(const QString& year);
    void addNotification();
    QVBoxLayout *mainLayout;
    void showNotifications();
    void loadHolidaysFromFile(QCalendarWidget *calendrier);


private:

    bool rdv_existe(int ID_RDV);

};



#endif
