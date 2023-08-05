#!/usr/bin/python3

import argparse, sys
from PIL import Image, ImagePalette

parser = argparse.ArgumentParser(prog="converter", description="Converts images to format compatible with Waveshare's 3 inch 4-colour panel.")

parser.add_argument("input")
parser.add_argument("-d", "--dither", action="store_true")
args = parser.parse_args()

DISP_WIDTH = 168
DISP_HEIGHT = 400

image = Image.open(args.input)

# Create a palette with the 4 colors supported by the panel
palette = Image.new("P", (1,1))
palette.putpalette((0,0,0,  255,255,255,  255,255,0,   255,0,0) + (0,0,0)*252)

# Check if we need to rotate the image
imwidth, imheight = image.size
if imwidth == DISP_WIDTH and imheight == DISP_HEIGHT:
    image_temp = image
elif imwidth == DISP_HEIGHT and imheight == DISP_WIDTH:
    image_temp = image.rotate(90, expand=True)
else:
    print("Invalid image dimensions: %d x %d, expected %d x %d" % (imwidth, imheight, DISP_WIDTH, DISP_HEIGHT))

# Convert the source image to the 4 colors, dithering if requested

if args.dither:
    image_4color = image_temp.convert("RGB").quantize(palette=palette, dither=Image.Dither.FLOYDSTEINBERG)
else: 
    image_4color = image_temp.convert("RGB").quantize(palette=palette, dither=Image.Dither.NONE)
# image_4color.show()
buf_4color = bytearray(image_4color.tobytes('raw'))


# into a single byte to transfer to the panel
buf = [0x00] * int(DISP_WIDTH * DISP_HEIGHT / 4)
idx = 0
for i in range(0, len(buf_4color), 4):
    buf[idx] = (buf_4color[i] << 6) + (buf_4color[i+1] << 4) + (buf_4color[i+2] << 2) + buf_4color[i+3]
    idx += 1

with open(args.input + ".h", "w") as output:
    variable_name = args.input.replace(".", "_")
    define_name = variable_name.upper() + "_H"
    output.writelines([f"#ifndef {define_name}\n", f"#define {define_name}\n", "#include \"Arduino.h\"\n" f"const uint8_t {variable_name}[] = {{\n"])
    # Split the buffer into lines with 100 items each 
    output.writelines([", ".join([str(el) for el in buf[n:n+100]]) + ",\n" for n in range(0, 16800, 100)])
    output.write("};\n#endif")
