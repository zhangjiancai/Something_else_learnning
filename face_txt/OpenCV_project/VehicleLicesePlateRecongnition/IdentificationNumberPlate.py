import cv2
import glob

templateDict = {
    0: '0', 1: '1', 2: '2', 3: '3', 4: '4', 5: '5', 6: '6', 7: '7', 8: '8', 9: '9',
    10: 'A', 11: 'B', 12: 'C', 13: 'D', 14: 'E', 15: 'F', 16: 'G', 17: 'H',
    18: 'J', 19: 'K', 20: 'L', 21: 'M', 22: 'N', 23: 'P', 24: 'Q', 25: 'R',
    26: 'S', 27: 'T', 28: 'U', 29: 'V', 30: 'W', 31: 'X', 32: 'Y', 33: 'Z',
    34: '京', 35: '津', 36: '冀', 37: '晋', 38: '蒙', 39: '辽', 40: '吉', 41: '黑',
    42: '沪', 43: '苏', 44: '浙', 45: '皖', 46: '闽', 47: '赣', 48: '鲁', 49: '豫',
    50: '鄂', 51: '湘', 52: '粤', 53: '桂', 54: '琼', 55: '渝', 56: '川', 57: '贵',
    58: '云', 59: '藏', 60: '陕', 61: '甘', 62: '青', 63: '宁', 64: '新',
    65: '港', 66: '澳', 67: '台'
}


def get_characters():
    c = []
    for i in range(0, 67):
        words = glob.glob('template/' + templateDict.get(i) + '/*.*')
        c.append(words)
    return c


def get_match_value(template, image):
    # 读取模板图像
    templateImage = cv2.imread(template)

    # 模板图像色彩空间转换，BGR-->灰度
    templateImage = cv2.cvtColor(templateImage, cv2.COLOR_BGR2GRAY)

    # 模板图像阈值处理，灰度-->二值
    ret, templateImage = cv2.threshold(templateImage, 0, 255, cv2.THRESH_OTSU)

    # 获取待识别图像的尺寸
    height, width = image.shape

    # 将模板图像调整为与待识别图像尺寸一致
    templateImage = cv2.resize(templateImage, (width, height))

    # 进行匹配操作并返回匹配值
    matchValue = cv2.matchTemplate(image, templateImage, cv2.TM_CCOEFF_NORMED)
    return matchValue


def match_chars(plates, chars):
    results = []  # 存储所有的识别结果

    for plateChar in plates:  # 逐个遍历要识别的字符
        bestMatch = []  # 最佳匹配

        for words in chars:  # 遍历字符模板
            match = []  # 每个字符的匹配值

            for word in words:  # 遍历模板
                result = get_match_value(word, plateChar)  # 计算匹配值
                match.append(result)

            bestMatch.append(max(match, default=0))  # 将每个字符模板的最佳匹配加入bestMatch

        i = bestMatch.index(max(bestMatch, default=0))  # 获取最佳匹配的字符模板索引
        r = templateDict[i]  # 获取单个待识别字符的识别结果
        results.append(r)  # 将每一个分割字符的识别结果加入results列表

    return results  # 返回所有的识别结果
