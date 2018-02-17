
#ifndef  CROWD_MESSAGE_H 

#define  CROWD_MESSAGE_H 

#include <string>

#ifdef CROWD_MESSAGE_EXPORTS
#define CROWD_MESSAGE_API __declspec(dllexport)
#else
#define CROWD_MESSAGE_API __declspec(dllimport)
#endif

/*-------------------------------------- Описание класса "Сообщение" */

    class CROWD_MESSAGE_API Crowd_Message {

    public:
                               char  Name[128] ;          /* Имя сообщения */
                               char  Type[128] ;          /* Тип сообщения */
                               char  Kind[128] ;          /* Вид сообщения */
                 class Crowd_Object *Object_s ;           /* Объект-отправитель */ 
                 class Crowd_Object *Object_r ;           /* Объект-получатель */ 

    public:

               virtual void  vReadSave     (char *, std::string *,    /* Считать данные из строки */
                                                           char * ) ;
               virtual void  vWriteSave    (std::string *) ;          /* Записать данные в строку */  
               virtual void  vFree         (void) ;                   /* Освободить ресурсы */  
               virtual void  vGetInfo      (std::string *) ;          /* Информация о сообщении */  
               virtual void  vFormDecl     (char *) ;                 /* Формировать описание */

               virtual  int  vCheck        (class Crowd_Object *) ;   /* Проверка объекта-получателя */

			     Crowd_Message() ;                        /* Конструктор */
			    ~Crowd_Message() ;                        /* Деструктор */
                                      } ;

/*-------------------------------------------------------------------*/

#endif        // CROWD_MESSAGE_H 
