#ifndef JOUEUR_H
#define JOUEUR_H

#include <iostream>

using namespace std;

class Plateau; // Forward declaration

class Joueur {
protected:
    int numero;

public:
    Joueur(int num);
    virtual void choisirCoup(Plateau& plateau) const = 0; // Méthode virtuelle pure
    virtual int obtenirDerniereColonne() const = 0; // Nouvelle méthode pour obtenir la dernière colonne jouée
    virtual ~Joueur() = default;
};

#endif // JOUEUR_H
