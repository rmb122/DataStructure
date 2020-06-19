import cv2
import numpy
import copy


def to_gray(img):
    gray_img = numpy.zeros(img.shape[:2], dtype=numpy.uint8)

    for y in range(img.shape[0]):
        for x in range(img.shape[1]):
            gray_img[y][x] = 0.114 * img[y][x][0] + 0.587 * img[y][x][1] + 0.229 * img[y][x][2]

    return gray_img


def to_black_white(gray_img):
    black_white_img = copy.deepcopy(gray_img)

    h = black_white_img.shape[0]
    w = black_white_img.shape[1]
    threshold_t = 0
    max_g = 0

    for t in range(255):
        n0 = black_white_img[numpy.where(black_white_img < t)]
        n1 = black_white_img[numpy.where(black_white_img >= t)]
        w0 = len(n0) / (h * w)
        w1 = len(n1) / (h * w)
        u0 = numpy.mean(n0) if len(n0) > 0 else 0.
        u1 = numpy.mean(n1) if len(n0) > 0 else 0.

        g = w0 * w1 * (u0 - u1) ** 2
        if g > max_g:
            max_g = g
            threshold_t = t

    black_white_img[black_white_img < threshold_t] = 0
    black_white_img[black_white_img >= threshold_t] = 255

    return black_white_img


def rgb2hsv(r, g, b):
    r, g, b = r / 255.0, g / 255.0, b / 255.0
    mx = max(r, g, b)
    mn = min(r, g, b)
    diff = mx - mn

    h = 0
    if mx == r:
        if g >= b:
            h = 60 * ((g - b) / diff) + 0
        else:
            h = 60 * ((g - b) / diff) + 360
    elif mx == g:
        h = 60 * ((b - r) / diff) + 120
    elif mx == b:
        h = 60 * ((r - g) / diff) + 240

    # 先计算L
    l = (mx + mn) / 2.0

    s = 0
    if l > 0 and l <= 0.5:
        s = (diff / l) / 2.0
    elif l > 0.5:
        s = (diff / (1 - l)) / 2.0

    return h, s, l


def to_hsv(img):
    hsv_img = numpy.zeros(img.shape, dtype=numpy.float32)

    for y in range(hsv_img.shape[0]):
        for x in range(hsv_img.shape[1]):
            hsv_img[y][x][2], hsv_img[y][x][1], hsv_img[y][x][0] = rgb2hsv(img[y][x][2], img[y][x][1], img[y][x][0])

    return hsv_img


def mean_filter(img):
    new_img = numpy.zeros((img.shape[0] - 2, img.shape[1] - 2, img.shape[2]), dtype=numpy.uint8)

    for y in range(1, img.shape[0] - 1):
        for x in range(1, img.shape[1] - 1):
            B, G, R = 0, 0, 0
            for x_shift in range(-1, 2):
                for y_shift in range(-1, 2):
                    B += img[y + y_shift][x + x_shift][0]
                    G += img[y + y_shift][x + x_shift][1]
                    R += img[y + y_shift][x + x_shift][2]

            B //= 9
            G //= 9
            R //= 9

            new_img[y - 1][x - 1] = B, G, R
    return new_img


def gauss_filter(img):
    new_img = numpy.zeros((img.shape[0] - 2, img.shape[1] - 2, img.shape[2]), dtype=numpy.uint8)
    gauss_template = [1, 2, 1, 2, 4, 2, 1, 2, 1]

    for y in range(1, img.shape[0] - 1):
        for x in range(1, img.shape[1] - 1):
            B, G, R = 0, 0, 0
            for x_shift in range(-1, 2):
                for y_shift in range(-1, 2):
                    B += (img[y + y_shift][x + x_shift][0] * gauss_template[x_shift + y_shift * 3 + 4])
                    G += (img[y + y_shift][x + x_shift][1] * gauss_template[x_shift + y_shift * 3 + 4])
                    R += (img[y + y_shift][x + x_shift][2] * gauss_template[x_shift + y_shift * 3 + 4])

            B //= 16
            G //= 16
            R //= 16

            new_img[y - 1][x - 1] = B, G, R
    return new_img


def median_filter(img):
    new_img = numpy.zeros((img.shape[0] - 2, img.shape[1] - 2, img.shape[2]), dtype=numpy.uint8)


    for y in range(1, img.shape[0] - 1):
        for x in range(1, img.shape[1] - 1):
            r_temp_list = []
            g_temp_list = []
            b_temp_list = []

            for x_shift in range(-1, 2):
                for y_shift in range(-1, 2):
                    b_temp_list.append(img[y + y_shift][x + x_shift][0])
                    g_temp_list.append(img[y + y_shift][x + x_shift][1])
                    r_temp_list.append(img[y + y_shift][x + x_shift][2])

            b_temp_list.sort()
            g_temp_list.sort()
            b_temp_list.sort()
            new_img[y - 1][x - 1] = b_temp_list[4], g_temp_list[4], r_temp_list[4]
    return new_img


