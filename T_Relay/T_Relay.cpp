/********************************************************************/
/*								    */
/*		МОДУЛЬ ЗАДАЧИ "МОДЕЛИРОВАНИЕ СЕТИ СООБЩЕНИЙ"        */
/*								    */
/********************************************************************/

#define  stricmp  _stricmp

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <malloc.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys\stat.h>

#include "..\Matrix\Matrix.h"

#include "..\Crowd_Feature\Crowd_Feature.h"
#include "..\Crowd_Message\Crowd_Message.h"
#include "..\Crowd_Object\Crowd_Object.h"
#include "..\Crowd_Kernel\Crowd_Kernel.h"

#include "T_Relay.h"


#pragma warning( disable : 4244 )
#pragma warning( disable : 4996 )


#define  _SECTION_FULL_NAME   "RELAY"
#define  _SECTION_SHRT_NAME   "RELAY"

#define  SEND_ERROR(text)    SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,        \
                                         (LPARAM) text)
#define  SEND_INFO(text)     SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_INFO_MESSAGE,         \
                                         (LPARAM) text)


#define  CREATE_DIALOG  CreateDialogIndirectParam


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


/********************************************************************/
/*								    */
/*		    	Программный модуль                          */

     static   Crowd_Module_Relay  ProgramModule ;


/********************************************************************/
/*								    */
/*		    	Идентификационный вход                      */

 T_RELAY_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 T_RELAY_API Crowd_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/********************************************************************/
/********************************************************************/
/**							           **/
/**	       ОПИСАНИЕ КЛАССА ЗАДАЧИ "СХЕМА ОТОБРАЖЕНИЯ"          **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*                            Список команд                         */

  struct Crowd_Module_Relay_instr  Crowd_Module_Relay_InstrList[]={

 { "help",       "?",  "#HELP (?) - список доступных команд", 
                        NULL,
                       &Crowd_Module_Relay::cHelp   },
 { "state",      "p",  "#STATE (S) - задание режима хранения состояний объектов", 
                        "STATE/M - хранение в памяти\n "
                        "STATE/F <путь> - хранение в файлах в указанной папке"
                        "STATE/L <путь> - хранение в памяти и файлах - только для просмотра",
                       &Crowd_Module_Relay::cState    },
 { "prepare",    "p",  "#PREPARE (P) - подготовка стартовой сцены", 
                        "PREPARE <имя сообщения> <временная зона> - поместить сообщение в очередь",
                       &Crowd_Module_Relay::cPrepare    },
 { "queue",      "q",  "#QUEUE (Q) - показать очередь сообщений", 
                        NULL,
                       &Crowd_Module_Relay::cQueue    },
 { "debug",      "d",  "#DEBUG (D) - открыть окно отладки", 
                        NULL,
                       &Crowd_Module_Relay::cDebug    },
 { "run",        "r",  "#RUN (R) - запустить моделирование", 
                        NULL,
                       &Crowd_Module_Relay::cRun    },
 {  NULL }
                                                                  } ;


/********************************************************************/
/*								    */
/*		     Общие члены класса             		    */

  struct Crowd_Module_Relay_instr *Crowd_Module_Relay::mInstrList=NULL ;

  struct             Crowd_MQueue  Crowd_Module_Relay::mQueue[_QUEUE_DEEP] ;

                             HWND  Crowd_Module_Relay::mQueueDlg ;
                             HWND  Crowd_Module_Relay::mDebugDlg ;

                              int  Crowd_Module_Relay::mStateRegime ;
                             char  Crowd_Module_Relay::mStateFolder[FILENAME_MAX] ;

                             long  Crowd_Module_Relay::mStep ;

/********************************************************************/
/*								    */
/*		       Конструктор класса			    */

     Crowd_Module_Relay::Crowd_Module_Relay(void)

{
                keyword="Crowd" ;
         identification="Relay" ;
               category="Task" ;

             mInstrList=Crowd_Module_Relay_InstrList ;
}


