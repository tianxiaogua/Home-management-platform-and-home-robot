import wave #用来保存生成的二进制音频数据，可以生成wav文件
from pyaudio import PyAudio, paInt16 #用来录音，处理声卡等问题
from playsound import playsound #用来播放mp3文件
"""
本模块作为百度语音识别模块的附属模块，用来实现录音和播放录音文件的功能。
初始化Contro_sound模块后会定义相关的录音文件的属性。
Get_sound：使用麦克风获取一段时长的录音
Play_sound：播放已经存在的音频文件
"""
"""
参考教程如下： https://blog.csdn.net/qq_36387683/article/details/91901815
             https://blog.csdn.net/exmlyshy/article/details/84760845
视频教程： https://www.bilibili.com/video/av74604640?p=1
这是第一个大坑！！！导入pyaudio会报错，报错内容如下：
python3 使用pyaudio OSError: [Errno -9996] Invalid input device (no default output device)-ubuntu16.04
解决办法如下：
重新安装： https://blog.csdn.net/Nicholas_Wong/article/details/77977715
更换安装内容： https://blog.csdn.net/a506681571/article/details/85201279
os.system('arecord -D "plughw:1,0" -d 3 -r 16000 -c 1 -t wav -f S16_LE record.wav')
os.system("omxplayer resources/ding.wav")
"""

class Speak_paly():
    def __init__(self):
        self.CHUNK = 2000#1024  # wav文件是由若干个CHUNK组成的，CHUNK我们就理解成数据包或者数据片段。
        self.FORMAT = paInt16  # 表示我们使用量化位数 16位来进行录音
        self.CHANNELS = 1  # 代表的是声道，1是单声道，2是双声道。
        self.RATE = 16000  # 采样率 一秒内对声音信号的采集次数，常用的有8kHz, 16kHz, 32kHz, 48kHz, 11.025kHz, 22.05kHz, 44.1kHz。
        #self.RECORD_SECONDS = 5  # 录制时间这里设定了5秒
        #self.Sound_time = Sound_time
        #self.path = FilePath #"resource/"
        #self.file_name = file_name

    def save_wave_file(self,pa, filename, data):
        '''save the date to the wavfile'''
        wf = wave.open(filename, 'wb')
        wf.setnchannels(self.CHANNELS)
        # wf.setsampwidth(sampwidth)
        wf.setsampwidth(pa.get_sample_size(self.FORMAT))
        wf.setframerate(self.RATE)
        wf.writeframes(b"".join(data))
        wf.close()

    """功能：使用麦克风获取一段时长的录音文件
    方法输入：定义文件路径名字、定义录音时长
    返回值：无"""
    def Get_sound(self,File_name = "resources/record.wav", Sound_time = 5):
        pa = PyAudio()
        stream = pa.open(format=self.FORMAT,
                         channels=self.CHANNELS,
                         rate=self.RATE,
                         input=True,
                         frames_per_buffer=self.CHUNK)
        print("开始录音")
        frames = []  # 定义一个列表
        for i in range(0, int(self.RATE / self.CHUNK * Sound_time)):  # 循环，采样率 44100 / 1024 * 5
            data = stream.read(self.CHUNK)  # 读取chunk个字节 保存到data中
            frames.append(data)  # 向列表frames中添加数据data
            print(".")
        print("录音结束")

        stream.stop_stream()
        #stream.close()  # 关闭
        #pa.terminate()  # 终结
        self.save_wave_file(pa, File_name, frames)

    """功能：播放已经存在的录音文件
    方法输入：文件路径名
    """
    def Play_sound(self,File_name = "resources/speak.wav"):
        try:
            wf = wave.open(File_name, 'rb')
        except :
            try:
                os.system("mpg123 "+File_name)#这里的mpg123是一个程序插件，可以用来播放ＭＰ３文件
                print("*"*5,"播放成功！播放格式非wav！播放了本地文件！")
                return True
            except:
                print("*"*5,"播放失败！没有此目录或播放格式非wav  ")
                return False
        p = PyAudio()
        stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                        channels=wf.getnchannels(),
                        rate=wf.getframerate(), output=True)
        # 读数据
        data = wf.readframes(self.CHUNK)
        print("*"*5,"正在播放","*"*5)
        # 播放流
        while len(data) > 0:
            stream.write(data)
            data = wf.readframes(self.CHUNK)
            print(".")
        stream.stop_stream()  # 暂停播放/录制
        stream.close()  # 终止播放
        p.terminate()  # 终止portaudio会话
        return True
    
""""  
wf = wave.open('spweak.wav', 'rb')
p = PyAudio()
stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                channels=wf.getnchannels(),
                rate=wf.getframerate(), output=True)
# 读数据
data = wf.readframes(self.CHUNK)
print("*"*5,"正在播放","*"*5)
# 播放流
while len(data) > 0:
    stream.write(data)
    data = wf.readframes(self.CHUNK)
    print(".")
stream.stop_stream()  # 暂停播放/录制
stream.close()  # 终止播放
p.terminate()  # 终止portaudio会话
"""
"""
#本模块的测试代码
if __name__ == '__main__':
    a = Contro_sound()
    a.Play_sound(File_name = "resource/voive/x.wav")#播放本地提示音文件
    #a.Get_sound(File_name = "resource/speak.wav", Sound_time = 2 )#进行录音
    #a.Play_sound(File_name = "resource/ding.wav")#播放提示音
    #a.Play_sound(File_name = "resource/speak.wav")#播放刚刚的录音文件件
"""


