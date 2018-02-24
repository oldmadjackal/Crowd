
#ifndef  CROWD_OBJECT_H 

#define  CROWD_OBJECT_H 

#include "..\Crowd_Feature\Crowd_Feature.h"
#include "..\Crowd_Communication\Crowd_Communication.h"

#include <string>

#ifdef CROWD_OBJECT_EXPORTS
#define CROWD_OBJECT_API __declspec(dllexport)
#else
#define CROWD_OBJECT_API __declspec(dllimport)
#endif

/*---------------------------------------------------- Коды статусов */

#define   _CROWD_OBJECT_UNKNOWN_ERROR      -999    /* Неизвестная ошибка */

/*------------------------------------- Коды видов целевых положений */

#define   _CROWD_ABSOLUTE_TARGET  0   /* Задается абсолютными координатами */ 

/*----------------------------------- Константы преобразования углов */

#define   _GRD_TO_RAD   0.017453
#define   _RAD_TO_GRD  57.296
#define   _PI           3.1415926

/*---------------------------------------- Описание элемента "Точка" */

   typedef struct {
                     double  x ;
                     double  y ;
                     double  z ;

                     double  azim ;
                     double  elev ;
                     double  roll ;

                        int  mark ;

                  } Crowd_Point ;

/*--------------------------------------- Описание элемента "Вектор" */

   typedef struct {
                     double  x ;
                     double  y ;
                     double  z ;

                        int  mark ;

                  } Crowd_Vector ;

/*------------------------- Описание элемента "Управляемый параметр" */

   typedef struct {
                     char  link[32] ;            /* Мнемо-ссылка на параметр */
                     char  section_name[32] ;    /* Название секции параметров */
                     char  parameter_name[32] ;  /* Название параметра */
                     char  type[8] ;             /* Тип параметра */  
                     void *value ;               /* Устанавливаемое значение */
                  } Crowd_ControlPar ;

/*------------------------------------ Интерфейс передачи контекстов */

  class CROWD_OBJECT_API Crowd_Transit {

    public:
                      char  action[1024] ;
              Crowd_Object *object ;

    public:
             virtual   int  vExecute(void) ;             /* Исполнение действия */
                                             
    public:
                            Crowd_Transit() ;            /* Конструктор */
                           ~Crowd_Transit() ;            /* Деструктор */

                                   } ; 

/*-------------------------------- Описание класса "Список объектов" */

  struct Crowd_Objects_List_Elem {
                               class Crowd_Object *object ;
                                             char  relation[128] ;
                                 }  ;

  class CROWD_OBJECT_API Crowd_Objects_List {

       public:
                Crowd_Objects_List_Elem *List ;
                                    int   List_cnt ;
                                    int   List_max ;

       public:

                        int  Add  (class Crowd_Object *, char *) ;    /* Добавление в список */
                       void  Clear(void) ;                            /* Очистка списка */

			     Crowd_Objects_List() ;                   /* Конструктор */
			    ~Crowd_Objects_List() ;                   /* Деструктор */

                                  } ;
/*----------------------------------------- Описание класса "Объект" */

  class CROWD_OBJECT_API Crowd_Object {

       public:
                       char  Name[128] ;           /* Имя обьекта */
                       char  Type[128] ;           /* Тип обьекта */
                       char  Decl[1024] ;          /* Описание обьекта */
                       char *Program ;             /* Программа поведения */

                     double  x_base ;              /* Координаты базовой точки */
                     double  y_base ;
                     double  z_base ;
                     double  x_base_stack ;
                     double  y_base_stack ;
                     double  z_base_stack ;

     struct Crowd_Parameter  *Parameters ;         /* Список параметров */
                        int   Parameters_cnt ;

              Crowd_Feature **Features ;           /* Список свойств */
                        int   Features_cnt ;

        Crowd_Communication **Communications ;     /* Список свойств */
                        int   Communications_cnt ;

   class Crowd_Objects_List   Units ;              /* Список объектов-составных частей */

              Crowd_Transit  *Context ;            /* Интерфейс передачи контекстов */
        class  Crowd_Kernel  *Module ;             /* Программный модуль объекта */

                        int   ErrorEnable ;        /* Флаг выдачи сообщений об ошибках */

       public:

   virtual class Crowd_Object *vCopy           (char *) ;                /* Копировать объект */
   virtual               void  vPush           (void)  ;                 /* Сохранить состояние объекта */
   virtual               void  vPop            (void)  ;                 /* Восстановить состояние объекта */

   virtual               void  vErrorMessage   (int) ;                   /* Вкл./Выкл. сообщений об ошибках */

   virtual               void  vReadSave       (std::string *) ;         /* Считать данные из строки */
   virtual               void  vWriteSave      (std::string *) ;         /* Записать данные в строку */
   virtual               void  vFree           (void) ;                  /* Освободить ресурсы */

   virtual               void  vFormDecl       (void) ;                  /* Формировать описание */

   virtual                int  vListControlPars(Crowd_ControlPar *) ;    /* Получить список параметров управления */
   virtual                int  vSetControlPar  (Crowd_ControlPar *) ;    /* Установить значение параметра управления */

   virtual                int  vGetPosition    (Crowd_Point *) ;         /* Работа с положением объекта */
   virtual               void  vSetPosition    (Crowd_Point *) ;

   virtual                int  vSpecial        (char *, void *) ;        /* Специальные действия */

   virtual                int  vEventStart     (void) ;                  /* Подготовка обработки событий */
   virtual                int  vEvent          (long, char *,            /* Обработка события */
                                                      void *, Crowd_Kernel *) ;
   virtual                int  vEventShow      (void) ;                  /* Отображение результата обработки события */

   virtual                int  vResetFeatures  (void *) ;                /* Сброс контекста проверки свойств */
   virtual                int  vPrepareFeatures(void *) ;                /* Подготовить свойства к проверке корректности */
   virtual                int  vCheckFeatures  (void *,                  /* Проверить корректность свойств */
                                                Crowd_Objects_List *) ;
   virtual               void  vSetFeature     (Crowd_Feature *) ;       /* Работа со свойствами */
   virtual               void  vGetFeature     (Crowd_Feature *) ;

                          int  iAngleInCheck   (double,                  /* Проверка попадания угла в диапазон */
                                                double, double) ;
                          int  iReplaceText    (char *, char *,          /* Замена текстового фрагмента в строке */
                                                        char *, int) ;

			     Crowd_Object      () ;                   /* Конструктор */
			    ~Crowd_Object      () ;                   /* Деструктор */

                                  } ;
/*-------------------------------------------------------------------*/

#endif  // CROWD_OBJECT_H
