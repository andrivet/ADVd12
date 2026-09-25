#!/usr/bin/env bash

img=fs.img
name=SD

pushd "../data" || exit

python ../scripts/adv_encode_assets.py ../png --outdir ../assets

echo "Create empty image..."
dd if=/dev/zero of=$img bs=1m count=200
echo "Attach empty image..."
dev="$(hdiutil attach -nomount $img)"
dev="$(echo $dev | xargs)"
echo "Attached to device $dev"
echo "Format image..."
diskutil erasedisk fat32 $name mbr "$dev"
mount_path=$(diskutil info ${dev}s1 | awk -F': *' '/Mount Point/ {print $2}')
echo "Volume is mounted as $mount_path"
echo "Copy assets.."
cp -r "../assets" "$mount_path/"
echo "Copy GCODE files..."
cp -r "../gcode/"*.gcode "$mount_path/"
echo "Clean folder..."
dot_clean "$mount_path"
echo "Detach volume..."
hdiutil detach "$dev"

echo "Copy SD image to data folder..."
cp "$img" "../../advd12/data/"

echo "Finished"

popd || exit
