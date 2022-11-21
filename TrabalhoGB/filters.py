import cv2 as cv
import numpy as np
from scipy.interpolate import UnivariateSpline


def sepia(img):
    img = np.array(img, dtype=np.float64)
    img = cv.transform(img, np.matrix([[0.272, 0.534, 0.131],
                                       [0.349, 0.686, 0.168],
                                       [0.393, 0.769, 0.189]]))
    img[np.where(img > 255)] = 255
    img = np.array(img, dtype=np.uint8)
    return img


def nitidez(img):
    kernel = np.array([[0, -1, 0],
                       [-1, 5, -1],
                       [0, -1, 0]])
    filter2d = cv.filter2D(src=img, ddepth=-1, kernel=kernel)

    return filter2d


def lapis(img, peb):
    sk_gray, sk_color = cv.pencilSketch(img, sigma_s=60, sigma_r=0.07, shade_factor=0.06)
    if peb:
        return sk_gray
    else:
        return sk_color


def LookupTable(x, y):
    spline = UnivariateSpline(x, y)
    return spline(range(256))


def verao(img):
    increaseLookupTable = LookupTable([0, 64, 128, 256], [0, 80, 160, 256])
    decreaseLookupTable = LookupTable([0, 64, 128, 256], [0, 50, 100, 256])
    blue_channel, green_channel, red_channel = cv.split(img)
    red_channel = cv.LUT(red_channel, increaseLookupTable).astype(np.uint8)
    blue_channel = cv.LUT(blue_channel, decreaseLookupTable).astype(np.uint8)
    img = cv.merge((blue_channel, green_channel, red_channel))

    return img


def inverno(img):
    increaseLookupTable = LookupTable([0, 64, 128, 256], [0, 80, 160, 256])
    decreaseLookupTable = LookupTable([0, 64, 128, 256], [0, 50, 100, 256])
    blue_channel, green_channel, red_channel = cv.split(img)
    red_channel = cv.LUT(red_channel, decreaseLookupTable).astype(np.uint8)
    blue_channel = cv.LUT(blue_channel, increaseLookupTable).astype(np.uint8)
    img = cv.merge((blue_channel, green_channel, red_channel))

    return img
