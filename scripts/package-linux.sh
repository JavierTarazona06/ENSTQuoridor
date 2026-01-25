#!/usr/bin/env bash
set -euo pipefail

# One-click Linux packaging: builds, stages AppDir, creates tarball and AppImage
# Requirements: curl, patchelf, linuxdeploy (auto-downloaded if missing), cmake, c++ toolchain.

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT/build/linux-release"
DIST_DIR="$ROOT/dist"
APPDIR="$DIST_DIR/AppDir"
TARBALL_DIR="$DIST_DIR/quoridor"
LINUXDEPLOY_BIN="$DIST_DIR/linuxdeploy-x86_64.AppImage"
VCPKG_TRIPLET="${VCPKG_TARGET_TRIPLET:-x64-linux-dynamic}"

VCPKG_TOOLCHAIN="${VCPKG_ROOT:-$ROOT/.vcpkg}/scripts/buildsystems/vcpkg.cmake"
TOOLCHAIN_ARG=()
if [ -f "$VCPKG_TOOLCHAIN" ]; then
  export VCPKG_TARGET_TRIPLET="$VCPKG_TRIPLET"
  export VCPKG_HOST_TRIPLET="$VCPKG_TRIPLET"
  TOOLCHAIN_ARG=(-DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN" -DVCPKG_TARGET_TRIPLET="$VCPKG_TRIPLET")
fi

if ! command -v patchelf >/dev/null 2>&1; then
  echo "Error: patchelf is required. Install it with: sudo apt-get install -y patchelf" >&2
  exit 1
fi

mkdir -p "$BUILD_DIR" "$DIST_DIR"
rm -rf "$APPDIR" "$TARBALL_DIR" "$BUILD_DIR"

cmake -S "$ROOT" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$APPDIR/usr" -DSFML_STATIC_LIBRARIES=OFF "${TOOLCHAIN_ARG[@]}"
cmake --build "$BUILD_DIR" --config Release -j"$(nproc)"
cmake --install "$BUILD_DIR" --config Release

# --- Prepare AppDir metadata ---
mkdir -p "$APPDIR/usr/share/icons/hicolor/256x256/apps"
if [ -f "$ROOT/assets/img/logo_ensta_zeb.png" ]; then
  cp "$ROOT/assets/img/logo_ensta_zeb.png" "$APPDIR/usr/share/icons/hicolor/256x256/apps/quoridor.png"
fi

cat > "$APPDIR/quoridor.desktop" <<'EOF'
[Desktop Entry]
Type=Application
Name=Quoridor
Exec=quoridor_game
Icon=quoridor
Categories=Game;
EOF

cat > "$APPDIR/AppRun" <<'EOF'
#!/usr/bin/env bash
set -euo pipefail
APPDIR="$(cd "$(dirname "$0")" && pwd)"
export LD_LIBRARY_PATH="$APPDIR/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
cd "$APPDIR/usr"
exec "$APPDIR/usr/bin/quoridor_game" "$@"
EOF
chmod +x "$APPDIR/AppRun"

# --- Tarball packaging with bundled shared libs ---
mkdir -p "$TARBALL_DIR/lib"
cp "$APPDIR/usr/bin/quoridor_game" "$TARBALL_DIR/"
cp -r "$APPDIR/usr/assets" "$TARBALL_DIR/"

# Copy vcpkg-built shared libs into AppDir and tarball lib folder upfront (SFML + deps)
VCPKG_LIB_DIR="$BUILD_DIR/vcpkg_installed/$VCPKG_TRIPLET/lib"
if [ -d "$VCPKG_LIB_DIR" ]; then
  mkdir -p "$APPDIR/usr/lib"
  cp -n "$VCPKG_LIB_DIR"/*.so* "$APPDIR/usr/lib/" 2>/dev/null || true
  mkdir -p "$TARBALL_DIR/lib"
  cp -n "$VCPKG_LIB_DIR"/*.so* "$TARBALL_DIR/lib/" 2>/dev/null || true
fi

patchelf --set-rpath '$ORIGIN/lib' "$TARBALL_DIR/quoridor_game"
ldd "$TARBALL_DIR/quoridor_game" | awk '$3 ~ /^\// {print $3}' | while read -r lib; do
  base="$(basename "$lib")"
  case "$base" in
    libc.so*|libm.so*|libdl.so*|librt.so*|libpthread.so*|libgcc_s.so*|libstdc++.so*|ld-linux*.so*) continue ;;
  esac
  cp -n "$lib" "$TARBALL_DIR/lib/" || true
done

cat > "$TARBALL_DIR/quoridor.sh" <<'EOF'
#!/usr/bin/env bash
set -euo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
export LD_LIBRARY_PATH="$HERE/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
cd "$HERE"
exec "$HERE/quoridor_game" "$@"
EOF
chmod +x "$TARBALL_DIR/quoridor.sh"

tar -C "$DIST_DIR" -czf "$DIST_DIR/Quoridor-x86_64.tar.gz" quoridor

# --- AppImage (optional but single-file) ---
if [ ! -x "$LINUXDEPLOY_BIN" ]; then
  curl -L "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage" -o "$LINUXDEPLOY_BIN"
  chmod +x "$LINUXDEPLOY_BIN"
fi

# Ensure linuxdeploy can resolve the bundled SFML libs during its scan
export LD_LIBRARY_PATH="$APPDIR/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
"$LINUXDEPLOY_BIN" --appdir "$APPDIR" --desktop-file "$APPDIR/quoridor.desktop" --executable "$APPDIR/usr/bin/quoridor_game" --output appimage

# Move resulting AppImage to dist with friendly name
if ls ./*.AppImage >/dev/null 2>&1; then
  APPIMAGE_FILE=$(ls -t ./*.AppImage | head -n1)
  mv "$APPIMAGE_FILE" "$DIST_DIR/Quoridor-x86_64.AppImage"
fi

echo "Created artifacts:"
echo "- $DIST_DIR/Quoridor-x86_64.tar.gz"
if [ -f "$DIST_DIR/Quoridor-x86_64.AppImage" ]; then
  echo "- $DIST_DIR/Quoridor-x86_64.AppImage"
fi
