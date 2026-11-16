#include <stdio.h>
#include "../include/jeu.h"

int main(void) {
    jeu *j = creer_jeu(NB_LIGNES, NB_COLONNES);
    jeu *c = copier_jeu(j);

    printf("Jeu %u x %u créé et copié\n", j->nb_lignes, j->nb_colonnes);

    liberer_jeu(c);
    liberer_jeu(j);
    return 0;
}