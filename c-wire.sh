#!/bin/bash

# Aide utilisateur
if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    echo "Usage: $0 <chemin_dat> <type_station> <type_consommateur> [<id_centrale>] [OPTIONS]"
    echo "OPTIONS :"
    echo "  --sort             Trier les données filtrées par capacité décroissante."
    echo "  --keep-tmp         Ne pas supprimer les fichiers temporaires."
    echo "  --debug            Activer les messages de débogage."
    exit 0
fi

# Chronométrage global
START_TIME=$(date +%s)

# Vérification des arguments
if [[ $# -lt 3 ]]; then
    echo "Erreur : Arguments insuffisants."
    echo "Usage: $0 <chemin_dat> <type_station> <type_consommateur> [<id_centrale>] [OPTIONS]"
    exit 1
fi

chemin_dat=$(realpath "$1")
type_station="$2"
type_consommateur="$3"
shift 3

id_centrale=""
keep_tmp=false
sort_data=false
debug=false

# Si un quatrième argument est fourni, c'est l'ID de centrale
if [[ $# -gt 0 && "$1" =~ ^[0-9]+$ ]]; then
    id_centrale="$1"
    shift
fi

# Gestion des options restantes
while [[ $# -gt 0 ]]; do
    case "$1" in
        --keep-tmp) keep_tmp=true ;;
        --sort) sort_data=true ;;
        --debug) debug=true ;;
        *) echo "Option inconnue : $1"; exit 1 ;;
    esac
    shift
done


# Vérification du fichier d'entrée
if [[ ! -f "$chemin_dat" ]]; then
    echo "Erreur : Le fichier $chemin_dat n'existe pas."
    exit 1
fi

# Préparation des fichiers temporaires
mkdir -p tmp
fichier_filtre="tmp/filtered.txt"

# Validation et nettoyage du fichier
echo "Validation et nettoyage du fichier d'entrée..."
awk -F';' '
    BEGIN { valid = 1 }
    NF != 8 { valid = 0; print "Erreur : Ligne invalide :", $0 > "/dev/stderr" }
    END { if (!valid) exit 1 }
' "$chemin_dat"

# Filtrage selon les critères
arg_fus="$type_station $type_consommateur"


case "$arg_fus" in
	"hvb comp")
    echo "ID centrale utilisé : $id_centrale"
	if [ -z "$id_centrale" ]; then
		station=$(grep  -P "^(\d+);(\d+);-;-;-;-;(\d+);-" $chemin_dat)
		usagers=$(grep  -P "^(\d+);(\d+);-;-;(\d+);-;-;(\d+)" $chemin_dat)
		filtre_type="hvb"
	else
		station=$(grep  -P "^($id_centrale);(\d+);-;-;-;-;(\d+);-" $chemin_dat)
		usagers=$(grep  -P "^($id_centrale);(\d+);-;-;(\d+);-;-;(\d+)" $chemin_dat)
		filtre_type="hvb"
	fi
	;;
	"hva comp")
    echo "ID centrale utilisé : $id_centrale"
	if [ -z "$id_centrale" ]; then
		station=$(grep  -P "^(\d+);(\d+);(\d+);-;-;-;(\d+);-" $chemin_dat)
		usagers=$(grep  -P "^(\d+);-;(\d+);-;(\d+);-;-;(\d+)" $chemin_dat)
		filtre_type="hva"
	else
		station=$(grep  -P "^($id_centrale);(\d+);(\d+);-;-;-;(\d+);-" $chemin_dat)
		usagers=$(grep  -P "^($id_centrale);-;(\d+);-;(\d+);-;-;(\d+)" $chemin_dat)
		filtre_type="hva"
	fi
	;;
	"lv comp")
    echo "ID centrale utilisé : $id_centrale"
	if [ -z "$id_centrale" ]; then
		station=$(grep  -P "^(\d+);-;(\d+);(\d+);-;-;(\d+);-" $chemin_dat)
		usagers=$(grep  -P "^(\d+);-;-;(\d+);(\d+);-;-;(\d+)" $chemin_dat)
		filtre_type="lv"
	else
		station=$(grep  -P "^($id_centrale);-;(\d+);(\d+);-;-;(\d+);-" $chemin_dat)
		usagers=$(grep  -P "^($id_centrale);-;-;(\d+);(\d+);-;-;(\d+)" $chemin_dat)
		filtre_type="lv"
	fi
	;;
	"lv indiv")
    echo "ID centrale utilisé : $id_centrale"
	if [ -z "$id_centrale" ]; then
		station=$(grep  -P "^(\d+);-;(\d+);(\d+);-;-;(\d+);-" $chemin_dat)
		usagers=$(grep  -P "^(\d+);-;-;(\d+);-;(\d+);-;(\d+)" $chemin_dat)
		filtre_type="lv"
	else 
		station=$(grep  -P "^($id_centrale);-;(\d+);(\d+);-;-;(\d+);-" $chemin_dat)
		usagers=$(grep  -P "^($id_centrale);-;-;(\d+);-;(\d+);-;(\d+)" $chemin_dat)
		filtre_type="lv"
	fi
	;;
	"lv all")
    echo "ID centrale utilisé : $id_centrale"
	if [ -z "$id_centrale" ]; then
		station=$(grep  -P "^(\d+);-;(\d+);(\d+);-;-;(\d+);-" $chemin_dat)
		usagers_1=$(grep  -P "^(\d+);-;-;(\d+);(\d+);-;-;(\d+)" $chemin_dat)
		usagers_2=$(grep  -P "^(\d+);-;-;(\d+);-;(\d+);-;(\d+)" $chemin_dat)
		filtre_type="lv"
	else
		station=$(grep  -P "^($id_centrale);-;(\d+);(\d+);-;-;(\d+);-" $chemin_dat)
		usagers_1=$(grep  -P "^($id_centrale);-;-;(\d+);(\d+);-;-;(\d+)" $chemin_dat)
		usagers_2=$(grep  -P "^($id_centrale);-;-;(\d+);-;(\d+);-;(\d+)" $chemin_dat)
		filtre_type="lv"
	fi
	usagers="$usagers_1$'\n'$usagers_2"
	;;
	*)
	echo "Erreur : mode $arg_fus non pris en charge."
	exit -1
