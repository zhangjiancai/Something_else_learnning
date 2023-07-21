import cv2
import matplotlib.pyplot as plt

# 读取图像
img = cv2.imread("lena.bmp")

# 对图像进行二值化处理
# cv2.threshold()函数用于对图像进行阈值处理
# 参数说明：
#   - img: 输入图像
#   - 127: 阈值，图像中像素值大于阈值的部分被设为255，小于等于阈值的部分被设为0
#   - 255: 阈值化后的最大值
#   - cv2.THRESH_BINARY: 阈值处理的类型，此处为二值化处理，大于阈值的部分设为最大值，小于等于阈值的部分设为0
# 返回值：
#   - t: 实际使用的阈值，对于二值化处理来说，通常与输入的阈值相同
#   - rst: 阈值处理后的图像
t, rst = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)

# 显示图像
plt.figure(figsize=(8, 8))
plt.rcParams['font.sans-serif'] = ['SimHei']
titles = [u'(a)原始图像', u'(b)二值化结果']
images = [img, rst]
for i in range(2):
    plt.subplot(1, 2, i+1)
    plt.imshow(images[i], 'gray')
    plt.title(titles[i])
    plt.xticks([])
    plt.yticks([])
plt.show()
