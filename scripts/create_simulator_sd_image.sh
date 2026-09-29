#!/usr/bin/env zsh
: '
Create a SD image 'fs.img' for the simulator.
It contains the assets generated from the PNG files in lcd/png and the GCODE files in lcd/gcode.
'

img="fs.img"
name="SD"
size=200 # in MB

if [[ "$OSTYPE" != "darwin"* ]]; then echo "Work only on macOS, sorry" ; exit 1; fi

scripts="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ret=$?; if [[ $ret != 0 ]]; then exit $ret; fi

lcd="$( cd "${scripts}/../lcd" && pwd )"
ret=$?; if [[ $ret != 0 ]]; then exit $ret; fi

data="$( cd "${scripts}/../data" && pwd )"
ret=$?; if [[ $ret != 0 ]]; then exit $ret; fi

python "${scripts}/adv_encode_assets.py" "${lcd}/png" --outdir "${lcd}/assets"

echo "Create empty image..."
dd if=/dev/zero of="$data/$img" bs=1m count=$size
echo "Attach empty image..."
dev="$(hdiutil attach -nomount $data/$img)"
dev="$(echo $dev | xargs)"
echo "Attached to device $dev"
echo "Format image..."
diskutil erasedisk fat32 $name mbr "$dev"
mount_path=$(diskutil info ${dev}s1 | awk -F': *' '/Mount Point/ {print $2}')
echo "Volume is mounted as $mount_path"
echo "Copy assets.."
cp -r "${lcd}/assets" "$mount_path/"
echo "Copy GCODE files..."
cp -r "${lcd}/gcode/"*.gcode "$mount_path/"
echo "Clean folder..."
dot_clean "$mount_path"
echo "Detach volume..."
hdiutil detach "$dev"

echo "Finished"