/********************************************************************/
/*								    */
/*		        Деструктор класса			    */

    Crowd_Module_Relay::~Crowd_Module_Relay(void)

{
}


/********************************************************************/
/*								    */
/*		        Получить параметр       		    */

     int  Crowd_Module_Relay::vGetParameter(char *name, char *value)

{
/*-------------------------------------------------- Описание модуля */

    if(!stricmp(name, "$$MODULE_NAME")) {

         sprintf(value, "%-20.20s -  Моделирование сети сообщений", identification) ;
                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*	                Специальный интерфейс                       */

  int  Crowd_Module_Relay::vSpecial(char *action, void *object, char *details)

{
  Crowd_Object *obj ;
          char  path[FILENAME_MAX] ;
          FILE *file ;

/*------------------------------------------- Запись сообщения в лог */

   if(!stricmp(action, "LOG")) {

         if(mDebugDlg)  SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)object) ;

                               }
/*------------------------------ Запись/считывание состояния объекта */

   if(!stricmp(action, "SAVE_STATE")) {

                       obj=(Crowd_Object *)object ;

     if(mStateRegime==_MEMORY_STATE ||
        mStateRegime==   _LOG_STATE   ) {
                  obj->State=(char *)realloc(obj->State, strlen(details)+8) ;
           strcpy(obj->State, details) ;
                                        }
     if(mStateRegime==  _FILE_STATE ||
        mStateRegime==   _LOG_STATE   ) {

                sprintf(path, "%s\\%s.state", mStateFolder, obj->Name) ;
             file=fopen(path, "wb") ;
          if(file==NULL)  return(-1) ;

                 fwrite(details, 1, strlen(details), file) ;
                 fclose(file) ;
                                        }                 
                                      }

   if(!stricmp(action, "READ_STATE")) {

                       obj=(Crowd_Object *)object ;

     if(mStateRegime==_MEMORY_STATE ||
        mStateRegime==   _LOG_STATE   ) {

           if(obj->State!=NULL)  strcpy(details, obj->State) ;
           else                  strcpy(details, "") ;
                                        }
     if(mStateRegime==  _FILE_STATE   ) {

                sprintf(path, "%s\\%s.state", mStateFolder, obj->Name) ;
             file=fopen(path, "rb") ;
          if(file==NULL)  return(-1) ;

                  fread(details, 1, 64000, file) ;
                 fclose(file) ;
                                        }                 

                                      }
/*------------------------------ Получение номера шага моделирования */

   if(!stricmp(action, "GET_STEP")) {

                       (*(double *)object)=mStep ;

                                    }
/*-------------------------------------------------------------------*/


   return(0) ;
}


/********************************************************************/
/*								    */
/*		   Регистрация сообщения в очереди                  */

  int  Crowd_Module_Relay::vAddMessage(Crowd_Message *message, int  zone)

{
#define  QUEUE      this->mQueue[zone].messages
#define  QUEUE_CNT  this->mQueue[zone].cnt
#define  QUEUE_MAX  this->mQueue[zone].max

/*--------------------------------------------- Выбор временной зоны */ 

   if(zone<    0        ||
      zone>=_QUEUE_DEEP   )  return(-1) ;

/*------------------------------------ Занесение сообщения в очередь */ 

   if(QUEUE_CNT==QUEUE_MAX) {

          QUEUE=(Crowd_Message **)realloc(QUEUE, (QUEUE_MAX+100)*sizeof(*QUEUE)) ;
       if(QUEUE==NULL)  return(-1) ;
                                                 QUEUE_MAX+=100 ;
                            }

          QUEUE[QUEUE_CNT]=message ;
                QUEUE_CNT++ ; 

/*-------------------------------------------------------------------*/ 

#undef  QUEUE
#undef  QUEUE_CNT
#undef  QUEUE_MAX

   return(0) ;
}


/********************************************************************/
/*								    */
/*		        Выполнить команду       		    */

  int  Crowd_Module_Relay::vExecuteCmd(const char *cmd)

