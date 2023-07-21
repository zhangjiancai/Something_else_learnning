import cv2


def segment_license_plate(image):
    image.copy()
    # 执行图像处理步骤
    image = cv2.GaussianBlur(image, (3, 3), 0)
    grayimage = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    image = cv2.threshold(grayimage, 0, 255, cv2.THRESH_OTSU)[1]
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (2, 2))
    image = cv2.dilate(image, kernel)
    contours, hierarchy = cv2.findContours(image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    chars = []
    for item in contours:
        rect = cv2.boundingRect(item)
        #        x, y, w, h = cv2.boundingRect(item)
        chars.append(rect)

    # 指定条件参数
    aspect_ratio_range = (1.5, 8.0)  # 高宽比范围
    min_width = 3  # 最小宽度

    # 根据 x 轴坐标值对包围框进行排序（从左到右排序）
    chars = sorted(chars, key=lambda s: s[0], reverse=False)

    plateChars = []
    for word in chars:
        if (word[3] > (word[2] * aspect_ratio_range[0])) and (word[3] < (word[2] * aspect_ratio_range[1])) and (
                word[2] > min_width):
            plateChar = image[word[1]:word[1] + word[3], word[0]:word[0] + word[2]]
            plateChars.append(plateChar)
    return plateChars
