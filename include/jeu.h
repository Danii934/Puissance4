#ifndef JEU_H
#define JEU_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

// Constantes et structures
#define NB_LIGNES 6
#define NB_COLONNES 7
#define NB_ALIGNES 4

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

/*Affichage du plateau de jeu*/
void afficher_jeu(jeu *j);

/*Affichage du curseur pour permettre au joueur de se deplacer de colonne en colonne*/
void afficher_curseur(jeu *j, uint32_t curseur);

/*Lit un caractère en entrée */
char lire_coup(void);

/*Vérifie si une colonne est pleine*/
bool colonnePleine(jeu *j, uint32_t c);

/*Pose un jeton dans la colonne indiquée*/
uint32_t appliquerCoup(jeu *j, uint32_t col, caseG couleur);

/*Verifie si toutes les colonnes sont pleines */
bool matchNul(jeu *j);

/*Fonctions de vérification de victoire*/
uint32_t compter(jeu *j, uint32_t l, uint32_t c,int32_t dl, int32_t dc, caseG couleur);
/*Haut bas*/
bool verifVerticale(jeu *j, uint32_t l, uint32_t c);

/*Gauche Droite*/
bool verifHorizontale(jeu *j, uint32_t l, uint32_t c);

/*Diagonale Descendante*/
bool verifDiagonaleDescendante(jeu *j, uint32_t l, uint32_t c);

/*Diagonale montante*/
bool verifDiagonaleMontante(jeu *j, uint32_t l, uint32_t c);

/*Utilsation des différentes vérifications*/
bool victoire(jeu *j, uint32_t l, uint32_t c);

/*Implémentation du mode JoueurVSJoueur :  Permet à un joue de jouer*/
uint32_t tourJoueur(jeu * j,caseG couleur,uint32_t *l,uint32_t *c);
#endif