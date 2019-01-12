/********************************************************************/
/*								    */
/*		МОДУЛЬ УПРАВЛЕНИЯ СВЯЗЬЮ "ПРЯМАЯ ПАРНАЯ"            */
/*								    */
/********************************************************************/

#ifdef L_PAIR_EXPORTS
#define L_PAIR_API __declspec(dllexport)
#else
#define L_PAIR_API __declspec(dllimport)
#endif

/*---------------------------------------------- Параметры генерации */

/*-------------------------------------------------------- Вид связи */

 struct Crowd_Communication_Pair_Kind {

                      char  name[64] ;                  /* Название вида */
                  COLORREF  color ;                     /* Цвет */

                                      }  ;

#define   _PAIR_KIND_MAX  50

/*---------------- Описание класса управления связью "Прямая парная" */

  class L_PAIR_API Crowd_Module_Pair : public Crowd_Kernel {

    public:

     static
             struct Crowd_Module_Pair_instr *mInstrList ;               /* Список команд */

     static
       struct Crowd_Communication_Pair_Kind  mKinds[_PAIR_KIND_MAX] ;   /* Список видов связи */
     static                             int  mKinds_cnt ;

                                        int  mShow_dlist ;

    public:
     virtual                 int  vGetParameter       (char *, char *) ;         /* Получить параметр */
     virtual                void  vStart              (void) ;                   /* Стартовая разводка */
     virtual Crowd_Communication *vCreateCommunication(Crowd_Object *,           /* Создать связь */
                                                       Crowd_Object *,
                                                       Crowd_Communication *) ;
     virtual                void  vShow               (char *) ;                 /* Отобразить связанные данные */
     virtual                 int  vExecuteCmd         (const char *) ;           /* Выполнить команду */
     virtual                void  vWriteSave          (std::string *) ;          /* Записать данные в строку */

    public:
                             int  cHelp               (char *) ;                 /* Инструкция Help */ 
                             int  cKind               (char *) ;                 /* Инструкция Kind */
                             int  cLink               (char *) ;                 /* Инструкция Link */
                             int  cColor              (char *) ;                 /* Инструкция Color */

                    Crowd_Object *FindObject          (char *) ;                 /* Поиск объекта по имени */ 
                             int  iShowScene          (void) ;                   /* Отображение сцены */

                                  Crowd_Module_Pair() ;               /* Конструктор */
	                         ~Crowd_Module_Pair() ;               /* Деструктор */
                                                       } ;

/*-------------------------------------------- Инструкции управления */

 struct Crowd_Module_Pair_instr {

           char                   *name_full ;           /* Полное имя команды */
           char                   *name_shrt ;           /* Короткое имя команды */
           char                   *help_row ;            /* HELP - строка */
           char                   *help_full ;           /* HELP - полный */
            int (Crowd_Module_Pair::*process)(char *) ;  /* Процедура выполнения команды */
                                }  ;

/*---------------------------- Описание класса связи "Прямая парная" */

  class L_PAIR_API Crowd_Communication_Pair : public Crowd_Communication {

//    public:       
//                           char  Kind[64] ;                  /* Вид связи */

    public:
            virtual void  vReadSave     (char *, std::string *,        /* Считать данные из строки */
                                                        char * ) ;
            virtual void  vGetInfo      (std::string *) ;              /* Выдать информацию о свойстве */
            virtual  int  vParameter    (char *, char *, char *) ;     /* Работа с параметрами */  

                    void  Show          (void) ;                       /* Отобразить объект */

	                  Crowd_Communication_Pair() ;                 /* Конструктор */
	                 ~Crowd_Communication_Pair() ;                 /* Деструктор */
                                                         } ;

/*--------------------------------------------- Диалоговые процедуры */

/* Файл  F_Pair.cpp */

/* Файл  F_Pair_dialog.cpp */
  INT_PTR CALLBACK  Communication_Pair_Help_dialog(HWND, UINT, WPARAM, LPARAM) ;
