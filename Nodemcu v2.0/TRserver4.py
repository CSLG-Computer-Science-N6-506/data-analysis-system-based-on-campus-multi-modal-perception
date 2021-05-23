#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# TIME为时间获取字符串
import socket
import threading
import random
import time
import csv
import os
import webbrowser
import pymysql
import time
import traceback
tcp_all = '/var/sensordata/server-sgf/tcpdata506-8008.csv'
#con = pymysql.connect(host="10.18.52.156", port=3306, user='root',
                      #database="cslg_sensordata", password="123456", charset='utf8')
pwd = (os.getcwd()).replace('/', '\\')


def tcplink(conn, addr):
    print("accept new connection from %s:%s" % addr)
    # conn.send(b"2\n")
    while True:
        # conn.send(b"wait one second")
        data = conn.recv(1024)
        d = time.strftime('%Y-%m-%d,%H:%M:%S', time.localtime(time.time()))
        data1 = (str(data, encoding="utf-8"))
        print(data1)
        if data1 == 'TIME':
            conn.send(bytes(d, encoding="utf8"))
            f1 = open('/var/sensordata/server-sgf/time506.csv', 'a')
            f1.write("%s," % d)
            f1.write("%s,%d\n" % addr)
            f1.close()
            break
        if len(data1.split(',')) > 4:
            print(data1)
            conn.send(b'welcome to server!')
            device_id0 = data1.split(',')[0]  # O-Tem-Press-Lig
            device_id = device_id0.split('-')[0]  # O
            name = device_id.split('-')[0]+'.csv'
            if device_id == 'Anano' or device_id == 'Nmcu':
                device_id = device_id0.split('-')[1]  # R3C3Q
                name = device_id0.split('-')[1]+'.csv'
            f1 = open(name, 'a')  # 文件写入
            f1.write(d)
            f1.write(",")
            f1.write(str(data, encoding="utf-8"))
            f1.write(",%s,%d\n" % addr)
            f1.close()
            f2 = open(tcp_all, 'a')
            f2.write(d)
            f2.write(",")
            f2.write(str(data, encoding="utf-8"))
            f2.write(",%s,%d\n" % addr)
            f2.close()
            # con = pymysql.connect(host="192.168.3.100",port=3306,user='root',database="sensor_all",password="123456",charset='utf8')
            # 连接数据库ip，端口号，用户名，密码
            # try:
            #     con.connect()
            #     cursor = con.cursor()  # 定义游标
            #     RSSI=data1.split(',')[6]  # 获取信号强度
            #     Lig = data1.split(',')[5]  # 获取光强数据
            #     Tem = data1.split(',')[2]  # 获取温度数据
            #     Press = data1.split(',')[4]  # 获取大气压强数据
            #     sql = ('insert into {} (date,time,Light,Temperature,Press,RSSI) VALUES ("%s","%s","%s","%s","%s","%s");' % (
            #         d.split(',')[0], d.split(',')[1], Lig, Tem, Press,RSSI)).format(device_id)  # 选择对应表插入数据
            #     print(sql)
            #     while True:
            #         try:
            #             with con.cursor() as cursor:  # 获取游标
            #                 cursor.execute(sql)
            #             con.commit()
            #             break
            #         except Exception as e:
            #             print(e)
            #            # traceback.print_exc()
            #             con.ping(True)
                # data=cursor.fetchall() #从游标中提取数据
            time.sleep(3)
            # except:
            #     pass
            break
    conn.close()
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("10.18.52.137", 8009))
s.listen()
print("Waiting for connection...")

while True:
    conn, addr = s.accept()
    t = threading.Thread(target=tcplink, args=(conn, addr))
    t.start()
