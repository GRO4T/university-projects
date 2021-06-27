from PIL import Image
import sys

if (len(sys.argv) != 8):
    print "Wrong number of arguments!"
    print "Usage: python2 sprite_cutter.py filename left_up_corner_x left_up_corner_y width height cols rows"
    print "e.g. python2 sprite_cutter.py filename 0 0 42 42 14 1"

filename = sys.argv[1]
sourceImg = Image.open(filename)
imgWidth, imgHeight = sourceImg.size

rectX = int(sys.argv[2])
rectY = int(sys.argv[3])
rectWidth = int(sys.argv[4])
rectHeight = int(sys.argv[5])
cols = int(sys.argv[6])
rows = int(sys.argv[7])
rowHeight = imgHeight / rows
colWidth = imgWidth / cols

destImg = Image.new("RGBA", (rectWidth * cols, rectHeight * rows))

for i in range(0, rows):
    for j in range(0, cols):
        cornerX = rectX + j * colWidth
        cornerY = rectY + i * rowHeight
        box = (cornerX, cornerY, cornerX + rectWidth, cornerY + rectHeight)
        img = sourceImg.crop(box)
        destImg.paste(img, (j*rectWidth, i*rectHeight), img.convert('RGBA'))

destImg.save("copy.png", "PNG")
