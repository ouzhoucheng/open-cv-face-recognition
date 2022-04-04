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

# 根据给定的（x，y）坐标绘制文本
def draw_text(img, text, x, y, color):
    if(color == "GREEN"): # 浅绿色
        cv2.putText(img, text, (x, y), cv2.FONT_HERSHEY_COMPLEX, 
            1, (128, 128, 0), 2)
    elif(color == "RED"): # 红色
        cv2.putText(img, text, (x, y), cv2.FONT_HERSHEY_COMPLEX, 
            1, (0, 0, 255), 2)
    elif(color == "WHITE"): # 白色
        cv2.putText(img, text, (x, y), cv2.FONT_HERSHEY_COMPLEX, 
            1, (255, 255, 255), 2)

# 加载已训练的模型
def Load_face_model():
    face_recognizer = cv2.face.LBPHFaceRecognizer_create()
    try:
        face_recognizer.read("face_model.xml")
        return face_recognizer
    except:
        print('No Module, please obtain first')
        noi = 1
        return noi

face_recognizer = Load_face_model()
Names = ["Trump", "Putin", "TrumpTin"]
# 此函数识别传递的图像中的人物并在检测到的脸部周围绘制一个矩形及其名称
def predict(test_img):
    #生成图像的副本，这样就能保留原始图像
    img = test_img.copy()
    #检测人脸
    face, rect = detect_face(img)
    try:
        #预测人脸
        label = face_recognizer.predict(face)
        print(label)
        # 在检测到的脸部周围画一个矩形
        draw_rectangle(img, rect)
        if label[1]<70:
            # 获取由人脸识别器返回的相应标签的名称
            label_text = Names[label[0]-1]
            # 标出预测的名字
            draw_text(img, label_text,  rect[0], rect[1] - 15, "WHITE")
        else:
            # 未识别，wrong people
            draw_text(img, "Stranger",  rect[0], rect[1] - 5, "RED")
    except Exception as err:
        print(err)
    #print(label)   
    #返回预测的图像
    return img

while 1:
    ret,frame = cap.read()

    try:
        predicted_img = predict(frame)
    except: # 未能识别到人脸
        print("no face")

    cv2.imshow("picture",predicted_img)
    if cv2.waitKey(1) == 27:
        break
cv2.destroyAllWindows()