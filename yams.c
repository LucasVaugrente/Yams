/**
 * 
 * \file yams1.c
 * 
 * \brief Projet YAMS SAE 1.01
 * 
 * \author Lucas Vaugrente 
 *
 * \date 28 novembre 2021
 * 
 * \version 1.0
 * 
 * Ce programme permet d'accueillir 2 joueurs qui pourront jouer à une partie de YAMS. Durant celle-ci, les joueurs pourront jouer chacun leurs tours avec un affichage de leurs scores en permanence, un affichage des dés lancés, des combinaisons à prendre, de la validation du joueur qui joue son tour et du tableau des scores finaux à la fin de la partie. Des phases de vérifications sont mises en place pour ne pas créer d'erreur lors de la partie, à savoir, la vérification d'une valeur d'un dé entrée à la main, la mise-à-jour de la feuille de marque, la vérification de la possiblité d'un choix de combinaison et la vérification de la validité d'une combinaison.
 * 
 */

// Déclarations de toutes les includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/**
*
* \def N
*
* \brief Constante pour le nombre de combinaisons
*
*/
# define N 12

/**
*
* \def N
*
* \brief Constante pour le nombre de dé
*
*/
# define DES 5

/**
*
* \def NBR_ESSAIS
*
* \brief Constante pour le nombre d'essais du joueur à chaque tour
*
*/
const int NBR_ESSAIS=3;

/**
*
* \typedef tabdes
*
* \brief type tableau de DES entiers
*
* Le type tabdes sert de stockage pour les dés du joueur, cela pourra être la valeur des dés, mais aussi les indices des dés que le joueur voudra garder.
*
*/
typedef int tabdes[DES];

/**
*
* \typedef tabfeuille
*
* \brief type tableau de JOUEURS tableaux contenant 18 entiers pour le score de toutes les combinaisons
*
* Le type tabdes sert à afficher la feuille de marque du jeu contenant tous les tableaux des joueurs.
*
*/
typedef int tabfeuille[2][18];

/**
*
* \typedef tabjoueur
*
* \brief type tableau de 18 entiers
*
* Le type tabdes sert à stocker les scores de toutes les combinaisons et des totaux du joueurs. La 18eme place est égale à -1, cela sert à comparer des valeurs du tableau avec celle-ci, suite à une succession d'erreurs lorsque je faisais des comparaisons avec un simple entier -1.
*
*/
typedef int tabjoueur[18];

// Déclarations de toutes les procédures et fonctions
void afficherFeuille(tabfeuille , char[19], int);
void afficherfeuillecombinaison(tabfeuille, int);
void lancerDes(int, tabdes, tabdes, tabdes);
void affichageDes(tabdes);
void validationJoueur(tabdes, tabfeuille, int, char[19]);
void choixCombinaison(tabfeuille, tabdes, int, char[19]);
int verifCombinaison(tabdes, int);
void majfeuille(tabfeuille, int);
void afficheResultat(tabfeuille, char[19], char[19]);
void remiseazero(tabdes);
void verifvalde(int);
void tri_des(tabdes, int);
int detecterdoublon(tabdes, int, int);

/**
 * 
 * \fn int main()
 * 
 * \brief Fonction principale du programme
 * 
 * Rassemble toutes les procédures et fonctions pour le déroulement de la partie de yams entre 2 joueurs. Cela demande le nom de chaque joueur et grâce à un switch fait jouer chaque joueur une fois sur deux leurs tours. Affiche la feuille de marque du joueur, les dés lancés à son premier essai et demande au joueur de vailder.
 * 
 */
