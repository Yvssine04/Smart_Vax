#ifndef VACCIN_H
#define VACCIN_H

#include <QObject>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMessageBox>
#include <QLineEdit>
#include <QDateEdit>

class Vaccin : public QObject
{
    Q_OBJECT

public:
    Vaccin(QObject *parent = nullptr);
    void loadVaccinData(QTableWidget *tabvaccin);
    void saveVaccinData(int reference, QString nom, QString type, int age_min, QString mode_admin, QString dose, QDate date_exp, double prix, int quantite);
    void deleteVaccin(int reference);
    void fetchVaccinData(int reference, QLineEdit *referenceEdit, QLineEdit *nomEdit, QLineEdit *typeEdit, QLineEdit *ageEdit, QLineEdit *modeEdit, QLineEdit *doseEdit, QDateEdit *dateEdit, QLineEdit *prixEdit, QLineEdit *quantiteEdit);

private:
    bool isReferenceExists(int reference);
};

#endif // VACCIN_H
