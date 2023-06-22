# coding: utf-8
#
import sys
import time
import os
import json
import math
import glob 
#
#  Глобальные переменные - настройки интерфейса
# 
__title         =""
__control_folder=""  # Обменная папка
__object_name   =""  # Интерфейсное имя управляемого объекта или *
__targets_path  =""  # Файл со списком агентов сцены

#
#  Глобальные переменные - настройки модели
# 
__agent_step    =""  # Размерность шага перемещения

#
#  Глобальные переменные - состояние агента и сцены
# 
__object        ={}  # Параметры объекта:
                     #   t
                     #   name
                     #   type
                     #   x,y,z
                     #   links
                     #   events
__targets       ={}  # Список объектов сцены
__targets_time  =""  # Временная метка списка объектов сцены
__commands      ={}  # Команды обратной связи на сцену:
                     #   commands
                     #   messages_cnt

#------------------------------------
#  Чтение конфигурационного файла
#------------------------------------
def read_config(path) :
    global  __title
    global  __control_folder
    global  __object_name
    global  __targets_path
    global  __agent_step

    try :
        file=open(path, "rt")
        rows=file.readlines()
        file.close
    except OSError :
        print("Configuration file read error : " + path)
        return -1

    for row in rows:
        words=row.replace("\n","").split("=", 1)
        if   words[0]=="Title"         :     __title       =words[1]
        elif words[0]=="ControlFolder" :   __control_folder=words[1]
        elif words[0]=="Object"        :    __object_name  =words[1]
        elif words[0]=="Targets"       :   __targets_path  =words[1]
        elif words[0]=="Step"          :     __agent_step  =words[1]
        else  :
            print("Unknown key in configuration file : ", words[0])
            return -1

    return 0
#------------------------------------
#  Считывание файла запроса
#------------------------------------ 
def read_request(path) :
    global  __object

    try :
        file=open(path, "rt")
        rows=file.read()
        file.close
    except OSError :
        print("Request file read error : " + path)
        return -1

    __object=json.loads(rows)

    return 0
#------------------------------------
#  Считывание файла списка объектов сцены
#------------------------------------ 
def read_targets(path) :
    global  __targets
    global  __targets_time

    try :
        file=open(path, "rt")
        rows=file.read()
        file.close
    except OSError :
        print("Objects(targets) file read error : " + path)
        return -1

    data=json.loads(rows)

    for target in data['objects']:
        __targets[target['name']]=target

    __targets_time=data["t"]

    return 0
#------------------------------------
#  Считывание файла списка объектов сцены
#------------------------------------ 
def write_response(path) :
    global  __object
    global  __commands

    try :
        file=open(path, "w")

        file.write("{ \"name\":\"" + __object["name"] + "\",\"t\":\"" + __object["t"] + "\",")
        file.write("\"x\":\"" + __object["x"] + "\",\"y\":\"" + __object["y"] + "\",\"z\":\"" + __object["z"] + "\",\r\n") 
        file.write("\"commands\":[ " + __commands['commands'] + " ]\r\n}\r\n") ;

        file.close
    except OSError :
        print("Response file write error : " + path)
        return -1

    return 0
#----------------------------------------------------------
#  Добавление команды обратной связи - отправка сообщения
#----------------------------------------------------------
def send_message(msg_type:str, kind:str, recipient:str, info:str, delay:int) :
    global  __object
    global  __commands

    __commands['messages_cnt']+=1

    command ="{ \"command\":\"sendmessage\","
    command+="\"name\":\"" + __object['name'] + "-" + str(__commands['messages_cnt']) + "\","
    command+="\"type\":\"" + msg_type + "\","
    command+="\"kind\":\"" + kind + "\","
    command+="\"recipient\":\"" + recipient + "\","
    command+="\"info\":\"" + info + "\","
    command+="\"delay\":\"" + str(delay) + "\"}\r\n"

    if len(__commands['commands'])>0 : __commands['commands']+=","

    __commands['commands']+=command

    return
#------------------------------------
#  Модель агента
#------------------------------------ 
def agent() :
    global  __object
    global  __targets
    global  __commands

    __commands['commands']=""

    for event in __object['events']:

        if event['sender']==__object['name'] : continue

        x_o=float(__object['x'])
        y_o=float(__object['y'])
        x_s=float(__targets[event['sender']]['x'])
        y_s=float(__targets[event['sender']]['x'])
        r  =math.sqrt( (x_o-x_s)**2+(y_o-y_s)**2 )
        a  =math.atan2(y_o-y_s, x_o-x_s)

        a +=float(__agent_step)/r ;

        __object['x'] = str(x_s+r*math.cos(a) )          # Изменение позиции 
        __object['y'] = str(y_s+r*math.sin(a) )

        send_message("Contact", "Info", __targets[event['sender']]['name'], event['info'], 0)   # Зеркалирование сообщения отправителю

    return
#
#*******************************
#             MAIN
#*******************************
#

#
# Инициализация
__commands['messages_cnt']=0
#
# Чтение конфигурационного файла
if len(sys.argv) < 2 :
        print("Configuration file is missed")
#       path="D:\\_Projects\\MGTU\\Crowd\\x64\\Debug\\Tests\\ExtAgent.cfg"
        sys.exit(1)

path=sys.argv[1]

status=read_config(path)
if status!=0 : sys.exit(2)
#
# ГЛАВНЫЙ ЦИКЛ ОЖИДАНИЯ И ИСПОЛНЕНИЯ
#
while(True):  

    print("Waiting...")
    time.sleep(1)
#
# Ожидание флаг-файла запроса
    if __object_name=="*" :
        files=glob.glob(__control_folder + "\\*.out.flag")
        if len(files)==0 : continue
        flag=files[0]
        object_name=os.path.split(flag)[1].split(".")[0]
    else:
        object_name=__object_name
        flag=__control_folder + "\\" + object_name + ".out.flag"
        if os.path.exists(flag)==False : continue

    os.remove(flag)
#
# Считывание файла запроса
    status=read_request(__control_folder + "\\" + object_name + ".out")
    if status!=0 : break
#
# Считывание файла списка агентов
    if __object["t"]!=__targets_time :
        status=read_targets(__targets_path)
        if status!=0 : break
#
# Вызов модели
    print("Request detected: object=" + __object["name"] + " step=" + __object["t"])    

    agent()
#
# Формирование ответа
    status=write_response(__control_folder + "\\" + object_name + ".in")
    if status!=0 : break
#
# Формирование флаг-файла готовности ответа
    try :
        flag=__control_folder + "\\" + object_name + ".in.flag"
        file=open(flag, "w")
        file.close()
    except OSError :
        print("Response ready flag-file write error : " + path)
        break

#   break
