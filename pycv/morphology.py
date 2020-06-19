import cv2
import numpy
import copy

def to_gray(img):
    gray_img = numpy.zeros(img.shape[:2], dtype=numpy.uint8)

    for y in range(img.shape[0]):
        for x in range(img.shape[1]):
            gray_img[y][x] = 0.114 * img[y][x][0] + 0.587 * img[y][x][1] + 0.229 * img[y][x][2]

    cv2.imshow('img', gray_img)
    cv2.waitKey()
    cv2.destroyWindow('img')
    return gray_img


def expansion(img):
    expansion_array = numpy.array([[0, 1, 0],
                                   [1, 1, 1],
                                   [0, 1, 0]])
    expansion_img = copy.deepcopy(img)

    for y in range(1, img.shape[0] - 1):
        for x in range(1, img.shape[1] - 1):
            if img[y][x] == 255:
                for x_shift in range(-1, 2):
                    for y_shift in range(-1, 2):
                        if expansion_array[y_shift + 1][x_shift + 1] == 1:
                            expansion_img[y + y_shift][x + x_shift] = 255

    return expansion_img


def corrosion(img):
    corrosion_array = numpy.array([[0, 1, 0],
                                   [1, 1, 1],
                                   [0, 1, 0]])
    corrosion_img = numpy.zeros((img.shape[0] - 2, img.shape[1] - 2), dtype=numpy.uint8)

    for y in range(1, img.shape[0] - 1):
        for x in range(1, img.shape[1] - 1):
            corrosion_flag = True
            for x_shift in range(-1, 2):
                for y_shift in range(-1, 2):
                    if img[y + y_shift][x + x_shift] != 255 and corrosion_array[y_shift + 1][x_shift + 1] == 1:
                        corrosion_flag = False

            if corrosion_flag:
                corrosion_img[y - 1][x - 1] = 255
            else:
                corrosion_img[y - 1][x - 1] = 0
    return corrosion_img


img: numpy.ndarray = cv2.imread('example.jpg', cv2.IMREAD_GRAYSCALE)
_, img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)

c_img = corrosion(img)
cv2.imwrite('corrosion.png', c_img)

e_img = expansion(img)
cv2.imwrite('expansion.png', e_img)
