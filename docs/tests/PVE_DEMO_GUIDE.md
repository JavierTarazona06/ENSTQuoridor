# Guide de Démonstration PVE (Joueur vs IA)

Ce document explique comment compiler et exécuter la démo visuelle PVE (Player vs AI) pour tester le moteur d'intelligence artificielle.

---

## 📋 Vue d'ensemble

Le `test_pve_visual` est un exécutable autonome qui permet de jouer au Quoridor contre l'IA en utilisant l'interface graphique existante. Cette démo est conçue pour vérifier le fonctionnement du moteur IA sans modifier le code du projet principal.

---

## 🔨 Compilation de la démo

Le `test_pve_visual` est compilé automatiquement avec tous les autres targets lors de l'exécution des scripts de bootstrap.

### Méthode 1 : Bootstrap en une commande (Recommandé)

C'est la méthode la plus simple pour compiler l'ensemble du projet, y compris la démo PVE.

**Windows (PowerShell) :**
```powershell
# Depuis la racine du dépôt
scripts\bootstrap.ps1 -Config Debug
```

**Linux/macOS (Bash) :**
```bash
# Depuis la racine du dépôt
chmod +x scripts/bootstrap.sh
scripts/bootstrap.sh Debug
```

### Méthode 2 : Compiler uniquement la démo PVE

Si le projet est déjà configuré, vous pouvez compiler uniquement la cible PVE :

**Windows :**
```powershell
cmake --build build/x64-windows --target test_pve_visual --config Debug
```

**Linux :**
```bash
cmake --build build/x64-linux --target test_pve_visual
```

**macOS (Apple Silicon) :**
```bash
cmake --build build/arm64-osx --target test_pve_visual
```

**macOS (Intel) :**
```bash
cmake --build build/x64-osx --target test_pve_visual
```

### Méthode 3 : Configuration et compilation manuelles

Pour les utilisateurs avancés souhaitant un contrôle total :

**Windows (Visual Studio) :**
```powershell
cd C:\chemin\vers\ENSTQuoridor
cmake --preset x64-windows
cmake --build build/x64-windows --target test_pve_visual --config Debug
```

**Linux :**
```bash
cd /chemin/vers/ENSTQuoridor
cmake --preset x64-linux
cmake --build build/x64-linux --target test_pve_visual
```

**macOS :**
```bash
cd /chemin/vers/ENSTQuoridor
cmake --preset arm64-osx    # Apple Silicon
# ou : cmake --preset x64-osx  # Intel
cmake --build build/arm64-osx --target test_pve_visual
```

---

## ▶️ Exécution de la démo

### Emplacements des exécutables

**Windows :**
```powershell
.\build\x64-windows\tests\Debug\test_pve_visual.exe
# Ou pour Release :
.\build\x64-windows\tests\Release\test_pve_visual.exe
```

**Linux :**
```bash
./build/x64-linux/tests/test_pve_visual
```

**macOS :**
```bash
./build/arm64-osx/tests/test_pve_visual    # Apple Silicon
./build/x64-osx/tests/test_pve_visual      # Intel
```

---

## 🎮 Contrôles du jeu

### Mode déplacement (par défaut)

| Action | Description |
|--------|-------------|
| Clic sur votre pion | Sélectionner le pion |
| Clic sur destination | Déplacer le pion vers cette case |
| Clic sur le pion sélectionné | Désélectionner le pion |

### Mode placement de mur

| Touche | Action |
|--------|--------|
| `W` | Activer/désactiver le mode placement de mur |
| `O` | Tourner le mur (horizontal ↔ vertical) |
| Déplacement souris | Aperçu de la position du mur |
| Clic gauche | Placer le mur à la position de l'aperçu |

### Sélection de la difficulté IA

| Touche | Difficulté | Description |
|--------|------------|-------------|
| `1` | Facile | Profondeur 1, bruit ±40 |
| `2` | Normal | Profondeur 1, bruit ±8 |
| `3` | Difficile | Profondeur 2, bruit ±3 |
| `4` | Enfer | Profondeur 4, aucun bruit |

### Autres contrôles

| Touche | Action |
|--------|--------|
| `R` | Réinitialiser la partie |
| Fermer la fenêtre | Quitter le jeu |

---

## 🎯 Système de difficulté

La difficulté de l'IA est contrôlée par deux facteurs :

1. **Profondeur de recherche** : Nombre de coups analysés à l'avance
2. **Injection de bruit** : Valeur aléatoire ajoutée aux scores d'évaluation

| Difficulté | Profondeur | Bruit | Taux d'erreur | Comportement |
|------------|------------|-------|---------------|--------------|
| **Facile** | 1 | ±40 | ~50% | Voit les coups immédiats mais choisit souvent au hasard |
| **Normal** | 1 | ±8 | ~15% | Voit les coups immédiats, erreurs occasionnelles |
| **Difficile** | 2 | ±3 | ~5% | Planifie à l'avance, erreurs rares |
| **Enfer** | 4 | 0 | 0% | Calcul profond avec évaluation parfaite |

### Pourquoi le bruit ?

- Sans bruit, même une IA de profondeur 1 jouerait toujours parfaitement dans son horizon
- Le bruit fait que l'IA choisit parfois des coups sous-optimaux, simulant des erreurs humaines
- Plus de bruit = plus d'"erreurs" = plus facile à battre

### Conseils stratégiques

- **Facile** : Idéal pour apprendre les mécaniques du jeu — l'IA fait de nombreux choix aléatoires
- **Normal** : Battable par la plupart des joueurs — concentrez-vous sur l'avancement tout en bloquant occasionnellement
- **Difficile** : Nécessite une planification soignée — contrôlez le centre et utilisez les murs stratégiquement
- **Enfer** : Niveau expert — uniquement gagnable avec un jeu optimal et un peu de chance