int main()
{
    srand(time(NULL));
    int i, tour_joueur;
    char nom1[19];
    char nom2[19];
    tabfeuille feuillemarque;
    tabdes numerodes, valeurdes, des;
    
    // Remplissage de la feuille de marque, toutes les valeurs sont égales à zéro.
    for(i=0;i<19;i++)
    {
        feuillemarque[0][i]=0;    
    }
    for(i=0;i<19;i++)
    {
        feuillemarque[1][i]=0;    
    }

    printf("Nom du joueur 1 :\n");
    scanf("%s", nom1);
    printf("Nom du joueur 2 :\n");
    scanf("%s", nom2);

    tour_joueur=1;

    for(i=0; i<26; i++)
    {
        switch (tour_joueur)
        {
            case 1:
                afficherFeuille(feuillemarque, nom1, 0);
                lancerDes(0, numerodes, valeurdes, des);
                validationJoueur(des, feuillemarque, 0, nom1);
                tour_joueur=2;
                break;
        
            case 2:
                afficherFeuille(feuillemarque, nom2, 1);
                lancerDes(0, numerodes, valeurdes, des);
                validationJoueur(des, feuillemarque, 1, nom2);
                tour_joueur=1;
                break;
        
            default:
                break;
        }
    }
    printf("Total de %s : %d\n", nom1, feuillemarque[0][16]);
    printf("Total de %s : %d\n", nom2, feuillemarque[1][16]);
    afficheResultat(feuillemarque, nom1, nom2);
    return EXIT_SUCCESS;
}

/**
 * 
 * \fn void afficherFeuille(tabfeuille tab, char joueur[19], int joueur)
 * 
 * \brief Procédure qui permet d'afficher la feuille de marque du joueur
 * 
 * \param tab: le tableau de la feuille de marque
 * \param joueur: le nom du joueur
 * \param j: indice qui permettra d'afficher la bonne valeur en cherchant dans le tab
 * 
 */
void afficherFeuille(tabfeuille tab, char joueur[19], int j)
{
    printf("╔═══════════════════════════════╗\n");
    printf("║ Grille de %-19s ║\n", joueur);
    printf("╚═══════════════════════════════╝\n");
    printf("╔═══════════════════════════╦══════╗\n");
    printf("║            YAMS           ║      ║\n");
    printf("╠═══════════════════════════╬══════╣\n");
    printf("║  1 [total de 1]     ⚀     ║ %3d  ║\n",tab[j][0]);
    printf("║  2 [total de 2]     ⚁     ║ %3d  ║\n",tab[j][1]);
    printf("║  3 [total de 3]     ⚂     ║ %3d  ║\n",tab[j][2]);
    printf("║  4 [total de 4]     ⚃     ║ %3d  ║\n",tab[j][3]);
    printf("║  5 [total de 5]     ⚄     ║ %3d  ║\n",tab[j][4]);
    printf("║  6 [total de 6]     ⚅     ║ %3d  ║\n",tab[j][5]);
    printf("╠═══════════════════════════╬══════╣\n");
    printf("║  Bonus si > 62    [35]    ║ %3d  ║\n",tab[j][6]);
    printf("╠═══════════════════════════╬══════╣\n");
    printf("║  Total supérieur--        ║ %3d  ║\n",tab[j][7]);
    printf("╠═══════════════════════════╬══════╣\n");
    printf("║  Brelan     [total]       ║ %3d  ║\n",tab[j][8]);
    printf("║  Carré      [total]       ║ %3d  ║\n",tab[j][9]);
    printf("║  Full House  [25]         ║ %3d  ║\n",tab[j][10]);
    printf("║  Petite suite [30]        ║ %3d  ║\n",tab[j][11]);
    printf("║  Grande suite [40]        ║ %3d  ║\n",tab[j][12]);
    printf("║  Yams        [50]         ║ %3d  ║\n",tab[j][13]);
    printf("║  Chance     [total]       ║ %3d  ║\n",tab[j][14]);
    printf("╠═══════════════════════════╬══════╣\n");
    printf("║  Total inférieur          ║ %3d  ║\n",tab[j][15]);
    printf("╠═══════════════════════════╬══════╣\n");
    printf("║  Total                    ║ %3d  ║\n",tab[j][16]);
    printf("╚═══════════════════════════╩══════╝\n");
}

