import cv2
import numpy as np

# 读取图像文件
img = cv2.imread('pill3.jpg')
img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 预处理
_, thresh = cv2.threshold(img_gray, 127, 255, cv2.THRESH_BINARY_INV)
kernel = np.ones((5, 5), np.uint8)
opening1 = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)

# 确定前景
dist_transform = cv2.distanceTransform(opening1, cv2.DIST_L2, 3)
_, fore = cv2.threshold(dist_transform, 0.7 * dist_transform.max(), 255, cv2.THRESH_BINARY)

# 去噪处理
opening2 = cv2.morphologyEx(fore, cv2.MORPH_CLOSE, kernel)

# 寻找轮廓
contours, _ = cv2.findContours(opening2.astype(np.uint8), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# 缺陷检测
result = img.copy()
count_good = 0
count_bad = 0
for contour in contours:
    # 计算面积
    area_contour = cv2.contourArea(contour)
    (x, y), radius = cv2.minEnclosingCircle(contour)
    area_circle = np.pi * radius ** 2

    # 面积比较
    ratio = area_contour / area_circle
    if ratio > 0.8:
        count_good += 1
        cv2.putText(result, 'OK', (int(x), int(y)), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    else:
        count_bad += 1
        cv2.putText(result, 'BAD', (int(x), int(y)), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

# 显示结果
cv2.putText(result, 'Count: {}'.format(count_good + count_bad), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255),
            2)
cv2.imshow('Original Image', img)
cv2.imshow('Threshold Image', thresh)
cv2.imshow('Opening1 Image', opening1)
cv2.imshow('Foreground Image', fore)
cv2.imshow('Opening2 Image', opening2)
cv2.imshow('Result', result)
cv2.waitKey(0)
cv2.destroyAllWindows()