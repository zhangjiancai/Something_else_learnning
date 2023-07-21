import cv2
import matplotlib.pyplot as plt

# 加载图像
image_path = 'gg.bmp'
image = cv2.imread(image_path)

# 去噪
image = cv2.GaussianBlur(image, (3, 3), 0)

# 灰度化
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# 二值化
ret, binary = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)

# 膨胀
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
dilated = cv2.dilate(binary, kernel)

# 寻找轮廓
contours, hierarchy = cv2.findContours(dilated, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# 分割字符
plate_contours = []
for contour in contours:
    (x, y, w, h) = cv2.boundingRect(contour)
    aspect_ratio = w / float(h)
    if 1.5 <= aspect_ratio <= 3.5 and w > 20:
        plate_contours.append(contour)
        cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 2)

# 显示结果
plt.figure(figsize=(10, 8))
plt.subplot(1, 2, 1)
plt.imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
plt.title('License Plate')

plt.subplot(1, 2, 2)
plt.imshow(binary, cmap='gray')
plt.title('Binary Image')

plt.tight_layout()
plt.show()
