import cv2
import matplotlib.pyplot as plt

# 读取图像
o = cv2.imread("kernel.bmp", cv2.IMREAD_UNCHANGED)

# 创建不同形状的结构元素
# cv2.getStructuringElement()函数用于创建指定形状和大小的结构元素
# 参数说明：
#   - cv2.MORPH_RECT: 结构元素的形状，此处指定为矩形
#   - (50, 50): 结构元素的大小，此处指定为50x50
kernel1 = cv2.getStructuringElement(cv2.MORPH_RECT, (50, 50))
kernel2 = cv2.getStructuringElement(cv2.MORPH_CROSS, (50, 50))
kernel3 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (50, 50))

# 膨胀操作
# cv2.dilate()函数用于对图像进行膨胀操作
# 参数说明：
#   - o: 输入图像
#   - kernel1: 结构元素，此处使用矩形结构元素
# 返回值：
#   - dst1: 膨胀后的图像
dst1 = cv2.dilate(o, kernel1)

# 类似地对o进行膨胀操作，但使用不同的结构元素
dst2 = cv2.dilate(o, kernel2)
dst3 = cv2.dilate(o, kernel3)

# 显示图像
plt.figure(figsize=(8, 8))
titles = [u'(a)original', u'(b)MORPH_RECT', u'(c)MORPH_CROSS', u'(d)MORPH_ELLIPSE']
images = [o, dst1, dst2, dst3]
for i in range(4):
    plt.subplot(2, 2, i+1)
    plt.imshow(images[i], 'gray')
    plt.title(titles[i])
    plt.xticks([])
    plt.yticks([])
plt.show()
