#!/usr/bin/python3.7
# coding=utf-8
from socket import *
import threading

class Tcp_server():
    def __init__(self):
        address = '192.168.31.84'  # 监听哪些网络  127.0.0.1是监听本机 0.0.0.0是监听整个网络
        #address = '127.0.0.1'  # 监听哪些网络  127.0.0.1是监听本机 0.0.0.0是监听整个网络
        port = 6424  # 监听自己的哪个端口
        # buffsize=1024           #接收从客户端发来的数据的缓存区大小
        self.s = socket(AF_INET, SOCK_STREAM)
        self.s.bind((address, port))
        self.s.listen(5)  # 最大连接数

    def wait_connect(self):
        print("waiting for connect!")
        clientsock, clientaddress = self.s.accept()
        print('connect from:', clientaddress)
        return clientsock, clientaddress

    def send_massage(self, sanddata, clientsock, clientaddress):
        clientsock.send(sanddata.encode())
        print(clientaddress, " sand ok")

    def reve_massage(self, clientsock, clientaddress):
        while True:
            recvdata = clientsock.recv(1024).decode('utf-8')
            print(clientaddress, recvdata)
            # 判断连接断开标志，如果接收到的数据长度是0，证明已经断开
            if len(recvdata) == 0:
                print(clientaddress, ' break link')
                break

        # return recvdata




if __name__ == '__main__':
    TCP = Tcp_server()
    Clientsock, Clientaddress = TCP.wait_connect()
    TCP.send_massage("aasd", Clientsock, Clientaddress)
    thread = threading.Thread(target=TCP.reve_massage,
                              args=(Clientsock, Clientaddress))  # t为新创建的线程
    thread.start()

