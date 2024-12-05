#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Produit
{
    QString Nomp, Categorie, Marque, Quantite;
    int Idp, Prix;

public:
    Produit() {}
    Produit(QString, QString, QString, QString, int, int);

    QString getNomp() const { return Nomp; }
    QString getCategorie() const { return Categorie; }
    QString getMarque() const { return Marque; }
    QString getQuantite() const { return Quantite; }
    int getIdp() const { return Idp; }
    int getPrix() const { return Prix; }

    void setNomp(QString n) { Nomp = n; }
    void setCategorie(QString c) { Categorie = c; }
    void setMarque(QString m) { Marque = m; }
    void setQuantite(QString q) { Quantite = q; }
    void setIdp(int id) { this->Idp = id; }
    void setPrix(int p) { this->Prix = p; }
    QSqlQueryModel* rechercher(int id); // Add this in the Produit class declaration

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int Idp);
    bool modifier(int Idp);
    QSqlQueryModel* fetchAll();
    QSqlQueryModel* trierParPrix(bool ascending);
     void   genererStatistiquesMarqueQuantite(QSqlQueryModel* model);


};

#endif // PRODUIT_H
