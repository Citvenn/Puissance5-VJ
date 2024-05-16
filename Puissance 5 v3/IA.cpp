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
    srand(time(nullptr)); 
}
//Lié au constructeur: initialise l'ia avec le num (jeton de jeu) et la profondeur du minimax

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
//partie 1: permet de compter le nombre de jetons consécutifs dans un motif pour le traiter par la suite

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
// partie 2: identifie les motifs (alignement de plusieurs jetons) pour calculer les csores associés

int IA::calculerScore(const vector<int>& motifs) const {
    vector<int> coefs = {0, 0, 1, 5, 10, 1000}; // Scores correspondants à 0, 1, 2, 3, 4, 5 jetons alignés
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
// identifie les motifs de 5 jetons gagnants, car leur score est particulier et doit avoir un poids plus élevé

int IA::scoreVictoire(const Plateau& plateau, pair<int, int> coup, int joueur) const {
    return coupGagnant(plateau, coup, joueur) ? 1000 : 0;
}
// associe une valeur à la victoire pour le calcul de score du minimax

double IA::scoreCentre(const Plateau& plateau, pair<int, int> coup) const {
    int colonne = coup.second;
    int centre = plateau.getNbColonne() / 2;
    return 1.0 / (abs(colonne - centre) + 1);
}
// méthode pour donner une valeur aux coups joués proches du centre (positions statégiques)

int IA::scoreJoueur(const Plateau& plateau, pair<int, int> dernierCoup, int profondeur) const {
    int score = 0;
    vector<int> motifsJoueur = motifsGrille(plateau, dernierCoup, 1);
    score -= (profondeur + 1) * 10 * calculerScore(motifsJoueur);
    score -= (profondeur + 1) * scoreCentre(plateau, dernierCoup);
    score -= (profondeur + 1) * 50 * scoreVictoire(plateau, dernierCoup, 1);
    return score;
}
// mise en place d'un calcul de score de joueur pour le minimax

int IA::scoreIA(const Plateau& plateau, pair<int, int> dernierCoup, int profondeur) const {
    int score = 0;
    vector<int> motifsIA = motifsGrille(plateau, dernierCoup, 2);
    score += (profondeur + 1) * 10 * calculerScore(motifsIA);
    score += (profondeur + 1) * 5 * scoreCentre(plateau, dernierCoup);
    score += (profondeur + 1) * 55 * scoreVictoire(plateau, dernierCoup, 2);
    return score;
}
// mise en place d'un calcul de score de l'ia pour le minimax

void IA::annulerCoup(Plateau& plateau, pair<int, int> coup) const {
    plateau.annulerPion(coup.first, coup.second);
}
// méthode pour le fonctionnement du minimax: lors du minimax les grilles possibles dans {profondeur} coups sont envisagés et générées en interne pour le calcul de score
// ces grilles sont crées pour permettre le calcul de score et doivent être détruites coup par coup ensuite

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
// méthode de génération des coups dans le minimax 

// Méthode minimax avec élagage alpha-bêta (cf cours DIA s5 flemme de réexpliquer)
pair<int, pair<int, int>> IA::minimax(Plateau& plateau, bool maximisation, int profondeur, int alpha, int beta, pair<int, int> dernierCoup) const {
    // cas de base : si le jeu est terminé ou si la profondeur maximale est atteinte, nécessaire acr minimax est récursif
    if (dernierCoup != pair<int, int>(-1, -1)) {
        if (profondeur == 0) {
            // évaluation du score pour le joueur ou l'IA en fonction de la profondeur
            int score = maximisation ? scoreJoueur(plateau, dernierCoup, profondeur) : scoreIA(plateau, dernierCoup, profondeur);
            return { score, dernierCoup };
        }
        if (coupGagnant(plateau, dernierCoup, maximisation ? 1 : 2)) {
            // évaluation du score de victoire
            int score = maximisation ? scoreJoueur(plateau, dernierCoup, profondeur) : scoreIA(plateau, dernierCoup, profondeur);
            return { score, dernierCoup };
        }
    }
    // initialisation de la meilleure valeur et du meilleur coup en fonction de la maximisation ou minimisation de chaque branche du minimax
    int meilleureValeur = maximisation ? numeric_limits<int>::min() : numeric_limits<int>::max();
    pair<int, int> meilleurCoup = { -1, -1 };

    // parcours de tous les coups possibles
    for (const auto& coup : coupsPossibles(plateau)) {
        int ligne = coup.first;
        int colonne = coup.second;
        // placeme,t de pion pour le joueur actuel
        plateau.placerPion(colonne, maximisation ? 2 : 1);
        // appel récursif de minimax avec les paramètres inversés pour maximisation
        auto [valeur, _] = minimax(plateau, !maximisation, profondeur - 1, alpha, beta, { ligne, colonne });
        // annulation du coup pour restaurer l'état précédent du plateau lors du parcours inverse de l'arbre du minimax
        annulerCoup(plateau, { ligne, colonne });

        // maj de la meilleure valeur et du meilleur coup en fonction de la maximisation ou minimisation
        if (maximisation) {
            if (valeur > meilleureValeur) {
                meilleureValeur = valeur;
                meilleurCoup = { ligne, colonne };
            }
            alpha = max(alpha, valeur); // maj de alpha
        } else {
            if (valeur < meilleureValeur) {
                meilleureValeur = valeur;
                meilleurCoup = { ligne, colonne };
            }
            beta = min(beta, valeur); // maj de beta
        }

        // élagage alpha-bêta : arrêter l'exploration si alpha >= beta
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
//méthode héritée de la classe joueur qui joue un coup en fct du resultat de miniamx

int IA::obtenirDerniereColonne() const {
    return derniereColonne;
}
//méthode héritée de a classe joueur qui aide à l'affichage du dernier coup joué vu que la grille est dure à lire à l'affichage

