import matplotlib.pyplot as plt

import ExtractLicensePlate
from VehicleLicesePlateRecongnition.IdentificationNumberPlate import get_characters, match_chars
from VehicleLicesePlateRecongnition.SplitLicensePlate import segment_license_plate
import cv2
from VehicleLicesePlateRecongnition.ExtractLicensePlate import extract_license_plate

image_path = "gua.jpg"
img = cv2.imread(image_path)
license_plate_positions = extract_license_plate(image_path)

if license_plate_positions:
    position = license_plate_positions[0]  # 假设只有一个车牌位置
    print("License Plate Position:", position)
    x, y, width, height = position
    img = img[y:(y + height), x:(x + width)]
    plt.imshow(img)
    plt.show()
    plate_chars = segment_license_plate(img)
    print("SplitLicensePlate:", plate_chars)
    # 获取字符模板列表
    char_templates = get_characters()
    # 对车牌字符进行识别
    recognized_chars = match_chars(plate_chars, char_templates)
    print("Recognized Characters:", recognized_chars)
else:
    print("No license plate found.")
