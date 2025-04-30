#ifndef VACCIN_H
#define VACCIN_H

#include <QObject>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMessageBox>

class Vaccin : public QObject
{
    Q_OBJECT

public:
    Vaccin(QObject *parent = nullptr);
    void loadVaccinData(QTableWidget *tabvaccin);
    void saveVaccinData(int reference, QString nom, QString type, int age_min, QString mode_admin, QString dose, QDate date_exp, double prix, int quantite);
};

#endif // VACCIN_H