{
   return( vExecuteCmd(cmd, NULL) ) ;
}


  int  Crowd_Module_Relay::vExecuteCmd(const char *cmd, Crowd_IFace *iface)

{
  static  int  direct_command ;   /* Флаг режима прямой команды */
         char  command[1024] ;
         char *instr ;
         char *end ;
          int  status ;
          int  i ;

/*--------------------------------------------- Идентификация секции */

             memset(command, 0, sizeof(command)) ;
            strncpy(command, cmd, sizeof(command)-1) ;
/*- - - - - - - - - - - - - - - - - - - -  Обработка адресных команд */
        if(command[0]=='&') {
                                end=command+1 ;
                            }
/*- - - - - - - - - - - - - - - - - - - - -  Обработка прямых команд */
   else if(!direct_command) {

         end=strchr(command, ' ') ;
      if(end!=NULL) {  *end=0 ;  end++ ;  }

      if(stricmp(command, _SECTION_FULL_NAME) &&
         stricmp(command, _SECTION_SHRT_NAME)   )  return(1) ;
                            }
/*- - - - - - - - - - - - - - - - - - - -  Обработка опросных команд */
   else                     {
                                end=command ;
                            }
/*----------------------- Включение/выключение режима прямой команды */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - -  Включение */
   if(end==NULL || end[0]==0) {
     if(!direct_command) {
                            direct_command=1 ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"Task Relay:") ;
        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_DIRECT_COMMAND, (LPARAM)identification) ;
                         }
                                    return(0) ;
                              }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - Выключение */
   if(end!=NULL && !strcmp(end, "..")) {

                            direct_command=0 ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"") ;
        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_DIRECT_COMMAND, (LPARAM)"") ;

                                           return(0) ;
                                       }
/*--------------------------------------------- Выделение инструкции */

       instr=end ;                                                  /* Выделяем слово с названием команды */

     for(end=instr ; *end!= 0  &&
                     *end!=' ' &&
                     *end!='>' &&
                     *end!='.' &&
                     *end!='/'    ; end++) ;

      if(*end!= 0 &&
         *end!=' '  )  memmove(end+1, end, strlen(end)+1) ;

      if(*end!=0) {  *end=0 ;  end++ ;  }
      else            end="" ;

   for(i=0 ; mInstrList[i].name_full!=NULL ; i++)                   /* Ищем команду в списке */
     if(!stricmp(instr, mInstrList[i].name_full) ||
        !stricmp(instr, mInstrList[i].name_shrt)   )   break ;

     if(mInstrList[i].name_full==NULL) {                            /* Если такой команды нет... */

          status=this->kernel->vExecuteCmd(cmd, iface) ;            /*  Пытаемся передать модулю ядра... */
       if(status)  SEND_ERROR("Секция RELAY: Неизвестная команда") ;
                                            return(-1) ;
                                       }
 
     if(mInstrList[i].process!=NULL)                                /* Выполнение команды */
                status=(this->*mInstrList[i].process)(end, iface) ;
     else       status=  0 ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*								    */
/*		      Реализация инструкции HELP                    */

  int  Crowd_Module_Relay::cHelp(char *cmd, Crowd_IFace *iface)

{ 
    DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_HELP", RT_DIALOG),
			   GetActiveWindow(), Task_Relay_Help_dialog) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      Реализация инструкции STATE                   */
/*								    */
/*      STATE/M                                                     */
/*      STATE/F <путь>                                              */
/*      STATE/L <путь>                                              */

  int  Crowd_Module_Relay::cState(char *cmd, Crowd_IFace *iface)

