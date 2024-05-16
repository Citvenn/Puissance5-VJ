#include "Plateau.h"

#include <iostream>
#include <iomanip> // for std::setw

using namespace std;

Plateau::Plateau(int lignes, int colonnes, int longueur) : nbLignes(lignes), nbColonnes(colonnes), longueurGagnante(longueur) {
    grille = vector<vector<int>>(nbLignes, vector<int>(nbColonnes, 0));
}

void Plateau::afficher() const {
    // Affiche les numéros de colonnes
    for (int col = 0; col < nbColonnes; ++col) {
        cout << setw(2) << col << " ";
    }
    cout << "\n"<< endl;

    // Affiche la grille
    for (const auto& ligne : grille) {
        for (int cellule : ligne) {
            cout << setw(2) << cellule << " ";
        }
        cout << endl;
    }
    cout << endl;
}
//méthode d'affichage de la grille avec una ffichage console

bool Plateau::estEmplacementValide(int colonne) const {
    return grille[0][colonne] == 0;
}
// vérifie qu'une case est vide (aka que la valeir de la case vaut 0 et pas 1 ou 2 de valeur de jeton)

int Plateau::obtenirLigneLibre(int colonne) const {
    for (int ligne = nbLignes - 1; ligne >= 0; --ligne) {
        if (grille[ligne][colonne] == 0) {
            return ligne;
        }
    }
    return -1;
}
//permet  la selection de ligne pour jouer

void Plateau::placerPion(int colonne, int joueur) {
    int ligne = obtenirLigneLibre(colonne);
    if (ligne != -1) {
        grille[ligne][colonne] = joueur;
    }
}
// modifie la valeur d'une case en celle du jeton de joueur (1 ou 2)

void Plateau::annulerPion(int ligne, int colonne) {
    if (ligne >= 0 && ligne < nbLignes && colonne >= 0 && colonne < nbColonnes) {
        grille[ligne][colonne] = 0;
    }
}

bool Plateau::verifierVictoire(int ligne, int colonne, int joueur) const {
    // Vérifie les lignes
    for (int row = 0; row < nbLignes; ++row) {
        for (int col = 0; col <= nbColonnes - longueurGagnante; ++col) {
            bool gagnant = true;
            for (int k = 0; k < longueurGagnante; ++k) {
                if (grille[row][col + k] != joueur) {
                    gagnant = false;
                    break;
                }
            }
            if (gagnant) return true;
        }
    }
    // Vérifie les colonnes
    for (int col = 0; col < nbColonnes; ++col) {
        for (int row = 0; row <= nbLignes - longueurGagnante; ++row) {
            bool gagnant = true;
            for (int k = 0; k < longueurGagnante; ++k) {
                if (grille[row + k][col] != joueur) {
                    gagnant = false;
                    break;
                }
            }
            if (gagnant) return true;
        }
    }
    // Vérifie les diagonales ascendantes (ligne + et colonne +)
    for (int row = 0; row <= nbLignes - longueurGagnante; ++row) {
        for (int col = 0; col <= nbColonnes - longueurGagnante; ++col) {
            bool gagnant = true;
            for (int k = 0; k < longueurGagnante; ++k) {
                if (grille[row + k][col + k] != joueur) {
                    gagnant = false;
                    break;
                }
            }
            if (gagnant) return true;
        }
    }
    // Vérifie les diagonales descendantes (lignes + et colonnes -)
    for (int row = 0; row <= nbLignes - longueurGagnante; ++row) {
        for (int col = longueurGagnante - 1; col < nbColonnes; ++col) {
            bool gagnant = true;
            for (int k = 0; k < longueurGagnante; ++k) {
                if (grille[row + k][col - k] != joueur) {
                    gagnant = false;
                    break;
                }
            }
            if (gagnant) return true;
        }
    }

    return false;
}
// méthode de vérification de victoire

bool Plateau::estPlein() const {
    for (int i = 0; i < nbColonnes; ++i) {
        if (estEmplacementValide(i)) {
            return false;
        }
    }
    return true;
}
// méthode de vérif de remplissage, utile en cas de match nul

//getters
vector<int> Plateau::getLigne(int ligne) const {
    if (ligne >= 0 && ligne < nbLignes) {
        return grille[ligne];
    }
    return {};
}

vector<int> Plateau::getColonne(int colonne) const {
    if (colonne >= 0 && colonne < nbColonnes) {
        vector<int> col;
        for (int i = 0; i < nbLignes; ++i) {
            col.push_back(grille[i][colonne]);
        }
        return col;
    }
    return {};
}

vector<int> Plateau::getDiagPos(int ligne, int colonne) const {
    vector<int> diag;
    int startRow = max(ligne - colonne, 0);
    int startCol = max(colonne - ligne, 0);
    while (startRow < nbLignes && startCol < nbColonnes) {
        diag.push_back(grille[startRow][startCol]);
        startRow++;
        startCol++;
    }
    return diag;
}

vector<int> Plateau::getDiagNeg(int ligne, int colonne) const {
    vector<int> diag;
    int startRow = min(ligne + colonne, nbLignes - 1);
    int startCol = max(colonne - (nbLignes - 1 - ligne), 0);
    while (startRow >= 0 && startCol < nbColonnes) {
        diag.push_back(grille[startRow][startCol]);
        startRow--;
        startCol++;
    }
    return diag;
}

