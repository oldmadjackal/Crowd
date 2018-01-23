
#ifndef  CROWD_FEATURE_H 

#define  CROWD_FEATURE_H 

#include <string>

#ifdef CROWD_FEATURE_EXPORTS
#define CROWD_FEATURE_API __declspec(dllexport)
#else
#define CROWD_FEATURE_API __declspec(dllimport)
#endif

/*------------------------------- Описание класса "Свойство объекта" */

    class CROWD_FEATURE_API Crowd_Feature {

    public:
                         char  Type[128] ;      /* Тип свойства */
           class Crowd_Object *Object ;         /* Обьект принадлежности */ 

    public:
               virtual void  vReadSave     (char *, std::string *,    /* Считать данные из строки */
                                                           char * ) ;
               virtual void  vWriteSave    (std::string *) ;          /* Записать данные в строку */  
               virtual void  vFree         (void) ;                   /* Освободить ресурсы */  
               virtual void  vGetInfo      (std::string *) ;          /* Информация о свойстве */  

               virtual  int  vParameter    (char *, char *, char *) ; /* Работа с параметрами */  

               virtual  int  vResetCheck   (void *) ;                 /* Сброс контекста проверки непротиворечивости свойства */  
               virtual  int  vPreCheck     (void *) ;                 /* Подготовка к проверке непротиворечивости свойства */  
               virtual  int  vCheck        (void *,                   /* Проверка непротиворечивости свойства */  
                                            class Crowd_Objects_List *) ;
               virtual void  vBodyAdd      (char *) ;                 /* Добавить тело */
               virtual void  vBodyDelete   (char *) ;                 /* Удалить тело */
               virtual void  vBodyBasePoint(char *,                   /* Задание базовой точки тела */
                                             double, double, double) ;
               virtual void  vBodyAngles   (char *,                   /* Задание ориентации тела */
                                             double, double, double) ;
               virtual void  vBodyMatrix   (char *, double[4][4]) ;   /* Задание матрицы положения тела */
               virtual void  vBodyShifts   (char *,                   /* Задание смещения положения и  */
                                             double, double, double,  /*    ориентации тела            */
                                             double, double, double ) ;
               virtual void  vBodyPars     (char *,                   /* Задание списка параметров */
                                             struct Crowd_Parameter *) ;

			     Crowd_Feature() ;                       /* Конструктор */
			    ~Crowd_Feature() ;                       /* Деструктор */
                                      } ;

/*-------------------------------------------------------------------*/

#endif        // CROWD_FEATURE_H 
