#!/usr/bin/python3.7
# coding=utf-8
from aip import AipSpeech #导入百度语音的ａｐｉ:  pip install baidu-aip
import os #用来执行打开音乐文件的终端shell指令
import sys #使用了内部的路径方法来导入自建模块的路径
"""自建模块需要手动导入模块路径！"""
sys.path.append("~/2PROGRAM/1-python/20.1.30-田小花语音机器人主程序/program/main")
"""串口通讯控制机器人运动模块"""
from speak_play import Speak_paly




"""
建立基于百度语音识别api的类有两个主要方法：
        1、Sound_recording：直接录音，该方法返回值是识别到的语音转成的字符串
        2、Sound_recording：此方法的输入值是想要转成语音的字符串，此方法会将字符串直接转换成语音播放出来
"""
class speaker():
    def __init__(self):
        app_id = "18342916"
        API_Key = "ZUmYyI8ViFGDL5cpDEbC1hlA"
        Secret_Key = "KcgQsVbBQFSTbq1Umj4EK9kw4Y2P8MvH"
        self.client = AipSpeech(app_id, API_Key, Secret_Key)#代入相关的账户信息创建了语音识别对象clien
        self.length = 2 #创建录音文件的时候规定的录音的时长，单位秒
        self.Contro_sound = Speak_paly()
#
    """使用话筒进行录音，返回值是识别到的话转换成的字符串"""
    def Sound_recording(self,sound_time = 3.5):
        """
        有两种录音和播放的方法，第一种是使用linux自带的音频播放软件aplay和录音软件arecord
        第二种方法是使用python内部模块
        相比较第一方法跟方便，但是在其他系统平台的代码移植性差，只适合linux环境，而第二种虽然复杂，但是受环境影响比较小
        os.system("aplay resource/ding.wav")#使用aplay程序打开录音播放叮的一声
        os.system("arecord -d %d -r 16000 -c 1 -t wav -f S16_LE resource/record.wav" % (self.length,) )#使用录音软件arecord进行录音，相关录音参数设置如函数内部所示。百度使用文件形式为.wav
        os.system("aplay resource/ding.wav")
        """
        self.Contro_sound.Play_sound("resources/ding.wav") #播放叮的一声
        self.Contro_sound.Get_sound(File_name = "resources/record.wav", Sound_time = sound_time)#进行录音
        self.Contro_sound.Play_sound("resources/ding.wav") #播放叮的一声

        with open("resources/record.wav", 'rb') as fp:#打开刚刚录好的音频文件，上传给百度并且可以得到一个字典，字典内容包含相关返回值
            result = self.client.asr(fp.read(), 'wav', 16000, {'dev_pid': 1536, })
        """把录音结果进行打印，实际中会遇到网络和录音无结果的情况，使用try进行应对"""
        try:
            print("___识别结果___：",result)
            print(result['result'][0])
            return result['result'][0]
        except:
            print("___识别不成功____")
            return False
    """使用话筒进行录音，返回值是识别到的话转换成的字符串"""
    def Sound_recording_2(self,sound_time = 3.5):
        """
        有两种录音和播放的方法，第一种是使用linux自带的音频播放软件aplay和录音软件arecord
        第二种方法是使用python内部模块
        相比较第一方法跟方便，但是在其他系统平台的代码移植性差，只适合linux环境，而第二种虽然复杂，但是受环境影响比较小
        os.system("aplay resource/ding.wav")#使用aplay程序打开录音播放叮的一声
        os.system("arecord -d %d -r 16000 -c 1 -t wav -f S16_LE resource/record.wav" % (self.length,) )#使用录音软件arecord进行录音，相关录音参数设置如函数内部所示。百度使用文件形式为.wav
        os.system("aplay resource/ding.wav")
        """
        self.Contro_sound.Play_sound("resources/ding.wav") #播放叮的一声
        self.Contro_sound.Get_sound(File_name = "resources/record.wav", Sound_time = sound_time)#进行录音
        #self.Contro_sound.Play_sound("resource/ding.wav") #播放叮的一声

        with open("resources/record.wav", 'rb') as fp:#打开刚刚录好的音频文件，上传给百度并且可以得到一个字典，字典内容包含相关返回值
            result = self.client.asr(fp.read(), 'wav', 16000, {'dev_pid': 1536, })
        """把录音结果进行打印，实际中会遇到网络和录音无结果的情况，使用try进行应对"""
        try:
            print("___识别结果___：",result)
            print(result['result'][0])
            return result['result'][0]
        except:
            print("___识别不成功____")
            return False
#
    """把识别到的内容以字符串的形式传递给百度,生成可以播放的MP3文件，无返回值"""
    def paly_cloud_vice(self,Speaker_say_message, per = 4, pit = 8):
        """使用百度的API，把字符串数据发送给百度服务器，百度传回语音二进制文件，
        result是代表文字转换成语音的二进制数据，
        需要良好的网络连接！
        """
        result = self.client.synthesis(Speaker_say_message,  #要翻译的文字
                                     "zh",#表示是中文
                                     1,   #表示第一个
                                     {"vol":5,#表示音量
                                      "spd":6,#表示音速
                                      "pit":pit,#表示语调
                                      "per":per#0是女生　1男生　３逍遥　４萝莉
                                      })
        with open("resources/speak.wav", "wb") as f:#创建并写入ｍｐ３文件对象
            f.write(result)#把转换出来的二进制语音文件写入到文件中
        #self.Contro_sound.Play_sound("resource/speak.wav")
        os.system("mpg123 "+"resources/speak.wav")  # 这里的mpg123是一个程序插件，可以用来播放ＭＰ３文件




    """把识别到的内容以字符串的形式传递给百度,生成可以播放的MP3文件，无返回值"""
    def paly_local_vice(self, Speaker_say_message, filepath = "resources/voive/" ,per = 4, pit = 7 ):
        
        try:
            os.system("mpg123 "+filepath+Speaker_say_message+".wav")# 这里的mpg123是一个程序插件，可以用来播放ＭＰ３文件
            print("play local sound")
            asd
            return True
            
        except:
            result = self.client.synthesis(Speaker_say_message,  #要翻译的文字
                                         "zh",#表示是中文
                                         1,   #表示第一个
                                         {"vol":5,#表示音量
                                          "spd":5,#表示音速
                                          "pit":pit,#表示语调
                                          "per":per#1是女生　２男生　３逍遥　４萝莉
                                          })
            with open(filepath+Speaker_say_message+".wav", "wb") as f:#创建并写入ｍｐ３文件对象
                f.write(result)#把转换出来的二进制语音文件写入到文件中
            #self.Contro_sound.Play_sound(filepath+Speaker_say_message+".wav")
            os.system("mpg123 "+filepath+Speaker_say_message+".wav")  # 这里的mpg123是一个程序插件，可以用来播放ＭＰ３文件
            print("*"*5," 播放网络文件")
            return True
#
    """播放语音"""
    #def play_sound(self):#播放转换后的内容
        #os.system("mpg123 resource/sprak.mp3")#这里的mpg123是一个程序插件，可以用来播放ＭＰ３文件

""""""
if __name__ == '__main__':
    a = speaker()
    #a.paly_cloud_vice("开始.进行.音频播放测试")
    a.paly_cloud_vice("开始.进行.音频播试")
    x = a.Sound_recording()
    if(x == "关灯"):
        a.paly_cloud_vice("好的.正在为您关灯")
    else:
        a.paly_cloud_vice("我没听清楚")

""""""
