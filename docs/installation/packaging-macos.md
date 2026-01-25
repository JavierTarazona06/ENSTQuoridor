# macOS Packaging Guide

This document explains how to create a distributable macOS application bundle (.app) and disk image (.dmg) for Quoridor.

## Quick Start

```bash
# One-command packaging
chmod +x scripts/package-macos.sh
./scripts/package-macos.sh
```

This will create:
- `dist/Quoridor-1.0-macOS-arm64.dmg` (or x86_64 on Intel Macs)
- `dist/Quoridor-1.0-macOS-arm64.tar.gz`
- `build/arm64-osx/Quoridor.app`

## Prerequisites

- macOS 11.0 (Big Sur) or later
- Xcode Command Line Tools: `xcode-select --install`
- CMake 3.20+: `brew install cmake`

## Generated Artifacts

### Application Bundle (.app)

The `.app` bundle is a self-contained macOS application:

```
Quoridor.app/
├── Contents/
│   ├── Info.plist          # App metadata
│   ├── PkgInfo              # Package type
│   ├── MacOS/
│   │   ├── Quoridor         # Launch wrapper script
│   │   └── Quoridor_bin     # Actual executable
│   ├── Resources/
│   │   └── assets/          # Game assets (fonts, images)
│   └── Frameworks/
│       └── *.dylib          # Embedded SFML libraries
```

### Disk Image (.dmg)

The `.dmg` file is a compressed installer that users can:
1. Double-click to mount
2. Drag Quoridor.app to Applications
3. Eject and delete the .dmg

## Manual Packaging Steps

If you prefer to build manually:

### 1. Build the Project

```bash
cmake --preset arm64-osx -DCMAKE_BUILD_TYPE=Release
cmake --build build/arm64-osx --config Release
```

### 2. Generate Icon (Optional)

```bash
chmod +x scripts/generate-icon.sh
./scripts/generate-icon.sh
```

This creates `assets/img/quoridor.icns` from the logo PNG.

### 3. Run Package Script

```bash
./scripts/package-macos.sh Release
```

## Using CPack

Alternatively, you can use CMake's CPack:

```bash
cd build/arm64-osx
cpack -G DragNDrop
```

This creates a basic DMG in the build directory.

## Code Signing (Optional)

For distribution outside the Mac App Store, you may want to sign the app:

```bash
# Sign the app bundle
codesign --force --deep --sign "Developer ID Application: Your Name" Quoridor.app

# Create a signed DMG
hdiutil create -volname "Quoridor" -srcfolder Quoridor.app -ov -format UDZO Quoridor-signed.dmg
codesign --sign "Developer ID Application: Your Name" Quoridor-signed.dmg
```

## Notarization (Optional)

For Gatekeeper approval on macOS 10.15+:

```bash
# Upload for notarization
xcrun notarytool submit Quoridor.dmg --apple-id YOUR_APPLE_ID --password YOUR_APP_PASSWORD --team-id YOUR_TEAM_ID --wait

# Staple the ticket
xcrun stapler staple Quoridor.dmg
```

## Troubleshooting

### "App is damaged and can't be opened"

This usually means Gatekeeper is blocking unsigned apps:

```bash
# Remove quarantine attribute
xattr -cr /Applications/Quoridor.app
```

Or right-click the app and select "Open" to bypass Gatekeeper.

### Missing Libraries

If the app fails to launch with library errors:

1. Check that all dylibs are in `Frameworks/`
2. Verify library paths: `otool -L Quoridor.app/Contents/MacOS/Quoridor_bin`
3. Re-run the packaging script

### Icon Not Showing

Regenerate the icon:

```bash
./scripts/generate-icon.sh
```

Then rebuild the package.

## Architecture Support

| Architecture | Preset | Output |
|--------------|--------|--------|
| Apple Silicon (M1/M2/M3) | `arm64-osx` | `Quoridor-1.0-macOS-arm64.dmg` |
| Intel | `x64-osx` | `Quoridor-1.0-macOS-x86_64.dmg` |

The script automatically detects your Mac's architecture.

## Bundle Structure Details

### Info.plist

Key configuration in the bundle:

| Key | Value | Description |
|-----|-------|-------------|
| `CFBundleIdentifier` | `com.ensta.quoridor` | Unique app identifier |
| `CFBundleVersion` | `1.0.0` | Build number |
| `LSMinimumSystemVersion` | `11.0` | Minimum macOS version |
| `NSHighResolutionCapable` | `true` | Retina display support |

### Library Embedding

SFML and its dependencies are embedded in `Frameworks/`:
- `libsfml-graphics.dylib`
- `libsfml-window.dylib`
- `libsfml-system.dylib`
- `libfreetype.dylib`
- Other transitive dependencies

Library paths are fixed using `install_name_tool` to use `@executable_path/../Frameworks/`.
