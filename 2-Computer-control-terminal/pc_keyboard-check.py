#!/usr/bin/python3.7
#encoding:utf-8
import pynput
import threading
from pc_robot_server import Tcp_server
"""此模块用来做上位机，把电脑当作遥控器来对机器人进行控制
"""
keyboard = pynput.keyboard.Controller()

TCP = Tcp_server()
Clientsock, Clientaddress = TCP.wait_connect()
thread = threading.Thread(target=TCP.reve_massage,
                          args=(Clientsock, Clientaddress))  # t为新创建的线程
thread.start()

"""把获取的键盘值显示出来"""
def on_press(key):
    try:
        print("key {} pressed".format(key.char))#输入类似abcd的值可以直接传换成字符串打印出来
        if key.char is 'w':
            print("向前")
            #contro_main.Driver_Set_Engine('a4a4')
            TCP.send_massage("a4a4", Clientsock, Clientaddress)
        elif key.char is 's':
            print("向后")
            #contro_main.Driver_Set_Engine('g8g8')
            TCP.send_massage("g8g8", Clientsock, Clientaddress)
        elif key.char is 'a':
            print("向左")
            #contro_main.Driver_Set_Engine('g7a4')
            TCP.send_massage("g7a4", Clientsock, Clientaddress)
        elif key.char is 'd':
            print("向右")
            #contro_main.Driver_Set_Engine('a4g7')
            TCP.send_massage("a4g7", Clientsock, Clientaddress)
    except AttributeError:
        print("special key {} pressed".format(key))#打印出来类似空格shift这样的功能按键

"""键盘抬起检测"""
def on_release(key):
    try:
        print("{} released".format(key))
        TCP.send_massage("a0a0", Clientsock, Clientaddress)
    except AttributeError:
        print("special key {} pressed".format(key))#打印出来类似空格shift这样的功能按键



# 键盘添加监听器
with pynput.keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()



