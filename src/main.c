#include "../include/jeu.h"

int main(void) {
    jeu *j = creer_jeu(NB_LIGNES, NB_COLONNES);
    for (uint32_t i = 0; i < 4; i++)
    {

        for (uint32_t k = i+1; k <4 ; k++)
        {
             appliquerCoup(j, i, JAUNE);
        }
        appliquerCoup(j,i,ROUGE);
    } 
    
    afficher_jeu(j);
    printf("Diagonale Descandante : %d\n", verifDiagonaleDescendante(j, 2,0));
    liberer_jeu(j);
    return 0;
}