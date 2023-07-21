import cv2
import matplotlib.pyplot as plt

# 读取图像
img = cv2.imread('count.jpg', 1)

# 转换为灰度图像
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 图像二值化
# cv2.threshold()函数用于对图像进行阈值处理
# 参数说明：
#   - gray: 输入图像
#   - 150: 阈值，图像中像素值大于阈值的部分被设为255，小于等于阈值的部分被设为0
#   - 255: 阈值化后的最大值
#   - cv2.THRESH_BINARY_INV: 阈值处理的类型，此处为反二值化处理，大于阈值的部分设为0，小于等于阈值的部分设为最大值
# 返回值：
#   - ret: 实际使用的阈值，对于反二值化处理来说，通常与输入的阈值相同
#   - binary: 阈值处理后的图像
ret, binary = cv2.threshold(gray, 150, 255, cv2.THRESH_BINARY_INV)

# 获取椭圆形的结构元素
kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))

# 腐蚀操作
# cv2.erode()函数用于对图像进行腐蚀操作
# 参数说明：
#   - binary: 输入图像
#   - kernel: 结构元素，用于定义腐蚀的形状和大小
#   - iterations: 腐蚀的迭代次数
# 返回值：
#   - erosion: 腐蚀后的图像
erosion = cv2.erode(binary, kernel, iterations=4)

# 膨胀操作
# cv2.dilate()函数用于对图像进行膨胀操作
# 参数说明：
#   - erosion: 输入图像
#   - kernel: 结构元素，用于定义膨胀的形状和大小
#   - iterations: 膨胀的迭代次数
# 返回值：
#   - dilation: 膨胀后的图像
dilation = cv2.dilate(erosion, kernel, iterations=3)

# 高斯模糊
# cv2.GaussianBlur()函数用于对图像进行高斯模糊处理
# 参数说明：
#   - dilation: 输入图像
#   - (3, 3): 高斯核的大小，此处为3x3的大小
#   - 0: 高斯核的标准差，0表示自动计算
# 返回值：
#   - gaussian: 高斯模糊后的图像
gaussian = cv2.GaussianBlur(dilation, (3, 3), 0)

# 查找轮廓
# cv2.findContours()函数用于查找图像中的轮廓
# 参数说明：
#   - gaussian: 输入图像
#   - cv2.RETR_TREE: 轮廓检索模式，表示检索所有轮廓并建立轮廓间的完整层次结构
#   - cv2.CHAIN_APPROX_SIMPLE: 轮廓近似方法，表示仅保留轮廓的端点信息
# 返回值：
#   - contours: 轮廓列表
#   - hierarchy: 轮廓的层次结构
contours, hierarchy = cv2.findContours(gaussian, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

contoursOK = []
for i in contours:
    # 通过轮廓的面积进行筛选
    if cv2.contourArea(i) > 30:
        contoursOK.append(i)

# 绘制轮廓
# cv2.drawContours()函数用于在图像上绘制轮廓
# 参数说明：
#   - img: 输入图像
#   - contoursOK: 轮廓列表
#   - -1: 绘制所有轮廓
#   - (0, 255, 0): 绘制的颜色，这里为绿色
#   - 1: 线条宽度
draw = cv2.drawContours(img, contoursOK, -1, (0, 255, 0), 1)

for i, j in zip(contoursOK, range(len(contoursOK))):
    # 计算轮廓的质心坐标
    M = cv2.moments(i)
    cX = int(M["m10"] / M["m00"])
    cY = int(M["m01"] / M["m00"])
    # 在图像上绘制轮廓序号
    cv2.putText(draw, str(j), (cX, cY), cv2.FONT_HERSHEY_PLAIN, 1.5, (0, 0, 255), 2)

plt.rcParams['font.sans-serif'] = ['SimHei']

plt.figure(figsize=(8, 8))
titles = [u'(a)高斯模糊', u'(b)绘制轮廓']
images = [gaussian, draw]
for i in range(2):
    plt.subplot(1, 2, i+1)
    plt.imshow(images[i], 'gray')
    plt.title(titles[i])
    plt.xticks([])
    plt.yticks([])
plt.show()
