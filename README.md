# ENSTQuoridor 🎮

Implémentation d'un jeu de plateau stratégique en C++ avec SFML 3.0, proposant des adversaires IA à quatre niveaux de difficulté et une architecture MVC propre.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-3.0-green.svg)
![CMake](https://img.shields.io/badge/CMake-3.20+-orange.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)
![Tests](https://img.shields.io/badge/tests-32%20réussis-brightgreen.svg)

## 📖 À propos

**Quoridor** est un jeu de stratégie à deux joueurs sur un plateau 9×9. Les joueurs jouent à tour de rôle en déplaçant leur pion d'une case ou en plaçant un mur entre les cases. L'objectif est d'atteindre la rangée opposée tout en respectant la règle fondamentale : toujours laisser au moins un chemin valide pour chaque joueur vers son objectif.

Ce projet est une implémentation moderne en C++ proposant :
- Modes **Humain vs Humain** et **Humain vs IA**
- **Quatre niveaux de difficulté IA** : Facile, Normal, Difficile, Hell
- **Architecture MVC propre** pour la maintenabilité et la testabilité
- **Support multiplateforme** (Windows, Linux, macOS)
- **Algorithmes avancés** : Minimax avec élagage α-β, recherche de chemin BFS

**Équipe** : Tarazona Javier, Liang Tianyi  
**Cours** : IN204 - Programmation Orientée Objet, ENSTA Paris  
**Date de soutenance** : 29 janvier 2026

---

## ✨ Fonctionnalités

### Gameplay
- ✅ **Plateau de jeu 9×9** conforme aux règles officielles du Quoridor
- ✅ **Déplacement des pions** avec mouvements orthogonaux et sauts
- ✅ **Placement de murs** avec détection de collision et validation de chemin
- ✅ **Détection de victoire** lorsqu'un joueur atteint la rangée opposée
- ✅ **Jeu au tour par tour** avec alternance des joueurs

### Intelligence Artificielle
- 🤖 **Algorithme Minimax** avec élagage Alpha-Bêta
- 🎯 **Heuristiques intelligentes** : estimation de distance, évaluation d'impact des murs, analyse de mobilité
- 🎲 **Quatre niveaux de difficulté** :

| Difficulté | Profondeur | Bruit | Comportement |
|------------|------------|-------|--------------|
| **Facile** | 1 | ±40 | ~50% coups sous-optimaux, bon pour apprendre |
| **Normal** | 1 | ±8 | ~15% erreurs, battable par la plupart des joueurs |
| **Difficile** | 2 | ±3 | ~5% erreurs, nécessite une planification soignée |
| **Enfer** | 4 | 0 | Jeu parfait, défi niveau expert |

- ⚡ **Performance optimisée** : Facile/Normal ~150μs, Difficile ~800μs, Enfer ~10ms par coup

### Technique
- 🏗️ **Architecture MVC** : Modèle (logique de jeu), Vue (rendu SFML), Contrôleur (entrées/boucle de jeu)
- 🧪 **32 tests unitaires** couvrant les règles, la recherche de chemin et l'IA (framework Catch2)
- 📦 **Packaging multiplateforme** : ZIP (Windows), DMG (macOS), AppImage/TGZ (Linux)
- 🎨 **Rendu 2D basé sur SFML 3.0** avec interface utilisateur réactive

---

## 🚀 Démarrage rapide

### Prérequis

| Plateforme | Exigences |
|------------|-----------|
| **Windows** | Visual Studio 2019+ avec "Développement Desktop C++", CMake 3.20+, Git |
| **Linux** | GCC 10+ ou Clang 11+, CMake 3.20+, Git, libs dev X11/Wayland |
| **macOS** | Xcode Command Line Tools (`xcode-select --install`), CMake 3.20+ (`brew install cmake`) |

> **Note** : vcpkg est automatiquement cloné et configuré par les scripts de bootstrap.

### Compilation en une commande

**Windows (PowerShell) :**
```powershell
git clone https://github.com/JavierTarazona06/ENSTQuoridor.git
cd ENSTQuoridor
scripts\bootstrap.ps1 -Config Release
.\build\x64-windows\Release\quoridor_game.exe
```

**Linux :**
```bash
git clone https://github.com/JavierTarazona06/ENSTQuoridor.git
cd ENSTQuoridor
chmod +x scripts/bootstrap.sh
./scripts/bootstrap.sh Release
./build/x64-linux/quoridor_game
```

**macOS :**
```bash
git clone https://github.com/JavierTarazona06/ENSTQuoridor.git
cd ENSTQuoridor
chmod +x scripts/bootstrap.sh
./scripts/bootstrap.sh Release
open ./build/arm64-osx/quoridor_game.app   # Apple Silicon
# ou : open ./build/x64-osx/quoridor_game.app  # Mac Intel
```

### Exécution des tests

```bash
# Exécuter les 32 tests
ctest --test-dir build/arm64-osx/tests -C Release

# Exécuter avec sortie détaillée
./build/arm64-osx/tests/test_all --success
```

---

## 🎮 Comment jouer

### Modes de jeu

| Mode | Description |
|------|-------------|
| **Humain vs Humain** | Deux joueurs sur la même machine, jouant à tour de rôle |
| **Humain vs IA** | Jouer contre l'ordinateur au niveau de difficulté choisi |

### Contrôles

#### Menu principal
| Touche | Action |
|--------|--------|
| `H` | Sélectionner mode Humain vs Humain |
| `A` | Sélectionner mode Humain vs IA |
| `1` / `2` / `3` / `4` | Sélectionner difficulté IA : Facile / Normal / Difficile / Enfer |
| `Entrée` | Démarrer la partie |
| `Échap` | Quitter l'application |

#### Contrôles en jeu
| Entrée | Action |
|--------|--------|
| **Clic gauche sur pion** | Sélectionner votre pion |
| **Clic gauche sur case** | Se déplacer vers cette case (si valide) |
| **Clic sur pion sélectionné** | Désélectionner le pion |
| `W` | Basculer en mode placement de mur |
| `O` | Tourner le mur (horizontal ↔ vertical) |
| `R` | Réinitialiser / Recommencer la partie |
| `1`-`4` | Changer la difficulté IA en cours de partie |
| `Échap` | Retourner au menu |

### Règles du jeu

1. **Objectif** : Être le premier à atteindre la rangée opposée
   - Joueur 1 (Blanc) : Commence en haut, objectif rangée du bas
   - Joueur 2 (Noir) : Commence en bas, objectif rangée du haut

2. **À chaque tour, choisir une action** :
   - **Déplacer le pion** : Une case orthogonalement (haut/bas/gauche/droite)
   - **Placer un mur** : Bloque le passage entre deux cases

3. **Sauts** :
   - Si l'adversaire est adjacent, vous pouvez sauter par-dessus
   - Si le saut direct est bloqué, le saut diagonal est autorisé

4. **Règles des murs** :
   - Chaque joueur dispose de **10 murs**
   - Les murs couvrent 2 cases et ne peuvent pas se chevaucher
   - **Doit toujours laisser un chemin** pour les deux joueurs vers leurs objectifs

---

## 📦 Packaging pour distribution



### Windows (.zip)
```powershell
cmake --build build/x64-windows --config Release
cd build/x64-windows
cpack -G ZIP
# Sortie : Quoridor-1.0-Windows-x64.zip
```

### macOS (.dmg)
```bash
./scripts/package-macos.sh Release
# Sortie : dist/Quoridor-1.0-macOS-arm64.dmg
```

### Linux (.tar.gz, .AppImage)
```bash
./scripts/package-linux.sh
# Sortie : dist/Quoridor-x86_64.tar.gz
#          dist/Quoridor-x86_64.AppImage
```


---

## 📚 Documentation

### Guides utilisateur

| Document | Description |
|----------|-------------|
| [Guide utilisateur](docs/USER_GUIDE.md) | Manuel complet des contrôles et règles |
| [Guide démo PVE](docs/tests/PVE_DEMO_GUIDE.md) | Comment jouer contre l'IA |
| [Documentation IA](docs/AI_DOCU.md) | Architecture Minimax, heuristiques, difficulté |

### Guides techniques

| Document | Description |
|----------|-------------|
| [Guide d'installation](docs/installation/INSTALLATION.md) | Compilation et exécution |
| [Packaging](docs/installation/PACKAGING.md) | Création de packages (Windows, Linux, macOS) |
| [Guide de création de tests](docs/tests/GUIDE_TESTS.md) | Comment écrire des tests Catch2 |

### Documents de conception

| Document | Description |
|----------|-------------|
| [Exigences](docs/planning/exigences/exigences_quoridor.pdf) | Spécification fonctionnelle |
| [Architecture](docs/planning/architecture/architecture_quoridor.pdf) | Architecture MVC du projet |
| [Planification](docs/planning/planification_quoridor.pdf) | Planning des itérations |


---

## 🤝 Équipe

| Membre | GitHub |
|--------| --------|
| **Javier Tarazona**  |[@JavierTarazona06](https://github.com/JavierTarazona06) |
| **Tianyi Liang** | [@TimmyLTY](https://github.com/TimmyLTY) |

---

## 📄 Licence

Ce projet est développé dans le cadre du cours IN204 à ENSTA Paris.  
Tous droits réservés par les auteurs. © 2025-2026

---

## 🙏 Remerciements

- **ENSTA Paris** — Cours IN204 Programmation Orientée Objet
- **Équipe SFML** — Simple and Fast Multimedia Library
- **Catch2** — Framework de test moderne C++
- **Mirko Marchesi** — Conception originale du jeu de plateau Quoridor

---

**Statut du projet** : ✅ Terminé  
**Dernière mise à jour** : 25 janvier 2026
