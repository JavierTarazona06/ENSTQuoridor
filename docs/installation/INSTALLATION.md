# Guide d'Installation

Ce guide explique comment compiler et exécuter l'application ENSTQuoridor sur Windows, Linux et macOS.

---

## 📋 Prérequis

### Pour les développeurs (compilation depuis les sources)

**Windows :**
- **Git** : https://git-scm.com/download/win
- **CMake 3.20+** : https://cmake.org/download/ (ajouter au PATH lors de l'installation)
- **Visual Studio Build Tools 2019** ou Visual Studio 2019/2022 : https://visualstudio.microsoft.com/downloads/
  - Lors de l'installation, sélectionner "Développement Desktop C++"
- vcpkg : le script de bootstrap le clone localement s'il n'est pas déjà installé

**Linux :**
- Git, CMake 3.20+, GCC/Clang
- vcpkg : le script de bootstrap le clone localement s'il n'est pas déjà installé

**macOS :**
- Xcode Command Line Tools : `xcode-select --install`
- CMake 3.20+ : `brew install cmake`
- vcpkg : le script de bootstrap le clone localement s'il n'est pas déjà installé

---

## 🔨 Compilation depuis les sources

### Configuration complète pour les nouveaux utilisateurs (Windows)

1. **Installer les prérequis** (voir ci-dessus)

2. **Cloner le dépôt :**
```powershell
git clone https://github.com/JavierTarazona06/ENSTQuoridor.git
cd ENSTQuoridor
```

3. **Exécuter le script de bootstrap :**
```powershell
scripts\bootstrap.ps1 -Config Debug
```

4. **Lancer l'application :**
```powershell
.\build\x64-windows\Debug\quoridor_game.exe
```

### Bootstrap en une commande (multiplateforme)

Ces scripts configurent vcpkg (si nécessaire), sélectionnent le bon preset CMake et compilent.

**Windows (PowerShell) :**
```powershell
# Depuis la racine du dépôt
scripts\bootstrap.ps1 -Config Debug

# Compilation seule (après bootstrap initial)
cmake --build build/x64-windows --config Debug
```

**Linux/macOS (Bash) :**
```bash
# Depuis la racine du dépôt
chmod +x scripts/bootstrap.sh
scripts/bootstrap.sh Debug

# Compilation seule
# Pour macOS (Apple Silicon) :
cmake --build build/arm64-osx --config Debug
# Pour macOS (Intel) :
cmake --build build/x64-osx --config Debug
# Pour Linux :
cmake --build build/x64-linux --config Debug
```

---

## 📂 Emplacements des exécutables

L'emplacement de l'exécutable dépend de votre plateforme et configuration :

### Windows

```powershell
# Jeu principal
.\build\x64-windows\Debug\quoridor_game.exe
# Ou pour Release :
.\build\x64-windows\Release\quoridor_game.exe

# Exécuter tous les tests :
ctest --test-dir build/x64-windows/tests -C Debug
# ou :
.\build\x64-windows\tests\Debug\test_all.exe

# Voir les tests individuels :
.\build\x64-windows\tests\Debug\test_all.exe --list-tests
# Exécuter un test spécifique :
.\build\x64-windows\tests\Debug\test_all.exe [tags]
```

### Linux

```bash
# Jeu principal
./build/x64-linux/quoridor_game

# Exécuter tous les tests :
ctest --test-dir build/x64-linux/tests -C Debug
# ou :
./build/x64-linux/tests/test_all

# Voir les tests individuels :
./build/x64-linux/tests/test_all --list-tests
# Exécuter un test spécifique :
./build/x64-linux/tests/test_all [tags]
```

### macOS

```bash
# Jeu principal
./build/arm64-osx/quoridor_game    # Apple Silicon
./build/x64-osx/quoridor_game      # Intel

# Exécuter tous les tests :
ctest --test-dir build/arm64-osx/tests -C Debug
# ou :
./build/arm64-osx/tests/test_all

# Voir les tests individuels :
./build/arm64-osx/tests/test_all --list-tests
# Exécuter un test spécifique :
./build/arm64-osx/tests/test_all [tags]
```

