#ifndef MEDECIN_H
#define MEDECIN_H
#include <QObject>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMessageBox>
class medecin:public QObject{
    Q_OBJECT

public:
    medecin();
    medecin(QObject *parent = nullptr);

    void loadMedData(QTableWidget *tabMed);

    bool saveMedData(QString id_e, const QString &prenom_e, const QString &nom_e,
                     const QString &annee_exp_e, const QString &specialite_e,
                     const QString &num_tel_e, const QString &mail_e, const QString sexe_e,
                     const QString &adresse_e, const QString &mdp_e);


    bool deleteMed(QString id_med);

    bool updateMed(QString id_e, const QString &prenom_e, const QString &nom_e,
                   const QString &annee_exp_e, const QString &specialite_e,
                   const QString &num_tel_e, const QString &mail_e, const QString sexe_e,
                   const QString &adresse_e, const QString &mdp_e);

    void searchMedData(QTableWidget *tabMed, const QString &searchText);
    void exporterCertificatTravailPDF(QTableWidget *tabEmployees);
    void TriMedData(QTableWidget *tabMed, int column);


    void loadInfData(QTableWidget *tabInf);

};








#endif // MEDECIN_H