/**
 * 
 * \fn void afficherfeuillecombinaison(tabfeuille tab, int j)
 * 
 * \brief Procédure qui permet d'afficher les combinaisons manquantes du joueur
 * 
 * \param tab: le tableau de la feuille de marque
 * \param j: indice qui permettra d'afficher la bonne valeur en cherchant dans le tableau
 * 
 * Affiche un tableau montrant toutes les combinaisons possibles, la valeur en face de chaque combinaison peut avoir une des 3 significations : 0 veut dire que la combinaison n'a pas encore été jouer. Un nombre supérieur à 0 veut dire qu'il a été jouer et que c'est le résultat d'un des anciens tours du joueur. -1 est équivalent à une croix 'X' dans les règles du jeu, cela veut dire que le joueur a sacrifié cette combinaison et qu'il ne peut plus s'en servir. Lorsque la valeur est égale à -1, c'est évidemment pas compter dans le score final.
 * 
 */
void afficherfeuillecombinaison(tabfeuille tab, int j)
{
    printf("╔═══════════════════════════════╗\n");
    printf("║ Combinaisons disponnibles :   ║\n");
    printf("╚═══════════════════════════════╝\n");
    printf("╔═════════════════════════════╦══════╗\n");
    printf("║  {0}  1 [total de 1]     ⚀  ║ %3d  ║\n",tab[j][0]);
    printf("║  {1}  2 [total de 2]     ⚁  ║ %3d  ║\n",tab[j][1]);
    printf("║  {2}  3 [total de 3]     ⚂  ║ %3d  ║\n",tab[j][2]);
    printf("║  {3}  4 [total de 4]     ⚃  ║ %3d  ║\n",tab[j][3]);
    printf("║  {4}  5 [total de 5]     ⚄  ║ %3d  ║\n",tab[j][4]);
    printf("║  {5}  6 [total de 6]     ⚅  ║ %3d  ║\n",tab[j][5]);
    printf("╠═════════════════════════════╬══════╣\n");
    printf("║  {8}   Brelan     [total]   ║ %3d  ║\n",tab[j][8]);
    printf("║  {9}   Carré      [total]   ║ %3d  ║\n",tab[j][9]);
    printf("║  {10}  Full House  [25]     ║ %3d  ║\n",tab[j][10]);
    printf("║  {11}  Petite suite [30]    ║ %3d  ║\n",tab[j][11]);
    printf("║  {12}  Grande suite [40]    ║ %3d  ║\n",tab[j][12]);
    printf("║  {13}  Yams       [50]      ║ %3d  ║\n",tab[j][13]);
    printf("║  {14}  Chance    [total]    ║ %3d  ║\n",tab[j][14]);
    printf("╚═════════════════════════════╩══════╝\n");
}

/**
 * 
 * \fn void lancerDes(int choix, tabdes indicedes, tabdes tabval, tabdes des)
 * 
 * \brief Procédure qui permet de générer des dés lancés aléatoirement selon un choix
 * 
 * \param int: le choix du joueur qui est volontairmeent choisi lors de l'appel.
 * \param indicedes: un tableau contenant les indices des dés gardés par le joueur
 * \param tabval: un tableau des valeurs des dés qui correspond au tour d'avant
 * \param des: le tableau qui va accueillir et afficher les nouveaux dés.
 * 
 * Si le choix est égal à 0, cela affiche 5 dés aléatoirement, si le choix est égal à 1, cela affiche 5 dés dont quelques uns qui ont gardés la même valeur au même indice que les 5 dés d'avant et les autres dés aléatoirement.
 * 
 */
void lancerDes(int choix, tabdes indicedes, tabdes tabval, tabdes des)
{
    int i, j;
    remiseazero(des);
    if (choix==0)
    {
        for(i=0;i<DES;i++)
        {
            des[i] = rand() % 6 + 1;
        }
        affichageDes(des);
    }
    else if(choix==1)
    {
        j=0;
        for(i=0;i<DES;i++)
        {
            if(indicedes[j]==i)
            {
                des[i]=tabval[indicedes[j]];
                j=j+1;
            }
            else
            {
                des[i] = rand() % 6 + 1;
            }
        }
        affichageDes(des);
    }
}

