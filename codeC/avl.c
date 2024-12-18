#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "avl.h"

#define DEBUG 1 // Activez ou désactivez les messages de débogage

static int hauteur(NoeudAVL *N) {
    if (N == NULL) return 0;
    return N->hauteur;
}

static int max(int a, int b) {
    return (a > b) ? a : b;
}


static NoeudAVL *nouveau_noeud(const char *cle, Donnees *valeur) {
    NoeudAVL *noeud = (NoeudAVL *)malloc(sizeof(NoeudAVL));
    if (!noeud) {
        fprintf(stderr, "Erreur : allocation mémoire échouée pour le nœud\n");
        exit(EXIT_FAILURE);
    }
    noeud->cle = strdup(cle);
    noeud->taille = 1;
    noeud->capacite = 2;
    noeud->valeurs = (Donnees **)malloc(noeud->capacite * sizeof(Donnees *));
    if (!noeud->valeurs) {
        fprintf(stderr, "Erreur : allocation mémoire échouée pour les valeurs\n");
        exit(EXIT_FAILURE);
    }
    noeud->valeurs[0] = valeur;
    noeud->hauteur = 1;
    noeud->gauche = NULL;
    noeud->droite = NULL;
    return noeud;
}


static NoeudAVL *rotation_droite(NoeudAVL *y) {
    NoeudAVL *x = y->gauche;
    NoeudAVL *T2 = x->droite;

    x->droite = y;
    y->gauche = T2;

    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;

    return x;
}

static NoeudAVL *rotation_gauche(NoeudAVL *x) {
    NoeudAVL *y = x->droite;
    NoeudAVL *T2 = y->gauche;

    y->gauche = x;
    x->droite = T2;

    x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
    y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;

    return y;
}

static int facteur_equilibre(NoeudAVL *N) {
    if (N == NULL) return 0;
    return hauteur(N->gauche) - hauteur(N->droite);

}


/* static void ajouter_valeur(NoeudAVL *noeud, Donnees *valeur) {
    if (noeud->taille == noeud->capacite) {
        noeud->capacite *= 2;
        noeud->valeurs = realloc(noeud->valeurs, noeud->capacite * sizeof(Donnees *));
        if (!noeud->valeurs) {
            fprintf(stderr, "Erreur : Réallocation de mémoire échouée\n");
            exit(EXIT_FAILURE);
        }
    }
    noeud->valeurs[noeud->taille++] = valeur;
}*/




NoeudAVL *inserer_avl(NoeudAVL *noeud, const char *cle, Donnees *valeur) {
    if (noeud == NULL) {
        return nouveau_noeud(cle, valeur);
    }

    if (strcmp(cle, noeud->cle) < 0) {
        noeud->gauche = inserer_avl(noeud->gauche, cle, valeur);
    } else if (strcmp(cle, noeud->cle) > 0) {
        noeud->droite = inserer_avl(noeud->droite, cle, valeur);
    } else {
        // Mise à jour du nœud existant
        noeud->valeurs[0]->total_capacity += valeur->total_capacity;
        noeud->valeurs[0]->total_load += valeur->total_load;
        free(valeur->station_id);
        free(valeur->station_type);
        free(valeur);
        return noeud;
    }

    noeud->hauteur = 1 + max(hauteur(noeud->gauche), hauteur(noeud->droite));
    int balance = facteur_equilibre(noeud);

    // Équilibrage de l'AVL
    if (balance > 1 && strcmp(cle, noeud->gauche->cle) < 0) return rotation_droite(noeud);
    if (balance < -1 && strcmp(cle, noeud->droite->cle) > 0) return rotation_gauche(noeud);
    if (balance > 1 && strcmp(cle, noeud->gauche->cle) > 0) {
        noeud->gauche = rotation_gauche(noeud->gauche);
        return rotation_droite(noeud);
    }
    if (balance < -1 && strcmp(cle, noeud->droite->cle) < 0) {
        noeud->droite = rotation_droite(noeud->droite);
        return rotation_gauche(noeud);
    }

    return noeud;
}


void afficher_avl(NoeudAVL *racine) {
    if (racine != NULL) {
        afficher_avl(racine->gauche);
        printf("Clé : %s, Taille : %d\n", racine->cle, racine->taille);
        for (int i = 0; i < racine->taille; i++) {
            printf("  Donnée %d - Capacity: %ld, Load: %ld\n", i + 1, racine->valeurs[i]->total_capacity, racine->valeurs[i]->total_load);
        }
        afficher_avl(racine->droite);
    }
}

 long somme_avl(NoeudAVL *racine) {
    if (racine == NULL) return 0;
    long somme = 0;
    for (int i = 0; i < racine->taille; i++) {
        somme += racine->valeurs[i]->total_capacity;
    }
    return somme + somme_avl(racine->gauche) + somme_avl(racine->droite);
}


long somme_loads(NoeudAVL *racine) {
    if (racine == NULL) return 0;
    long somme = 0;
    for (int i = 0; i < racine->taille; i++) {
        somme += racine->valeurs[i]->total_load;
    }
    return somme + somme_loads(racine->gauche) + somme_loads(racine->droite);
}


// Fonction pour libérer un arbre AVL récursivement
void liberer_avl(NoeudAVL *racine) {
    if (racine != NULL) {
        liberer_avl(racine->gauche);
        liberer_avl(racine->droite);

        if (racine->valeurs) {
            for (int i = 0; i < racine->taille; i++) {
                if (racine->valeurs[i]) {
                    /*if (racine->valeurs[i]->power_plant) {
                        free((char *)racine->valeurs[i]->power_plant);
                        racine->valeurs[i]->power_plant = NULL;
                    }*/
                    free(racine->valeurs[i]);
                    racine->valeurs[i] = NULL;
                }
            }
            free(racine->valeurs);
            racine->valeurs = NULL;
        }

        if (racine->cle) {
            free((char *)racine->cle);
            racine->cle = NULL;
        }

        free(racine);
        racine = NULL;
    }
}
