## 环境监测代码说明

**硬件接线图及PCB原理图地址如下：**

```https://oshwhub.com/YWJL/nodemcu-v2-0_copy```

```https://lceda.cn/editor#id=|93ed3ca0e88e4248a313931244c7f088|95118be39cc84e8e89e1c2054fb126b2|10ccc78deb634d89a45171e4a9ef0a0a```

**PCB打样下单压缩包：**

> Nmcu_v2.0.zip



### tcp_connect_v1_2.ino 与 tcp_connect_v1_2.txt 的区别

**代码中使用的模块是 BMP280，GY-30， DS1302**

> 1. ino 文件是基于 HTTP 协议的数据交互方式，适用于与 fastAPI 构建的后端项目通信
> 2. txt文件是基于 TCP 的数据交互方式，适用于现有的由 socket 搭建的后端，TRserver4.py 

### 服务器文件说明:

> 1. TRserver4.py 为 socket 建立的 TCP 服务器，接收到的数据保存在csv中，与 txt 文件配套使用，注意端口即可
> 2. TRserver4.py 有两个模块功能，校准时间与保存数据

### data文件夹说明

**存放直到 2021-05-23 时刻的数据**

### db 文件夹说明

**数据缺失严重，不建议使用**


