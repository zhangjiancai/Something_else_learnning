import cv2
import numpy as np

# 读取图像文件
img = cv2.imread('cc.bmp', cv2.IMREAD_GRAYSCALE)

# 进行二值化处理
_, binary = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)

# 查找轮廓
contours, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

# 构造包围圆形
(x, y), radius = cv2.minEnclosingCircle(contours[0])
center = (int(x), int(y))
radius = int(radius)

# 绘制包围圆形
result = cv2.cvtColor(img, cv2.COLOR_GRAY2BGR)
cv2.circle(result, center, radius, (0, 0, 255), 2)

# 显示结果
cv2.imshow('Original Image', img)
cv2.imshow('Enclosing Circle', result)
cv2.waitKey(0)
cv2.destroyAllWindows()