{
#define   _PARS_MAX  10

            char *pars[_PARS_MAX] ;
            char *end ;
             int  i ;

/*---------------------------------------- Разборка командной строки */
/*- - - - - - - - - - - - - - - - - - -  Выделение ключей управления */
       if(*cmd=='/') {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end!=NULL)  *end=0 ;
                
                if(strchr(cmd, 'm')!=NULL ||
                   strchr(cmd, 'M')!=NULL   ) mStateRegime=_MEMORY_STATE ;
                if(strchr(cmd, 'f')!=NULL ||
                   strchr(cmd, 'F')!=NULL   ) mStateRegime=  _FILE_STATE ;
                if(strchr(cmd, 'l')!=NULL ||
                   strchr(cmd, 'L')!=NULL   ) mStateRegime=   _LOG_STATE ;

                if(end!=NULL)  cmd=end+1 ;
                else           cmd= "" ;
                     }
/*- - - - - - - - - - - - - - - - - - - - - - - -  Разбор параметров */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

      if(pars[0]!=NULL)
       if(*pars[0]!=0)  strcpy(mStateFolder, pars[0]) ;

/*---------------------------------------- Контроль папки сохранения */
  
    if(mStateRegime==_FILE_STATE ||
       mStateRegime== _LOG_STATE   ) {

     if(mStateFolder[0]==0) {
                      SEND_ERROR("Не задан путь папки сохранения состояний объектов") ;
                                     return(-1) ;
                            }

     if(access(mStateFolder, 0x00)) {
                      SEND_ERROR("Указанная папка сохранения состояний объектов не существует") ;
                                     return(-1) ;
                                    }

                                     }
/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      Реализация инструкции PREPARE                 */
/*								    */
/*      PREPARE <имя сообщения> <временная зона>                    */

  int  Crowd_Module_Relay::cPrepare(char *cmd, Crowd_IFace *iface)

{
#define   _PARS_MAX  10

            char *pars[_PARS_MAX] ;
            char *name_msg ;
            char *zone_txt ;
             int  zone ;
   Crowd_Message *message ;
             int  status ;
            char *end ;
             int  i ;

/*---------------------------------------- Разборка командной строки */
/*- - - - - - - - - - - - - - - - - - -  Выделение ключей управления */
/*- - - - - - - - - - - - - - - - - - - - - - - -  Разбор параметров */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name_msg=pars[0] ;
                     zone_txt=pars[1] ;

/*------------------------------------------- Контроль имени объекта */

    if(zone_txt==NULL) {                                            /* Если имя не задано... */
                      SEND_ERROR("Должно быть задан индекс временной зоны: 0 ... 10000 \n"
                                 "Например: PREPARE <имя сообщения> <временная зона>") ;
                                     return(-1) ;
                       }

       message=FindMessage(name_msg) ;                              /* Ищем сообщение по имени */
    if(message==NULL)  return(-1) ;

/*---------------------------------- Регистрация сообщения в очереди */

                                   zone=strtol(zone_txt, &end, 10) ;
       status=vAddMessage(message, zone) ;
    if(status) {
                  SEND_ERROR("Ошибка регистрации сообщения в очереди") ;
                                     return(-1) ;
               }
/*----------------------------------------------- Отображение данных */

    if(this->mQueueDlg)
        SendMessage(this->mQueueDlg, WM_USER, (WPARAM)_USER_REFRESH, NULL) ;

/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      Реализация инструкции QUEUE                   */

  int  Crowd_Module_Relay::cQueue(char *cmd, Crowd_IFace *iface)

{
/* 
  HWND  hDlg ;


    hDlg=CreateDialogIndirect(GetModuleHandle(NULL),
                               (LPCDLGTEMPLATE)Resource("IDD_QUEUE", RT_DIALOG),
                                  GetActiveWindow(), Task_Relay_Queue_dialog) ;
                   ShowWindow(hDlg, SW_SHOW) ;
*/

      DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_QUEUE", RT_DIALOG),
			   GetActiveWindow(), Task_Relay_Queue_dialog) ;
   return(0) ;
}


/********************************************************************/
/*								    */
/*		      Реализация инструкции DEBUG                   */

  int  Crowd_Module_Relay::cDebug(char *cmd, Crowd_IFace *iface)

{ 
      DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_DEBUG", RT_DIALOG),
			   GetActiveWindow(), Task_Relay_Debug_dialog) ;
   return(0) ;
}


