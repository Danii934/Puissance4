#include "../include/jeu.h"


jeu *creer_jeu(uint32_t nb_lignes, uint32_t nb_colonnes) {
    jeu *j = malloc(sizeof(jeu));
    assert(j != NULL);

    j->nb_lignes = nb_lignes;
    j->nb_colonnes = nb_colonnes;

    j->grille = malloc(sizeof(caseG *) * nb_lignes);
    assert(j->grille != NULL);

    for (uint32_t i = 0; i < nb_lignes; i++) {
        j->grille[i] = malloc(sizeof(caseG) * nb_colonnes);
        assert(j->grille[i] != NULL);
        for (uint32_t k = 0; k < nb_colonnes; k++)
            j->grille[i][k] = VIDE;
    }
    return j;
}

jeu *copier_jeu(jeu *src) {
    assert(src != NULL);
    jeu *copie = creer_jeu(src->nb_lignes, src->nb_colonnes);

    for (uint32_t i = 0; i < src->nb_lignes; i++)
        for (uint32_t k = 0; k < src->nb_colonnes; k++)
            copie->grille[i][k] = src->grille[i][k];
    return copie;
}

void liberer_jeu(jeu *j) {
    assert(j != NULL);
    for (uint32_t i = 0; i < j->nb_lignes; i++)
        free(j->grille[i]);
    free(j->grille);
    free(j);
}