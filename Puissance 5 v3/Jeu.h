#ifndef JEU_H
#define JEU_H

#include <iostream>
#include "Plateau.h"
#include "Joueur.h"
#include "IA.h"

using namespace std;

enum ModeDeJeu { PvP, PvE };

class Jeu {
private:
    Plateau plateau;
    Joueur* joueur1;
    Joueur* joueur2;
    ModeDeJeu mode;
    int tour;

public:
    Jeu();
    ~Jeu();
    void puissance5();
    void afficherTour() const;
    void joueurSuivant();
    void choisirModeDeJeu();
    void choisirTailleGrille();
    void coupDeJeu(int joueur);
};

#endif 
