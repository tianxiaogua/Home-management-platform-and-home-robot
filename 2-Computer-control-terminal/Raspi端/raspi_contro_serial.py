#!/usr/bin/python3.7
#encoding:utf-8
import time

"""tools模块可以直接查看usb上连接的串口
    使用serial、pyserial两个包"""
import serial.tools.list_ports
import threading
#
"""串口发送和接收在两个类中打开使用，萹蓄在两个线程中运行
    实际测试过程中发现，串口读取和发送使用同一个串口对象会发生接受串口信息丢失的现象
    需要在两个子线程中分别实例化两个串口对象，一个用来发送，一个接收"""
"""向串口发送数据类"""

class Serial_Robot():
    def __init__(self):
        plist = list(serial.tools.list_ports.comports())
        if len(plist) <= 0:
            print("没有发现端口!")
        else:
            plist_0 = list(plist[0])
            serialName = plist_0[0]
            try:
                self.ser = serial.Serial(serialName, 115200,timeout=60)  # 设置了第一个串口号，波特率，读超时设置
                print("正在建立串口发送！ 可用端口名>>>"+serialName)
                print("正常连接中")
            except:
                print("serial error!")
                print('打开串口失败，请检查串口连接并确定无其他程序占用串口')
#
    '''PC端向单片机通过串口发送数据'''
    def sand_message(self,message):
        message = str(message)
        try:
            self.ser.write((message+'\r\n').encode())
        except:
            print("serial error!")
            print('打开串口失败，请检查串口连接并确定无其他程序占用串口')

    '''PC端通过串口接收单片机数据'''

    def recive_serial(self):
        try:
            count = self.ser.inWaiting()
            if count > 0:
                recive_serial = self.ser.read(count)
                print(recive_serial)
                return recive_serial
        except:
            print("serial error!")
            print('打开串口失败，请检查串口连接并确定无其他程序占用串口')
            return None


"""下面是测试代码"""
def fun():
    while True:
        test.recive_serial()
if __name__ == "__main__":
    test = Serial_Robot()
    t = threading.Thread(target=fun)
    t.start()
    while True:
        value_input = input("输入：")
        test.sand_message(value_input)

