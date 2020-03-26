# 小花机器人树莓派部分程序结构说明：

## 1、独立的语音识别部分

​		**speak_Main.py  speak_play.py  resources** 

​		运行**speak_Main.py  ，开始进行基于百度API的语音识别功能；

​		是机器人系统最底层的模块，可以自己独立运行，会在其他模块中调用speak_Main.py  模块



## 2、查天气模块

​		**findWeather.py**

​		一个可以独立运行的底层模块，通过读取中国气象网站的数据来获取天气。



## 3、使用Snowbody的语音唤醒方案

​		**speak_Main.py  + speak_play.py  + resources**

​		**findWeather.py**

​		**snowbody_Main.py + sonwbodydetect.py + snowbodydecoder.py + sonwbodydetect.so**

​		在原来语音识别模块的基础上增加了语音唤醒。以查找天气模块和语音识别模块为基础构成了snowbody_Main.py内的主要功能。使用github项目进行改进。程序开始处于休眠等待语音唤醒接段，唤醒后执行语音识别。

