#!/usr/bin/env bash

set -euo pipefail

TARGET="/Volumes/MKS"
SOURCE=".pio/build/advd12_230/Robin_nano35.bin"
ASSETS="../lcd/assets"

# Check that the target volume exists
if [[ ! -d "$TARGET" ]]; then
    echo "Error: $TARGET is not mounted."
    exit 1
fi

# Remove the _assets directory if it exists
if [[ -d "$TARGET/_assets" ]]; then
    echo "Removing $TARGET/_assets..."
    rm -rf "$TARGET/_assets"
fi

# Build the firmware
echo "Build firmware..."
pio run -e advd12_230

# Check that the source file exists
if [[ ! -f "$SOURCE" ]]; then
    echo "Error: Source file '$SOURCE' not found."
    exit 1
fi

# Check that the assets folder exists
if [[ ! -d "$ASSETS" ]]; then
    echo "Error: Assets folder '$ASSETS' not found."
    exit 1
fi

# Remove any .cur and .bin files
echo "Removing existing .cur and .bin files..."
find "$TARGET" -maxdepth 1 \( -name "*.cur" -o -name "*.bin" \) -delete

# Copy the new firmware and assets

echo "Copying firmware..."
cp "$SOURCE" "$TARGET/"
cp -r "$ASSETS" "$TARGET/"

echo "Eject microSD..."
diskutil eject "$TARGET"

echo "Done."
