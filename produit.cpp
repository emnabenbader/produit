#include "produit.h"
#include "qsqlerror.h"
#include "connection.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
 #include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFont>
#include <QMap>
#include "StatistiquesGraphWidget.h"



// Debugging function to check database connection
bool checkDatabaseConnection() {
    QSqlDatabase db = QSqlDatabase::database(); // Get the default database connection
    if (!db.isOpen()) {
        if (!db.open()) {
            //qDebug() << "Failed to open database:" << db.lastError().text();
            //return false;
        }
    }
    return true;
}

// Définition du constructeur
Produit::Produit(QString Nomp, QString Categorie, QString Marque, QString Quantite, int Idp, int Prix) {
    this->Nomp = Nomp;
    this->Categorie = Categorie;
    this->Marque = Marque;
    this->Quantite = Quantite;
    this->Idp = Idp;
    this->Prix = Prix;
}
bool Produit::ajouter() {
    // Vérifier que le prix est supérieur à zéro
    if (Prix <= 0) {
        qDebug() << "Erreur : Le prix doit être supérieur à zéro.";
        return false;
    }

    // Vérifier que la quantité est un nombre valide et non négatif
    bool ok;
    int quantiteInt = Quantite.toInt(&ok); // Convertir la quantité en entier
    if (!ok || quantiteInt < 0) {
        qDebug() << "Erreur : La quantité doit être un nombre entier positif.";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO PRODUIT (NOMP, CATEGORIE, MARQUE, QUANTITE, IDP, PRIX) "
                  "VALUES (:Nomp, :Categorie, :Marque, :Quantite, :Idp, :Prix)");

    query.bindValue(":Nomp", Nomp);
    query.bindValue(":Categorie", Categorie);
    query.bindValue(":Marque", Marque);
    query.bindValue(":Quantite", Quantite);  // La quantité reste une chaîne dans la base de données
    query.bindValue(":Idp", Idp);
    query.bindValue(":Prix", Prix);

    if (!query.exec()) {
        qDebug() << "Erreur d'ajout de produit:" << query.lastError().text();
        return false;
    }
    return true;
}




bool Produit::supprimer(int Idp) {
    if (!checkDatabaseConnection()) return false;

    QSqlQuery query;
    query.prepare("DELETE FROM PRODUIT WHERE IDP = :Idp");
    query.bindValue(":Idp", Idp);

    if (!query.exec()) {
        qDebug() << "Error deleting product:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Produit::modifier(int Idp) {
    if (!checkDatabaseConnection()) return false;

    QSqlQuery query;
    query.prepare("UPDATE PRODUIT SET NOMP = :Nomp, CATEGORIE = :Categorie, MARQUE = :Marque, QUANTITE = :Quantite, PRIX = :Prix "
                  "WHERE IDP = :Idp");

    query.bindValue(":Nomp", Nomp);
    query.bindValue(":Categorie", Categorie);
    query.bindValue(":Marque", Marque);
    query.bindValue(":Quantite", Quantite);
    query.bindValue(":Prix", Prix);
    query.bindValue(":Idp", Idp);

    if (!query.exec()) {
        qDebug() << "Error updating product:" << query.lastError().text();
        return false;
    }
    return true;
}
QSqlQueryModel* Produit::rechercher(int id)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    // Prepare and execute query
    QSqlQuery query;
    query.prepare("SELECT * FROM PRODUIT WHERE IDP = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche de produit :" << query.lastError().text();
        delete model; // Clean up in case of error
        return nullptr;
    }

    model->setQuery(query); // Set the query results in the model
    return model;
}

QSqlQueryModel* Produit::trierParPrix(bool ascending)
{
    // Check if the database connection is open
    if (!checkDatabaseConnection()) return nullptr;

    // Create a model to store the sorted results
    QSqlQueryModel* model = new QSqlQueryModel();

    // Set up the sorting order
    QString order = ascending ? "ASC" : "DESC";

    // Construct the SQL query to sort by PRIX
    QString queryStr = "SELECT * FROM PRODUIT ORDER BY PRIX " + order;

    // Execute the query
    model->setQuery(queryStr);

    // Check for errors
    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors du tri par prix : " << model->lastError().text();
    }

    // Return the model containing the sorted results
    return model;
}



void Produit::genererStatistiquesMarqueQuantite(QSqlQueryModel* model) {
    // Vérification que le modèle a suffisamment de colonnes
    if (model->columnCount() > 4) {
        QMap<QString, int> quantiteParMarque;

        // Récupérer les données et accumuler les quantités par marque
        for (int row = 0; row < model->rowCount(); ++row) {
            QString marque = model->data(model->index(row, 2)).toString(); // Colonne 2 pour MARQUE
            int quantite = model->data(model->index(row, 3)).toInt();     // Colonne 3 pour QUANTITE

            if (!marque.isEmpty()) {
                quantiteParMarque[marque] += quantite; // Accumuler la quantité pour chaque marque
            }
        }

        // Calculer le total des quantités
        int totalQuantite = 0;
        for (auto it = quantiteParMarque.begin(); it != quantiteParMarque.end(); ++it) {
            totalQuantite += it.value();
        }

        // Créer le widget pour afficher le graphique
        StatistiquesGraphWidget *graphWidget = new StatistiquesGraphWidget(quantiteParMarque, totalQuantite);
        graphWidget->resize(700, 400); // Ajuster la taille du graphique
        graphWidget->show();
    }
}
QSqlQueryModel* Produit::fetchAll()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM produits"); // Replace "produits" with your table name
    return model;
}
