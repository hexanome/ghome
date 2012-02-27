/*************************************************************************
                           GTHREAD.H  -  Description
                             -------------------
    début                : 31/01/2012
    copyright            : (C) 2012 par H4111
*************************************************************************/

/*---------- Interface du fichier gthread.c -------*/
/*------------------------------------------------------------------------
Ces fonctions permettent de creer des nouvelles tâches
L'ordonnanceur est un Round Robin
//------------------------------------------------------------------------*/

/*------------------------------------------------------------------ Types*/
/*Le type de la fonction à appeller, elle ne renvoit rien et à des arguments de types void* */
typedef void (func_t)(void *);

/*------------------------------------------------------------- Fonctions */
/*
 * Permet de creer un contexte pour l'ordonnanceur
 * stack_size correspond à la taille de la pile d'execution (16384 est conseillé si vous ne savez pas quoi mettre)
 * f correspond à la fonction qui va être appellé par l'ordonnanceur
 * args correspond aux arguments à envoyé avec la fonction f
 */
int create_ctx(int stack_size, func_t f, void *args);
/*
 * Permet lancer l'ordonnanceur
 * Le changement de contexte se fera toutes les 8ms.
 */
void start_sched();