---

## 📜 Règles du jeu

- **Joueur 1 (Humain)** : Pion rouge, commence en haut, objectif rangée du bas
- **Joueur 2 (IA)** : Pion bleu, commence en bas, objectif rangée du haut
- Chaque joueur dispose de 10 murs à placer
- Les murs ne peuvent pas bloquer complètement le chemin d'un joueur vers son objectif
- Le premier joueur à atteindre sa rangée d'objectif gagne

---

## 🔧 Détails techniques

### Architecture

La démo réutilise les composants existants suivants :
- `Board` — Gestion de l'état du plateau
- `State` — État du jeu (joueur courant, statut de la partie)
- `Rules` — Validation des mouvements et des murs
- `AI` — Moteur de décision IA
- `Render2D` — Rendu SFML
- `GameView` — Visualisation du jeu

### Implémentation de l'IA

L'IA utilise l'algorithme Minimax avec élagage Alpha-Bêta et injection de bruit pour l'équilibrage de la difficulté :

| Difficulté | Profondeur | Niveau de bruit | Temps de réponse typique |
|------------|------------|-----------------|--------------------------|
| Facile | 1 | ±40 | ~150μs |
| Normal | 1 | ±8 | ~150μs |
| Difficile | 2 | ±3 | ~800μs |
| Enfer | 4 | 0 | ~8-10ms |

### Fonction d'évaluation

L'IA évalue les positions du plateau en utilisant 5 facteurs pondérés :

| Facteur | Poids | Description |
|---------|-------|-------------|
| Différence de distance | ×10 | Différence de plus court chemin vers l'objectif |
| Bonus fin de partie | ×15 | Valeur supplémentaire proche de la victoire |
| Avantage en murs | ×2 | Avoir plus de murs que l'adversaire |
| Mobilité | ×3 | Nombre de mouvements disponibles |
| Efficacité des murs | ×5 | Murs qui bloquent l'adversaire vs murs gaspillés |

### Structure des fichiers

```
tests/
├── test_pve_visual.cpp    # Code source de la démo PVE
└── CMakeLists.txt         # Configuration de build (inclut la cible test_pve_visual)
```

---

## 🐛 Dépannage

### Problèmes généraux

- **"Could not find toolchain file"** : Assurez-vous que `VCPKG_ROOT` est défini ou laissez les scripts de bootstrap s'en charger
- **Si la compilation échoue** : Supprimez le dossier build et relancez le bootstrap
  - Windows : `Remove-Item -Recurse -Force .\build\x64-windows; scripts\bootstrap.ps1 -Config Debug`
  - Linux/macOS : `rm -rf build; scripts/bootstrap.sh Debug`

### SFML introuvable

Assurez-vous que vcpkg est correctement configuré et que les dépendances sont installées :

**macOS :**
```bash
cmake --preset arm64-osx   # Ceci devrait installer SFML via vcpkg automatiquement
```

**Linux :**
```bash
# Option 1 : Utiliser vcpkg (recommandé)
cmake --preset x64-linux

# Option 2 : Installer SFML au niveau système
sudo apt-get install libsfml-dev    # Ubuntu/Debian
sudo dnf install SFML-devel         # Fedora
sudo pacman -S sfml                  # Arch Linux
```

### La fenêtre n'apparaît pas

**Spécifique Linux :** Assurez-vous qu'un serveur d'affichage est en cours d'exécution (X11 ou Wayland).
```bash
echo $DISPLAY   # Devrait afficher quelque chose comme ":0"
```

**Spécifique Windows :** Assurez-vous que les DLLs SFML sont dans votre PATH ou dans le même répertoire que l'exécutable.

### Linux : "cannot open shared object file"

```bash
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
sudo ldconfig
```

### Windows : Erreurs de DLL manquantes

1. Copiez les DLLs SFML depuis `build/vcpkg_installed/x64-windows/bin/` vers le répertoire de l'exécutable
2. Ou ajoutez le répertoire bin de vcpkg à votre PATH

### Erreurs de chargement de polices

La démo nécessite les polices du répertoire `assets/fonts/`. Assurez-vous que :
1. Le dossier `assets` existe à la racine du projet
2. Les fichiers de police sont présents dans `assets/fonts/arial/`
3. Vous exécutez l'exécutable depuis le répertoire racine du projet

### L'IA prend trop de temps

Essayez de baisser le niveau de difficulté (appuyez sur `1` ou `2`). Le mode Enfer (profondeur 4) peut prendre plus de temps dans les états de plateau complexes.

---

## ✅ Liste de vérification

Utilisez cette démo pour vérifier :

- [ ] L'IA répond correctement aux mouvements du joueur
- [ ] L'IA effectue des mouvements valides (pions et murs)
- [ ] Les différents niveaux de difficulté montrent des comportements différents
- [ ] La détection de victoire fonctionne pour les deux joueurs
- [ ] La validation du placement des murs fonctionne correctement
- [ ] La réinitialisation du jeu fonctionne correctement

---

## 📚 Voir aussi

- [Guide d'installation](../installation/INSTALLATION.md) — Instructions de compilation et d'exécution
- [Guide utilisateur](../USER_GUIDE.md) — Guide utilisateur complet
- [Documentation IA](../AI_DOCU.md) — Détails de l'algorithme IA
- [Architecture](../planning/architecture/architecture_quoridor.pdf) — Architecture du projet

---

**Dernière mise à jour** : 25 janvier 2026
