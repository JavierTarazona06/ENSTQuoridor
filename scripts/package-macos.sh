#!/usr/bin/env bash
set -euo pipefail

# ============================================================================
# macOS Packaging Script for Quoridor
# Creates a self-contained .app bundle and .dmg installer
# ============================================================================

# Determine script location and project root
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Configuration
CONFIG="${1:-Release}"
ARCH="$(uname -m)"  # arm64 or x86_64

if [[ "$ARCH" == "arm64" ]]; then
    PRESET="arm64-osx"
    TRIPLET="arm64-osx"
else
    PRESET="x64-osx"
    TRIPLET="x64-osx"
fi

BUILD_DIR="$ROOT/build/$PRESET"
DIST_DIR="$ROOT/dist"
APP_NAME="Quoridor"
APP_BUNDLE="$BUILD_DIR/$APP_NAME.app"
DMG_NAME="Quoridor-1.0-macOS-$ARCH"

echo "============================================"
echo "  macOS Packaging Script"
echo "  Architecture: $ARCH"
echo "  Configuration: $CONFIG"
echo "  Preset: $PRESET"
echo "============================================"

# Create dist directory
mkdir -p "$DIST_DIR"

# Step 1: Configure and build
echo ""
echo "[1/6] Configuring CMake..."
cmake --preset "$PRESET" -DCMAKE_BUILD_TYPE="$CONFIG"

echo ""
echo "[2/6] Building project..."
cmake --build "$BUILD_DIR" --config "$CONFIG" --parallel

# Step 2: Create the .app bundle structure
echo ""
echo "[3/6] Preparing .app bundle..."

# CMake already creates quoridor_game.app, we'll enhance it
SOURCE_APP="$BUILD_DIR/quoridor_game.app"

# Check if CMake created the app bundle
if [[ ! -d "$SOURCE_APP" ]]; then
    echo "ERROR: CMake did not create $SOURCE_APP"
    echo "Expected bundle at: $SOURCE_APP"
    exit 1
fi

# If we want a different name, copy it, otherwise work with existing
if [[ "$APP_BUNDLE" != "$SOURCE_APP" ]]; then
    rm -rf "$APP_BUNDLE"
    cp -R "$SOURCE_APP" "$APP_BUNDLE"
fi

APP_CONTENTS="$APP_BUNDLE/Contents"
APP_MACOS="$APP_CONTENTS/MacOS"
APP_RESOURCES="$APP_CONTENTS/Resources"
APP_FRAMEWORKS="$APP_CONTENTS/Frameworks"

# Create directories if they don't exist
mkdir -p "$APP_FRAMEWORKS"
mkdir -p "$APP_RESOURCES"

# Copy assets - always ensure assets directory exists in Resources
if [[ ! -d "$APP_RESOURCES/assets" ]]; then
    mkdir -p "$APP_RESOURCES/assets"
    cp -R "$ROOT/assets/"* "$APP_RESOURCES/assets/"
fi

# Copy icon if it exists (update if not already there)
ICON_FILE="$ROOT/assets/img/quoridor.icns"
if [[ ! -f "$APP_RESOURCES/quoridor.icns" ]]; then
    if [[ -f "$ICON_FILE" ]]; then
        cp "$ICON_FILE" "$APP_RESOURCES/quoridor.icns"
    else
        # Try to generate icon
        if [[ -x "$ROOT/scripts/generate-icon.sh" ]]; then
            "$ROOT/scripts/generate-icon.sh" 2>/dev/null || true
            if [[ -f "$ICON_FILE" ]]; then
                cp "$ICON_FILE" "$APP_RESOURCES/quoridor.icns"
            fi
        fi
    fi
fi

# Create Info.plist
cat > "$APP_CONTENTS/Info.plist" << 'PLIST'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleName</key>
    <string>Quoridor</string>
    <key>CFBundleDisplayName</key>
    <string>Quoridor</string>
    <key>CFBundleIdentifier</key>
    <string>com.ensta.quoridor</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>CFBundleExecutable</key>
    <string>quoridor_game</string>
    <key>CFBundleIconFile</key>
    <string>quoridor.icns</string>
    <key>LSMinimumSystemVersion</key>
    <string>11.0</string>
    <key>NSHighResolutionCapable</key>
    <true/>
    <key>NSPrincipalClass</key>
    <string>NSApplication</string>
    <key>LSApplicationCategoryType</key>
    <string>public.app-category.games</string>
</dict>
</plist>
PLIST

# Create PkgInfo
echo -n "APPL????" > "$APP_CONTENTS/PkgInfo"

# Step 3: Embed SFML frameworks/dylibs
echo ""
echo "[4/6] Embedding SFML frameworks..."

VCPKG_LIB_DIR="$BUILD_DIR/vcpkg_installed/$TRIPLET/lib"

if [[ -d "$VCPKG_LIB_DIR" ]]; then
    # Copy all dylibs
    find "$VCPKG_LIB_DIR" -name "*.dylib" -exec cp {} "$APP_FRAMEWORKS/" \; 2>/dev/null || true
    
    # Also check for SFML frameworks if using framework build
    VCPKG_FRAMEWORK_DIR="$BUILD_DIR/vcpkg_installed/$TRIPLET/lib"
    if [[ -d "$VCPKG_FRAMEWORK_DIR" ]]; then
        find "$VCPKG_FRAMEWORK_DIR" -name "*.framework" -exec cp -R {} "$APP_FRAMEWORKS/" \; 2>/dev/null || true
    fi
