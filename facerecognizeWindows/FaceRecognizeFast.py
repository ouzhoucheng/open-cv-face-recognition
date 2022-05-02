import cv2 # openCV
import numpy as np
import os # 文件路径相关
import tkinter as tk # 事件
import sys # 关闭程序

window = tk.Tk() # 创建基本窗口
window.title('人脸识别系统') # 命名
sw = window.winfo_screenwidth() # 获取屏幕宽
sh = window.winfo_screenheight() # 获取屏幕高
wx = 250 # 窗口长
wh = 250 # 窗口高
window.geometry("%dx%d+%d+%d" %(wx,wh,(sw-wx)/2 + 300,(sh-wh)/2))#窗口至指定位置

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
mask_cascade = cv2.CascadeClassifier('no_mask_detect_cascade.xml')

subjects = [] # 名字列表

# 保存名字列表
def SaveNames(NameSave1):
    global subjects
    fileObject = open('Names.txt', 'a') # 追加方式打开记录名字的txt
    fileObject.write(NameSave1) # 追加一个姓名
    fileObject.write('\n') # 换行
    fileObject.close() # 关闭

# 读取名字列表
def ReadNames():
    global subjects
    NamesF = open("Names.txt","r") # 只读方式打开记录名字的txt
    subjects = NamesF.readlines() # 行读取姓名
    NamesF.close() # 关闭

# 根据给定的（x，y）坐标和宽度高度在图像上绘制矩形
def draw_rectangle(img, rect):
    (x, y, w, h) = rect # 矩形跟踪人脸
    cv2.rectangle(img, (x, y), (x + w, y + h), (128, 128, 0), 2)

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

# 人脸检测
def detect_face(img):
    # 将测试图像转换为灰度图像，因为opencv人脸检测器需要灰度图像
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # 检测多尺度图像，返回值是一张脸部区域信息的列表（x,y,宽,高）
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.2,
        minNeighbors=5,minSize=(20, 20)) #人脸的最小范围，如果比20*20像素小忽略)
    # 如果未检测到面部，则返回原始图像
    if (len(faces) == 0):
        return None, None 
    # 目前假设只有一张脸，xy为左上角坐标，wh为矩形的宽高
    (x, y, w, h) = faces[0]
    # 返回图像的正面部分
    return gray[y:y + w, x:x + h], faces[0]

# 检测是否佩戴口罩
def detect_facemask(img):
    # 将测试图像转换为灰度图像，因为opencv人脸检测器需要灰度图像
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # 检测多尺度图像，返回值是一张脸部区域信息的列表（x,y,宽,高）
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.2,
        minNeighbors=5,minSize=(50, 50)) #人脸的最小范围，如果比20*20像素小忽略
    # 如果未检测到面部，则返回原始图像
    if (len(faces) == 0):
        return None, None, None
    # 目前假设只有一张脸，xy为左上角坐标，wh为矩形的宽高
    (x, y, w, h) = faces[0]
    no_mask = mask_cascade.detectMultiScale(gray[y:y + w, x:x + h],
        scaleFactor  = 1.01, minNeighbors = 4)
    # 返回图像的正面部分
    return gray[y:y + w, x:x + h], faces[0], no_mask

# 添加一位仁兄进入列表，拍摄15张照片并保存
def Capture_Image():
    cap = cv2.VideoCapture(0) # 读取摄像头
    if not cap.isOpened():
        print("cant initial camera")
    
    # 创建新文件夹来保存图片
    path = os.getcwd() # 获取当前工作路径
    # print(path)
    ls = os.listdir(path+"/face_data") # 查看有几个文件夹
    new_folder = str(int(max(ls))+1) # 文件夹名称123...
    os.mkdir(path+"/face_data/"+new_folder) # 创建
    
    nimg = 0
    while True:
        ret, frame = cap.read() # 画面图像
        if not ret:
            print("cant obtain image")
            cap.release();
            break
        
        try:
            img = frame.copy() # 
            face, rect = detect_face(img) # 寻找可以识别人脸的图片
            draw_rectangle(img, rect) # 框选人脸
            # 保存图片
            cv2.imwrite("face_data/"+new_folder+"/"+str(nimg)+".jpg", frame)
            nimg = nimg + 1
        except Exception as err: # 未能识别到人脸
            print(err)
        if nimg == 16:
            break
        draw_text(img, "Obtaining picture...", 20, 30, "WHITE")
        draw_text(img, "Please keep only 1 person ", 20, 60, "WHITE")
        draw_text(img, "in the camera", 20, 90, "WHITE")
        cv2.imshow("picture", img)
        cv2.waitKey(1)
    cap.release();
    cv2.destroyAllWindows()   
    
