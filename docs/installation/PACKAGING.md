# Guide de Packaging

Ce guide explique comment créer des packages distribuables de Quoridor pour Windows, Linux et macOS.

---

## 📋 Vue d'ensemble

| Plateforme | Format | Script | Sortie |
|------------|--------|--------|--------|
| **Windows** | ZIP | CPack | `Quoridor-1.0-Windows.zip` |
| **Linux** | tar.gz, AppImage | `scripts/package-linux.sh` | `dist/Quoridor-x86_64.*` |
| **macOS** | DMG, app | `scripts/package-macos.sh` | `dist/Quoridor-1.0-macOS-*.dmg` |

---

## 🪟 Windows (.zip)

### Prérequis

- Visual Studio 2019+ avec "Développement Desktop C++"
- CMake 3.20+

### Étapes

1. **Compiler en mode Release :**
```powershell
# Configurer (si pas déjà fait)
cmake --preset x64-windows

# Compiler en Release
scripts\bootstrap.ps1 -Config Release
# Ou compilation seule
cmake --build build\x64-windows --config Release
```

2. **Générer le package ZIP :**
```powershell
cd build\x64-windows
cpack -G ZIP -C Release
```

### Contenu du package

Le fichier `Quoridor-1.0-Windows.zip` contient :
- `quoridor_game.exe` — Exécutable principal
- DLLs SFML (`sfml-graphics-3.dll`, `sfml-window-3.dll`, `sfml-system-3.dll`)
- Dossier `assets/` — Polices et images

### Installation pour l'utilisateur

1. Extraire le ZIP dans un dossier quelconque
2. Double-cliquer sur `quoridor_game.exe`

---

## 🐧 Linux (.tar.gz, .AppImage)

### Prérequis

- GCC 10+ ou Clang 11+
- CMake 3.20+
- (Optionnel) `libfuse2` pour AppImage

### Étapes

1. **Compiler en mode Release :**
```bash
chmod +x scripts/bootstrap.sh
./scripts/bootstrap.sh Release
```

2. **Générer les packages :**
```bash
chmod +x scripts/package-linux.sh
./scripts/package-linux.sh
```

### Artefacts générés

| Fichier | Description |
|---------|-------------|
| `dist/Quoridor-x86_64.tar.gz` | Archive portable |
| `dist/Quoridor-x86_64.AppImage` | Application autonome |

### Installation pour l'utilisateur

**Option 1 : Archive tar.gz**
```bash
cd dist
tar -xvzf Quoridor-x86_64.tar.gz
cd quoridor
chmod +x quoridor_game
./quoridor_game
```

**Option 2 : AppImage**
```bash
chmod +x Quoridor-x86_64.AppImage
./Quoridor-x86_64.AppImage
```

> **Note** : Certaines distributions nécessitent `libfuse2` pour les AppImages :
> ```bash
> sudo apt install libfuse2  # Ubuntu/Debian
> ```

---

## 🍎 macOS (.dmg, .app)

### Prérequis

- macOS 11.0 (Big Sur) ou ultérieur
- Xcode Command Line Tools : `xcode-select --install`
- CMake 3.20+ : `brew install cmake`

### Démarrage rapide

```bash
chmod +x scripts/package-macos.sh
./scripts/package-macos.sh
```

### Artefacts générés

| Fichier | Description |
|---------|-------------|
| `dist/Quoridor-1.0-macOS-arm64.dmg` | Image disque (Apple Silicon) |
| `dist/Quoridor-1.0-macOS-x86_64.dmg` | Image disque (Intel) |
| `dist/Quoridor-1.0-macOS-arm64.tar.gz` | Archive tar.gz (Apple Silicon) |
| `dist/Quoridor-1.0-macOS-x86_64.tar.gz` | Archive tar.gz (Intel) |
| `build/arm64-osx/Quoridor.app` | Bundle d'application (Apple Silicon) |
| `build/x64-osx/Quoridor.app` | Bundle d'application (Intel) |

> **Note** : Le script `package-macos.sh` renomme automatiquement `quoridor_game.app` en `Quoridor.app` pour une meilleure présentation.

### Structure du bundle .app

```
Quoridor.app/
├── Contents/
│   ├── Info.plist          # Métadonnées de l'application
│   ├── MacOS/
│   │   ├── quoridor_game         # Script wrapper
│   │   └── quoridor_game_bin     # Exécutable principal
│   ├── Resources/
│   │   └── assets/          # Polices, images
│   └── Frameworks/
│       └── *.dylib          # Bibliothèques SFML
```

### Installation pour l'utilisateur

1. Double-cliquer sur le fichier `.dmg` pour le monter
2. Glisser `Quoridor.app` vers le dossier Applications
3. Éjecter le DMG

> ⚠️ **Note Gatekeeper** : Si vous obtenez l'erreur "L'application est endommagée ou incomplète", exécutez :
> ```bash
> xattr -cr /Applications/Quoridor.app
> ```
> Ou clic droit sur l'application → "Ouvrir" pour contourner Gatekeeper.

### Étapes manuelles (optionnel)

```bash
# 1. Compiler le projet
cmake --preset arm64-osx -DCMAKE_BUILD_TYPE=Release
cmake --build build/arm64-osx --config Release

# 2. Générer l'icône (optionnel)
chmod +x scripts/generate-icon.sh
./scripts/generate-icon.sh

# 3. Créer le package
./scripts/package-macos.sh Release
```

### Utilisation de CPack

```bash
cd build/arm64-osx
cpack -G DragNDrop
```

### Architectures supportées

| Architecture | Preset | Sortie |
|--------------|--------|--------|
| Apple Silicon (M1/M2/M3) | `arm64-osx` | `Quoridor-1.0-macOS-arm64.dmg` |
| Intel | `x64-osx` | `Quoridor-1.0-macOS-x86_64.dmg` |

Le script détecte automatiquement l'architecture de votre Mac.

---

## 🔐 Signature et notarisation (macOS)

### Signature du code (optionnel)

Pour une distribution en dehors du Mac App Store :

```bash
# Signer le bundle
codesign --force --deep --sign "Developer ID Application: Votre Nom" Quoridor.app

# Créer un DMG signé
hdiutil create -volname "Quoridor" -srcfolder Quoridor.app -ov -format UDZO Quoridor-signed.dmg
codesign --sign "Developer ID Application: Votre Nom" Quoridor-signed.dmg
```

### Notarisation (optionnel)

Pour l'approbation de Gatekeeper sur macOS 10.15+ :

```bash
# Soumettre pour notarisation
xcrun notarytool submit Quoridor.dmg \
  --apple-id VOTRE_APPLE_ID \
  --password VOTRE_MOT_DE_PASSE_APP \
  --team-id VOTRE_TEAM_ID --wait

# Agrafer le ticket
xcrun stapler staple Quoridor.dmg
```

## 📚 Voir aussi

- [Guide d'installation](INSTALLATION.md) — Compilation et exécution
- [Guide utilisateur](../USER_GUIDE.md) — Manuel utilisateur complet

---

**Dernière mise à jour** : 27 janvier 2026