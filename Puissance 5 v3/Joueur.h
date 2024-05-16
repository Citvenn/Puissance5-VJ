#ifndef JOUEUR_H
#define JOUEUR_H

#include <iostream>

using namespace std;

class Plateau; 

class Joueur {
protected:
    int numero;

public:
    Joueur(int num);
    virtual void choisirCoup(Plateau& plateau) const = 0; 
    virtual int obtenirDerniereColonne() const = 0; 
    virtual ~Joueur() = default;
};
// cette classe a finit par passer en virtual pour toutes les méthodes parce que j'avais des 
// pronlèmes avec la génération des coups dans le minimax
// et puis ça permet d'avoir joueurhumain et ia séparé ce qui rend l'héritage un peu plus simple

#endif 
