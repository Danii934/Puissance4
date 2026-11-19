#include <assert.h>
#include <stdlib.h>
#include "../include/arbre.h"

// Le nœud devient propriétaire de j : il le libérera dans libererNoeud.
noeud *creerNoeud(jeu *j) {
    assert(j != NULL);
    noeud *n = calloc(1, sizeof(noeud));   // pointeurs à NULL, compteurs à 0
    assert(n != NULL);
    n->config = j;
    n->tour = INIT;
    return n;
}

arbre *creerArbre(noeud *r) {
    assert(r != NULL);
    arbre *a = calloc(1, sizeof(arbre));
    assert(a != NULL);
    a->racine = r;
    return a;
}

/* ---------- Enfants ---------- */
 
// Ajoute un enfant après le dernier (l'ordre des colonnes est conservé).
// j doit être une copie propre à cet enfant.
void AjouterEnfant(noeud *pere, jeu *j, bool gagne, role p, uint32_t choix) {
    assert(pere != NULL);
    noeud *n = creerNoeud(j);
    n->tour = p;
    n->choix = choix;
    n->victoire = gagne;
 
    if (pere->enfants == NULL) {
        pere->enfants = n;
    } else {
        noeud *dernier = pere->enfants;
        while (dernier->frere != NULL)
            dernier = dernier->frere;
        dernier->frere = n;
    }
    pere->nb_enfants++;
}
 
// Détache et renvoie le premier enfant (NULL s'il n'y en a pas).
noeud *RetirerEnfant(noeud *pere) {
    assert(pere != NULL);
    if (pere->enfants == NULL)
        return NULL;
 
    noeud *n = pere->enfants;
    pere->enfants = n->frere;
    n->frere = NULL;
    pere->nb_enfants--;
    return n;
}
 
/* ---------- Libération ---------- */
 
void libererNoeud(noeud *n) {
    if (n == NULL) return;
    noeud *m;
    while ((m = RetirerEnfant(n)) != NULL)
        libererNoeud(m);    
    liberer_jeu(n->config);
    free(n);
}
 
void libererArbre(arbre *a) {
    assert(a != NULL);
    libererNoeud(a->racine);
    free(a);
}
 
/* ---------- Informations ---------- */
 
uint32_t hauteurNoeud(noeud *r) {
    assert(r != NULL);
    uint32_t max = 0;
    for (noeud *m = r->enfants; m != NULL; m = m->frere) {
        uint32_t h = 1 + hauteurNoeud(m);
        if (h > max) max = h;
    }
    return max;                 // une feuille renvoie 0
}
 
uint32_t hauteurArbre(arbre *a) {
    assert(a != NULL);
    return hauteurNoeud(a->racine);
}
 
uint32_t compterNoeuds(noeud *n) {
    if (n == NULL) return 0;
    uint32_t total = 1;
    for (noeud *e = n->enfants; e != NULL; e = e->frere)
        total += compterNoeuds(e);
    return total;
}
 
/* ---------- Construction ---------- */
 
void developperCoupsNoeud(noeud *r) {
    assert(r != NULL);
    if (r->victoire) return;
 
    role prochain = (r->tour == IA) ? JOUEUR : IA;
    caseG couleur = (prochain == JOUEUR) ? ROUGE : JAUNE;
 
    for (uint32_t i = 0; i < r->config->nb_colonnes; i++) {
        if (!colonnePleine(r->config, i)) {
            jeu *copie = copier_jeu(r->config);
            uint32_t l = appliquerCoup(copie, i, couleur);
            bool gagne = victoire(copie, l, i);
            AjouterEnfant(r, copie, gagne, prochain, i);
        }
    }
}
 
void developperCoupsNiveau(noeud *r, uint32_t niveau, uint32_t hauteur_max) {
    if (r == NULL || r->victoire || niveau >= hauteur_max) return;
 
    developperCoupsNoeud(r);
    for (noeud *e = r->enfants; e != NULL; e = e->frere)
        developperCoupsNiveau(e, niveau + 1, hauteur_max);
}
 
void developperCoupsArbre(arbre *a) {
    assert(a != NULL);
    developperCoupsNiveau(a->racine, 0, HAUTEUR_MAX);
}

/**************************************
***************************************
Implémentation du minimax et du mode IA
***************************************
****************************************/


/*Utilisation de l'algorithme minimax pour le choix du prochain coup*/
int32_t minimax(noeud *r) {
    assert(r != NULL);

    if (r->enfants == NULL) {                 
        r->score = scoreGrille(r->config);
        return r->score;
    }

    bool maximiser = (r->enfants->tour == IA);
    int32_t meilleur = minimax(r->enfants);
    for (noeud *e = r->enfants->frere; e != NULL; e = e->frere) {
        int32_t s = minimax(e);
        if (maximiser ? (s > meilleur) : (s < meilleur))
            meilleur = s;
    }
    r->score = meilleur;
    return meilleur;
}
/*Permet a l'IA de jouer*/
void tourIA(jeu *j, uint32_t *l, uint32_t *c) {
    assert(j != NULL);
    assert(!matchNul(j));

    arbre *a = creerArbre(creerNoeud(copier_jeu(j)));
    developperCoupsArbre(a);
    int32_t best = minimax(a->racine);

    noeud *m = a->racine->enfants;
    while (m->score != best)           // premier enfant au meilleur score
        m = m->frere;
    *c = m->choix;

    libererArbre(a);                   // après avoir lu m->choix
    *l = appliquerCoup(j, *c, JAUNE);
}

/*Mode Joueur vs IA*/
void joueurVSIA() {
    jeu *j = creer_jeu(NB_LIGNES, NB_COLONNES);
    role tour = JOUEUR;
    uint32_t l = 0, c = 0;

    while (true) {
        if (tour == JOUEUR) {
            puts("\nA vous de jouer");
            if (!tourJoueur(j, ROUGE, &l, &c)) {
                puts("Vous avez abandonné");
                break;
            }
        } else {
            tourIA(j, &l, &c);
            puts("\nL'IA a joué");
        }

        if (victoire(j, l, c)) {
            afficher_jeu(j);
            puts(tour == JOUEUR ? "Victoire !" : "L'IA a gagné");
            break;
        }
        if (matchNul(j)) {
            afficher_jeu(j);
            puts("Match nul");
            break;
        }
        tour = (tour == IA) ? JOUEUR : IA;
    }
    liberer_jeu(j);
}