#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

#define DEBUG 1 // Activez ou désactivez les messages de débogage

// Déclare la fonction pour charger les données dans un arbre AVL
NoeudAVL *charger_dat_dans_avl(const char *nom_fichier, const char *filter_type);
void ecrire_avl(NoeudAVL *racine, FILE *fichier);
// Déclare la fonction pour générer un fichier de sortie à partir d'un arbre AVL
int generer_fichier_sortie(const char *nom_fichier, NoeudAVL *racine);

#endif // IO_H