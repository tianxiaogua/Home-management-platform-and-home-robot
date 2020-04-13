#!/usr/bin/python3.7
# coding=utf-8
#客户端与上一个没有任何改变
from socket import *
import threading
import time
import robot_contro_main #串口控制


address="192.168.43.10"   #8266的服务器的ip地址
#address="127.0.0.1"   #8266的服务器的ip地址
port=6424           #8266的服务器的端口号
buffsize=1024        #接收数据的缓存大小
s=socket(AF_INET, SOCK_STREAM)
s.connect((address,port))

def fun():
    while True:
        recvdata=s.recv(buffsize).decode('utf-8')
        print("\n接收的数据是："+recvdata)
        robot_contro_main.Driver_Set_Engine(recvdata)

t = threading.Thread(target=fun)  # t为新创建的线程，专门用来接收从服务器发送过来的数据
t.start()

while True:
    senddata=input('\n想要发送的数据：')
    if senddata=='exit':
        break
    s.send(senddata.encode())

s.close()
