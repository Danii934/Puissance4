#include "../include/jeu.h"

int main(void) {
    // menu();
    // grille vide : score attendu 0
    jeu * j = creer_jeu(NB_LIGNES,NB_COLONNES);
    printf("%d\n", scoreGrille(j));

    // 3 jaunes alignés en bas, case suivante vide :
    j->grille[5][0] = JAUNE; j->grille[5][1] = JAUNE; j->grille[5][2] = JAUNE;
    printf("%d\n", scoreGrille(j));
    liberer_jeu(j);
    return 0;
}