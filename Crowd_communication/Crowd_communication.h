
#ifndef  CROWD_COMMUNICATION_H 

#define  CROWD_COMMUNICATION_H 

#include <string>

#ifdef CROWD_COMMUNICATION_EXPORTS
#define CROWD_COMMUNICATION_API __declspec(dllexport)
#else
#define CROWD_COMMUNICATION_API __declspec(dllimport)
#endif

/*---------------------------------- Описание класса "Связь объекта" */

    class CROWD_COMMUNICATION_API Crowd_Communication {

    public:
                               char  Type[128] ;          /* Тип связи */
                               char  Kind[64] ;           /* Вид связи */
                 class Crowd_Object *Object_m ;           /* Обьект, основной */ 
                 class Crowd_Object *Object_s ;           /* Обьект, связанный */ 

                                int  Visible ;            /* Видимость */
                           COLORREF  Color ;              /* Цвет */

    public:
               virtual void  vReadSave     (char *, std::string *,    /* Считать данные из строки */
                                                           char * ) ;
               virtual void  vWriteSave    (std::string *) ;          /* Записать данные в строку */  
               virtual void  vFree         (void) ;                   /* Освободить ресурсы */  
               virtual void  vGetInfo      (std::string *) ;          /* Информация о свойстве */  

               virtual  int  vCheck        (void *,                   /* Проверка непротиворечивости свойства */  
                                            class Crowd_Objects_List *) ;

			     Crowd_Communication() ;                  /* Конструктор */
			    ~Crowd_Communication() ;                  /* Деструктор */
                                      } ;

/*-------------------------------------------------------------------*/

#endif        // CROWD_FEATURE_H 
