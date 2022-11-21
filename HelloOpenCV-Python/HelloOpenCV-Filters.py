import numpy as np
import cv2 as cv

img = cv.imread('mario.png')  # original

grayscale = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
blur = cv.blur(img, (5, 5))
Gblur = cv.GaussianBlur(img, (5, 5), 0)
canny = cv.Canny(Gblur, 100, 200)


cv.imshow("Imagem", img)
cv.imshow("Cinza", grayscale)
# cv.imshow("Blur", blur)
# cv.imshow("Gaussian Blur", Gblur)
cv.imshow("Canny", canny)

k = cv.waitKey(0)
