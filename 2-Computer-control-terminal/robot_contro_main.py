from raspi_contro_serial import Serial_Robot
import threading
import time

"""此模块用来做机器人的直接控制，应用在树莓派上，使用串口来对驱动板做直接的控制
    分为多个控制部分，
    串口发送速度为100ms每条"""
serial = Serial_Robot()
time.sleep(0.2)

def Driver_LED_on():
    time.sleep(0.1)
    serial.sand_message('01')
    time.sleep(0.1)

def Driver_LED_off():
    time.sleep(0.1)
    serial.sand_message('00')
    time.sleep(0.1)

def Driver_print(value):
    serial.sand_message('1'+str(value))
    time.sleep(0.1)

def fun_Steering_engine(num,value):
    if value < 10:
        return  num + '00' + str(value)
    elif value < 100:
        return  num + '0' + str(value)
    else:
        return num + str(value)

def Driver_Steering_engine_2(num):
    value = ''
    for i in num:
        if i == 'num1':
            if num[i]<10:
                value = 'a' + '00'+str(num[i])
            elif num[i]<100:
                value = 'a' + '0'+str(num[i])
            else:
                value = 'a' + str(num[i])
        elif i == 'num2':
            value = value + fun_Steering_engine('b',num[i])
        elif i == 'num3':
            value = value + fun_Steering_engine('c',num[i])
        elif i == 'num4':
            value = value + fun_Steering_engine('d',num[i])
        elif i == 'num5':
            value = value + fun_Steering_engine('e',num[i])
        elif i == 'num6':
            value = value + fun_Steering_engine('f',num[i])
        elif i == 'num7':
            value = value + fun_Steering_engine('g',num[i])
        elif i == 'num8':
            value = value + fun_Steering_engine('h',num[i])
        elif i == 'num9':
            value = value + fun_Steering_engine('i',num[i])
        elif i == 'num10':
            value = value + fun_Steering_engine('j',num[i])
        elif i == 'num11':
            value = value + fun_Steering_engine('k',num[i])
        elif i == 'num12':
            value = value + fun_Steering_engine('l',num[i])
        elif i == 'num13':
            value = value + fun_Steering_engine('m',num[i])
        elif i == 'num14':
            value = value + fun_Steering_engine('n',num[i])
        elif i == 'num15':
            value = value + fun_Steering_engine('o',num[i])
        elif i == 'num16':
            value = value + fun_Steering_engine('p',num[i])
    print(value)
    serial.sand_message('2' + value)
    time.sleep(0.15)

def Driver_Steering_engine(i):
    serial.sand_message('2' + i)
    time.sleep(0.1)

def Driver_Set_Engine(value):
    serial.sand_message('3' + value)
    time.sleep(0.1)



"""下面是测试代码"""
def fun():
    while True:
        serial.recive_serial()

def main():
    Driver_Steering_engine('a180b000c180d000')
    time.sleep(0.5)
    Driver_print('set motor')
    Driver_Steering_engine('a090b090c180d000')
    time.sleep(0.5)
    Driver_print('set motor')
    Driver_Steering_engine('2c090d090')
    time.sleep(0.5)
    Driver_print('set motor')
    Driver_Steering_engine('a180b000c180d000')
    time.sleep(0.5)
    Driver_print('set motor')

    Driver_Set_Engine('a4a0')
    Driver_print('set motor')
    time.sleep(0.5)
    Driver_Set_Engine('a0a0')
    Driver_print('set motor')
    time.sleep(0.5)
    Driver_Set_Engine('g8a0')
    Driver_print('set motor')
    time.sleep(0.5)
    Driver_Set_Engine('a0a0')
    Driver_print('set motor')
    time.sleep(0.5)



#t = threading.Thread(target=fun, daemon=True)
t = threading.Thread(target=fun, )
t.start()
"""while True:
    main()"""

    
