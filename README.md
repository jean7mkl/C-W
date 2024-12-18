# Analyse de Capacite et Visualisation des Donnees

Ce projet permet de filtrer, traiter et visualiser des donnees en utilisant un programme C pour calculer les capacites, et GnuPlot pour generer des graphiques pertinents.

## **Fonctionnalites**
- Calcul de la capacite en fonction des stations LV.
- Generation de graphiques via **GnuPlot**.
- Gestion d'entrees multiples avec des resultats dans des fichiers de sortie.
- Robustesse a travers la gestion des erreurs (fichiers invalides, arguments incorrects).

## **Prerequis**

### **Environnement de developpement**
- **GCC** : Compilateur C
- **GnuPlot** : Visualisation des donnees
- **Dr. Memory** (Windows) ou **Valgrind** (Linux) : Test des fuites memoires

### **Installation des outils**
#### **Sous Linux (WSL)** :
```bash
sudo apt-get update
sudo apt-get install gcc gnuplot valgrind
```

#### **Sous Windows** :
- Telecharge et installe [GnuPlot](http://www.gnuplot.info/).
- Utilise [Dr. Memory](https://drmemory.org/) pour tester les fuites memoires.

## **Compilation**
Pour compiler le programme, utilise la commande suivante :
```bash
gcc -o main main.c -lm
```

## **Execution**
Pour executer le programme, lance la commande suivante :
```bash
./main <fichier_entree> <fichier_sortie> <mode>
```

### **Arguments**
- `<fichier_entree>` : Chemin vers le fichier contenant les donnees.
- `<fichier_sortie>` : Fichier ou les resultats sont sauvegardes.
- `<mode>` : Mode d'execution :
  - `1` : Mode **LV All**
  - `2` : Mode **HVA Comp**

### **Exemple**
```bash
./main ../tmp/filtered.dat output/output_lv.dat 1
```

## **Visualisation des graphiques**
Pour generer et afficher les graphiques, utilise **GnuPlot** :
```gnuplot
load "scripts/plot_capacity.gnu"
```

### **Graphiques generes**
1. **Capacite vs Station LV**
2. **Capacites Min et Max**

## **Tests des Fuites Memoires**
- **Sous Linux (Valgrind)** :
  ```bash
  valgrind --leak-check=full ./main ../tmp/filtered.dat output/output_lv.dat 1
  ```

- **Sous Windows (Dr. Memory)** :
  ```bash
  drmemory.exe -- ./main ../tmp/filtered.dat output/output_lv.dat 1
  ```

## **Structure du projet**
```bash
.
├── main.c                   # Code source principal
├── Makefile                 # Automatisation de la compilation
├── tmp/                     # Fichiers temporaires
├── output/                  # Fichiers de sortie
├── scripts/                 # Scripts GnuPlot
└── README.md                # Documentation
```

## **Ameliorations futures**
- Ajout d'autres modes de calcul.
- Optimisation des performances pour des donnees volumineuses.
- Support multiplateforme ameliore.

## **Auteurs**
- **Ton Nom**
- **Ton Ecole/Entreprise**

## **Licence**
Ce projet est sous licence MIT.