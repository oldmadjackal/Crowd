/********************************************************************/
/*                                                                  */
/*        МОДУЛЬ УПРАВЛЕНИЯ СООБЩЕНИЕМ "ШИРОКОВЕЩАНИЕ"              */
/*                                                                  */
/********************************************************************/

#ifdef M_BROADCAST_EXPORTS
#define M_BROADCAST_API __declspec(dllexport)
#else
#define M_BROADCAST_API __declspec(dllimport)
#endif

/*---------------------------------------------- Параметры генерации */

/*------------ Описание класса управления сообщением "Широковещание" */

  class M_BROADCAST_API Crowd_Module_Broadcast : public Crowd_Kernel {

    public:

     static
             struct Crowd_Module_Broadcast_instr *mInstrList ;               /* Список команд */


    public:
     virtual                 int  vGetParameter       (char *, char *) ;         /* Получить параметр */
     virtual                void  vStart              (void) ;                   /* Стартовая разводка */
     virtual       Crowd_Message *vCreateMessage      (Crowd_Object *,           /* Создать сообщение */
                                                       Crowd_Object *, Crowd_Message *) ;
     virtual                 int  vExecuteCmd         (const char *) ;           /* Выполнить команду */
     virtual                void  vWriteSave          (std::string *) ;          /* Записать данные в строку */

    public:
                             int  cHelp               (char *) ;                 /* Инструкция Help */ 
                             int  cCreate             (char *) ;                 /* Инструкция Create */

                    Crowd_Object *FindObject          (char *) ;                 /* Поиск объекта по имени */ 
                   Crowd_Message *FindMessage         (char *) ;                 /* Поиск сообщения по имени */ 

                                  Crowd_Module_Broadcast() ;               /* Конструктор */
                                 ~Crowd_Module_Broadcast() ;               /* Деструктор */
                                                       } ;

/*-------------------------------------------- Инструкции управления */

 struct Crowd_Module_Broadcast_instr {

           char                          *name_full ;           /* Полное имя команды */
           char                          *name_shrt ;           /* Короткое имя команды */
           char                          *help_row ;            /* HELP - строка */
           char                          *help_full ;           /* HELP - полный */
            int (Crowd_Module_Broadcast::*process)(char *) ;  /* Процедура выполнения команды */
                                     }  ;

/*----------------------- Описание класса сообщением "Широковещание" */

  class M_BROADCAST_API Crowd_Message_Broadcast : public Crowd_Message {

//    public:       
//                           char  Kind[64] ;                  /* Вид связи */

    public:
            virtual void  vReadSave     (char *, std::string *,        /* Считать данные из строки */
                                                        char * ) ;
            virtual void  vFree         (void) ;                       /* Освободить ресурсы */  
            virtual void  vGetInfo      (std::string *) ;              /* Выдать информацию о свойстве */
            virtual  int  vParameter    (char *, char *, char *) ;     /* Работа с параметрами */  
            virtual  int  vFormBySpec   (char *) ;                     /* Формировать сообщение по спецификации */
            virtual  int  vCheck        (class Crowd_Object *) ;       /* Проверка наличия связи */ 

	                  Crowd_Message_Broadcast() ;                 /* Конструктор */
	                 ~Crowd_Message_Broadcast() ;                 /* Деструктор */
                                                         } ;

/*--------------------------------------------- Диалоговые процедуры */

/* Файл  M_Broadcast.cpp */

/* Файл  M_Broadcast_dialog.cpp */
  INT_PTR CALLBACK  Message_Broadcast_Help_dialog(HWND, UINT, WPARAM, LPARAM) ;
