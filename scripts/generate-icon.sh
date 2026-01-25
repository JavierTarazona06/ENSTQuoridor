#!/usr/bin/env bash
set -euo pipefail

# ============================================================================
# Generate macOS .icns icon from PNG
# ============================================================================

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

INPUT_PNG="$ROOT/assets/img/logo_ensta_zeb.png"
OUTPUT_ICNS="$ROOT/assets/img/quoridor.icns"
ICONSET_DIR="$ROOT/build/quoridor.iconset"

echo "Generating macOS icon from $INPUT_PNG..."

# Check if input exists
if [[ ! -f "$INPUT_PNG" ]]; then
    echo "Error: Input PNG not found: $INPUT_PNG"
    exit 1
fi

# Create iconset directory
rm -rf "$ICONSET_DIR"
mkdir -p "$ICONSET_DIR"

# Generate all required sizes
# macOS requires specific sizes for the iconset
sips -z 16 16     "$INPUT_PNG" --out "$ICONSET_DIR/icon_16x16.png" 2>/dev/null
sips -z 32 32     "$INPUT_PNG" --out "$ICONSET_DIR/icon_16x16@2x.png" 2>/dev/null
sips -z 32 32     "$INPUT_PNG" --out "$ICONSET_DIR/icon_32x32.png" 2>/dev/null
sips -z 64 64     "$INPUT_PNG" --out "$ICONSET_DIR/icon_32x32@2x.png" 2>/dev/null
sips -z 128 128   "$INPUT_PNG" --out "$ICONSET_DIR/icon_128x128.png" 2>/dev/null
sips -z 256 256   "$INPUT_PNG" --out "$ICONSET_DIR/icon_128x128@2x.png" 2>/dev/null
sips -z 256 256   "$INPUT_PNG" --out "$ICONSET_DIR/icon_256x256.png" 2>/dev/null
sips -z 512 512   "$INPUT_PNG" --out "$ICONSET_DIR/icon_256x256@2x.png" 2>/dev/null
sips -z 512 512   "$INPUT_PNG" --out "$ICONSET_DIR/icon_512x512.png" 2>/dev/null
sips -z 1024 1024 "$INPUT_PNG" --out "$ICONSET_DIR/icon_512x512@2x.png" 2>/dev/null

# Convert iconset to icns
iconutil -c icns "$ICONSET_DIR" -o "$OUTPUT_ICNS"

# Clean up
rm -rf "$ICONSET_DIR"

echo "Created: $OUTPUT_ICNS"