/********************************************************************/
/*								    */
/*		      Реализация инструкции RUN                     */
/*								    */
/*       RUN                                                        */

  int  Crowd_Module_Relay::cRun(char *cmd, Crowd_IFace *iface)

{
            int  debug_flag ;
   Crowd_Object *object ;
  Crowd_Message  init_msg ;
            int  status ;
           char  decl[1024] ;
           char  text[1024] ;
            int  exit_flag ;
            int  n ;
            int  i ;

#define   OBJECTS       Crowd_Kernel::kernel->kernel_objects
#define   OBJECTS_CNT   Crowd_Kernel::kernel->kernel_objects_cnt

#define   QUEUE         this->mQueue[0].messages 
#define   QUEUE_CNT     this->mQueue[0].cnt 

/*---------------------------------------- Разборка командной строки */
/*- - - - - - - - - - - - - - - - - - -  Выделение ключей управления */
                        debug_flag=0 ;

       if(*cmd=='/') {
 
                if(*cmd=='/')  cmd++ ;

//                 end=strchr(cmd, ' ') ;
//              if(end==NULL) {
//                     SEND_ERROR("Некорректный формат команды") ;
//                                     return(-1) ;
//                            }
//                *end=0 ;

                if(strchr(cmd, 'd')!=NULL ||
                   strchr(cmd, 'D')!=NULL   ) debug_flag=1 ;

//                           cmd=end+1 ;
                     }
/*------------------------------------------------------- Подготовка */

     for(i=0 ; i<OBJECTS_CNT ; i++)  OBJECTS[i]->vEventStart() ;

                 init_msg.Object_s=NULL ;
          strcpy(init_msg.Type, "system") ;
          strcpy(init_msg.Kind, "initialisation") ;

     for(i=0 ; i<OBJECTS_CNT ; i++)  OBJECTS[i]->vEvent(0, "MESSAGE", (void *)&init_msg, this) ;

/*----------------------------------------------------- Главный цикл */

       if(debug_flag)  this->debug_next=_CROWD_KERNEL_WAIT_STEP ;
       else            this->debug_next=    0 ;

   for(mStep=0 ; ; ) {

           if(this->debug_stop)  break ;                            /* Если внешнее прерывание... */

           if(this->debug_next==_CROWD_KERNEL_WAIT_STEP) {          /* Управление пошаговым режимом */
                                     Sleep(100) ;
                                      continue ;
                                                         }
           if(this->debug_next==_CROWD_KERNEL_NEXT_STEP) {
                      this->debug_next=_CROWD_KERNEL_WAIT_STEP ;
                                                         }

                        mStep++ ;

                          sprintf(text, "=== Step %d  -  %d messages", mStep, this->mQueue[0].cnt) ; 
       if(mDebugDlg)  SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;

/*----------------------------------------------- Временная разрядка */

    if(this->kernel->show_time_step!=0) {

                Sleep(1000*this->kernel->show_time_step) ;

                                        }
/*---------------------------------------------- Обработка сообщений */

     for(n=0 ; n<this->mQueue[0].cnt ; n++) {

         if(mDebugDlg) {
                                   QUEUE[n]->vFormDecl(decl) ;
                          sprintf(text, "--- Message : %4d  -  %s ", n, decl) ; 
                      SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;
                       }
/*- - - - - - - - - - - - - - - -  Определение объекта - обработчика */
       for(i=0 ; i<OBJECTS_CNT ; i++) {
              if(QUEUE[n]->Object_r!=NULL    )  object=QUEUE[n]->Object_r ;
         else if(QUEUE[n]->vCheck(OBJECTS[i]))  object=OBJECTS[i] ; 
         else                                     continue ;
/*- - - - - - - - - - - - - - - - - - - - - - -  Обработка сообщения */
         if(mDebugDlg) {
                          sprintf(text, " Object : %s", object->Name) ; 
                      SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;
                       }

            status=object->vEvent(mStep, "MESSAGE", (void *)QUEUE[n], this) ;
         if(status)  break ;

                   object->vEventShow() ;

         if(QUEUE[n]->Object_r!=NULL)  break ;
                                      }
/*- - - - - - - - - - - - - - - - - - - - - - - - Удаление сообщения */
         if(QUEUE[n]->Object_r==NULL)  delete QUEUE[n] ;            /* Если сообщение не именованое - удаляем его */
                                              QUEUE[n]=NULL ;

         if(mQueueDlg)  SendMessage(this->mQueueDlg, WM_USER, (WPARAM)_USER_REFRESH, (LPARAM)NULL) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                            }

       if(status)  break ;

/*----------------------------------------------- Отображение данных */

                               this->kernel->vShow(NULL) ;

/*------------------------------------------ Сдвиг очереди сообщений */

                  free(this->mQueue[0].messages) ;                  /* Освобождаем стек первого слота очереди */

                                             exit_flag=1 ;

      for(i=1 ; i<_QUEUE_DEEP ; i++) {

                this->mQueue[i-1]=this->mQueue[i] ;
             if(this->mQueue[i].cnt) exit_flag=0 ;
                                     }

           memset(&this->mQueue[_QUEUE_DEEP-1], 0,                  /* Сбрасываем последний слот очереди */
                           sizeof(this->mQueue[0])) ;

            if(exit_flag)  break ;

/*----------------------------------------------------- Главный цикл */
                     }

         if(mDebugDlg) {
                          sprintf(text, "=== Exit") ; 
                      SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;
                       }

            SEND_INFO("Моделирование завершено") ;

/*-------------------------------------------------------------------*/

#undef    QUEUE
#undef    QUEUE_CNT

#undef    OBJECTS
#undef    OBJECTS_CNT

   return(0) ;
}


