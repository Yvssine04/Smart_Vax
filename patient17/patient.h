#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QDate>

class Patient {
public:
    Patient(int cinPass = 0, int numDossier = 0, const QString& nom = "", const QString& prenom = "",
            const QDate& dateNaissance = QDate(), const QString& tel = "", const QString& mail = "",
            int codePostal = 0, const QString& sexe = "", const QString& adresse = "",
            const QString& cnam = "");

    // Getters
    int cinPass() const { return m_cinPass; }
    int numDossier() const { return m_numDossier; }
    QString nom() const { return m_nom; }
    QString prenom() const { return m_prenom; }
    QDate dateNaissance() const { return m_dateNaissance; }
    QString tel() const { return m_tel; }
    QString mail() const { return m_mail; }
    int codePostal() const { return m_codePostal; }
    QString sexe() const { return m_sexe; }
    QString adresse() const { return m_adresse; }
    QString cnam() const { return m_cnam; }

    // Setters
    void setCinPass(int cinPass) { m_cinPass = cinPass; }
    void setNumDossier(int numDossier) { m_numDossier = numDossier; }
    void setNom(const QString& nom) { m_nom = nom; }
    void setPrenom(const QString& prenom) { m_prenom = prenom; }
    void setDateNaissance(const QDate& dateNaissance) { m_dateNaissance = dateNaissance; }
    void setTel(const QString& tel) { m_tel = tel; }
    void setMail(const QString& mail) { m_mail = mail; }
    void setCodePostal(int codePostal) { m_codePostal = codePostal; }
    void setSexe(const QString& sexe) { m_sexe = sexe; }
    void setAdresse(const QString& adresse) { m_adresse = adresse; }
    void setCnam(const QString& cnam) { m_cnam = cnam; }

private:
    int m_cinPass;
    int m_numDossier;
    QString m_nom;
    QString m_prenom;
    QDate m_dateNaissance;
    QString m_tel;
    QString m_mail;
    int m_codePostal;
    QString m_sexe;
    QString m_adresse;
    QString m_cnam;
};

#endif // PATIENT_H
