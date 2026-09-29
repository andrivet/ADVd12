#!/usr/bin/env python3
"""
Batch-encode PNG files to ADVd12 .bin assets.

- If filename is "logo.png" => RAW RGB565 (little-endian), must be 480x320
- Otherwise, use this layout:
    header
    line 0 offset (4 bytes)
    line 1 offset (4 bytes)
    ...
    line h offset (4 bytes)
    line 0 RLE data (variable length)
    line 1 RLE data (variable length)
    ...
    line h RLE data (variable length)

    Each RLE data is encoded as:
    - run length (1 byte)
    - pixel value (2 bytes, RGB565 little-endian)
    Run length is 1..255, pixel value is repeated for that many pixels.

    header is:
    - 4 bytes signature "RLE0"
    - 2 bytes width (little-endian)
    - 2 bytes height (little-endian)
    - 2 bytes lines offsets start (= header size)
    - 2 bytes RLE data start

Output:
- For each input *.png -> output *.bin with same stem.

Usage:
  python adv_encode_assets.py /path/to/pngs
  python adv_encode_assets.py /path/to/pngs --outdir /path/to/bins
"""

import argparse
from typing import cast
import fnmatch
import struct
import sys
from pathlib import Path
from PIL import Image


def pack_lvgl_header(cf: int, w: int, h: int) -> bytes:
    """
      cf = header & 0x1F
      w  = (header >> 10) & 0x7FF   (11 bits)
      h  = (header >> 21) & 0x7FF   (11 bits)
    """
    if not (0 <= cf <= 0x1F):
        raise ValueError("cf out of range (0..31)")
    if not (1 <= w <= 0x7FF):
        raise ValueError("width out of range (1..2047)")
    if not (1 <= h <= 0x7FF):
        raise ValueError("height out of range (1..2047)")

    header = (cf & 0x1F) | ((w & 0x7FF) << 10) | ((h & 0x7FF) << 21)
    return struct.pack("<I", header)


def rgb888_to_rgb565(r: int, g: int, b: int) -> int:
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def image_to_rgb565(img: Image.Image) -> bytes:
    im = img.convert("RGB")
    w, h = im.size

    out = bytearray(w * h * 2)
    idx = 0
    for y in range(h):
        for x in range(w):
            r, g, b = cast(tuple[int, int, int], im.getpixel((x, y)))
            rgb565 = rgb888_to_rgb565(r, g, b)
            struct.pack_into("<H", out, idx, rgb565)
            idx += 2
    return bytes(out)


def image_to_rgb565_lines(img: Image.Image) -> list[list[int]]:
    im = img.convert("RGB")
    w, h = im.size

    lines: list[list[int]] = []
    for y in range(h):
        line: list[int] = []
        for x in range(w):
            r, g, b = cast(tuple[int, int, int], im.getpixel((x, y)))
            rgb565 = rgb888_to_rgb565(r, g, b)
            line.append(rgb565)
        lines.append(line)
    return lines


def encode_rle_line(line: list[int]) -> bytes:
    out = bytearray()
    width = len(line)
    i = 0

    while i < width:
        px = line[i]
        run_len = 1

        while i + run_len < width and line[i + run_len] == px and run_len < 255:
            run_len += 1

        out += struct.pack("<BH", run_len, px)
        i += run_len

    return bytes(out)


def encode_to_rle(img: Image.Image, bin_path: Path) -> int:
    w, h = img.size
    lines = image_to_rgb565_lines(img)

    blobs: list[bytes] = []
    offsets: list[int] = []

    offset = 0
    for line in lines:
        blob = encode_rle_line(line)
        offsets.append(offset)
        blobs.append(blob)
        offset += len(blob)

    with open(bin_path, "wb") as f:
        f.write(struct.pack("<4sHH", b"RLE0", w, h))

        for offset in offsets:
            f.write(struct.pack("<I", offset))

        for blob in blobs:
            f.write(blob)

    return bin_path.stat().st_size


def encode_asset_to_bin(png_path: Path, outdir: Path) -> str:
    try:
        index = int(png_path.stem)
    except ValueError:
        raise ValueError(f"Unexpected asset name '{png_path.stem}'")

    bin_name = f"{index:04d}.bin"
    bin_path = outdir / bin_name
    img = Image.open(png_path)
    encode_to_rle(img, bin_path)

    return bin_name


def encode_logo_to_bin(png_path: Path, bin_path: Path) -> None:
    img = Image.open(png_path)
    w, h = img.size

    if (w, h) != (480, 320):
        raise ValueError(f"'{png_path.name}' size is {w}x{h} (expected 480x320)")

    with open(bin_path, "wb") as f:
        # RAW RGB565
        f.write(image_to_rgb565(img))


def encode_files(files: list[Path], outdir: Path) -> bool:
    ok = 0
    failed = 0

    outdir.parent.mkdir(parents=True, exist_ok=True)

    for png_path in files:
        bin_path = outdir / (png_path.stem + ".bin")
        try:
            if png_path.stem == "logo":
                encode_logo_to_bin(png_path, bin_path)
            else:
                bin_path = encode_asset_to_bin(png_path, outdir)

            #print(f"[OK] {png_path.name} -> {bin_path}")
            ok += 1
        except Exception as e:
            print(f"[FAIL] {png_path.name}: {e}", file=sys.stderr)
            failed += 1

    print(f"Done. Encoded: {ok}, failed: {failed}")
    return failed == 0


def main() -> int:
    ap = argparse.ArgumentParser(description="Convert a directory of PNG files to ADVd12 .bin assets.")
    ap.add_argument("indir", help="Directory containing .png files")
    ap.add_argument("--outdir", help="Output directory (default: same as indir)")
    args = ap.parse_args()

    indir = Path(args.indir)
    if not indir.is_dir():
        print(f"Error: {indir} is not a directory", file=sys.stderr)
        return 2

    outdir = Path(args.outdir) if args.outdir else indir

    files = [p for p in sorted(indir.iterdir())
             if p.is_file() and fnmatch.fnmatch(p.name, '*.png')]

    if not files:
        print(f"No files matching *.png in {indir}")
        return 0

    ok = encode_files(files, outdir)
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
