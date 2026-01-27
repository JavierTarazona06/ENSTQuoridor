# INSTALLATION

## 📋 Prérequis (développeurs)

**Windows**
- Git : https://git-scm.com/download/win
- CMake 3.20+ : https://cmake.org/download/ (ajouter au PATH)
- Visual Studio Build Tools 2019 ou VS 2019/2022 (workload « Desktop development with C++ »)
- vcpkg : cloné automatiquement par le script de bootstrap si absent

**Linux**
- Git, CMake 3.20+, GCC/Clang
- vcpkg : cloné automatiquement par le script de bootstrap si absent

**macOS**
- Xcode Command Line Tools : `xcode-select --install`
- CMake 3.20+ : `brew install cmake`
- vcpkg : cloné automatiquement par le script de bootstrap si absent

---

## 🔨 Compilation depuis les sources

### Configuration complète (Windows)
```powershell
git clone https://github.com/JavierTarazona06/ENSTQuoridor.git
cd ENSTQuoridor
scripts\bootstrap.ps1 -Config Debug
.\build\x64-windows\Debug\quoridor_game.exe
```

### Bootstrap en une commande (multiplateforme)
Ces scripts configurent vcpkg, sélectionnent le preset CMake et compilent.

- **Windows (PowerShell)**
```powershell
scripts\bootstrap.ps1 -Config Debug
cmake --build build/x64-windows --config Debug
```

- **Linux / macOS (Bash)**
```bash
chmod +x scripts/bootstrap.sh
scripts/bootstrap.sh Debug
# Builds directs
cmake --build build/x64-linux   --config Debug    # Linux
cmake --build build/arm64-osx   --config Debug    # macOS Apple Silicon
cmake --build build/x64-osx     --config Debug    # macOS Intel
```

---

## 📂 Emplacements des exécutables et tests

**Windows**
```powershell
.\build\x64-windows\Debug\quoridor_game.exe
.\build\x64-windows\Release\quoridor_game.exe
ctest --test-dir build/x64-windows/tests -C Debug
.\build\x64-windows\tests\Debug\test_all.exe [tags]
```

**Linux**
```bash
./build/x64-linux/quoridor_game
ctest --test-dir build/x64-linux/tests -C Debug
./build/x64-linux/tests/test_all [tags]
```

**macOS**
```bash
./build/arm64-osx/quoridor_game      # Apple Silicon
./build/x64-osx/quoridor_game        # Intel
ctest --test-dir build/arm64-osx/tests -C Debug
./build/arm64-osx/tests/test_all [tags]
# Variante x64 :
ctest --test-dir build/x64-osx/tests -C Debug
./build/x64-osx/tests/test_all [tags]
```

---

## 📦 Distribution pour utilisateurs finaux

### ZIP Windows
```powershell
cmake --preset x64-windows
scripts\bootstrap.ps1 -Config Release
cmake --build build\x64-windows --config Release
cd build\x64-windows
cpack -G ZIP -C Release
```
Produit `Quoridor-1.0-Windows.zip` avec l'exécutable, DLL SFML et `assets/`.

### Tar.gz / AppImage Linux
```bash
scripts/package-linux.sh
```
Génère `dist/Quoridor-x86_64.tar.gz` et `dist/Quoridor-x86_64.AppImage`.

### macOS
Voir `PACKAGING.md` pour créer le `.app` et le `.dmg`.

---

## 🐛 Dépannage
- « Could not find toolchain file » : vérifier `VCPKG_ROOT` ou relancer le bootstrap.
- Échec de build : supprimer le dossier build concerné puis rebootstrap.
- Vérifier SFML : `cmake --preset <preset>` doit afficher `-- Found SFML ...`.

### Problèmes AppImage (Linux)
Symptômes : « Cannot mount AppImage », « Can't open squashfs image: Bad address », sortie FUSE au lancement.

Causes fréquentes : absence de FUSE v2 (`libfuse2`) ou environnement sans FUSE (WSL).

Correctifs :
```bash
# Ubuntu/Debian
sudo apt update && sudo apt install -y libfuse2 squashfuse
# Fedora/RHEL/CentOS
sudo dnf install -y fuse   # ou yum install -y fuse
# Arch/Manjaro
sudo pacman -S fuse2
```

Fallback sans FUSE :
```bash
cd dist
chmod +x Quoridor-x86_64.AppImage
APPIMAGE_EXTRACT_AND_RUN=1 ./Quoridor-x86_64.AppImage
# ou :
./Quoridor-x86_64.AppImage --appimage-extract
./squashfs-root/AppRun
```

Note WSL : FUSE n'est pas supporté ; utiliser le paquet `tar.gz` ou l'extraction ci-dessus.

---

**Dernière mise à jour** : 27 janvier 2026
