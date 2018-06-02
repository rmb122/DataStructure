from PIL import Image as image
from PIL import ImageFilter

def loadPic(name):
    piexls = list()
    img = image.open(name)
    imgPix = img.convert('RGBA').load()
    for x in range(img.size[0]):
        temp = list()
        for y in range(img.size[1]):
            temp.append(imgPix[x, y])
        piexls.append(temp)
    return piexls


def graify(srcPiexls):
    piexls = list()
    for x in range(len(srcPiexls)):
        temp = list()
        for y in range(len(srcPiexls[x])):
            grayscale = (srcPiexls[x][y][0] + srcPiexls[x][y][1] + srcPiexls[x][y][2]) // 3
            temp.append(grayscale)
        piexls.append(temp)
    return piexls


def pixs2pic(piexls, mode):
    img = image.new(mode, (len(piexls), len(piexls[0])))
    for x in range(len(piexls)):
        for y in range(len(piexls[x])):
            img.putpixel((x, y), piexls[x][y])
    return img


def sharpifyRGBA(piexls):
    newPiexls = list()
    laplaTemplate = [-1, -1, -1, -1, 9, -1, -1, -1, -1]
    overfolw = lambda x: x if x <= 255 else 255
    underflow = lambda x: x if x >= 0 else 0
    for x in range(1, len(piexls) - 1):
        temp = list()
        for y in range(1, len(piexls[x]) - 1):
            R, G, B = 0, 0, 0
            curr = 0
            for xShift in range(-1, 2):
                for yShift in range(-1, 2):
                    R += (piexls[x + xShift][y + yShift][0] * laplaTemplate[curr])
                    G += (piexls[x + xShift][y + yShift][1] * laplaTemplate[curr])
                    B += (piexls[x + xShift][y + yShift][2] * laplaTemplate[curr])
                    curr += 1
            R = overfolw(underflow(R))
            G = overfolw(underflow(G))
            B = overfolw(underflow(B))
            temp.append((R, G, B, piexls[x][y][3]))
        newPiexls.append(temp)
    return newPiexls
            

def sharpifyGray(piexls):
    newPiexls = list()
    laplaTemplate = [-1, -1, -1, -1, 9, -1, -1, -1, -1]
    overfolw = lambda x: x if x <= 255 else 255
    underflow = lambda x: x if x >= 0 else 0
    for x in range(1, len(piexls) - 1):
        temp = list()
        for y in range(1, len(piexls[x]) - 1):
            grayscale = 0
            curr = 0 
            for xShift in range(-1, 2):
                for yShift in range(-1, 2):
                    grayscale += (piexls[x + xShift][y + yShift]* laplaTemplate[curr])      
                    curr += 1
            grayscale = overfolw(underflow(grayscale))
            temp.append(grayscale)
        newPiexls.append(temp)
    return newPiexls


piexls = loadPic("test.png")
#piexls = graify(piexls)
piexls = sharpifyRGBA(piexls)
pixs2pic(piexls, "RGBA").save("233.png")
#i = image.open("test.png")
#i.filter(ImageFilter.SHARPEN).show()
pass