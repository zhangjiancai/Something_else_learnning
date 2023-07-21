import cv2


def preprocess_image(image_path):
    image = cv2.imread(image_path)
    raw_image = image.copy()

    image = cv2.GaussianBlur(image, (3, 3), 0)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    canny_edges = cv2.Canny(image, 50, 150)

    kernelX = cv2.getStructuringElement(cv2.MORPH_RECT, (17, 5))
    closed_image = cv2.morphologyEx(canny_edges, cv2.MORPH_CLOSE, kernelX)

    kernelY = cv2.getStructuringElement(cv2.MORPH_RECT, (1, 19))
    opened_image = cv2.morphologyEx(closed_image, cv2.MORPH_OPEN, kernelY)

    return opened_image, raw_image


def find_contours(image, retrieval_mode=cv2.RETR_EXTERNAL, approximation_method=cv2.CHAIN_APPROX_SIMPLE):
    contours, _ = cv2.findContours(image, retrieval_mode, approximation_method)
    return contours


def draw_contours(image, contours):
    return cv2.drawContours(image.copy(), contours, -1, (0, 0, 255), 3)


def extract_bounding_rect(contours):
    license_plate_rects = []

    for contour in contours:
        rect = cv2.boundingRect(contour)

        if rect[3] * 3 < rect[2] < rect[3] * 6.5:
            license_plate_rects.append(rect)

    return license_plate_rects


def extract_license_plate(image_path):
    image, raw_image = preprocess_image(image_path)
    contours = find_contours(image)
    license_plate_positions = extract_bounding_rect(contours)
    return license_plate_positions


if __name__ == "__main__":
    image_path = "gua.jpg"
    image, raw_image = preprocess_image(image_path)
    contours = find_contours(image)
    license_plate_positions = extract_bounding_rect(contours)

    if license_plate_positions:
        position = license_plate_positions[0]  # 假设只有一个车牌位置
        print("License Plate Position:", position)
        x, y, width, height = position
        img = image[y:(y + height), x:(x + width)]
        cv2.imshow('license plate', img)
        cv2.waitKey(0)  # 等待按键
        cv2.destroyAllWindows()  # 关闭窗口
    else:
        print("No license plate found.")
