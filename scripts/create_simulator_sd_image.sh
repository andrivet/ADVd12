#!/usr/bin/env zsh

: '
Create an SD image "fs.img" for the simulator.
It contains the assets generated from the PNG files in lcd/png
and the GCODE files in lcd/gcode.
'

set -euo pipefail

img="fs.img"
name="ADVd12"
size=200 # MB

if [[ "$OSTYPE" != darwin* ]]; then echo "Works only on macOS, sorry" ; exit 1 ; fi

scripts="${0:A:h}"
lcd="$(cd "${scripts}/../lcd" && pwd)"
data="$(cd "${scripts}/../data" && pwd)"
img="${data}/${img}"

dev=""

cleanup() {
    if (( ZSH_SUBSHELL != 0 )); then
        return
    fi

    if [[ -n "$dev" ]]; then
        echo "Detach volume..."
        diskutil eject "$dev" || true
        dev=""
    fi
}

trap cleanup EXIT INT TERM HUP

echo "**** Generate assets..."
python "${scripts}/adv_encode_assets.py" "${lcd}/png" --outdir "${lcd}/assets"

echo
echo "**** Create empty image..."
rm -f "${img}"
mkfile "${size}m" "${img}"
echo "Image created"

echo
echo "**** Attach image..."
dev="$(diskutil image attach -nomount "${img}")"
dev="${dev%%[[:space:]]*}"
echo "Attached to device $dev"

echo
echo "**** Format image..."
diskutil erasedisk fat32 "$name" mbr "$dev"

partition="${dev}s1"

echo
echo "**** Mount volume..."
diskutil mount "${partition}"

echo
echo "**** Determine mount point..."
mount_path="$(diskutil info "${partition}" | awk -F': *' '/Mount Point/ {print $2}')"
if [[ -z "${mount_path}" || ! -d "${mount_path}" ]]; then
    echo "Unable to determine mount point"
    exit 1
fi
echo "Volume is mounted as ${mount_path}"

# Prevent macOS tracking
mkdir -p "${mount_path}/.fseventsd"
touch "${mount_path}/.fseventsd/no_log"

echo
echo "**** Copy assets..."
cp -R "${lcd}/assets" "${mount_path}/"
echo "Assets copied"


gcode_files=("${lcd}/gcode/"*.gcode(N))
if (( ${#gcode_files} )); then
    echo
    echo "**** Copy GCODE files..."
    cp "${gcode_files[@]}" "${mount_path}/"
    echo "Files copied"
fi

echo
echo "**** Clean volume..."
dot_clean "${mount_path}"
find "${mount_path}" -name '.DS_Store' -delete
rm -rf "${mount_path}/.fseventsd"

cleanup

echo "Finished"
