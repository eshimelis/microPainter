# Austin Chun
# Micro-P's Final Project
# Convert a .png image file to a .bmp image file using PIL

# Import Pillow library (image processing library)
from PIL import Image

# Load file
file_in = "image0.png"
img = Image.open(file_in)

# print(img.width) 
# print(img.height) 

# Scale image to max 144 pixels tall
maxsize = (1000000, 144)
img.thumbnail(maxsize, Image.ANTIALIAS)
bg = Image.new("RGB", img.size, (0,0,0))

bg.paste(img,img)

# print(img.width)
# print(img.height)

# Write out bmp file
bg.save("image0.bmp")