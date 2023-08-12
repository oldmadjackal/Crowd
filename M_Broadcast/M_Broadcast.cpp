/*********************************************************************/
/*                                                                   */
/*            МОДУЛЬ УПРАВЛЕНИЯ СООБЩЕНИЕМ "ШИРОКОВЕЩАНИЕ"           */
/*                                                                   */
/*********************************************************************/


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

#include "..\Crowd_Message\Crowd_Message.h"
#include "..\Crowd_Object\Crowd_Object.h"
#include "..\Crowd_Kernel\Crowd_Kernel.h"

#include "M_Broadcast.h"

#pragma warning(disable : 4996)

#define  SEND_ERROR(text)    SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,        \
                                         (LPARAM) text)

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

/*********************************************************************/
/*                                                                   */
/*                      Программный модуль                           */

     static   Crowd_Module_Broadcast  ProgramModule ;


/*********************************************************************/
/*                                                                   */
/*                      Идентификационный вход                       */

 M_BROADCAST_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 M_BROADCAST_API Crowd_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/*********************************************************************/
/*********************************************************************/
/**                                                                 **/
/**   ОПИСАНИЕ КЛАССА МОДУЛЯ УПРАВЛЕНИЯ СООБЩЕНИЯ "ШИРОКОВЕЩАНИЕ"   **/
/**                                                                 **/
/*********************************************************************/
/*********************************************************************/

/*********************************************************************/
/*                                                                   */
/*                            Список команд                          */

  struct Crowd_Module_Broadcast_instr  Crowd_Module_Broadcast_InstrList[]={

 { "help",    "?",  "#HELP   - список доступных команд", 
                     NULL,
                    &Crowd_Module_Broadcast::cHelp   },
 { "create",  "cr", "#CREATE (CR)   - создать сообщение от объекта к объекту", 
                    " CREATE <имя сообщения> <объект-отправитель> <вид сообщения> [<данные>]\n",
                    &Crowd_Module_Broadcast::cCreate },
 {  NULL }
                                                                          } ;


/*********************************************************************/
/*                                                                   */
/*                          Общие члены класса                       */

    struct Crowd_Module_Broadcast_instr *Crowd_Module_Broadcast::mInstrList=NULL ;


/*********************************************************************/
/*                                                                   */
/*                          Конструктор класса                       */

     Crowd_Module_Broadcast::Crowd_Module_Broadcast(void)

{
           keyword="Crowd" ;
    identification="Broadcast" ;
          category="Message" ;

        mInstrList=Crowd_Module_Broadcast_InstrList ;
}


/*********************************************************************/
/*                                                                   */
/*                          Деструктор класса                        */

    Crowd_Module_Broadcast::~Crowd_Module_Broadcast(void)

{
}


/********************************************************************/
/*                                                                  */
/*                       Получить параметр                          */

     int  Crowd_Module_Broadcast::vGetParameter(char *name, char *value)

