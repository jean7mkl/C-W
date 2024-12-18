#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "avl.h"

#define DEBUG 1

long total_capacity = 0;
long total_load = 0;


// Replace '-' with '0'
void RemplacerTiretParZero(char *ligne) {
    for (int i = 0; ligne[i] != '\0'; i++) {
        if (ligne[i] == '-') {
            ligne[i] = '0';
        }
    }
}

// Fonction pour charger le fichier dans un arbre AVL avec un filtre spécifique
NoeudAVL *charger_dat_dans_avl(const char *nom_fichier, const char *filter_type) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        perror("Erreur d'ouverture du fichier d'entrée");
        return NULL;
    }

    char ligne[1024];
    NoeudAVL *racine = NULL;

    /*if (has_header) {
        fgets(ligne, sizeof(ligne), fichier); // Ignorer l'en-tête
        if (DEBUG) printf("DEBUG: En-tête ignoré : %s\n", ligne);
    }*/

    while (fgets(ligne, sizeof(ligne), fichier)) {
        RemplacerTiretParZero(ligne);

        char cle[256] = {0};
        long capacity = 0, load = 0;
        int champs_extraits = 0;

        // Appliquer le filtre en fonction du type
        if (strcmp(filter_type, "hvb comp") == 0) {
            champs_extraits = sscanf(ligne, "%*[^;];%255[^;];%*[^;];%*[^;];%*[^;];%*[^;];%ld;%ld", cle, &capacity, &load);
        } else if (strcmp(filter_type, "hva comp") == 0) {
            champs_extraits = sscanf(ligne, "%*[^;];%*[^;];%255[^;];%*[^;];%*[^;];%*[^;];%ld;%ld", cle, &capacity, &load);
        } else if (strcmp(filter_type, "lv comp") == 0) {
            champs_extraits = sscanf(ligne, "%*[^;];%*[^;];%*[^;];%255[^;];%*[^;];%*[^;];%ld;%ld", cle, &capacity, &load);
        } else if (strcmp(filter_type, "company") == 0) {
            champs_extraits = sscanf(ligne, "%*[^;];%*[^;];%*[^;];%*[^;];%255[^;];%*[^;];%ld;%ld", cle, &capacity, &load);
        } else {
            fprintf(stderr, "DEBUG: Type de filtre inconnu : %s\n", filter_type);
            continue;
        }

        if (DEBUG) {
            printf("DEBUG: Ligne = %s\n", ligne);
            printf("DEBUG: Champs extraits = %d, Clé = '%s', Capacity = %ld, Load = %ld\n", 
                   champs_extraits, cle, capacity, load);
        }

        // Inclure uniquement les lignes avec des valeurs significatives
        if (champs_extraits >= 3 && (capacity != 0 || load != 0)) {
            // Créer une nouvelle entrée pour l'AVL
            Donnees *valeur = (Donnees *)malloc(sizeof(Donnees));
            if (!valeur) {
                perror("Erreur d'allocation mémoire");
                fclose(fichier);
                return racine;
            }
            valeur->capacity = capacity;
            valeur->load = load;
            valeur->power_plant = strdup("Unknown");

            // Ajouter aux totaux globaux
            total_capacity += capacity;
            total_load += load;

            // Insérer dans l'AVL
            racine = inserer_avl(racine, cle, valeur);
            if (DEBUG) printf("DEBUG: Insertion : Clé = '%s', Capacity = %ld, Load = %ld\n", cle, capacity, load);
        }
    }

    printf("Total Capacity: %ld\n", total_capacity);
    printf("Total Load: %ld\n", total_load);

    fclose(fichier);
    return racine;
}





void ecrire_avl(NoeudAVL *racine, FILE *fichier) {
    if (racine != NULL) {
        ecrire_avl(racine->gauche, fichier);
        for (int i = 0; i < racine->taille; i++) {
            fprintf(fichier, "%s;%ld;%ld\n", racine->cle, racine->valeurs[i]->capacity, racine->valeurs[i]->load);
        }
        ecrire_avl(racine->droite, fichier);
    }
}

int generer_fichier_sortie(const char *nom_fichier, NoeudAVL *racine) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (!fichier) {
        perror("Erreur d'ouverture du fichier de sortie");
        return 0;
    }

    fprintf(fichier, "Key;Capacity;Load\n");
    ecrire_avl(racine, fichier);

    fclose(fichier);
    return 1;
}
