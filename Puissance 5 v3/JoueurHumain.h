#ifndef JOUEURHUMAIN_H
#define JOUEURHUMAIN_H

#include <iostream>
#include "Joueur.h"

using namespace std;

class JoueurHumain : public Joueur {
private:
    mutable int derniereColonne; // mutable pour permettre la modification dans une méthode const

public:
    JoueurHumain(int num);
    void choisirCoup(Plateau& plateau) const override;
    int obtenirDerniereColonne() const override;
};

#endif 
