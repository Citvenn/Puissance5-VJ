#ifndef PLATEAU_H
#define PLATEAU_H

#include <iostream>
#include <vector>

using namespace std;

class Plateau {
private:
    std::vector<vector<int>> grille;
    int nbLignes;
    int nbColonnes;
    int longueurGagnante;

public:
    Plateau(int lignes = 6, int colonnes = 12, int longueur = 5); // Dimensions par défaut
    void afficher() const;
    bool estEmplacementValide(int colonne) const;
    int obtenirLigneLibre(int colonne) const;
    void placerPion(int colonne, int joueur);
    void annulerPion(int ligne, int colonne); // Nouvelle méthode pour annuler un coup
    bool verifierVictoire(int ligne, int colonne, int joueur) const;
    bool estPlein() const;
    
    int getNbColonne() const { return nbColonnes; }
    vector<int> getLigne(int ligne) const;
    vector<int> getColonne(int colonne) const;
    vector<int> getDiagPos(int ligne, int colonne) const;
    vector<int> getDiagNeg(int ligne, int colonne) const;
};

#endif // PLATEAU_H
