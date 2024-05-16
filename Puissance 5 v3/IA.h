#ifndef IA_H
#define IA_H

#include "Plateau.h"
#include "Joueur.h"
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

class IA : public Joueur {
private:
    int profondeur;
    mutable int derniereColonne; // mutable pour permettre la modification dans une méthode const

    int comptageConsecutifs(const vector<int>& ligne, int joueur) const;
    vector<int> motifsGrille(const Plateau& plateau, pair<int, int> coup, int joueur) const;
    int calculerScore(const vector<int>& motifs) const;
    bool coupGagnant(const Plateau& plateau, pair<int, int> coup, int joueur) const;
    int scoreVictoire(const Plateau& plateau, pair<int, int> coup, int joueur) const;
    double scoreCentre(const Plateau& plateau, pair<int, int> coup) const;
    int scoreJoueur(const Plateau& plateau, pair<int, int> dernierCoup, int profondeur) const;
    int scoreIA(const Plateau& plateau, pair<int, int> dernierCoup, int profondeur) const;
    void annulerCoup(Plateau& plateau, pair<int, int> coup) const;
    pair<int, pair<int, int>> minimax(Plateau& plateau, bool maximisation, int profondeur, int alpha, int beta, std::pair<int, int> dernierCoup) const;

public:
    IA(int num, int profondeur);
    void choisirCoup(Plateau& plateau) const override;
    int obtenirDerniereColonne() const override;
    vector<pair<int, int>> coupsPossibles(const Plateau& plateau) const; // Nouvelle méthode parce que la vérification est pas exactement la même que sur python
};

#endif // IA_H
