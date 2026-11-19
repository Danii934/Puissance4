#ifndef ARBRE_H
#define ARBRE_H
 
#include <stdbool.h>
#include <stdint.h>
#include "jeu.h"
 
#define HAUTEUR_MAX 5
 
typedef enum role { IA, JOUEUR, INIT } role;
 
typedef struct noeud {
    uint32_t nb_enfants;
    uint32_t choix;          // colonne jouée pour arriver à ce nœud
    struct noeud *frere;     // nœud suivant du même niveau
    struct noeud *enfants;   // premier enfant
    jeu *config;             // propre à ce nœud (copie)
    bool victoire;
    int32_t score;
    role tour;               // qui a joué le coup menant à ce nœud (INIT pour la racine)
} noeud;
 
typedef struct arbre {
    noeud *racine;
} arbre;
 
/* Création / libération */
noeud *creerNoeud(jeu *j);
arbre *creerArbre(noeud *r);
void libererNoeud(noeud *n);
void libererArbre(arbre *a);
 
/* Manipulation des enfants */
void AjouterEnfant(noeud *pere, jeu *j, bool gagne, role p, uint32_t choix);
noeud *RetirerEnfant(noeud *pere);
 
/* Informations sur l'arbre */
uint32_t hauteurNoeud(noeud *r);
uint32_t hauteurArbre(arbre *a);
uint32_t compterNoeuds(noeud *n);
 
/* Construction de l'arbre */
void developperCoupsNoeud(noeud *r);
void developperCoupsNiveau(noeud *r, uint32_t niveau, uint32_t hauteur_max);
void developperCoupsArbre(arbre *a);
 
#endif