/**
 * 
 * \fn void affichageDes(tabdes tab)
 * 
 * \brief Procédure qui permet d'afficher des dés lancés aléatoirement selon un choix
 * 
 * \param tab: le tableau contenant les valeurs des dés
 * 
 * Parcourt le tableau et grâce à un switch, affiche le bon affichage de dé correspondant à la valeur dans le tableau. Cela a pour objectif d'être plus visuel.
 * 
 */
void affichageDes(tabdes tab)
{
    int i;
    for(i=0;i<DES;i++)
    {
        switch(tab[i])
        {
            case 1:
                printf("Dé %d :\n", i+1);
                printf("┌───────┐\n");
                printf("│       │\n");
                printf("│   O   │\n");
                printf("│       │\n");
                printf("└───────┘\n");
                break;
            case 2:
                printf("Dé %d :\n", i+1);
                printf("┌───────┐\n");
                printf("│ O     │\n");
                printf("│       │\n");
                printf("│     O │\n");
                printf("└───────┘\n");
                break;
            case 3:
                printf("Dé %d :\n", i+1);
                printf("┌───────┐\n");
                printf("│ O     │\n");
                printf("│   O   │\n");
                printf("│     O │\n");
                printf("└───────┘\n");
                break;
            case 4:
                printf("Dé %d :\n", i+1);
                printf("┌───────┐\n");
                printf("│ O   O │\n");
                printf("│       │\n");
                printf("│ O   O │\n");
                printf("└───────┘\n");
                break;
            case 5:
                printf("Dé %d :\n", i+1);
                printf("┌───────┐\n");
                printf("│ O   O │\n");
                printf("│   O   │\n");
                printf("│ O   O │\n");
                printf("└───────┘\n");
                break;
            case 6:
                printf("Dé %d :\n", i+1);
                printf("┌───────┐\n");
                printf("│ O   O │\n");
                printf("│ O   O │\n");
                printf("│ O   O │\n");
                printf("└───────┘\n");
                break;
        }
    }
}

/**
 * 
 * \fn validationJoueur(tabdes tabval, tabfeuille feuillemarque, int j, char nom[19])
 * 
 * \brief Procédure qui demande au joueur de valider ses dés
 * 
 * \param tabval: le tableau des valeurs des dés 
 * \param feuillemarque: le tableau de la feuille de marque 
 * \param j: indice qui permettra d'afficher la bonne valeur en cherchant dans le tableau
 * \param nom: le nom du joueur
 * 
 * Demande au joueur s'il veut relancer ou non, s'il le veut, cela demande s'il veut "tous" relancer ou non, sinon il demande au joueur quel(s) dé(s) il veut garder. Cela prend en compte le nombre de tours qu'il a, c'est-à-dire 3 et si jamais il les use tous, les dés seront validés même si le joueur ne veut pas.
 * 
 */
void validationJoueur(tabdes tabval, tabfeuille feuillemarque, int j, char nom[19])
{
    int i, choix, reponse, reponse2, indicede, nbrtour;
    tabdes numdes, des, tabvalautre;
    nbrtour=1;
    for(i=0;i<DES;i++)
    {
        des[i]=tabval[i];
    }
    do
    {
        printf("Voulez vous relancer ? 1 pour oui, 0 pour non : \n");
        scanf("%d", &choix);
        if(choix==1)
        {
            printf("Voulez-vous tous relancer ? 1 pour oui, 0 pour non\n");
            scanf("%d", &reponse);
            if(reponse==1)
            {
                printf("Redistribution des dés...\n");
                lancerDes(0, numdes, tabvalautre, des);
                printf("Il vous reste %d essai.\n", 2-nbrtour);
            }
            else if(reponse==0)
            {
                printf("Si vous saisissez plusieurs dés, entrez les dés dans l'ordre croissant.\n");
                printf("Veuillez saisir un dé que vous voulez garder :\n");
                scanf("%d", &indicede);
                verifvalde(indicede);
                numdes[0]=indicede-1;
                i=1;
                do // Pour l'instant le "do" ne prend pas en compte le cas où le joueur sélectionne les 5 dés !
                {
                    printf("Voulez-vous garder un autre dé ? 1 pour oui, 0 pour non\n");
                    scanf("%d", &reponse2);
                    if (reponse2==1)
                    {
                        printf("Veuillez saisir un dé que vous voulez garder :\n");
                        scanf("%d", &indicede);
                        numdes[i]=indicede-1;
                        i=i+1;
                    }      
                }while (reponse2!=0);
                printf("Redistribution des dés...\n");
                lancerDes(1, numdes, tabval, des);
                printf("Il vous reste %d essai.\n", 2-nbrtour);
            }
            nbrtour=nbrtour+1;      
        }
        else if ( (choix<0)||(choix>1) )
        {
            printf("Erreur : Nombre non conforme à ce qui est demandé.\n");
        }   
    } while ((choix!=0)&&(nbrtour<NBR_ESSAIS));
    printf("Vous avez épuisé vos 3 essais !\n");
    choixCombinaison(feuillemarque, des, j, nom);
}

