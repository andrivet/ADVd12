#!/usr/bin/env zsh

: '
Create a new release image. The argument is the name of the build environment.
'

set -euo pipefail

name="ADVd12"
size=200 # MB

if [[ "$OSTYPE" != darwin* ]]; then echo "Works only on macOS, sorry" ; exit 1 ; fi
if [[ $# -ne 2 ]] ; then echo "Invalid number of arguments" ; exit 1; fi

version=$1
env=$2
scripts="${0:A:h}"
release="$(cd "${scripts}/../../releases/v${version}" && pwd)"
root="$(cd "${scripts}/.." && pwd)"
lcd="$(cd "${scripts}/../lcd" && pwd)"
img="${release}/${env}-${version}.img"

echo "======================================================="
echo "Create ADVd12 version ${version} for environment ${env}"
echo "======================================================="

platformio run -d "${root}" -e "${env}"

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

echo
echo "**** Copy firware..."
cp "${root}/.pio/build/${env}/Robin_nano35.bin" "${mount_path}/"
echo "Firmware copied"

echo
echo "**** Clean volume..."
dot_clean "${mount_path}"
find "${mount_path}" -name '.DS_Store' -delete
rm -rf "${mount_path}/.fseventsd"

cleanup

echo
echo "**** Compress the SD image..."
zip -j "${img}.zip" "${img}"
rm "${img}"

echo "Finished"
echo
