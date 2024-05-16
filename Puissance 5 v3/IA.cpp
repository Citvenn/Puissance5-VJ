#include "IA.h"

#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

IA::IA(int num, int profondeur) : Joueur(num), profondeur(profondeur), derniereColonne(-1) {
    srand(time(nullptr)); // Initialise le générateur de nombres aléatoires
}

int IA::comptageConsecutifs(const vector<int>& ligne, int joueur) const {
    int consecutifs = 0, maxConsecutifs = 0;
    for (int p : ligne) {
        if (p == joueur) {
            consecutifs++;
            maxConsecutifs = max(maxConsecutifs, consecutifs);
        } else {
            consecutifs = 0;
        }
    }
    return maxConsecutifs;
}

vector<int> IA::motifsGrille(const Plateau& plateau, pair<int, int> coup, int joueur) const {
    int a = coup.first, b = coup.second;
    vector<int> motifs;
    // Ligne
    vector<int> ligne = plateau.getLigne(a);
    motifs.push_back(comptageConsecutifs(ligne, joueur));
    // Colonne
    vector<int> colonne = plateau.getColonne(b);
    motifs.push_back(comptageConsecutifs(colonne, joueur));
    // Diagonale croissante
    vector<int> diagPos = plateau.getDiagPos(a, b);
    motifs.push_back(comptageConsecutifs(diagPos, joueur));
    // Diagonale décroissante
    vector<int> diagNeg = plateau.getDiagNeg(a, b);
    motifs.push_back(comptageConsecutifs(diagNeg, joueur));
    return motifs;
}

int IA::calculerScore(const vector<int>& motifs) const {
    vector<int> coefs = {0, 0, 1, 5, 10, 1000}; // Scores correspondants à 0, 1, 2, 3, 4, 5 alignements
    int score = 0;
    for (int n : motifs) {
        if (n >= coefs.size()) {
            score += coefs.back();
        } else {
            score += coefs[n];
        }
    }
    return score;
}

bool IA::coupGagnant(const Plateau& plateau, pair<int, int> coup, int joueur) const {
    vector<int> motifs = motifsGrille(plateau, coup, joueur);
    return any_of(motifs.begin(), motifs.end(), [](int n) { return n >= 5; });
}

int IA::scoreVictoire(const Plateau& plateau, pair<int, int> coup, int joueur) const {
    return coupGagnant(plateau, coup, joueur) ? 1000 : 0;
}

double IA::scoreCentre(const Plateau& plateau, pair<int, int> coup) const {
    int colonne = coup.second;
    int centre = plateau.getNbColonne() / 2;
    return 1.0 / (abs(colonne - centre) + 1);
}

int IA::scoreJoueur(const Plateau& plateau, pair<int, int> dernierCoup, int profondeur) const {
    int score = 0;
    vector<int> motifsJoueur = motifsGrille(plateau, dernierCoup, 1);
    score -= (profondeur + 1) * 10 * calculerScore(motifsJoueur);
    score -= (profondeur + 1) * scoreCentre(plateau, dernierCoup);
    score -= (profondeur + 1) * 50 * scoreVictoire(plateau, dernierCoup, 1);
    return score;
}

int IA::scoreIA(const Plateau& plateau, pair<int, int> dernierCoup, int profondeur) const {
    int score = 0;
    vector<int> motifsIA = motifsGrille(plateau, dernierCoup, 2);
    score += (profondeur + 1) * 8 * calculerScore(motifsIA);
    score += (profondeur + 1) * 2 * scoreCentre(plateau, dernierCoup);
    score += (profondeur + 1) * 50 * scoreVictoire(plateau, dernierCoup, 2);
    return score;
}

void IA::annulerCoup(Plateau& plateau, pair<int, int> coup) const {
    plateau.annulerPion(coup.first, coup.second);
}

vector<pair<int, int>> IA::coupsPossibles(const Plateau& plateau) const {
    vector<pair<int, int>> coups;
    for (int col = 0; col < plateau.getNbColonne(); ++col) {
        if (plateau.estEmplacementValide(col)) {
            int ligne = plateau.obtenirLigneLibre(col);
            coups.push_back({ligne, col});
        }
    }
    return coups;
}

// Méthode minimax avec élagage alpha-bêta
pair<int, pair<int, int>> IA::minimax(Plateau& plateau, bool maximisation, int profondeur, int alpha, int beta, pair<int, int> dernierCoup) const {
    // Cas de base : si le jeu est terminé ou si la profondeur maximale est atteinte
    if (dernierCoup != pair<int, int>(-1, -1)) {
        if (profondeur == 0) {
            // Évaluer le score pour le joueur ou l'IA en fonction de la profondeur
            int score = maximisation ? scoreJoueur(plateau, dernierCoup, profondeur) : scoreIA(plateau, dernierCoup, profondeur);
            return { score, dernierCoup };
        }
        if (coupGagnant(plateau, dernierCoup, maximisation ? 1 : 2)) {
            // Évaluer le score de victoire
            int score = maximisation ? scoreJoueur(plateau, dernierCoup, profondeur) : scoreIA(plateau, dernierCoup, profondeur);
            return { score, dernierCoup };
        }
    }

    // Initialiser la meilleure valeur et le meilleur coup en fonction de la maximisation ou minimisation
    int meilleureValeur = maximisation ? numeric_limits<int>::min() : numeric_limits<int>::max();
    pair<int, int> meilleurCoup = { -1, -1 };

    // Parcourir tous les coups possibles
    for (const auto& coup : coupsPossibles(plateau)) {
        int ligne = coup.first;
        int colonne = coup.second;
        // Placer le pion pour le joueur actuel
        plateau.placerPion(colonne, maximisation ? 2 : 1);
        // Appel récursif de minimax avec les paramètres inversés pour maximisation
        auto [valeur, _] = minimax(plateau, !maximisation, profondeur - 1, alpha, beta, { ligne, colonne });
        // Annuler le coup pour restaurer l'état précédent du plateau
        annulerCoup(plateau, { ligne, colonne });

        // Mise à jour de la meilleure valeur et du meilleur coup en fonction de la maximisation ou minimisation
        if (maximisation) {
            if (valeur > meilleureValeur) {
                meilleureValeur = valeur;
                meilleurCoup = { ligne, colonne };
            }
            alpha = max(alpha, valeur); // Mise à jour de alpha
        } else {
            if (valeur < meilleureValeur) {
                meilleureValeur = valeur;
                meilleurCoup = { ligne, colonne };
            }
            beta = min(beta, valeur); // Mise à jour de beta
        }

        // Élagage alpha-bêta : arrêter l'exploration si alpha >= beta
        if (alpha >= beta) {
            break;
        }
    }

    return { meilleureValeur, meilleurCoup };
}

void IA::choisirCoup(Plateau& plateau) const {
    auto [_, meilleurCoup] = minimax(plateau, true, profondeur, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), { -1, -1 });
    derniereColonne = meilleurCoup.second;
    if (meilleurCoup.second >= 0 && meilleurCoup.second < plateau.getNbColonne()) {
        plateau.placerPion(meilleurCoup.second, numero);
    } else {
        std::cerr << "Erreur: Coup invalide choisi par l'IA." << std::endl;
    }
}

int IA::obtenirDerniereColonne() const {
    return derniereColonne;
}

