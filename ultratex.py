# depends on Pillow or PIL being installed
from PIL import Image
import os
import sys
import math

"""
usage: python ultratex.py mysprite.png
"""


in_file = sys.argv[1]
basename, file_ext = os.path.splitext(os.path.basename(in_file))

img = Image.open(in_file)
rgb_im = img

print(rgb_im.size)

# texformat = "RGBA8888"
texformat = "RGBA5551"

preview_tokens = [" ", ".", ":", "-", "=", "+", "*", "#", "%", "@"]


aligner = ""
if False:
    aligner = "__attribute__((aligned(16)))"

preview = []
output = []
for row in range(rgb_im.size[1]):
    output_row = []
    for col in range(rgb_im.size[0]):
        pixel = rgb_im.getpixel((col, row))
        alpha = 255  # fully opaque if alpha channel is not present
        if len(pixel) == 4:
            alpha = pixel[3]
        preview_char = preview_tokens[
            math.floor(
                (
                    ((pixel[0] + pixel[1] + pixel[2]) / (255 * 3) * (alpha / 255))
                    * (len(preview_tokens) - 1)
                )
            )
        ]
        preview.append(preview_char + preview_char)
        # print(pixel)
        if texformat == "RGBA5551":
            out_px = pixel[0] >> 3  # 8 bit to 5 bit
            out_px = out_px << 5
            out_px += pixel[1] >> 3
            out_px = out_px << 5
            out_px += pixel[2] >> 3
            out_px = out_px << 1
            out_px += alpha >> 7  # 8 bit to 1 bit
        elif texformat == "RGBA8888":
            out_px = pixel[0]
            out_px = out_px << 8
            out_px += pixel[1]
            out_px = out_px << 8
            out_px += pixel[2]
            out_px = out_px << 8
            out_px += alpha
        output_row.append("0x" + hex(out_px)[2:].rjust(4, "0"))
    output.append(", ".join(output_row))
    preview.append("\n")

out = """
/*
Name: %s
Texture preview:
%s
*/
unsigned short Sprite_%s[] %s = {
  %s
};
""" % (
    basename,
    "".join(preview),
    basename,
    aligner,
    ",\n  ".join(output),
)

outfile = open(basename + ".h", "w")
outfile.write(out)
outfile.close()
