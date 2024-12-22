Projet informatique du premier semestre de Pré-ing 2. 

1-Placer le dossier C-wire dans votre dossier personnel et placer votre fichier au format .csv à traiter, dans le fichier input de C-wire (1 seule fichier au format .csv à traiter)

2-Ouvrir le terminal, lancer le script c-wire.sh en utilisant : ./C-wire/c-wire.sh <type de station à traiter> <type de consommateur à traiter> <facultatif: Identifiant de centrale (nombre entier)>

Stations disponibles:
	-hvb
        -hva
        -lv
Consommateurs disponibles:
        -comp
        -indiv
        -all
Exemples de saisis de commande:
        ./C-wire/c-wire.sh hvb comp
        ./C-wire/c-wire.sh lv all 1
        
3-Pour afficher un message d'aide utiliser l'argument -h  avec c-wire.sh (si présent, tous les autres arguments sont ignorées, qu'importe la position de l'argument) en utilisant : ./C-wire/c-wire.sh <-h>

Exemples de saisis de <-h>:
	./C-wire/c-wire.sh -h
	
4-Les nouveaux fichiers de données crées lors des traitements apparaissent dans le répertoire C-wire et les graphiques dans le répertoire graphs de C-wire
