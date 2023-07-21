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
    cv2.imshow('license plate', img)
    cv2.waitKey(0)  # 等待按键
    cv2.destroyAllWindows()  # 关闭窗口
else:
    print("No license plate found.")
