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

    while (fgets(ligne, sizeof(ligne), fichier)) {
        RemplacerTiretParZero(ligne);
        if (DEBUG) printf("DEBUG: Ligne lue = '%s'\n", ligne);

        char station_id[256] = {0};
        char station_type[16] = {0};
        long capacity = 0, load = 0;

        if (strcmp(filter_type, "hvb") == 0) {
            if (DEBUG) printf("DEBUG: Filtre HVB activé.\n");
            if (sscanf(ligne, "%*[^;];%255[^;];%*[^;];%*[^;];%*[^;];%*[^;];%ld;%ld", station_id, &capacity, &load) == 3) {
                strcpy(station_type, "HVB");
                if (DEBUG) printf("DEBUG: Ligne HVB valide. ID: %s, Capacity: %ld, Load: %ld\n", station_id, capacity, load);
            } else {
                if (DEBUG) printf("DEBUG: Ligne HVB ignorée : '%s'\n", ligne);
                continue;
            }
        } else if (strcmp(filter_type, "hva") == 0) {
            if (DEBUG) printf("DEBUG: Filtre HVA activé.\n");
            if (sscanf(ligne, "%*[^;];%*[^;];%255[^;];%*[^;];%*[^;];%*[^;];%ld;%ld", station_id, &capacity, &load) == 3) {
                strcpy(station_type, "HVA");
                if (DEBUG) printf("DEBUG: Ligne HVA valide. ID: %s, Capacity: %ld, Load: %ld\n", station_id, capacity, load);
            } else {
                if (DEBUG) printf("DEBUG: Ligne HVA ignorée : '%s'\n", ligne);
                continue;
            }
        } else if (strcmp(filter_type, "lv") == 0) {
            if (DEBUG) printf("DEBUG: Filtre LV activé.\n");
            if (sscanf(ligne, "%*[^;];%*[^;];%*[^;];%255[^;];%*[^;];%*[^;];%ld;%ld", station_id, &capacity, &load) == 3) {
                strcpy(station_type, "LV");
                if (DEBUG) printf("DEBUG: Ligne LV valide. ID: %s, Capacity: %ld, Load: %ld\n", station_id, capacity, load);
            } else {
                if (DEBUG) printf("DEBUG: Ligne LV ignorée : '%s'\n", ligne);
                continue;
            }
        } else {
            if (DEBUG) printf("DEBUG: Type de filtre inconnu : '%s'. Ligne ignorée : '%s'\n", filter_type, ligne);
            continue;
        }

        // Vérifiez si la ligne contient des données valides
        if (capacity > 0 || load > 0) {
            Donnees *donnee = malloc(sizeof(Donnees));
            if (!donnee) {
                perror("Erreur d'allocation mémoire");
                fclose(fichier);
                return racine;
            }

            donnee->station_type = strdup(station_type);
            donnee->station_id = strdup(station_id);
            donnee->total_capacity = capacity;
            donnee->total_load = load;

            racine = inserer_avl(racine, station_id, donnee);
            if (DEBUG) printf("DEBUG: Donnée insérée dans AVL : ID: %s, Capacity: %ld, Load: %ld\n", station_id, capacity, load);
        } else {
            if (DEBUG) printf("DEBUG: Données invalides ou vides. Ligne ignorée : '%s'\n", ligne);
        }
    }

    fclose(fichier);
    return racine;
}




void ecrire_avl(NoeudAVL *racine, FILE *fichier) {
    if (racine != NULL) {
        ecrire_avl(racine->gauche, fichier);
        fprintf(fichier, "%s;%ld;%ld\n", racine->cle, racine->valeurs[0]->total_capacity, racine->valeurs[0]->total_load);
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
