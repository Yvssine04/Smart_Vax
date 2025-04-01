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
                         double facturation);
    void loadAppointments(QListWidget *liste_att);
    void infoEdit(int idRDV, QLineEdit *CIN_rdv_2, QDateEdit *daterdv_2, QComboBox *vaccin_3, QLineEdit *adresse_2, QLineEdit *nom_rdv_2, QLineEdit *prenom_rdv_2, QDateTimeEdit *dispo_rdv_2, QComboBox *medecin_att_2, QComboBox *infirmier_att_2, QLineEdit *salle_att_2, QDoubleSpinBox *facturation_2);
    void supprimerRdv(int idRdv);
    int modifier_rdv(int idRdv, const QString &CINi, const QString &vaccin, const QString &date_rdvNaiss, const QString &adresse,
                     const QString &nom, const QString &prenom, const QString &dispo,
                     const QString &medecin, const QString &infirmier, const QString &salle,
                     double facturation );
    void generateCertificate(int id_rdv);

    NotificationWidget *notificationWidget;

    void addNotification();
    QVBoxLayout *mainLayout;
    void showNotifications();


private:

    bool rdv_existe(int ID_RDV);

};


struct SpecialDate {
    QDate date;
    QString text;
};

class CustomCalendar : public QCalendarWidget {
    Q_OBJECT

private:
    QVector<SpecialDate> specialDates;  // List of dates with text

public:
    explicit CustomCalendar(QWidget *parent = nullptr);

    void addEvent(QDate date, QString text);  // Add custom text

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const ;
};


#endif
