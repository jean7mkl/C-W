#ifndef AVL_H
#define AVL_H

#include <stdlib.h>

typedef struct Donnees {
    char *station_type;  // Type de station (HVB, HVA, LV)
    char *station_id;    // ID de la station
    long total_capacity; // Capacité totale de la station
    long total_load;     // Charge totale de la station
} Donnees;

typedef struct NoeudAVL {
    const char *cle;
    Donnees **valeurs;
    int taille;
    int capacite;
    int hauteur;
    struct NoeudAVL *gauche;
    struct NoeudAVL *droite;
} NoeudAVL;

NoeudAVL *inserer_avl(NoeudAVL *racine, const char *cle, Donnees *valeur);
void afficher_avl(NoeudAVL *racine);
void liberer_avl(NoeudAVL *racine);
long somme_avl(NoeudAVL *racine);
long somme_loads(NoeudAVL *racine);
long somme_loads(NoeudAVL *racine);


#endif