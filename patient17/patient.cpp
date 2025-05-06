#include "patient.h"

Patient::Patient(int cinPass, int numDossier, const QString& nom, const QString& prenom,
                 const QDate& dateNaissance, const QString& tel, const QString& mail,
                 int codePostal, const QString& sexe, const QString& adresse,
                 const QString& cnam)
    : m_cinPass(cinPass), m_numDossier(numDossier), m_nom(nom), m_prenom(prenom),
    m_dateNaissance(dateNaissance), m_tel(tel), m_mail(mail),
    m_codePostal(codePostal), m_sexe(sexe), m_adresse(adresse), m_cnam(cnam) {}