fi

# Step 4: Fix library paths using install_name_tool
echo ""
echo "[5/6] Fixing library paths..."

# Get list of dylibs to fix
EXECUTABLE="$APP_MACOS/$APP_NAME"

# Function to fix a single library reference
fix_lib_path() {
    local target="$1"
    local lib_name="$2"
    local framework_path="@executable_path/../Frameworks/$lib_name"
    
    # Check if library exists in Frameworks
    if [[ -f "$APP_FRAMEWORKS/$lib_name" ]]; then
        install_name_tool -change "@rpath/$lib_name" "$framework_path" "$target" 2>/dev/null || true
        install_name_tool -change "/usr/local/lib/$lib_name" "$framework_path" "$target" 2>/dev/null || true
        
        # Also try vcpkg paths
        install_name_tool -change "$VCPKG_LIB_DIR/$lib_name" "$framework_path" "$target" 2>/dev/null || true
    fi
}

# Fix the main executable
EXECUTABLE="$APP_MACOS/quoridor_game"  # CMake uses the target name

# Check if executable exists
if [[ ! -f "$EXECUTABLE" ]]; then
    echo "ERROR: Executable not found at $EXECUTABLE"
    echo "Contents of $APP_MACOS:"
    ls -la "$APP_MACOS"
    exit 1
fi

if [[ -d "$APP_FRAMEWORKS" ]]; then
    for dylib in "$APP_FRAMEWORKS"/*.dylib; do
        if [[ -f "$dylib" ]]; then
            lib_name="$(basename "$dylib")"
            fix_lib_path "$EXECUTABLE" "$lib_name"
            
            # Also fix the dylib's own references
            install_name_tool -id "@executable_path/../Frameworks/$lib_name" "$dylib" 2>/dev/null || true
            
            # Fix inter-library dependencies
            for other_dylib in "$APP_FRAMEWORKS"/*.dylib; do
                if [[ -f "$other_dylib" ]]; then
                    other_name="$(basename "$other_dylib")"
                    fix_lib_path "$dylib" "$other_name"
                fi
            done
        fi
    done
fi

# Add rpath to executable
install_name_tool -add_rpath "@executable_path/../Frameworks" "$EXECUTABLE" 2>/dev/null || true

# Step 5: Create wrapper script for assets path (if not already done)
echo ""
echo "Creating launch wrapper..."

# Check if wrapper already exists
if [[ ! -f "$APP_MACOS/quoridor_game_bin" ]]; then
    # Rename the actual executable
    mv "$EXECUTABLE" "$APP_MACOS/quoridor_game_bin"

    # Create wrapper script
    cat > "$EXECUTABLE" << 'WRAPPER'
#!/bin/bash
# Wrapper script to set working directory for assets
DIR="$(cd "$(dirname "$0")" && pwd)"
RESOURCES="$(cd "$DIR/../Resources" && pwd)"
cd "$RESOURCES"
exec "$DIR/quoridor_game_bin" "$@"
WRAPPER

    chmod +x "$EXECUTABLE"
fi

# Step 6: Create DMG
echo ""
echo "[6/6] Creating DMG installer..."

# Clear quarantine attributes and sign with ad-hoc signature
echo "Clearing quarantine attributes..."
xattr -cr "$APP_BUNDLE"

echo "Signing with ad-hoc signature..."
codesign --force --deep --sign - "$APP_BUNDLE" 2>/dev/null || echo "Warning: Ad-hoc signing failed (non-critical)"

DMG_PATH="$DIST_DIR/$DMG_NAME.dmg"
rm -f "$DMG_PATH"

# Create temporary directory for DMG contents
DMG_TEMP="$BUILD_DIR/dmg_temp"
rm -rf "$DMG_TEMP"
mkdir -p "$DMG_TEMP"

# Copy app bundle
cp -R "$APP_BUNDLE" "$DMG_TEMP/"

# Create symlink to Applications (for drag-and-drop install)
ln -s /Applications "$DMG_TEMP/Applications"

# Create DMG
hdiutil create -volname "$APP_NAME" \
    -srcfolder "$DMG_TEMP" \
    -ov -format UDBZ \
    "$DMG_PATH"

# Clean up
rm -rf "$DMG_TEMP"

# Also create a tarball for alternative distribution
echo ""
echo "Creating tarball..."
TARBALL_PATH="$DIST_DIR/$DMG_NAME.tar.gz"
tar -C "$BUILD_DIR" -czf "$TARBALL_PATH" "$APP_NAME.app"

echo ""
echo "============================================"
echo "  Packaging Complete!"
echo "============================================"
echo ""
echo "Created artifacts:"
echo "  - $DMG_PATH"
echo "  - $TARBALL_PATH"
echo "  - $APP_BUNDLE"
echo ""
echo "To install:"
echo "  1. Open the .dmg file"
echo "  2. Drag Quoridor.app to Applications"
echo "  3. Launch from Applications folder"
echo ""
