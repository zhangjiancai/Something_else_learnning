# ExtractLicensePlate.py

import cv2


def preprocess_image(image_path):
    image = cv2.imread(image_path)
    raw_image = image.copy()

    image = cv2.GaussianBlur(image, (3, 3), 0)
    image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    SobelX = cv2.Sobel(image, cv2.CV_16S, 1, 0)
    absX = cv2.convertScaleAbs(SobelX)

    _, image = cv2.threshold(absX, 0, 255, cv2.THRESH_OTSU)

    kernelX = cv2.getStructuringElement(cv2.MORPH_RECT, (17, 5))
    image = cv2.morphologyEx(image, cv2.MORPH_CLOSE, kernelX)

    kernelY = cv2.getStructuringElement(cv2.MORPH_RECT, (1, 19))
    image = cv2.morphologyEx(image, cv2.MORPH_OPEN, kernelY)

    image = cv2.medianBlur(image, 15)

    return image, raw_image


def find_contours(image):
    contours, _ = cv2.findContours(image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
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
