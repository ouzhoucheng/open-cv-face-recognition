import numpy as np
import cv2
# 打开笔记本摄像头
cap = cv2.VideoCapture(0) 
# 加载级联分类器
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml') 


# 人脸检测
def detect_face(img):
    # 将测试图像转换为灰度图像，因为opencv人脸检测器需要灰度图像
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # 检测多尺度图像，返回值是一张脸部区域信息的列表（x,y,宽,高）
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.2,minNeighbors=5,minSize=(20, 20)) #人脸的最小范围，如果比20*20像素小忽略)
    # 如果未检测到面部，则返回原始图像
    if (len(faces) == 0):
        return None, None 
    # 目前假设只有一张脸，xy为左上角坐标，wh为矩形的宽高
    (x, y, w, h) = faces[0]
    # 返回图像的正面部分
    return gray[y:y + w, x:x + h], faces[0]

# 根据给定的（x，y）坐标和宽度高度在图像上绘制矩形
def draw_rectangle(img, rect):
    (x, y, w, h) = rect # 矩形跟踪人脸
    cv2.rectangle(img, (x, y), (x + w, y + h), (255, 128, 0), 2)
    cv2.rectangle(img, (x+2, y+2), (x + w-2, y + h-2), (0, 255, 128), 2)

# 两个列表分别保存所有的脸部和标签
faces = []
labels = []
Names = ["Trump", "Putin", "TrumpTin"]
NameNumber = 2
times = 0
while 1:
    ret,frame = cap.read()

    try:
        face, rect = detect_face(frame) # 寻找可以识别人脸的图片
        draw_rectangle(frame, rect) # 框选人脸

        if face is not None:
            #将脸添加到脸部列表并添加相应的标签
            faces.append(face)
            labels.append(NameNumber)
            times += 1
            print(times)
        if times > 15:
            # 创建LBPH识别器并开始训练，当然也可以选择Eigen或者Fisher识别器
            face_recognizer = cv2.face.LBPHFaceRecognizer_create()
            face_recognizer.train(faces, np.array(labels))
            # 保存训练好的模型
            face_recognizer.save("face_model.xml")
            break   
        
    except: # 未能识别到人脸
        print("no face")

    cv2.imshow("picture",frame)
    if cv2.waitKey(1) == 27:
        break
cv2.destroyAllWindows()