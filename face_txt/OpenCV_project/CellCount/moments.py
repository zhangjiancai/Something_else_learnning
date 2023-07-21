import cv2
import matplotlib.pyplot as plt

# 读取图像
o = cv2.imread('cat3.jpg', 1)
oc = o.copy()

# 转换为灰度图像
gray = cv2.cvtColor(o, cv2.COLOR_BGR2GRAY)

# 进行二值化处理
# cv2.threshold()函数将灰度图像二值化为黑白图像
# 参数说明：
#   - gray: 输入的灰度图像
#   - 127: 阈值，灰度值大于阈值的像素设为255（白色），灰度值小于等于阈值的像素设为0（黑色）
#   - 255: 最大像素值，当像素值大于阈值时的新值
#   - cv2.THRESH_BINARY: 二值化类型，指定为cv2.THRESH_BINARY表示黑白二值化
ret, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

# 提取轮廓
# cv2.findContours()函数用于提取图像中的轮廓
# 参数说明：
#   - binary: 输入的二值图像
#   - cv2.RETR_LIST: 轮廓检索模式，指定为cv2.RETR_LIST表示提取所有轮廓
#   - cv2.CHAIN_APPROX_SIMPLE: 轮廓逼近方法，指定为cv2.CHAIN_APPROX_SIMPLE表示使用简化的轮廓表示
contours, hierarchy = cv2.findContours(binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

# 在原图上绘制轮廓
# cv2.drawContours()函数用于在图像上绘制轮廓
# 参数说明：
#   - o: 输入的图像
#   - contours: 轮廓列表
#   - 0: 绘制轮廓的索引，此处指定为0表示绘制第一个轮廓
#   - (0, 0, 255): 绘制轮廓的颜色，此处指定为红色
#   - 3: 绘制轮廓的线宽
x = cv2.drawContours(o, contours, 0, (0, 0, 255), 3)

# 计算轮廓的矩
# cv2.moments()函数计算轮廓的矩
m = cv2.moments(contours[0])
m00 = m['m00']
m10 = m['m10']
m01 = m['m01']

# 计算中心点坐标
cx = int(m10 / m00)
cy = int(m01 / m00)

# 在图像上绘制标注文字
# cv2.putText()函数用于在图像上绘制文字
# 参数说明：
#   - o: 输入的图像
#   - "cat": 要绘制的文字内容
#   - (cx, cy): 文字的位置坐标
#   - cv2.FONT_HERSHEY_SIMPLEX: 字体类型，此处指定为简单字体
#   - 1: 字体大小
#   - (0, 0, 255): 字体颜色，此处指定为红色
#   - 3: 字体线宽
cv2.putText(o, "cat", (cx, cy), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 3)

# 用来正常显示中文标签
plt.rcParams['font.sans-serif'] = 'simHei'

# 显示图形
plt.figure(figsize=(8, 8))
titles = [u'(a)原始图像', u'(b)结果图像']
images = [oc, o]

for i in range(2):
    plt.subplot(1, 2, i+1)
    plt.imshow(images[i], 'gray')
    plt.title(titles[i])
    plt.xticks([])
    plt.yticks([])

plt.show()
