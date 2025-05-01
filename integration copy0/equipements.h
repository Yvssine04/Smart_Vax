#ifndef EQUIPEMENTS_H
#define EQUIPEMENTS_H

#include <QObject>
#include <QSqlQuery>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateTimeEdit>
#include <QString>
#include <QSqlQueryModel>
#include <QQuickWidget>
#include <QMap>
QMap<QString, int> calculerStatistiques();
class Equipements {
private:
    QString reference;
    QString nom;
    QString type;
    QString date_limite;
    QString fournisseur;
    int quantite;
    double prix;
    QSqlQueryModel *model;


public:
    // Constructeurs
    Equipements();
    Equipements(QString ref, QString n, QString t, QString dl, QString f, int q, double p)
        : reference(ref), nom(n), type(t), date_limite(dl), fournisseur(f), quantite(q), prix(p) {}

    // Getters
    QString getReference() const { return reference; }
    QString getNom() const { return nom; }
    QString getType() const { return type; }
    QString getDateLimite() const { return date_limite; }
    QString getFournisseur() const { return fournisseur; }
    int getQuantite() const { return quantite; }
    double getPrix() const { return prix; }

    // Setters
    void setReference(QString ref) { reference = ref; }
    void setNom(QString n) { nom = n; }
    void setType(QString t) { type = t; }
    void setDateLimite(QString dl) { date_limite = dl; }
    void setFournisseur(QString f) { fournisseur = f; }
    void setQuantite(int q) { quantite = q; }
    void setPrix(double p) { prix = p; }

    // Fonctions CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimerEquipement(QString reference);
    QSqlQueryModel* rechercherEquipement(const QString& reference);
    QSqlQueryModel* afficherEtTrierParPrix();
    bool modifier();
 void onStatistiqueActionTriggered();
    void sortEquipementTable(QTableWidget *tableEqui, int column); // Ensure this line is correct
 QMap<QString, int> calculerStatistiques();
};

#endif // EQUIPEMENTS_H
