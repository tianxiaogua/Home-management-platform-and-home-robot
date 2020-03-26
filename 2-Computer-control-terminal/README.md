# 电脑控制终端

## 1、目标实现

​		为了方便对树莓派机器人进行控制，除了需要在网络端控制机器人外还需要在电脑上制作一个临时处理的应用。需要实现的目标就是控制机器人的动作，方便调试。

​		使用TCP通讯连接到树莓派上的client，client把接收到的数据转换成可以控制肢体动作的语句。 



## 2、使用说明：

1、电脑上使用pc_robot_server文件和pc_keyboard_check文件来处理键盘扫描和tcp通信

2、树莓派上使用raspi_contro_client、raspi_contro_main和raspi_contral_serial文件来和下位机驱动板进行串口通信，使用raspi_robot_client来和电脑上位机TCP服务器端进行通信



#### 在电脑上运行pc_keyboard_check.py文件，在树莓派上运行raspi_contro_client.py文件

