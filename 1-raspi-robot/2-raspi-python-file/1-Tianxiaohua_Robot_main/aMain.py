#!/usr/bin/python3.7
# coding=utf-8
"""导入线程处理模块"""
import threading
"""获得延时操作"""
import time
"""只是用来代替执行终端指令和更改模块路径"""
import sys
"""自建模块需要手动导入模块路径！"""
sys.path.append(sys.path[0])
"""串口通讯控制机器人运动模块"""
#import contro_robot
"""语音识别模块"""
from speak_Main import speaker
#import contro_vision
import speak_Main_chat as trainRobot
import snowbodyMain
#from communication import communicat#此模块用来用来和物联网模块及逆行TCP通讯。
import findWeather

Sand_value = ['00', '00', 'hello','00']
Reve_value = ['']
feedback = 0

"""构建机器人主体，包含机器人一般和相关属性变化控制"""
class Robot_head():
    def __init__(self,name):
        self.name = name
        self.local_value = ['10', '01', '11','00']
        self.Init_set_serial() #串口数据连接初始化

    """串口初始化，开始让串口发送数据等"""
    def Init_set_serial(self):
        self.Serial_port = contro_robot.SerialControRobot()
        """建立接收串口信息的独立线程"""
        self.Serial_receve_robot = contro_robot.SerialReceveRobot()

    """测试程序,无实际意义,未执行"""
    def fun_1(self):
        while True:
            time.sleep(0.5)
            self.Serial_port.sand_message(Sand_value[0])
            time.sleep(0.5)
            self.Serial_port.sand_message(Sand_value[1])


def sand_fun():
    while True:
        Tianxiaohua.Serial_port.sand_message(Sand_value[0])
        time.sleep(0.02)

def rece_fun():
    while True:
        Reve_value[0] = Tianxiaohua.Serial_receve_robot.recive_serial()
        if(Reve_value[0]!= None):
            print("打印接收到的信息：",Reve_value[0])


def fun_(Object):
    while True: 
        Object.sand_value(Sand_value[2])
        
def robot_question(Object):
    Object.paly_cloud_vice("嗯，怎么了")
    value_input = Object.Sound_recording_2(sound_time = 3)
    try:
        if "关灯" in value_input:
            Object.paly_cloud_vice("好")
            return 1 
        elif "开灯" in value_input:
            Object.paly_cloud_vice("好的")
            return 2
        elif "天气" in value_input:
            a = findWeather.return_weather()
            print("天气查询的结果：",a)
            Object.paly_cloud_vice("今天的天气")
            Object.paly_cloud_vice(a["现在天气"][0])#a["现在天气"][1]
            Object.paly_cloud_vice(a["现在天气"][1])
            return 0
        elif "聊天" in value_input or "聊聊天" in value_input:
            trainRobot.main_train(Object)
            return 3
        else:
            Object.paly_cloud_vice("你说啥")
    except:
        return 0

     
def Do_something(value,Object1 = 0 ,Object2 = 0):
    if value == 1:
        Sand_value[0] = '00'
    elif value == 2:
        Sand_value[0] = '11'
    elif value == 3:
        try:
            Object1.sand_value(Sand_value[2])
        except:
            print("sand fallse")
    elif value == 4:
        trainRobot.main_train(Object2)
    else:
        pass

"""程序执行入口"""
if __name__ == "__main__":
    
    #Tianxiaohua = Robot_head("name")
    print("begin 1")
    Speak = speaker()
    #Internet_of_things = communicat()#此模块用来用来和物联网模块及逆行TCP通讯。
    print("begin 2")
    Speak.paly_cloud_vice("初始化中")#欢迎使用
    value_input = Speak.Sound_recording_2(sound_time = 0.1)
    Speak.paly_cloud_vice("请多 指教")#欢迎使用
    
    #Thread_sand_value_test = threading.Thread(target=sand_fun, name='子线程')  # 建立子线程
    #Thread_sand_value_test.start()  # 线程开始

    #Thread_reve_value_test = threading.Thread(target=rece_fun, name='子线程')  # 建立子线程
    #Thread_reve_value_test.start()  # 线程开始

    #Thread_reve_value_test_3 = threading.Thread(target=fun_, name ='子线程', args = Internet_of_things)  # 建立子线程
    #Thread_reve_value_test_3.start()  # 线程开始
    
    while True:
        snowbodyMain.Wait_for_question()#程序会在此处等待，直到识别到语音唤醒程序
        a = robot_question(Speak)
        Do_something( value = a, Object2 = Speak)#开始运行语音识别程序
        
        