/**
 * 
 * \fn void choixCombinaison(tabfeuille tab, tabdes des, int joueur, char nom[19])
 * 
 * \brief Procédure qui demande au joueur son choix de la combinaison
 * 
 * \param tab: le tableau de la feuille de marque
 * \param des: le tableau des valeurs des dés
 * \param joueur: le chiffre qui designe le joueur 1 ou le 2
 * \param nom: le nom du joueur
 * 
 * Affiche les combinaisons restantes du joueur, demande au joueur d'entrer un réel qui correspond à la combinaison qu'il veut choisir, qui celui-ci correspond à l'indice de la combinaison dans le tableau du joueur. Si le joueur ne rentre pas un nombre qui génère une erreur, la procédure appelle verifCombinaison pour vérifier si les dés correspondent à la combinaison choisie.
 * 
 */
void choixCombinaison(tabfeuille tab, tabdes des, int joueur, char nom[19])
{
    tab[joueur][17]=(-1);
    int reponse, score;
    printf(" \n");
    printf("Voici les combinaisons restantes :\n");
    printf("Vous devez sélectionner le numéro {?} correspondant au chiffre en face de la combinaison à choisir : {0} pour total de 1 , etc.\n");
    printf("Attention, si les dés ne correspondent pas à la combinaison que vous avez selectionné,\n");
    printf("cela veut dire que vous sacrifié cette combinaison. Ainsi la valeur sera égale à -1, mais cela n'affectera pas le score final\n");
    printf(" \n");
    afficherfeuillecombinaison(tab, joueur);
    printf(" \n");
    printf("Votre choix :\n");
    scanf("%d", &reponse);
    do
    {
        if ((reponse==6)||(reponse==7)||(reponse<0)||(reponse>14))
        {
            printf("Vous devez sélectionner une combinaison entre (0 et 5) et (8 et 14).\n");
            printf("Ressaisissez :\n");
            scanf("%d", &reponse);
        }
        else if ( (tab[joueur][reponse])==(tab[joueur][17]) || (tab[joueur][reponse]>=1) )
        {
            printf("Imppossible de prendre cette combinaison, il faut selectionner une combinaison valide (différent de -1) et \n");
            printf("Ressaisissez :\n");
            scanf("%d", &reponse);
        }
    } while ( ( (tab[joueur][reponse])==(tab[joueur][17]) ) || (tab[joueur][reponse]>=1) || ( ((reponse==6)||(reponse==7)||(reponse<0)||(reponse>14)) ) );

    // Vérification de la combinaison séléctionnée
    score=verifCombinaison(des, reponse);
    printf("Score à mettre : %d\n", score);

    // Mise à jour de la feuille du joueur
    tab[joueur][reponse]=score;
    majfeuille(tab, joueur);
}

