# Elysia-Voice-alarm-clock

**一个基于F103C8T6的爱莉希雅语音闹钟**

# 硬件基础

- F103C8T6最小系统板
- DS1302 时钟模块
- MP3-TF-16P / DF player mini 模块
- 可以格式化的TF卡
- 扬声器 (< 3w)
- 0.96寸OLED显示屏 (I2C四引脚)
- 按键8个

- DS18B20 温度模块 (可选)

**接线图**

![](docs\README\imgs\wiring_diagram.jpg)

# 使用教程

下载项目根目录的hex文件

下载语音包: [百度网盘](https://pan.baidu.com/s/1zjnlRwGfmdQZtVHG2aF9Dg?pwd=73k6) 密码:73k6

根据接线图接线

首先格式化TF卡

解压语音包, 将文件夹内所有目录拷贝到TF卡根目录中

最后TF卡中是这个样子

<img src=".\docs\README\imgs\TF_contains.png" alt="TF_contains" style="zoom:50%;" />

把TF插入MP3模块

最后烧录项目根目录的hex文件即可.



# 想要自定义?

##            语音文件存储约定

文件夹的命名方式为"01","02", "03"..."99", 每个文件夹下最大支持255个音频

> (1)、指定文件夹播放是我们制定的扩展功能，默认文件夹的命名方式为"01","11"这样的方式因为我 们的模块不支持汉字名称的文件夹名称识别，为了系统的稳定性和歌曲切换的速度，每个文件夹下 默认最大支持 255 首歌,最多支持 99 个文件夹的分类，如果客户有特殊要求，需要按照英文名称来 分类，我们也是可以实现的，但是名称只能是“GUSHI”、“ERGE”等英文名称组成。但是 mp3 文 件是需要增加前缀的，可以在“不得不爱.mp3”基础上改成“002 不得不爱.mp3”.  (2)、例如指定"01"文件夹的 100xxx.MP3 文件,串口发送的指令为:7E FF 06 0F 00 01 64 xx xx EF  DH:代表的是文件夹的名字,默认支持 99 个文件,即 01 -- 99 的命名 DL:代表的是曲目,默认最多 255 首歌，即 0x01 ~ 0xFF  (3)、为了模块的标准性，必须同时指定文件夹和文件名，来锁定一个文件。单独指定文件夹或者单 独指定文件名也是可以的，但是这样文件的管理会变差。指定文件夹和指定曲目是支持 MP3、WAV 
>
> ​															----MP3-TF-16P用户数据手册

- 01~09 ---> 天气

    - 01   --->    天气-下雨"

    - 02   --->    天气-阳光"
    -  03   --->    天气-降温"

    -  ...(其他补充)

    

- 11~19 ---> 情景

    - 11  --->    情景-休息时间"

    - 12  --->    情景-任务设定"

    - 13  --->    情景-任务完成"

    - 14  --->    情景-打招呼"

    - 15  --->    情景-起床"

    - 16  --->    情景-挂机"

    - 17  --->    情景-失败"

    -  ...(其他补充)

    

- 21~29 ---> 互动

    - 21  --->    互动-闲聊"

    - 22  --->    互动-吃"

    - 23  --->    互动-举起来"

    - 24  --->    互动-新衣服"

    - 25  --->    互动-晃动"

    - 26  --->    互动-谢谢"

    - ...(其他补充)

    

- 31~39 ---> 时间

    - 31  --->    时间-早上-问候"
    - 32  --->    时间-晚上"
    - 33  --->    时间-深夜"
    - ...(其他补充)

    

- 41~49 ---> 季节

    - 41  --->    季节-冬天"
    - 42  --->    季节-秋天"
    - 43  --->    季节-夏天"
    - ...(其他补充)

    

- ...(其他补充)



- 80 音乐 



- 81~99 杂项

    - 81  --->    杂项-角色生日"

    - 82  --->    杂项-生日"

    - 83  --->    杂项-周一"

    - ...(其他补充)

## 源代码的宏定义修改

~~目前自定义需要修改代码的宏定义, 以后可能会优化~~

...先不写了, 完善再说
