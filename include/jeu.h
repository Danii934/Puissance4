#ifndef JEU_H
#define JEU_H

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

// Constantes et structures
#define NB_LIGNES 6
#define NB_COLONNES 7

typedef enum caseG { VIDE, ROUGE, JAUNE } caseG;

typedef struct jeu {
    uint32_t nb_lignes;
    uint32_t nb_colonnes;
    caseG **grille;  // Plateau de jeu   
} jeu;

/*Initialisation du plateau de jeu*/
jeu *creer_jeu(uint32_t nb_lignes, uint32_t nb_colonnes);

/*Copie du plateau de jeu*/
jeu *copier_jeu(jeu *src);

/*Libération mémoire du jeu*/
void liberer_jeu(jeu *j);

#endif