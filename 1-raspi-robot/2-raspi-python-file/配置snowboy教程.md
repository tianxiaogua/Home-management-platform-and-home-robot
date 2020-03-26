## 如果不是最新版本的snowboy可能不支持Ubuntu等运行在电脑上的操作系统，因为在编译时生成的so文件不能对应电脑的操作系统，在运行时就会出错。



## 本教程转自：

#### https://github.com/dingdang-robot/dingdang-robot/wiki/configuration#

### and

## snowboy进行语音唤醒，编译snowboy文件：

## https://blog.csdn.net/weixin_40490238/article/details/88848335



#### 1、更新pip

```
pip3 install --upgrade pip
```

#### 2、安装依赖环境

```
sudo apt-get install python-pyaudio python3-pyaudio sox
```

```
pip install pyaudio
```



#### 3、测试音频录制

```
rec temp.wav
```



#### 4、如果此时报错这里时会报错，can't open input 'default': ....... 这时安装配置麦克

```
sudo apt-get install alsa-utils pulseaudio
```



#### 5、ctrl+c停止录音，aplay test.wav 播放成功。



#### 6、下载snowboy：https://github.com/kitt-ai/snowboy



#### 7、安装swig（3.0.10或者更高的版本）

```
sudo apt-get install swig
```



#### 8、然后安装`atlas`矩阵计算库：(必须安装，否则编译报错)

```
sudo apt-get install libatlas-base-dev
```



#### 9、打开snowboy的下载包，解压，编译后得到了编译好的文件_snowboydetect.so

```
cd /snowboy/swig/Python3
```

```
make
```

#### 10、下载snowboy官方raspberry的包：https://s3-us-west-2.amazonaws.com/snowboy/snowboy-releases/rpi-arm-raspbian-8.0-1.1.1.tar.bz2

#### 解压此包，将_snowboydetect.so放入该目录

#### 在此生成自己的唤醒词：https://snowboy.kitt.ai/hotword/34901

#### 然后将自己的**.pmdl文件放入目录

#### 11、运行

```
python3 demo.py jingling.pmdl
```

