#include <stdio.h>
#include "../include/arbre.h"
#include "../include/jeu.h"

int main(void) {
    jeu *j = creer_jeu(NB_LIGNES, NB_COLONNES);
    printf("Score grille vide : %d (attendu : 0)\n", scoreGrille(j));
 
    arbre *a = creerArbre(creerNoeud(copier_jeu(j)));
    developperCoupsArbre(a);
    printf("hauteur : %u (attendu : 5), noeuds : %u (attendu : 19608)\n",
           hauteurArbre(a), compterNoeuds(a->racine));
 
    libererArbre(a);
    liberer_jeu(j);
    return 0;
}
