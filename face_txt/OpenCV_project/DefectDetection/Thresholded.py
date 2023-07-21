
import cv2
import numpy as np

# 读取图像文件
img = cv2.imread('coins.jpg', cv2.IMREAD_GRAYSCALE)

# 图像预处理（利用开运算去噪）
kernel = np.ones((5, 5), np.uint8)
opening = cv2.morphologyEx(img, cv2.MORPH_OPEN, kernel)

# 距离变换
dist_transform = cv2.distanceTransform(opening, cv2.DIST_L2, 3)

# 阈值分割
_, thresholded = cv2.threshold(dist_transform, 0.7 * dist_transform.max(), 255, cv2.THRESH_BINARY)

# 显示结果
cv2.imshow('Original Image', img)
cv2.imshow('Distance Transform', dist_transform)
cv2.imshow('Thresholded Image', thresholded)
cv2.waitKey(0)
cv2.destroyAllWindows()

