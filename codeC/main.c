#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "avl.h"
#include "io.h"

#define DEBUG 1

int main(int argc, char *argv[]) {
    if (DEBUG) {
        printf("DEBUG: argc = %d\n", argc);
        for (int i = 0; i < argc; i++) {
            printf("DEBUG: argv[%d] = '%s'\n", i, argv[i]);
        }
    }

    // Vérification du nombre d'arguments
    if (argc != 4) {
        fprintf(stderr, "Erreur : Nombre d'arguments incorrect : %d\n", argc);
        fprintf(stderr, "Usage: %s <fichier_entree.dat> <fichier_sortie.dat> <filter_type>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Affectation des arguments aux variables
    const char *fichier_entree = argv[1];
    const char *fichier_sortie = argv[2];
    const char *filter_type = argv[3];

    if (DEBUG) {
        printf("DEBUG: Chemin d'entrée : %s\n", fichier_entree);
        printf("DEBUG: Chemin de sortie : %s\n", fichier_sortie);
        printf("DEBUG: Type de filtre : %s\n", filter_type);
    }


    if (filter_type == NULL || strlen(filter_type) == 0) {
    fprintf(stderr, "Erreur : Filter type est NULL ou vide.\n");
    return EXIT_FAILURE;
    }

    // Vérification des filtres interdits dans le nom de sortie
    if (strstr(fichier_sortie, "hvb_all") || strstr(fichier_sortie, "hvb_indiv") ||
        strstr(fichier_sortie, "hva_all") || strstr(fichier_sortie, "hva_indiv")) {
        fprintf(stderr, "Erreur : Cas interdit détecté dans les options de sortie (%s).\n", fichier_sortie);
        return EXIT_FAILURE;
    }

    if (DEBUG) {
        printf("DEBUG: Chemin d'entrée : %s\n", fichier_entree);
        printf("DEBUG: Filter type : %s\n", filter_type);
    }

    // Vérification de l'accessibilité du fichier d'entrée
    if (access(fichier_entree, F_OK) != 0) {
        perror("Erreur : Le fichier d'entrée est inaccessible");
        return EXIT_FAILURE;
    }

    // Chargement des données dans l'arbre AVL avec le filtre spécifié
    NoeudAVL *arbre = charger_dat_dans_avl(fichier_entree, filter_type);
    if (!arbre) {
        fprintf(stderr, "Erreur : L'arbre AVL est vide. Vérifiez le format du fichier d'entrée %s.\n", fichier_entree);
        return EXIT_FAILURE;
    }
     // Affichage du contenu de l'AVL pour le débogage
    if (DEBUG) {
        printf("DEBUG: Contenu de l'AVL après chargement :\n");
        afficher_avl(arbre);
    }

    // Calcul des sommes pour Capacity et Load
    long somme_capacity = somme_avl(arbre);
    long somme_load = somme_loads(arbre);

    printf("Consommation totale (Capacity) : %ld\n", somme_capacity);
    printf("Charge totale (Load) : %ld\n", somme_load);

    // Génération du fichier de sortie
    if (!generer_fichier_sortie(fichier_sortie, arbre)) {
        fprintf(stderr, "Erreur : Impossible de générer le fichier de sortie\n");
        liberer_avl(arbre);
        return EXIT_FAILURE;
    }

    if (access(fichier_sortie, F_OK) == 0) {
        printf("Le fichier de sortie %s a été généré avec succès.\n", fichier_sortie);
    } else {
        fprintf(stderr, "Erreur : Le fichier de sortie n'existe pas.\n");
        liberer_avl(arbre);
        return EXIT_FAILURE;
    }

    // Libération de la mémoire
    liberer_avl(arbre);
    printf("Traitement terminé. Résultats enregistrés dans %s\n", fichier_sortie);
    return EXIT_SUCCESS;
}
