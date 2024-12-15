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

    if (argc != 4) {
        fprintf(stderr, "Erreur : Nombre d'arguments incorrect : %d\n", argc);
        fprintf(stderr, "Usage: %s <fichier_entree.dat> <fichier_sortie.dat> <has_header>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *fichier_entree = argv[1];
    const char *fichier_sortie = argv[2];
    int has_header = atoi(argv[3]);

    // Vérification des cas interdits
    if (strstr(fichier_sortie, "hvb_all") || strstr(fichier_sortie, "hvb_indiv") ||
        strstr(fichier_sortie, "hva_all") || strstr(fichier_sortie, "hva_indiv")) {
        fprintf(stderr, "Erreur : Cas interdit détecté dans les options de sortie (%s).\n", fichier_sortie);
        return EXIT_FAILURE;
    }

    if (DEBUG) {
        printf("DEBUG: Chemin d'entrée : %s\n", fichier_entree);
    }

    if (access(fichier_entree, F_OK) != 0) {
        perror("Erreur : Le fichier d'entrée est inaccessible");
        return EXIT_FAILURE;
    }

    NoeudAVL *arbre = charger_dat_dans_avl(fichier_entree, has_header);
    if (!arbre) {
        fprintf(stderr, "Erreur : Arbre AVL non généré\n");
        return EXIT_FAILURE;
    }

    long somme = somme_avl(arbre);
    printf("Consommation totale (Capacity) : %ld\n", somme);

    if (!generer_fichier_sortie(fichier_sortie, arbre)) {
        fprintf(stderr, "Erreur : Impossible de générer le fichier de sortie\n");
        liberer_avl(arbre);
        return EXIT_FAILURE;
    }

    liberer_avl(arbre);
    printf("Traitement terminé. Résultats enregistrés dans %s\n", fichier_sortie);
    return EXIT_SUCCESS;
}