/********************************************************************/
/*								    */
/*		   Поиск обьекта по имени                           */

  Crowd_Object *Crowd_Module_Relay::FindObject(char *name)

{
     char   text[1024] ;
      int   i ;

#define   OBJECTS       this->kernel->kernel_objects 
#define   OBJECTS_CNT   this->kernel->kernel_objects_cnt 

/*------------------------------------------- Поиск объекта по имени */ 

       for(i=0 ; i<OBJECTS_CNT ; i++)                               /* Ищем объект по имени */
         if(!stricmp(OBJECTS[i]->Name, name))  break ;

    if(i==OBJECTS_CNT) {                                            /* Если имя не найдено... */
                           sprintf(text, "Объекта с именем '%s' "
                                         "НЕ существует", name) ;
                        SEND_ERROR(text) ;
                            return(NULL) ;
                       }
/*-------------------------------------------- Контроль типа объекта */ 

/*-------------------------------------------------------------------*/ 

   return((Crowd_Object *)OBJECTS[i]) ;
  
#undef   OBJECTS
#undef   OBJECTS_CNT

}


/********************************************************************/
/*								    */
/*		   Поиск сообщения по имени                         */

  Crowd_Message *Crowd_Module_Relay::FindMessage(char *name)

{
     char   text[1024] ;
      int   i ;

#define   MESSAGES       this->kernel->kernel_messages 
#define   MESSAGES_CNT   this->kernel->kernel_messages_cnt 

/*----------------------------------------- Поиск сообщения по имени */ 

       for(i=0 ; i<MESSAGES_CNT ; i++)                              /* Ищем объект по имени */
         if(!stricmp(MESSAGES[i]->Name, name))  break ;

   if(i==MESSAGES_CNT) {                                            /* Если имя не найдено... */
                           sprintf(text, "Сообщения с именем '%s' "
                                         "НЕ существует", name) ;
                        SEND_ERROR(text) ;
                            return(NULL) ;
                       }
/*------------------------------------------ Контроль типа сообщения */ 

/*-------------------------------------------------------------------*/ 

   return((Crowd_Message *)MESSAGES[i]) ;
  
#undef   MESSAGES
#undef   MESSAGES_CNT

}


