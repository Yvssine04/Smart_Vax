#ifndef RENDEZ_VOUS_H
#define RENDEZ_VOUS_H

#include "qlistwidget.h"
#include <QObject>
#include <QSqlQuery>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>

class rendez_vous : public QObject {
    Q_OBJECT

public:
    explicit rendez_vous(QObject *parent = nullptr);
    virtual ~rendez_vous();

    void loadVaccins(QComboBox *comboBox);
    void saveAppointment(const QString &CIN, const QString &vaccin, const QString &date_rdvNaiss, const QString &adresse,
                                        const QString &nom, const QString &prenom, const QString &dispo,
                                        const QString &medecin, const QString &infirmier, const QString &salle,
                                        double facturation);
    void loadAppointments(QListWidget *liste_att);
    void infoEdit(int CIN, QLineEdit *CIN_rdv_2, QDateEdit *daterdv_2, QComboBox *vaccin_3, QLineEdit *adresse_2, QLineEdit *nom_rdv_2, QLineEdit *prenom_rdv_2, QDateTimeEdit *dispo_rdv_2, QLineEdit *medecin_att_2, QLineEdit *infirmier_att_2, QLineEdit *salle_att_2, QDoubleSpinBox *facturation_2);
    void supprimerRdv(int CIN);
    void modifier_rdv(int CINi ,const QString &CIN, const QString &vaccin,const QString &date_rdvNaiss, const QString &adresse,
                  const QString &nom, const QString &prenom, const QString &dispo,
                  const QString &medecin, const QString &infirmier, const QString &salle,
                      double facturation );

private:

    bool rdv_existe(int ID_RDV);

};
#endif