{
/*-------------------------------------------------- Описание модуля */

    if(!stricmp(name, "$$MODULE_NAME")) {

         sprintf(value, "%-20.20s -  Управление сообщением 'Широковещание'", identification) ;
                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/*********************************************************************/
/*                                                                   */
/*                      Стартовая разводка                           */

    void  Crowd_Module_Broadcast::vStart(void)

{
}


/*********************************************************************/
/*                                                                   */
/*                        Создать сообщение                          */

    Crowd_Message *Crowd_Module_Broadcast::vCreateMessage(Crowd_Object  *object_s,
                                                          Crowd_Object  *object_r,
                                                          Crowd_Message *message_ext  )

{
  Crowd_Message *message ;


    if(message_ext!=NULL)  message= message_ext ;
    else                   message=new Crowd_Message_Broadcast ;

         message->Object_s=object_s ;
         message->Object_r=object_r ;

  return(message) ;
}


/*********************************************************************/
/*                                                                   */
/*                         Выполнить команду                         */

  int  Crowd_Module_Broadcast::vExecuteCmd(const char *cmd)

{
  static  int  direct_command ;   /* Флаг режима прямой команды */
         char  command[1024] ;
         char *instr ;
         char *end ;
          int  status ;
          int  i ;

#define  _SECTION_FULL_NAME   "BROADCAST"
#define  _SECTION_SHRT_NAME   "BRO"

/*--------------------------------------------- Идентификация секции */

             memset(command, 0, sizeof(command)) ;
            strncpy(command, cmd, sizeof(command)-1) ;

   if(!direct_command) {
         end=strchr(command, ' ') ;
      if(end!=NULL) {  *end=0 ;  end++ ;  }

      if(stricmp(command, _SECTION_FULL_NAME) &&
         stricmp(command, _SECTION_SHRT_NAME)   )  return(1) ;
                       }
   else                {
                             end=command ;
                       }
/*----------------------- Включение/выключение режима прямой команды */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - -  Включение */
   if(end==NULL || end[0]==0) {
     if(!direct_command) {
                            direct_command=1 ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"Broadcast message:") ;
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
         end=strchr(instr, ' ') ;
      if(end!=NULL) {  *end=0 ;  end++ ;  }
      else              end="" ;

   for(i=0 ; mInstrList[i].name_full!=NULL ; i++)                   /* Ищем команду в списке */
     if(!stricmp(instr, mInstrList[i].name_full) ||
        !stricmp(instr, mInstrList[i].name_shrt)   )   break ;

     if(mInstrList[i].name_full==NULL) {                            /* Если такой команды нет... */

          status=this->kernel->vExecuteCmd(cmd) ;                   /*  Пытаемся передать модулю ядра... */
       if(status)  SEND_ERROR("Секция BROADCAST: Неизвестная команда") ;
                                            return(0) ;
                                       }
 
     if(mInstrList[i].process!=NULL)                                /* Выполнение команды */
                status=(this->*mInstrList[i].process)(end) ;
     else       status=  0 ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*                                                                  */
/*                      Записать данные в строку                    */

    void  Crowd_Module_Broadcast::vWriteSave(std::string *text)

{

/*----------------------------------------------- Заголовок описания */

     *text="#BEGIN MODULE BROADCAST\n" ;

/*------------------------------------------------ Концовка описания */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*                                                                  */
/*                   Реализация инструкции Help                     */

  int  Crowd_Module_Broadcast::cHelp(char *cmd)

{ 
    DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_HELP", RT_DIALOG),
			   GetActiveWindow(), Message_Broadcast_Help_dialog) ;

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    Реализация инструкции CREATE                  */
/*                                                                  */
/*      CREATE <имя сообщения> <объект-отправитель> ...             */
/*                    ...<вид сообщения> [<данные>]                 */

  int  Crowd_Module_Broadcast::cCreate(char *cmd)

{
#define   _PARS_MAX  10

                          char *pars[_PARS_MAX] ;
                          char *name_msg ;
                          char *name_1 ;
                          char *name_2 ;
                          char *kind ;
                          char *info ;
                 Crowd_Message *message ;
                  Crowd_Object *object_1 ;
       Crowd_Message_Broadcast *broadcast ;
                          char *end ;
                           int   i ;

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

                     name_msg = pars[0] ;
                     name_1   = pars[1] ;
                     kind     = pars[2] ;
                     info     = pars[3] ;
/*- - - - - - - - - - - - - - - - - - - - -  Контроль полноты данных */
    if(kind==NULL) {                                                /* Если вид сообщения не задан... */
                      SEND_ERROR("Неверный формат сообщения. \n"
                                 "Например: CREATE <имя сообщения> <объект-отправитель> <вид сообщения> [<данные>]") ;
                                     return(-1) ;
                   }
/*------------------------------------------- Контроль имени объекта */

       message=FindMessage(name_msg) ;                              /* Ищем сообщение по имени */
    if(message!=NULL) {
                        SEND_ERROR("Сообщение с таким именем уже существует") ;
                          return(-1) ;
                      }

       object_1=FindObject(name_1) ;                                /* Ищем объекты по имени */
    if(object_1==NULL)  return(-1) ;

/*----------------------------------------------- Создание сообщения */

#define   MESSAGES       this->kernel->kernel_messages 
#define   MESSAGES_CNT   this->kernel->kernel_messages_cnt 


      broadcast=(Crowd_Message_Broadcast *)ProgramModule.vCreateMessage(object_1, NULL, NULL) ;

                     strncpy(broadcast->Name, name_msg, sizeof(broadcast->Name)-1) ;
                     strncpy(broadcast->Kind, kind,     sizeof(broadcast->Kind)-1) ;
    if(info!=NULL) {
                             broadcast->Info=(char *)calloc(1, strlen(info)+8) ;
                      strcpy(broadcast->Info, info) ;
                   }

       MESSAGES=(Crowd_Message **)
                 realloc(MESSAGES, (MESSAGES_CNT+1)*sizeof(*MESSAGES)) ;
    if(MESSAGES==NULL) {
              SEND_ERROR("Секция BROADCAST: Переполнение памяти") ;
                                return(NULL) ;
                      }

              MESSAGES[MESSAGES_CNT]=broadcast ;
                       MESSAGES_CNT++ ;

#undef   MESSAGES
#undef   MESSAGES_CNT

/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                 Поиск обьекта по имени                           */

  Crowd_Object *Crowd_Module_Broadcast::FindObject(char *name)

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
/*                                                                  */
/*                    Поиск сообщения по имени                      */

  Crowd_Message *Crowd_Module_Broadcast::FindMessage(char *name)

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
//                      SEND_ERROR(text) ;
                            return(NULL) ;
                       }
/*------------------------------------------ Контроль типа сообщения */ 

/*-------------------------------------------------------------------*/ 

   return((Crowd_Message *)MESSAGES[i]) ;
  
#undef   MESSAGES
#undef   MESSAGES_CNT

}


/********************************************************************/
/********************************************************************/
/**                                                                **/
/**           ОПИСАНИЕ КЛАССА СООБЩЕНИЯ "ШИРОКОВЕЩАНИЕ"            **/
/**                                                                **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*                       Конструктор класса                         */

     Crowd_Message_Broadcast::Crowd_Message_Broadcast(void)

{
   strcpy(Type, "Broadcast") ;
}


/********************************************************************/
/*                                                                  */
/*                       Деструктор класса                          */

    Crowd_Message_Broadcast::~Crowd_Message_Broadcast(void)

{
     vFree() ;
}


/********************************************************************/
/*								    */
/*		      Освободить ресурсы        		    */

    void Crowd_Message_Broadcast::vFree(void)

{
    if(Info!=NULL)  free(Info) ;
}


/********************************************************************/
/*                                                                  */
/*                    Считать данные из строки                      */

    void  Crowd_Message_Broadcast::vReadSave(       char *title,  
                                             std::string *data, 
                                                    char *reference)

{
/*----------------------------------------------- Контроль заголовка */

       if(stricmp(title, "MESSAGE BROADCAST"))  return ;

/*-------------------------------------------------------------------*/

}


/********************************************************************/
/*                                                                  */
/*                  Выдать информацию о сообщении                   */

    void Crowd_Message_Broadcast::vGetInfo(std::string *text)

{
//        *text="Show: Yes\r\n" ;
}


/********************************************************************/
/*                                                                  */
/*                       Работа с параметрами                       */

     int Crowd_Message_Broadcast::vParameter(char *name, char *action, char *value)

{
/*------------------------------------------------- Входной контроль */

    if(name  ==NULL)  return(-1) ;
    if(action==NULL)  return(-2) ;

/*-------------------------------------------------------------------*/

   return(-1) ;
}


/********************************************************************/
/*                                                                  */
/*              Формировать сообщение по специфкации                */

    int  Crowd_Message_Broadcast::vFormBySpec(char *text)

{
   return(0) ;
}

/********************************************************************/
/*								    */
/*                    Проверка наличия связи                        */  

    int Crowd_Message_Broadcast::vCheck(class Crowd_Object *object)

{
     return(1) ;
}
