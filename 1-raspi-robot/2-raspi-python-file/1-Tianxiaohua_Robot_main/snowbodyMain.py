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
"""语音识别模块"""
from speak_Main import speaker

import snowboydecoder
import sys
import signal
import findWeather


class mainsnow():
    
    def __init__(self):
        self.interrupted = False
        self.mainCheck()
        
    def interrupt_callback(self):
        #global interrupted
        return self.interrupted

    def mainCheck(self):
        print("begin to check:")
        model = "小花.pmdl"#sys.argv[1]
        # capture SIGINT signal, e.g., Ctrl+C
        #signal.signal(signal.SIGINT, signal_handler)
        signal.signal(signal.SIGINT, True)

        detector = snowboydecoder.HotwordDetector(model, sensitivity=0.45)
        print('Listening... Press Ctrl+C to exit')
        # main loop
        detector.start( detected_callback = snowboydecoder.play_audio_file,
                        interrupt_check = self.interrupt_callback,
                        sleep_time=0.03)
        detector.terminate()


"""功能:程序会在此处等待，直到识别到语音唤醒程序，方便外部函数直接调用"""
def Wait_for_question():
    a = mainsnow()
    
    
def robot_question(Object):
    Wait_for_question()
    Object.paly_cloud_vice("嗯，怎么了")
    value_input = Object.Sound_recording_2(sound_time = 3)
    """由于可能没有识别结果可能会发生错误，需要在这里进行错误处理"""
    try: 
        if "关灯" in value_input:
            Object.paly_cloud_vice("好")
            return 1 
        elif "开灯" in value_input:
            Object.paly_cloud_vice("好的")
            return 2
        elif "天气" in value_input:
            a = findWeather.return_weather()
            print(a)
            Object.paly_cloud_vice("今天的天气")
            Object.paly_cloud_vice(a["现在天气"][0])#a["现在天气"][1]
            Object.paly_cloud_vice(a["现在天气"][1])
            return 0
        elif "数据" in value_input:
            Object.paly_cloud_vice("没问题")
            return 3
        elif "聊天" in value_input or "聊聊天" in value_input:
            return 4
        else:
            Object.paly_cloud_vice("你说啥")
    except:
        
        return 0

if __name__ == '__main__':
    while True:
        Speak = speaker()
        robot_question(Speak)