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
    bool isReferenceExists(int reference);
    void updateVaccinData(int reference, QString nom, QString type, int age_min, QString mode_admin, QString dose, QDate date_exp, double prix, int quantite);
    void filterVaccinTable(QTableWidget *table, const QString &searchText);
    void sortVaccinTable(QTableWidget *tablevaccin, int column, Qt::SortOrder order = Qt::AscendingOrder);
    void exporterVaccinPDF(QTableWidget *tabvaccin);

private:
    void insertVaccinData(int reference, QString nom, QString type, int age_min, QString mode_admin, QString dose, QDate date_exp, double prix, int quantite);
};

#endif // VACCIN_H