# 准备训练的数据
def prepare_training_data(data_folder_path):
    # 获取数据文件夹中的目录（每个主题的一个目录）
    dirs = os.listdir(data_folder_path)
    #print(dirs)
    # 两个列表分别保存所有的脸部和标签
    faces = []
    labels = []
    # 浏览每个目录并访问其中的图像
    for dir_name in dirs:
        # dir_name(str类型)即标签
        label = int(dir_name)
        # 建立包含当前主题主题图像的目录路径
        subject_dir_path = data_folder_path + "/" + dir_name
        print(subject_dir_path+"\n")
        # 获取给定主题目录内的图像名称
        subject_images_names = os.listdir(subject_dir_path)
        # 浏览每张图片并检测脸部，然后将脸部信息添加到脸部列表faces[]
        for image_name in subject_images_names:
            # 建立图像路径
            image_path = subject_dir_path + "/" + image_name
            # 读取图像
            image = cv2.imread(image_path)
            # 显示图像0.1s
            # cv2.imshow("Training on image...", image)
            # cv2.waitKey(10)
            # 检测脸部
            face, rect = detect_face(image)
            # 我们忽略未检测到的脸部
            if face is not None:
                #将脸添加到脸部列表并添加相应的标签
                faces.append(face)
                labels.append(label)
    # cv2.waitKey(1)
    cv2.destroyAllWindows()
    #最终返回值为人脸和标签列表
    return faces, labels

# 开始训练
def Renew_face_model():
    # 调用prepare_training_data（）函数
    faces, labels = prepare_training_data("face_data")
    # 创建LBPH识别器并开始训练，当然也可以选择Eigen或者Fisher识别器
    face_recognizer = cv2.face.LBPHFaceRecognizer_create()
    face_recognizer.train(faces, np.array(labels))
    # 保存训练好的模型
    face_recognizer.save("face_model.xml")
    return face_recognizer

name = 0 # 第几个名字
def EntName():
    window1 = tk.Tk() # 创建输入框
    tk.Label(window1, text="输入姓名").grid(row=0) # 提示输入内容
    window1.geometry("%dx%d+%d+%d" %(250,75,(sw-wx)/2,(sh-wh)/2)) # 窗口至指定位置
    EnStr = tk.Entry(window1) # 设定输入栏
    EnStr.grid(row=0, column=1, padx=10, pady=5) # 输入栏位置信息
    # 按钮事件
    def show():
        global name
        name = EnStr.get() # 读取输入信息
        EnStr.quit()
    #构建按钮
    tk.Button(window1, text="确认", width=10, command=show).grid(row=3, column=0, sticky="w", padx=10, pady=5)
    window1.mainloop()

# 录入新的人脸
def renew_main(): 
    global subjects
    EntName() # 输入姓名
    subjects.append(name) # 将名字加入数组
    SaveNames(name) # 保存这个名字
    Capture_Image() # 抓取图像
    Renew_face_model() # 更新模型

# 此函数识别传递的图像中的人物并在检测到的脸部周围绘制一个矩形及其名称
def predict(test_img):
    #生成图像的副本，这样就能保留原始图像
    img = test_img.copy()
    #检测人脸
    face, rect, no_mask = detect_facemask(img)
    try:
        #预测人脸
        label = face_recognizer.predict(face)
        # 在检测到的脸部周围画一个矩形
        draw_rectangle(img, rect)
        if label[1]<70:
            # 获取由人脸识别器返回的相应标签的名称
            label_text = subjects[label[0]-1]
            # 标出预测的名字
            draw_text(img, label_text,  rect[0], rect[1] - 15, "WHITE")
            # draw_text(img, "Identity Confirmed!",  rect[0], rect[1] - 40, "GREEN")
            #draw_text(img, "WEAR MASK", 100, 100)
        else:
            # 未识别，wrong people
            draw_text(img, "Stranger",  rect[0], rect[1] - 5, "RED")
            #draw_text(img, "WEAR MASK", 100, 100)
        # 未戴口罩
        if (len(no_mask) != 0):
            #draw_rectangle(img, no_mask[0])
            draw_text(img, "please wear mask!!!",  20, 20, "RED")
        
    except Exception as err:
        print(err)
    #print(label)   
    #返回预测的图像
    return img

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


def face_recognize_video():
    global face_recognizer
    global subjects
    face_recognizer = Load_face_model()
    # ReadNames()
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("cant initial camera")

    while (1):                   
        ret, frame = cap.read()
        if not ret:
            print("cant obtain image")
            break

        predicted_img = predict(frame)
        draw_text(predicted_img, "press ESC or Q to exit", 20, 450, "RED")
        cv2.imshow("predict1", predicted_img)

        Key = cv2.waitKey(1)
        if Key == 27 or Key == ord('q'):
            break
        
    cap.release()
    cv2.destroyAllWindows() 

# 移除所有已训练的数据
def remove_face_data():
    try:
        os.remove('face_model.xml') # 删除模型文件
    except:
        pass
    # os.remove('Names.txt')
    RemoveName = open('Names.txt', 'w') # 写模式打开名单txt
    RemoveName.truncate( ) # 移除名单内姓名
    subjects.clear() # 清空数组

# 关闭
def restart_program():
    print("退出...")
    sys.exit(0) # 关闭程序

# 先读取已经保存的姓名
ReadNames()
bt_add = tk.Button(window,text='录入人脸数据',height=2,width=18,command=renew_main)
bt_add.place(x=50,y=20)
bt_start = tk.Button(window,text='识别人脸',height=2,width=18,command=face_recognize_video)
bt_start.place(x=50,y=70)
bt_rm = tk.Button(window,text='移除所有数据',height=2,width=18,command=remove_face_data)
bt_rm.place(x=50,y=120)
bt_rm = tk.Button(window,text='关闭',height=2,width=18,command=restart_program)
bt_rm.place(x=50,y=170)
window.mainloop()