import cv2
from matplotlib import pyplot
import numpy

img: numpy.ndarray = cv2.imread('example.jpg', cv2.IMREAD_GRAYSCALE)

gray_count = [0 for i in range(256)]

for y in range(img.shape[0]):
    for x in range(img.shape[1]):
        gray_count[img[y][x]] += 1


pyplot.bar(range(len(gray_count)), gray_count)
pyplot.savefig('histo')

gray_sum_count = [0 for i in range(256)]

for i in range(len(gray_count)):
    gray_sum_count[i] = gray_count[i]
    if i != 0:
        gray_sum_count[i] += gray_sum_count[i - 1]

pyplot.bar(range(len(gray_sum_count)), gray_sum_count)
pyplot.savefig('sum_histo')