---

## 🎮 Démo PVE (Joueur vs IA)

Une démo visuelle (`test_pve_visual`) est également compilée automatiquement, vous permettant de jouer contre l'IA :

**Windows :**
```powershell
.\build\x64-windows\tests\Debug\test_pve_visual.exe
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

Voir le [Guide démo PVE](../PVE_DEMO_GUIDE.md) pour les contrôles détaillés et l'utilisation.

---

## 🐛 Dépannage

- **"Could not find toolchain file"** : Assurez-vous que `VCPKG_ROOT` est défini ou laissez les scripts de bootstrap s'en charger
- **Si la compilation échoue** : Supprimez le dossier build et relancez le bootstrap
  - Windows : `Remove-Item -Recurse -Force .\build\x64-windows; scripts\bootstrap.ps1 -Config Debug`
  - Linux/macOS : `rm -rf build; scripts/bootstrap.sh Debug`
- **Vérifier l'intégration SFML** : `cmake --preset x64-windows` (ou `x64-linux`, `arm64-osx`) devrait afficher `-- Found SFML ...`

---

## 📦 Pour les utilisateurs finaux (sans outils de développement)

Si vous souhaitez distribuer l'application à des utilisateurs n'ayant pas d'outils de développement installés, vous devrez créer un package portable contenant :
- L'exécutable (`.exe` sur Windows)
- Les fichiers DLL SFML
- Le dossier Assets

### Création d'un package ZIP distribuable (Windows)

D'abord, compilez la version Release :

```powershell
# Configurer (si pas déjà fait)
cmake --preset x64-windows

# Compiler en Release
scripts\bootstrap.ps1 -Config Release
# Ou juste le programme
cmake --build build\x64-windows --config Release
```

Puis générez le package ZIP portable :

```powershell
cd build\x64-windows
cpack -G ZIP -C Release
```

Cela crée `Quoridor-1.0-Windows.zip` contenant :
- `quoridor_game.exe`
- DLLs SFML (`sfml-graphics-3.dll`, `sfml-window-3.dll`, `sfml-system-3.dll`, `sfml-audio-3.dll`)
- Dossier `assets/` avec les polices et images

Le ZIP peut être distribué aux utilisateurs qui peuvent simplement extraire et double-cliquer sur `quoridor_game.exe` pour lancer le jeu.

### Création d'un package distribuable (Linux)

Pour générer des packages pour utilisateurs finaux, exécutez :

```bash
scripts/package-linux.sh
```

Étapes recommandées (tar.gz) :
1. Ouvrez un terminal et extrayez l'archive depuis la racine du dépôt :
   ```bash
   cd dist
   tar -xvzf Quoridor-x86_64.tar.gz
   ```
2. Entrez dans le dossier extrait :
   ```bash
   cd quoridor
   ```
3. Accordez la permission d'exécution si nécessaire :
   ```bash
   chmod +x quoridor_game
   ```
4. Lancez le programme :
   ```bash
   ./quoridor_game
   ```

Un `Quoridor-x86_64.AppImage` est également produit dans `dist/`. Rendez-le exécutable et lancez-le :
```bash
chmod +x Quoridor-x86_64.AppImage
./Quoridor-x86_64.AppImage
```
Certaines distributions nécessitent `libfuse2` pour exécuter les AppImages.

### Création d'un package distribuable (macOS)

Voir le guide [Packaging](PACKAGING.md) pour créer un `.app` et un `.dmg`.

---

## 📚 Voir aussi

- [Guide démo PVE](../tests/PVE_DEMO_GUIDE.md) — Comment jouer contre l'IA
- [Packaging](PACKAGING.md) — Création de packages distribuables
- [Guide utilisateur](../USER_GUIDE.md) — Manuel utilisateur complet

---

**Dernière mise à jour** : 25 janvier 2026