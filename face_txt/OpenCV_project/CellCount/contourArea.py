import cv2
import matplotlib.pyplot as plt

# 读取图像
o = cv2.imread('opencv.png')
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

# 根据面积筛选轮廓
area = []
contoursOK = []
for i in contours:
    if cv2.contourArea(i) > 1000:
        contoursOK.append(i)
cv2.drawContours(o, contoursOK, -1, (0, 0, 255), 8)

# 绘制所有轮廓
n = len(contours)
for i in range(n):
    print("contours[" + str(i) + "]面积=", cv2.contourArea(contours[i]))
    cv2.drawContours(o, contours, i, (0, 0, 255), 3)

# 用来正常显示中文标签
plt.rcParams['font.sans-serif'] = ['SimHei']

# 显示图形
plt.figure(figsize=(8, 8))
titles = [u'(a)original', u'(b)result']
images = [oc, o]
for i in range(2):
    plt.subplot(1, 2, i+1)
    plt.imshow(images[i], 'gray')
    plt.title(titles[i])
    plt.xticks([])
    plt.yticks([])
plt.show()
