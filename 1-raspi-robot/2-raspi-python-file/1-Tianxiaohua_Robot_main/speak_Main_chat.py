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
        
def make_it(text):
    x = text.find("#")
    y = text.find("\n")
    return text[:x],text[x+1:y]

def init_Answer():
    result = {}
    with open('Answer.txt', 'r') as f:
        for line in f.readlines():
            text_1, text_2 = make_it(text=line)
            result[text_1] = text_2
    f.close()
    return result

def write_answer(value_answer,value_quesstion):
    str = value_answer + '#' + value_quesstion + '\n'
    with open(r'Answer.txt', 'a') as f:
        f.write(str)
    return init_Answer()


def main_train(Object):
    answer = init_Answer()
    Speak = Object
    #Speak = speaker()
    #value_input = Speak.Sound_recording(sound_time = 0.01)
    #Speak.Contro_sound.Play_sound()
    Speak.paly_cloud_vice("请指教")
    for i in answer:
        print("××问题：", i, "××回答：", answer[i])
    while True:
        value_input = Speak.Sound_recording(sound_time = 3)
        # value_input = input("请输入>>>")
        if(value_input == "笑话再见" or value_input == "再见" or value_input == "在线"):
            Speak.paly_cloud_vice("再见")
            break
        elif(value_input == '笑话跟我说' or value_input == "陈华跟我说"):
            Speak.paly_cloud_vice("恩.你说")
            x = Speak.Sound_recording()
            Speak.paly_cloud_vice("恩")
            y = Speak.Sound_recording()
            Speak.paly_cloud_vice("记住啦")
            #x = input("恩 你说")
            #y = input("恩")
            #print("记住啦")
            answer = write_answer(x,y)
        elif(value_input == '显示'):
            for i in answer:
                Speak.paly_cloud_vice("请查看")
                print("××问题：", i, "××回答：", answer[i])
        elif(value_input == '退出程序'):
            Speak.paly_cloud_vice("再见")
            sys.exit()
        else:
            try:
                Speak.paly_cloud_vice(answer[value_input])
                #print(answer[value_input])
            except:
                Speak.paly_cloud_vice("恩?你说啥")
                #print("恩？你说啥？")

if __name__ == '__main__':
    Speak = speaker()
    value_input = Speak.Sound_recording(sound_time = 0.01)
    while True:
        a = mainsnow()
        main_train(Speak)
    