def bilateral_filter(img, radius, sigma_color, sigma_space) :
    B, G, R = cv2.split(img)
    B_tran, G_tran, R_tran = cv2.split(img)
    img_height = len(B)
    img_width = len(B[0])
    # 计算灰度值模板系数表
    color_coeff = -0.5 / (sigma_color * sigma_color)
    weight_color = []       # 存放颜色差值的平方
    for i in range(256) :
        weight_color.append(numpy.exp(i * i * color_coeff))
    # 计算空间模板
    space_coeff = -0.5 / (sigma_space * sigma_space)
    weight_space = []     # 存放模板系数
    weight_space_row = [] # 存放模板 x轴 位置
    weight_space_col = [] # 存放模板 y轴 位置
    maxk = 0
    for i in range(-radius, radius+1) :
        for j in range(-radius, radius+1) :
            r_square = i*i + j*j
            r = numpy.sqrt(r_square)
            weight_space.append(numpy.exp(r_square * space_coeff))
            weight_space_row.append(i)
            weight_space_col.append(j)
            maxk = maxk + 1
    # 进行滤波
    for row in range(img_height) :
        for col in range(img_width) :
            value = 0
            weight = 0
            for i in range(maxk) :
                m = row + weight_space_row[i]
                n = col + weight_space_col[i]
                if m < 0 or n < 0 or m >= img_height or n >= img_width:
                    val = 0
                else :
                    val = B[m][n]
                w = numpy.float32(weight_space[i]) * numpy.float32(weight_color[numpy.abs(val - B[row][col])])
                value = value + val * w
                weight = weight + w
            B_tran[row][col] = numpy.uint8(value / weight)
    # 绿色通道
    for row in range(img_height) :
        for col in range(img_width) :
            value = 0
            weight = 0
            for i in range(maxk) :
                m = row + weight_space_row[i]
                n = col + weight_space_col[i]
                if m < 0 or n < 0 or m >= img_height or n >= img_width :
                    val = 0
                else :
                    val = G[m][n]
                w = numpy.float32(weight_space[i]) * numpy.float32(weight_color[numpy.abs(val - G[row][col])])
                value = value + val * w
                weight = weight + w
            G_tran[row][col] = numpy.uint8(value / weight)
    # 红色通道
    for row in range(img_height) :
        for col in range(img_width) :
            value = 0
            weight = 0
            for i in range(maxk) :
                m = row + weight_space_row[i]
                n = col + weight_space_col[i]
                if m < 0 or n < 0 or m >= img_height or n >= img_width :
                    val = 0
                else :
                    val = R[m][n]
                w = numpy.float32(weight_space[i]) * numpy.float32(weight_color[numpy.abs(val - R[row][col])])
                value = value + val * w
                weight = weight + w
            R_tran[row][col] = numpy.uint8(value / weight)

    return cv2.merge([B_tran, G_tran, R_tran])


def sharp_filter(img):
    new_img = numpy.zeros((img.shape[0] - 2, img.shape[1] - 2, img.shape[2]), dtype=numpy.uint8)
    sharp_template = [-1, -1, -1, -1, 9, -1, -1, -1, -1]

    def check(i):
        if i < 0:
            return 0
        elif i > 255:
            return 255
        else:
            return i

    for y in range(1, img.shape[0] - 1):
        for x in range(1, img.shape[1] - 1):
            B, G, R = 0, 0, 0
            for x_shift in range(-1, 2):
                for y_shift in range(-1, 2):
                    B += (img[y + y_shift][x + x_shift][0] * sharp_template[x_shift + y_shift * 3 + 4])
                    G += (img[y + y_shift][x + x_shift][1] * sharp_template[x_shift + y_shift * 3 + 4])
                    R += (img[y + y_shift][x + x_shift][2] * sharp_template[x_shift + y_shift * 3 + 4])

            new_img[y - 1][x - 1] = check(B), check(G), check(R)
    return new_img


img: numpy.ndarray = cv2.imread('example.jpg')

#cv2.imwrite('gray.png', to_gray(img))
#cv2.imwrite('bin.png', to_black_white(to_gray(img)))
cv2.imwrite('hsv.png', to_hsv(img))
#cv2.imwrite('mean_filter.png', mean_filter(img))
#cv2.imwrite('median_filter.png', median_filter(img))
#cv2.imwrite('gauss_filter.png', median_filter(img))
#cv2.imwrite('bilateral_filter.png', median_filter(img))
#cv2.imwrite('sharp_filter.png', median_filter(img))
