# coding: utf-8
#
import sys
import time
import os
import json
import math
import glob

import flask

#
#  Глобальные переменные - настройки интерфейса
# 
__title         =""
__control_folder=""  # Обменная папка
__object_name   =""  # Интерфейсное имя управляемого объекта или *
__targets_path  =""  # Файл со списком агентов сцены

__port          =""  # Номер порта сетевого интерфейса, если не задан - используется файловый интерфейс

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
                     #   x, y, z
                     #   gas, gas_max, gas_renew
                     #   links
                     #   events
                     #   callback
__targets       ={}  # Список объектов сцены
__targets_time  =""  # Временная метка списка объектов сцены
__commands      ={}  # Команды обратной связи на сцену:
                     #   commands
                     #   messages_cnt
__callback      ={}  # Ответы на сообщения:
                     #   replies

#------------------------------------
#  Чтение конфигурационного файла
#------------------------------------
def read_config(path) :
    global  __title
    global  __control_folder
    global  __object_name
    global  __targets_path
    global  __port
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
        elif words[0]=="Port"          :      __port       =words[1]
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
#  Формирование ответа
#------------------------------------ 
def form_response() :
    global  __object
    global  __commands
    global  __callback

    response ="{ \"name\":\"" + __object["name"] + "\",\"t\":\"" + __object["t"] + "\","
    response+="\"x\":\"" + __object["x"] + "\",\"y\":\"" + __object["y"] + "\",\"z\":\"" + __object["z"] + "\"," 
    response+="\"commands\":[ " + __commands['commands'] + " ]," ;
    response+="\"callback\":[ " + __callback['replies'] + " ]}" ;

    return response
#------------------------------------
#  Формирование файла ответа
#------------------------------------ 
def write_response(path, response) :

    try :
        file=open(path, "w")
        file.write(response)
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

    name    = __object['name'] + "-" + str(__commands['messages_cnt'])

    command ="{ \"command\":\"sendmessage\","
    command+="\"name\":\"" + name + "\","
    command+="\"type\":\"" + msg_type + "\","
    command+="\"kind\":\"" + kind + "\","
    command+="\"recipient\":\"" + recipient + "\","
    command+="\"info\":\"" + info + "\","
    command+="\"delay\":\"" + str(delay) + "\"}"

    if len(__commands['commands'])>0 : __commands['commands']+=","

    __commands['commands']+=command

    return name
#----------------------------------------------------------
#  Добавление ответа на сообщение
#----------------------------------------------------------
def send_reply(sender:str, msg_id:str, data:str) :
    global  __callback

    reply ="\"sender\":\"" + sender + "\","
    reply ="\"msgId\":\"" + msg_id + "\","
    reply+="\"data\":\"" + data + "\"}"

    if len(__callback['replies'])>0 : __callback['replies']+=","

    __callback['replies']+=reply

    return
#------------------------------------
#  Модель агента
#------------------------------------ 
def agent() :
    global  __object
    global  __targets
    global  __commands

    __commands['commands']=""
    __callback['replies' ]=""

    for event in __object['events']:

        if event['sender']==__object['name'] : continue

        x_o=float(__object['x'])
        y_o=float(__object['y'])
        x_s=float(__targets[event['sender']]['x'])
        y_s=float(__targets[event['sender']]['y'])
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
# Настройка сетевого интерфейса

app = flask.Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def api():
    global __object
    global __targets
    global __targets_time

    request =flask.request.data.decode('UTF-8')
    data=json.loads(request)

    if data['method']=="targets":             # targets - Получение списка агентов сцены

        for target in data['objects'] :
            __targets[target['name']]=target

        __targets_time=data["t"]

        response="{ \"result\":\"success\" }"

    elif data['method']=="start":             # start - Запуск расчета агента

        __object=data
        agent()
        response="{ \"result\":\"success\" }"

    elif data['method']=="get":               # get - Получение результата расчета

        response=form_response()

    else :                                    # Неизвестная команда
        response="{ \"result\":\"error\", \"details\":\"Unknown method\" }"

    return response

@app.route('/hello', methods=['GET', 'POST'])
def hello():
    return "Hello Python!"

#
# Инициализация
__commands['messages_cnt']=0
#
# Чтение конфигурационного файла
if len(sys.argv) < 2 :
        print("Configuration file is missed")
#        sys.exit(1)

path="D:\\_Projects\\MGTU\\Crowd\\x64\\Debug\\Tests\\ExtAgent.cfg"
#path=sys.argv[1]

status=read_config(path)
if status!=0 : sys.exit(2)

if __port!="" :
    if __name__ == '__main__' : app.run('localhost', 4449)
    sys.exit(0)
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
    response=form_response()
    status=write_response(__control_folder + "\\" + object_name + ".in", response)
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
