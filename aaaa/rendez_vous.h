#ifndef RENDEZ_VOUS_H
#define RENDEZ_VOUS_H

#include "qlistwidget.h"
#include <QObject>
#include <QSqlQuery>
#include <QComboBox>
#include <QLineEdit>

class rendez_vous : public QObject {
    Q_OBJECT

public:
    explicit rendez_vous(QObject *parent = nullptr);
    virtual ~rendez_vous();

    void loadVaccins(QComboBox *comboBox);
    void saveAppointment(const QString &CIN, const QString &vaccin,const QString &date_rdv, const QString &adresse,
                         const QString &nom, const QString &prenom, const QString &dispo,
                         const QString &medecin, const QString &infirmier, const QString &salle,
                         double facturation );
    void loadAppointments(QListWidget *liste_att);
    void supprimerRdv(int CIN);

private:

};
#endif