esac
    echo -e "$station\n$usagers" > "$fichier_filtre"
   	

# Vérification si le fichier filtré est vide
if [[ ! -s "$fichier_filtre" ]]; then
    echo "Erreur : Aucun résultat après le filtrage."
    exit 1
fi

echo "Filtrage terminé avec succès. Les résultats sont dans $fichier_filtre."

# Compilation des fichiers C
if [[ ! -f main ]]; then
    echo "Compilation du programme C..."
    cd codeC
    make
    cd ..
    if [[ $? -ne 0 ]]; then
        echo "Erreur : Compilation échouée."
        exit 1
    fi
fi

fichier_filtre_abs=$(realpath "$fichier_filtre")
output_file_abs=$(realpath "output.dat")

if [[ "$debug" == true ]]; then
    echo "DEBUG: Chemin absolu du fichier d'entrée : $fichier_filtre_abs"
    echo "DEBUG: Chemin absolu du fichier de sortie : $output_file_abs"
    echo "DEBUG: Appel de ./main avec : ./main \"$fichier_filtre_abs\" \"$output_file_abs\" 1"
fi

# Appel de l'exécutable
EXECUTABLE="codeC/main"
./$EXECUTABLE "$fichier_filtre_abs" "$output_file_abs" "$filtre_type" 
retour_main=$?

# Gestion du fichier de sortie
if [[ $retour_main -ne 0 || ! -f "$output_file_abs" ]]; then
    echo "Erreur : Pas de fichier de sortie généré ou échec de ./main."
    exit 1
fi

mkdir -p output
mv "$output_file_abs" "output/output_${type_station}_${type_consommateur}.dat"
echo "Résultats enregistrés dans output/output_${type_station}_${type_consommateur}.dat"

# Nettoyage si demandé
if [[ "$keep_tmp" == false ]]; then
    rm -rf tmp
fi

# Temps total d'exécution
END_TIME=$(date +%s)
ELAPSED=$((END_TIME - START_TIME))
echo "Temps total : $ELAPSED secondes."