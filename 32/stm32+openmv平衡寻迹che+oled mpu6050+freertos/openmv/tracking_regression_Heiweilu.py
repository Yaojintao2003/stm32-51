# Untitled - By: 黑尾鹿 - 周一 5月 9 2022


import sensor, image, time
from pyb import UART,LED
#from pid import PID
import ustruct              #打包和解压缩原始数据类型
THRESHOLD = (70, 100, -128, 127, -128, 106)   # 灰度阈值
#rho_pid = PID(p=0.19, i=0 , d=0)        #y=ax+b b截距
#theta_pid = PID(p=0.002, i=0, d=0)      #a斜率

#开启LED灯进行补光
#LED(1).on()
#LED(2).on()
#LED(3).on()

sensor.reset()
sensor.set_vflip(True)#打开摄像头垂直翻转模式
sensor.set_hmirror(True)#水平镜像模式
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQQVGA) # 80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
sensor.skip_frames(time = 2000)     # 跳过2s帧
clock = time.clock()                # to process a frame sometimes.

uart = UART(4,115200)   #定义串口3变量
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters


#**************************传输数据的函数********************************************************
def sending_data(theta_err):
    global uart;
    data = ustruct.pack("<bbhb",
                   0x2C,                      #帧头1
                   0x12,                      #帧头2
                   int(theta_err), # up sample by 4   #数据1
                   0x5B)
    uart.write(data);   #必须要传入一个字节数组
#**************************传输数据的函数********************************************************


while(True):
    clock.tick()

    img = sensor.snapshot().binary([THRESHOLD]) #线设置为白色，其他为黑色,binary(二进制)根据像素是否在阈值列表 thresholds 中的阈值内，将图像中的所有像素设置为黑色或白色。
    img.dilate(2) #膨胀（去除噪点）
    #img.erode(2)# 腐蚀 （可有可无）
    line = img.get_regression([(100,100)], robust = True) #线性拟合(回归)返回直线蓝色 返回的是一个 image.line对象 robust(坚定的)为True，则使用Theil-Sen线性回归算法，它计算图像中所有阈值像素的斜率的中位数
    if (line):
        #rho_err = abs(line.rho())-img.width()/2  #蓝色直线偏移距离，line.rho()返回霍夫变换后的直线p值 img.width()/2图像宽度除以二即视野中线
        if line.theta()> 90:  #返回霍夫变换(特征提取算法，比如投票提取直线)后的直线的角度（0-179度）。
            theta_err = line.theta()-180  #次操作是将坐标180 90 0变为 -90 0 90
        else:
            theta_err = line.theta()
        img.draw_line(line.line(), color = 127)  #画出蓝色直线 line.line()返回一个直线元组(x1, y1, x2, y2) ，用于如 image.draw_line 等其他的 image 方法。

        if line.magnitude()>8:      #线性回归效果，好进行下一步，否则不进行s
            if theta_err<0:#左偏
                sending_data(theta_err+200)
                print(theta_err+200)
            else:#右偏
                sending_data(theta_err)
                print(theta_err)
            #print(line.magnitude(),theta_err)#(线性回归效果，直线角度(左偏负右偏正)）
            #if -40<b_err<40 and -30<t_err<30:
            #rho_output = rho_pid.get_pid(rho_err,1)
            #theta_output = theta_pid.get_pid(theta_err,1)
            #xw = rho_output+theta_output  #角度，距离PID线性叠加
            #if(xw<0):
                #speed = (rho_output+theta_output+12)  #角度，距离PID线性叠加
           # else:
               # speed = rho_output+theta_output
            #print(rho_output,theta_output,xw)
        #else:
           # speed=0.0




