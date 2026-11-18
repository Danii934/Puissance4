#include "../include/jeu.h"
#include <assert.h>
#include <stdio.h>

#define ANSI_ROUGE "\033[1;31m"
#define ANSI_JAUNE "\033[1;33m"
#define ANSI_RESET "\033[0m"


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

void afficher_jeu(jeu *j) {
    assert(j != NULL);

    for (uint32_t i = 0; i < j->nb_lignes; i++) {
        for (uint32_t k = 0; k < j->nb_colonnes; k++) {
            switch (j->grille[i][k]) {
                case ROUGE:
                    printf(ANSI_ROUGE "R" ANSI_RESET " ");
                    break;
                case JAUNE:
                    printf(ANSI_JAUNE "J" ANSI_RESET " ");
                    break;
                default:
                    printf(". ");
                    break;
            }
        }
        putchar('\n');
    }
}

void afficher_curseur(jeu *j, uint32_t curseur) {
    assert(j != NULL);
    assert(curseur < j->nb_colonnes);

    for (uint32_t k = 0; k < j->nb_colonnes; k++)
        printf(k == curseur ? "^" : "  ");
    putchar('\n');
}

char lire_coup(void) {
    char c;
    while (1) {
        printf("Votre choix (q = gauche, d = droite, j = jouer, x = abandonner) : ");
        if (scanf(" %c", &c) != 1)
            return 'x';                    

        int ch;                            
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;

        c = (char)tolower((unsigned char)c);
        if (c == 'q' || c == 'd' || c == 'j' || c == 'x')
            return c;
    }
}
bool colonnePleine(jeu *j, uint32_t c) {
    assert(j != NULL);
    assert(c < j->nb_colonnes);
    return j->grille[0][c] != VIDE;
}

uint32_t appliquerCoup(jeu *j, uint32_t col, caseG couleur) {
    assert(j != NULL);
    assert(col < j->nb_colonnes);
    assert(couleur != VIDE);
    assert(!colonnePleine(j, col));     

    uint32_t i = j->nb_lignes - 1;
    while (j->grille[i][col] != VIDE)
        i--;                             
    j->grille[i][col] = couleur;
    return i;
}

uint32_t tourJoueur(jeu * j,caseG couleur,uint32_t *l,uint32_t *c){
    uint32_t pos_curseur = 0;
    char move;
    uint32_t pas_joue = 1;
    while(pas_joue){
        afficher_jeu(j);
        afficher_curseur(j,pos_curseur);
        move = lire_coup();

        switch (move)
        {
            case 'q':
                pos_curseur = (pos_curseur == 0) ? j->nb_colonnes -1 : pos_curseur -1;
                printf("Postion curseur colonne %d\n",pos_curseur);
                break;
            case 'd':
                pos_curseur = (pos_curseur == j->nb_colonnes -1) ? 0 : pos_curseur +1;
                printf("Postion curseur colonne %d\n",pos_curseur);
                break;
            case 'j':
                if(!colonnePleine(j,pos_curseur)){
                    *l = appliquerCoup(j,pos_curseur,couleur);
                    *c = pos_curseur;
                    pas_joue = 0;
                }
                else
                {
                    puts("Choisissez une autre colonne svp");
                }
                break;
            default :
                return 0;
                break;
        }

    }
    return 1;
}
bool matchNul(jeu *j) {
    assert(j != NULL);
    for (uint32_t k = 0; k < j->nb_colonnes; k++)
        if (!colonnePleine(j, k))
            return false;
    return true;
}

// Compte les pions consécutifs de cette couleur à partir de (l, c),
// dans la direction (dl, dc), sans compter la case (l, c) elle-même.
uint32_t compter(jeu *j, uint32_t l, uint32_t c,
                 int32_t dl, int32_t dc, caseG couleur) {
    uint32_t n = 0;
    /*Premier pas*/

    /*indice  Ligne*/
    int32_t i = (int32_t)l + dl;
    /*Indice colonne*/
    int32_t k = (int32_t)c + dc;

    while (i >= 0 && i < (int32_t)j->nb_lignes &&
           k >= 0 && k < (int32_t)j->nb_colonnes &&
           j->grille[i][k] == couleur) {
        n++;
        i += dl;
        k += dc;
    }
    return n;
}

bool verifVerticale(jeu *j, uint32_t l, uint32_t c) {
    caseG couleur = j->grille[l][c];
    // haut (-1, 0) et bas (+1, 0)
    return 1 + compter(j, l, c, -1, 0, couleur)
             + compter(j, l, c,  1, 0, couleur) >= NB_ALIGNES;
}

bool verifHorizontale(jeu *j, uint32_t l, uint32_t c) {
    caseG couleur = j->grille[l][c];
    // gauche (0, -1) et droite (0, +1)
    return 1 + compter(j, l, c, 0, -1, couleur)
             + compter(j, l, c, 0,  1, couleur) >= NB_ALIGNES;
}

bool verifDiagonaleDescendante(jeu *j, uint32_t l, uint32_t c) {
    caseG couleur = j->grille[l][c];
    // haut-gauche (-1, -1) et bas-droite (+1, +1)
    return 1 + compter(j, l, c, -1, -1, couleur)
             + compter(j, l, c,  1,  1, couleur) >= NB_ALIGNES;
}

bool verifDiagonaleMontante(jeu *j, uint32_t l, uint32_t c) {
    caseG couleur = j->grille[l][c];
    // haut-droite (-1, +1) et bas-gauche (+1, -1)
    return 1 + compter(j, l, c, -1,  1, couleur)
             + compter(j, l, c,  1, -1, couleur) >= NB_ALIGNES;
}

bool victoire(jeu *j, uint32_t l, uint32_t c) {
    assert(j != NULL);
    assert(l < j->nb_lignes && c < j->nb_colonnes);
    assert(j->grille[l][c] != VIDE);

    return verifVerticale(j, l, c)
        || verifHorizontale(j, l, c)
        || verifDiagonaleDescendante(j, l, c)
        || verifDiagonaleMontante(j, l, c);
}