#!/usr/bin/env zsh

: '
Create a new release image. The argument is the name of the build environment.
'

set -euo pipefail

version="1.0.0"
name="ADVd12"
size=200 # MB

if [[ "$OSTYPE" != darwin* ]]; then echo "Works only on macOS, sorry" ; exit 1 ; fi
if [[ $# -ne 1 ]] ; then echo "Invalid number of arguments" ; exit 1; fi

env=$1
scripts="${0:A:h}"
mkdir -p "${scripts}/../../releases/v${version}"
release="$(cd "${scripts}/../release/v${version}" && pwd)"
root="$(cd "${scripts}/.." && pwd)"
lcd="$(cd "${scripts}/../lcd" && pwd)"
img="${release}/${env}"

echo "======================================"
echo "Create ADVd12 version ${version}"
echo "======================================"
echo
echo "BE SURE YOU HAVE CHANGED:"
echo "- CHANGELOG.md"
echo "- bump-my-version <part>"
echo "- User Manual if necessary"
echo
pause 'Press any key to continue or Ctrl-C to abort...'

echo
echo "***** Remove previous files if any"
rm -rf "${release:?}/*"

echo
echo "***** Compile Mainboard firmwares..."
echo
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

echo
echo "**** Copy firware..."
cp "${root}/.pio/build/advi3pp_51/Robin_nano35.bin" "${mount_path}/"

echo
echo "**** Clean volume..."
dot_clean "${mount_path}"
find "${mount_path}" -name '.DS_Store' -delete
rm -rf "${mount_path}/.fseventsd"

cleanup

echo "Finished"
