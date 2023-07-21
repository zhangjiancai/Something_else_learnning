import cv2
import numpy as np
import matplotlib.pyplot as plt

img1 = cv2.imread("opening.bmp")
img2 = cv2.imread("opening2.bmp")
k = np.ones((10, 10), np.uint8)

r1 = cv2.morphologyEx(img1, cv2.MORPH_OPEN, k)
r2 = cv2.morphologyEx(img2, cv2.MORPH_OPEN, k)

plt.figure(figsize=(8, 8))
plt.rcParams['font.sans-serif'] = 'simHei'
titles = [u'(a)img1', u'(b)result1', u'(c)img2', u'(d)result2']
images = [img1, r1, img2, r2]

for i in range(4):
    plt.subplot(2, 2, i + 1), plt.imshow(images[i], 'gray')
    plt.title(titles[i])
    plt.xticks([]), plt.yticks([])

plt.show()
