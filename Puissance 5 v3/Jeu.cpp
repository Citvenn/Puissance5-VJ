#include "Jeu.h"
#include "IA.h"
#include "Joueur.h"
#include "JoueurHumain.h"

#include <iostream>
#include <limits>

using namespace std;

Jeu::Jeu() : joueur1(nullptr), joueur2(nullptr), tour(1) {
    choisirTailleGrille();
    choisirModeDeJeu();
}

Jeu::~Jeu() {
    delete joueur1;
    delete joueur2;
}

void Jeu::choisirTailleGrille() {
    int lignes, colonnes;
    const int longueurGagnante = 5; // Longueur gagnante fixée à 5 pour p5
    bool saisieValide = false;

    while (!saisieValide) {
        cout << "Entrez le nombre de lignes: ";
        cin >> lignes;

        if (cin.fail() || lignes < longueurGagnante) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrée invalide. Le nombre de lignes doit être un nombre positif et supérieur ou égal à " << longueurGagnante << "." << endl;
            continue;
        }

        cout << "Entrez le nombre de colonnes: ";
        cin >> colonnes;

        if (cin.fail() || colonnes < longueurGagnante) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrée invalide. Le nombre de colonnes doit être un nombre positif et supérieur ou égal à " << longueurGagnante << "." << endl;
            continue;
        }

        saisieValide = true;
    }

    plateau = Plateau(lignes, colonnes, longueurGagnante);
}

void Jeu::choisirModeDeJeu() {
    int choix;
    bool choixValide = false;

    while (!choixValide) {
        cout << "Choisissez le mode de jeu: \n";
        cout << "1: Joueur contre Joueur (PvP)\n";
        cout << "2: Joueur contre IA (PvE)\n";
        cout << "Choix: ";
        cin >> choix;

        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Entrée invalide. Veuillez entrer un nombre." << endl;
            continue;
        }

        if (choix == 1) {
            mode = PvP;
            choixValide = true;
            joueur1 = new JoueurHumain(1);
            joueur2 = new JoueurHumain(2);
        } else if (choix == 2) {
            mode = PvE;
            choixValide = true;
            joueur1 = new JoueurHumain(1);
            joueur2 = new IA(2, 8); // Profondeur fixée à 5 au minimum pour difficulté facile, ne pas dépasser 8 sinon l'ia met des heures à jouer
            int ordre;
            bool ordreValide = false;
            while (!ordreValide) {
                cout << "Voulez-vous commencer ou laisser l'IA commencer ? \n";
                cout << "1: Vous commencez\n";
                cout << "2: L'IA commence\n";
                cout << "Choix: ";
                cin >> ordre;

                if (cin.fail()) {
                    cin.clear(); // Clear the error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Entrée invalide. Veuillez entrer un nombre." << endl;
                    continue;
                }

                if (ordre == 1) {
                    tour = 1;
                    ordreValide = true;
                } else if (ordre == 2) {
                    tour = 2;
                    ordreValide = true;
                } else {
                    cout << "Choix invalide. Réessayez.\n";
                }
            }
        } else {
            cout << "Choix invalide. Réessayez.\n";
        }
    }
}

void Jeu::puissance5() {
    bool jeuEnCours = true;
    while (jeuEnCours) {
        plateau.afficher();
        afficherTour();
        coupDeJeu(tour);

        int dernierCoupColonne = (tour == 1) ? joueur1->obtenirDerniereColonne() : joueur2->obtenirDerniereColonne();
        int dernierCoupLigne = plateau.obtenirLigneLibre(dernierCoupColonne) + 1; // Ligne où le dernier coup a été joué

        if (plateau.verifierVictoire(dernierCoupLigne, dernierCoupColonne, tour)) {
            plateau.afficher();
            cout << "Joueur " << tour << " gagne!" << endl;
            jeuEnCours = false;
        } else if (plateau.estPlein()) {
            plateau.afficher();
            cout << "Match nul!" << endl;
            jeuEnCours = false;
        } else {
            joueurSuivant();
        }
    }
}

void Jeu::afficherTour() const {
    cout << "Tour du joueur " << tour << ": ";
}

void Jeu::joueurSuivant() {
    tour = (tour == 1) ? 2 : 1;
}

void Jeu::coupDeJeu(int joueur) {
    int colonne;

    if (joueur == 1) {
        joueur1->choisirCoup(plateau);
    } else {
        joueur2->choisirCoup(plateau);
    }

    // Afficher la colonne où le pion a été placé
    colonne = (joueur == 1) ? joueur1->obtenirDerniereColonne() : joueur2->obtenirDerniereColonne();
    cout << "Colonne choisie: " << colonne << endl;
}
