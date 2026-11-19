#include "../include/jeu.h"
#include "../include/arbre.h"

void menu();

int main(void) {
    menu();
    return 0;
}

void menu()
{
    char choix;
    puts("**************************************************");
    puts("*******************PUISSANCE 4********************");
    puts("**************************************************");
    do
    {
        puts("Choisissez un mode :");
        puts("1 : Joueur contre Joueur");
        puts("2 : Joueur contre IA");
        puts("3 : Quitter");
        printf("Votre choix : ");
        scanf(" %c",&choix);
    } while (choix != '1' && choix != '2' && choix != '3');
    switch (choix)
    {
        case '1':
            joueurVSjoueur();
            break;
        
        case '2' :
            joueurVSIA();
            break;
        
        default:
            puts("A la prochaine :)");
            break;
    }
}