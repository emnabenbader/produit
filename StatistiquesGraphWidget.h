// StatistiquesGraphWidget.h
#ifndef STATISTIQUESGRAPHWIDGET_H
#define STATISTIQUESGRAPHWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPainter>
#include <QVector>
#include <QString>

class StatistiquesGraphWidget : public QWidget {
    Q_OBJECT

public:
    StatistiquesGraphWidget(QMap<QString, int> quantiteParMarque, int totalQuantite, QWidget *parent = nullptr)
        : QWidget(parent), m_quantiteParMarque(quantiteParMarque), m_totalQuantite(totalQuantite) {}

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        int width = this->width();
        int height = this->height();

        // Réduire la taille du cercle pour faire de la place aux légendes
        int margin = 60;  // Espace autour du graphique
        QRectF pieRect(margin, margin, width / 2 - 2 * margin, height - 2 * margin);

        // Palette de couleurs (nuances de bleu avec des variations)
        QVector<QColor> colorPalette = {
            QColor(70, 130, 180), QColor(100, 149, 237), QColor(176, 196, 222),
            QColor(30, 144, 255), QColor(0, 191, 255), QColor(135, 206, 250)
        };

        int startAngle = 0; // Angle de départ pour le premier secteur
        int colorIndex = 0; // Index des couleurs dans la palette

        // Dessiner les secteurs du graphique circulaire
        for (auto it = m_quantiteParMarque.begin(); it != m_quantiteParMarque.end(); ++it) {
            int spanAngle = (360 * it.value()) / m_totalQuantite; // Calcul de l'angle pour chaque secteur
            painter.setBrush(colorPalette[colorIndex % colorPalette.size()]); // Utilisation de la couleur
            painter.drawPie(pieRect, startAngle * 16, spanAngle * 16);
            startAngle += spanAngle; // Mettre à jour l'angle de départ pour le secteur suivant
            colorIndex++;
        }

        // Afficher les légendes à droite du graphique
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 10, QFont::Bold)); // Taille de police ajustée pour plus de lisibilité
        startAngle = 0;
        colorIndex = 0;

        // Positionnement des légendes
        int legendX = width / 2 + margin + 80; // X de départ des légendes, tiré plus à droite
        int legendY = height / 2 - (25 * m_quantiteParMarque.size()) / 2; // Centrer verticalement les légendes
        int lineHeight = 25; // Hauteur entre chaque ligne de légende

        // Dessiner les légendes avec carré coloré à gauche et texte à droite
        for (auto it = m_quantiteParMarque.begin(); it != m_quantiteParMarque.end(); ++it) {
            int spanAngle = (360 * it.value()) / m_totalQuantite;

            // Dessiner un petit carré de couleur avant le texte
            int squareSize = 12; // Taille du petit carré
            int squareX = legendX; // La position du carré
            int squareY = legendY + lineHeight / 2 - squareSize / 2; // Centrer le carré verticalement sur la ligne

            painter.setBrush(colorPalette[colorIndex % colorPalette.size()]); // Appliquer la couleur du secteur
            painter.drawRect(squareX, squareY, squareSize, squareSize); // Dessiner le carré

            // Afficher le texte : nom de la marque et pourcentage
            QString text = QString("%1 (%2%)")
                               .arg(it.key())
                               .arg(QString::number((it.value() * 100.0) / m_totalQuantite, 'f', 1));

            // Calculer la position du texte à côté du carré
            int textX = squareX + squareSize + 5; // Décalage pour placer le texte à droite du carré
            int textY = legendY + lineHeight / 2 + painter.fontMetrics().height() / 3; // Ajuster la position verticale du texte

            painter.drawText(textX, textY, text); // Afficher le texte

            // Mettre à jour la position pour la ligne suivante
            legendY += lineHeight; // Déplacer verticalement pour la prochaine ligne de légende

            startAngle += spanAngle; // Mettre à jour pour le prochain secteur
            colorIndex++;
        }
    }

private:
    QMap<QString, int> m_quantiteParMarque;
    int m_totalQuantite;
};

#endif // STATISTIQUESGRAPHWIDGET_H