/**
 * 
 * \fn int verifCombinaison(tabdes tab, int choix)
 * 
 * \brief Fonction qui vérifie si les dés peuvent correspondre au choix de la combinaison du joueur
 * 
 * \param tab: le tableau des valeurs des dés
 * \param choix : entier correspondant à l'indice attribué à l'une des combinaisons 
 * 
 * \return : -1 si la combinaison n'est pas valide avec les dés sinon un entier correspondant au résultat
 * obtenu avec la combinaison affectée.
 * 
 * Grâce au choix entré en paramètre, vérifie si la combinaison marche.
 * 
 */
int verifCombinaison(tabdes tab, int choix)
{
    int resultat, i, j, compteur, val1, val2, compteur1, compteur2, doublon; 
    bool valide;
    const tabdes grande_suite_1={1,2,3,4,5};
    const tabdes grande_suite_2={2,3,4,5,6};
    const tabdes exception_petite_suite_1={1,3,4,5,6};
    switch (choix)
    {
    // Vérification du total de 1
    case 0:
        compteur=0;
        for(i=0;i<DES;i++)
        {
            if(tab[i]==1)
            {
                compteur=compteur+1;
            }
        }
        resultat=compteur*1;
        if (resultat==0)
        {
            resultat=(-1);
        }
        return resultat;
        break;

    // Vérification du total de 2
    case 1:
        compteur=0;
        for(i=0;i<DES;i++)
        {
            if(tab[i]==2)
            {
                compteur=compteur+1;
            }
        }
        resultat=compteur*2;
        if (resultat==0)
        {
            resultat=(-1);
        }
        return resultat;
        break;

    // Vérification du total de 3
    case 2:
        compteur=0;
        for(i=0;i<DES;i++)
        {
            if(tab[i]==3)
            {
                compteur=compteur+1;
            }
        }
        resultat=compteur*3;
        if (resultat==0)
        {
            resultat=(-1);
        }
        return resultat;
        break;

    // Vérification du total de 4
    case 3:
        compteur=0;
        for(i=0;i<DES;i++)
        {
            if(tab[i]==4)
            {
                compteur=compteur+1;
            }
        }
        resultat=compteur*4;
        if (resultat==0)
        {
            resultat=(-1);
        }
        return resultat;
        break;

    // Vérification du total de 5
    case 4:
        compteur=0;
        for(i=0;i<DES;i++)
        {
            if(tab[i]==5)
            {
                compteur=compteur+1;
            }
        }
        resultat=compteur*5;
        if (resultat==0)
        {
            resultat=(-1);
        }
        return resultat;
        break;

    // Vérification du total de 6
    case 5:
        compteur=0;
        for(i=0;i<DES;i++)
        {
            if(tab[i]==6)
            {
                compteur=compteur+1;
            }
        }
        resultat=compteur*6;
        if (resultat==0)
        {
            resultat=(-1);
        }
        return resultat;
        break;

    // Vérification du Brelan
    case 8:
        valide=false;
        resultat=(-1);
        compteur=0;
        i=1;
        while(i<=6)
        {
            for(j=0;j<DES;j++)
            {
                if(tab[j]==i)
                {
                    compteur=compteur+1;
                    if(compteur==3)
                    {
                        valide=true;
                        i=i+10;
                    }
                }
                if(j==4)
                {
                    compteur=0;
                }
            }
            i=i+1;
        }
        if (valide)
        {
            for(i=0;i<DES;i++)
            {
                resultat=resultat+tab[i];
            }
            resultat=resultat+1;
        }
        return resultat;
        break;

    // Vérification du Carré
    case 9:
        valide=false;
        resultat=(-1);
        compteur=0;
        i=1;
        while(i<=6)
        {
            for(j=0;j<DES;j++)
            {
                if(tab[j]==i)
                {
                    compteur=compteur+1;
                    if(compteur==4)
                    {
                        valide=true;
                        j=j+10;
                        i=i+10;
                    }
                }
                if(j==4)
                {
                    compteur=0;
                }
            }
            i=i+1;
        }
        if (valide)
        {
            for(i=0;i<DES;i++)
            {
                resultat=resultat+tab[i];
            }
            resultat=resultat+1;
        }
        return resultat;
        break;

    // Vérification du Full House
    case 10:
        resultat=(-1);
        compteur1=0;
        compteur2=0;
        val1=tab[0];
        val2=0;
        i=1;
        while ( (val2==0) && (i<6) )
        {
            if (val1!=tab[i])
            {
                val2=tab[i];
            }
            i=i+1;
        }
        for(i=0;i<DES;i++)
        {
            if (tab[i]==val1)
            {
                compteur1=compteur1+1;
            }
            else if (tab[i]==val2)
            {
                compteur2=compteur2+1;
            }
        }
        if  ( ( (compteur1==2)||(compteur1==3) ) && ( (compteur2==2) || (compteur2==3) ) )
        {
            resultat=25;
        }
        return resultat;
        break;

        // Vérification de la petite suite
        case 11:
            resultat=(-1);
            tri_des(tab, 4);
            for(i=0;i<DES;i++)
            {
                doublon=detecterdoublon(tab, 4, tab[i]);
                if(doublon!=(-1))
                {
                    i=i+10;
                    tab[doublon]=20;
                }
            }
            tri_des(tab, 4);
            if(doublon==(-1))
            {
                for(i=0;i<DES-1;i++)
                {
                    if(tab[i+1]-tab[i]==1)
                    {
                        resultat=30;
                    }
                    else if ( ( (tab[i]==4) && (tab[i]-tab[i-1]==1) ) || ( (tab[i]==5) && (tab[i]-tab[i-1]==1) ) || ( (tab[i]==6) && (tab[i]-tab[i-1]==1) ) )
                    {
                        resultat=30;
                    }
                    else
                    {
                        resultat=(-1);
                    }
                }
            } 
            else
            {
                for(i=0;i<DES-1;i++)
                {
                    if(tab[i+1]-tab[i]==1)
                    {
                        resultat=30;
                    }
                    else if ( ( (tab[i]==4) && (tab[i]-tab[i-1]==1) ) || ( (tab[i]==5) && (tab[i]-tab[i-1]==1) ) || ( (tab[i]==6) && (tab[i]-tab[i-1]==1) ) )
                    {
                        resultat=30;
                    }
                    else if(tab[i]==exception_petite_suite_1[i])
                    {
                        resultat=30;
                    }
                    else
                    {
                        resultat=(-1);
                    }
                }
            }
            return resultat;
            break;

        // Vérification de la grande suite
        case 12:
            resultat=(-1);
            tri_des(tab, 4);
            for(i=0;i<DES;i++)
            {
                if(tab[0]==1)
                {
                    if(tab[i]==grande_suite_1[i])
                    {
                        resultat=40;
                    }
                    else
                    {
                        resultat=(-1);
                        i=i+10;
                    }
                }
                else if(tab[0]==2)
                {
                    if(tab[i]==grande_suite_2[i])
                    {
                        resultat=40;
                    }
                    else
                    {
                        resultat=(-1);
                        i=i+10;
                    }
                }
            }
            return resultat;
            break;

        // Vérification du Yams
        case 13:
            resultat=(-1);
            for(i=0;i<DES;i++)
            {
                if(tab[i]==tab[i+1])
                {
                    resultat=50;
                }
                else
                {
                    i=i+10;
                }
            }
            return resultat;
            break;

        // Vérification de la Chance
        case 14:
            resultat=0;
            for(i=0;i<DES;i++)
            {
                resultat=resultat+tab[i];
            }
            return resultat;
            break;

        default:
            break;
        }
        return EXIT_SUCCESS;
}

