#include "JoueurHumain.h"
#include "Plateau.h"

#include <iostream>
#include <limits>

using namespace std;

JoueurHumain::JoueurHumain(int num) : Joueur(num), derniereColonne(-1) {}
//initialisation

void JoueurHumain::choisirCoup(Plateau& plateau) const {
    int colonne;
    bool coupValide = false;
    while (!coupValide) {
        cout << "Joueur " << numero << ", entrez le numéro de colonne: ";
        cin >> colonne;
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(std::numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Entrée invalide. Veuillez entrer un nombre." << endl;
            continue;
        }
        if (colonne >= 0 && colonne < plateau.getNbColonne() && plateau.estEmplacementValide(colonne)) {
            derniereColonne = colonne; // Mise à jour de derniereColonne
            plateau.placerPion(colonne, numero);
            coupValide = true;
        } else {
            cout << "Colonne invalide ou pleine. Réessayez: " << endl;
        }
    }
}
// méthode héritée de la classe joueur qui permet à un utilisateur de jouer son coup

int JoueurHumain::obtenirDerniereColonne() const {
    return derniereColonne;
}
//méthode qui permet l'affichage du dernier coup joué pour aidder à l'initialisation
