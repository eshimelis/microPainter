# Austin Chun & Eyassu Shimelis
# Micro-P's Final Project
# Convert a .png image file to a .bmp image file using PIL

# Import Pillow library (image processing library)
from PIL import Image

# Load file
img = Image.open("image0.png")

# Make sure in RGBA format
img = img.convert("RGBA")


# Scaling factor
(width, height) = img.size
# Max height of 144 pixels
scale = 144.0 / height
print ("Scale: ", scale)
newHeight = scale*height
# Adjust horizontal resolution
newWidth = 1.5*scale*width

print("Height: ", newHeight, ", Width: ", newWidth)


# Scales image to max height 144, and some ratio for width to keep high resolution horizontally
img = img.resize( (int(newWidth), int(newHeight)), Image.ANTIALIAS)

# Add a background to fix bugs
bg = Image.new("RGB", img.size, (0,0,0) )
# Paste image on background and use a mask to fix bugs
# # (I beleive the bugs were the transparencies)
bg.paste(img , img)

# Write out bmp file
bg.save("image0.bmp")


# # Austin Chun
# # Micro-P's Final Project
# # Convert a .png image file to a .bmp image file using PIL
#
# # Import Pillow library (image processing library)
# from PIL import Image
#
# # Load file
# file_in = "image0.png"
# img = Image.open(file_in)
#
# # print(img.width)
# # print(img.height)
#
# # Scale image to max 144 pixels tall
# maxsize = (1000000, 144)
# img.thumbnail(maxsize, Image.ANTIALIAS)
# bg = Image.new("RGB", img.size, (0,0,0))
#
# bg.paste(img,img)
#
# # print(img.width)
# # print(img.height)
#
# # Write out bmp file
# bg.save("image0.bmp")