/**
 * 
 * \fn void majfeuille(tabfeuille tab,  int j)
 * 
 * \brief Procédure qui permet de mettre à jour la feuille de marque du joueur
 * 
 * \param tab : la feuille de marque
 * \param j : indice qui permettra d'afficher la bonne valeur en cherchant dans le tableau
 * 
 * Met à jour les totaux dans la feuille de marque. Si la dernière valeur ajoutée est (-1), cela n'affectera pas le total.
 * 
 */
void majfeuille(tabfeuille tab,  int j)
{
    int i, compteur;
    compteur=0;
    for(i=0;i<=5;i++)
    {
        if (tab[j][i]!=(-1))
        {
            compteur=compteur+tab[j][i];
            tab[j][7]=tab[j][7]+tab[j][i];
        }
    }
    if(compteur>62)
    {
        tab[j][6]=35;    
    }
    else
    {
        tab[j][6]=(-1);
    }
    for(i=8;i<=14;i++)
    {
        if (tab[j][i]!=(-1))
        {
            tab[j][15]=tab[j][15]+tab[j][i];
        }
    }
    tab[j][16]=tab[j][7]+tab[j][15];
}

/**
 * 
 * \fn void afficheResultat(tabfeuille tab, char joueur1[19], char joueur2[19])
 * 
 * \brief Procédure qui affiche le gagnant de la partie
 * 
 * \param tab: le tableau de la feuille de marque
 * \param joueur1: le nom du joueur 1
 * \param joueur2: le nom du joueur 2
 * 
 * Si le score du joueur 1 est supérieur à celui du joueur 2, cela affiche le joueur 1 comme gagnant, si c'est l'inverse, cela affiche le joueur 2 comme gagnant, sinon cela affiche égalité.
 * 
 */
void afficheResultat(tabfeuille tab, char joueur1[19], char joueur2[19])
{
    int score1, score2;
    score1=tab[0][16];
    score2=tab[1][16];
    if(score1>score2)
    {
        printf("%s est le vainqueur de la partie !\n", joueur1);
    }
    else if(score2>score1)
    {
        printf("%s est le vainqueur de la partie !\n", joueur2);
    }
    else if(score1==score2)
    {
        printf("Egalité !\n");
    }
}

/**
 * 
 * \fn void verifvalde(int x)
 * 
 * \brief Procédure qui permet de vérifier si la valeur entrée correspond à une valeur d'un dé (entre 1 et 6)
 * 
 * \param x: la valeur vérifiée
 * 
 */
void verifvalde(int x)
{
    if ( (x<1) || (x>6) )
    {
        do
        {
            printf("La valeur doit être comprise entre 1 et 6 :\n");
            scanf("%d", &x);
        } while ( (x<1) || (x>6) );
    }
    printf("Valide !\n");
}

/**
 * 
 * \fn void tri_des(tabdes t)
 * 
 * \brief Procédure qui trie dans l'ordre croissant les dés
 * 
 * \param tab : le tableau des dés
 * \param taille : la taille du tableau
 * 
 * Reprend le principe du tri insertion vu en cours.
 * 
 */
void tri_des(tabdes tab, int taille)
{
    int i, plus_petit, j, temp;
 
    for(i=0;i<DES-1;i++)
    {
        plus_petit = i;
        for (j=i;j<DES;j++)
        {
            if (tab[j] < tab[plus_petit])
            {
                plus_petit=j;
            }   
        }
        temp = tab[i];
        tab[i] = tab[plus_petit];
        tab[plus_petit] = temp;
    }
}

/**
 * 
 * \fn void remiseazero(tabdes tab)
 * 
 * \brief Procédure qui remplit un tableau de dés de 0.
 * 
 * \param tab: le tableau des dés
 * 
 */
void remiseazero(tabdes tab)
{
    int i;
    for(i=0;i<DES;i++)
    {
        tab[i]=0;
    }
}

/**
 * 
 * \fn int detecterdoublon(tabdes tab, int nbElem, int elem)
 * 
 * \brief Procédure qui détecte s'il y a un doublon dans un tableau
 * 
 * \param tab: le tableau des dés
 * \param nbElem: le  nombre d'éléments dans le tableau
 * \param elem: l'entier recherché
 * 
 * \return: un entier correspondant au doublon qui est dans le tableau
 * 
 */
int detecterdoublon(tabdes tab, int nbElem, int elem)
{
    int i, j;
    for(i=0;i<nbElem;i++)
    {
        if(tab[i]==elem)
        {
            for(j=i+1;j<nbElem;j++)
            {
                if(tab[j]==elem)
                {
                    return j;
                }
            }
        }    
    }
    return -1;
}