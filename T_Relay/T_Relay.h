
/********************************************************************/
/*								    */
/*		МОДУЛЬ ЗАДАЧИ "МОДЕЛИРОВАНИЕ СЕТИ СООБЩЕНИЙ"        */
/*								    */
/********************************************************************/

#ifdef T_RELAY_EXPORTS
#define T_RELAY_API __declspec(dllexport)
#else
#define T_RELAY_API __declspec(dllimport)
#endif


/*---------------------------------------------- Параметры генерации */


/*------------------------------------ Описание очередей и сообщений */

     struct  Crowd_MQueue {
                       class Crowd_Message **messages ;
                                       int   cnt ;
                                       int   max ;
                          } ;

#define  _QUEUE_DEEP   10000

/*----------------------- Описание класса задачи "Схема отображения" */

  class T_RELAY_API Crowd_Module_Relay : public Crowd_Kernel {

    public:

     static
        struct Crowd_Module_Relay_instr *mInstrList ;                   /* Список команд */

     static               Crowd_MQueue  mQueue[_QUEUE_DEEP] ;           /* Очередь сообщений */

     static                       HWND  mQueueDlg ;                     /* Окно отображения состояния очереди */
     static                       HWND  mDebugDlg ;                     /* Окно отладки */

     static                        int  mStateRegime ;                  /* Способ сохранения состояния объектов */
#define                                   _MEMORY_STATE   0              /* Сохранение в памяти */
#define                                     _FILE_STATE   1              /* Сохранение в файлах */
#define                                      _LOG_STATE   2              /* Сохранение в памяти и в файлах - только на просмотр */
     static                       char  mStateFolder[FILENAME_MAX] ;    /* Папка файлов состояний объектов */

     static                       long  mStep ;                         /* Номер шага моделирования */

    public:
     virtual         int  vGetParameter (char *, char *)   ;            /* Получить параметр */
     virtual         int  vExecuteCmd   (const char *) ;                /* Выполнить команду */
     virtual         int  vExecuteCmd   (const char *, Crowd_IFace *) ;
     virtual         int  vSpecial      (char *, void *, char *) ;      /* Специальный интерфейс */
     virtual         int  vAddMessage   (Crowd_Message *, int) ;        /* Регистрация сообщения в очереди */

    public:
                     int  cHelp         (char *, Crowd_IFace *) ;       /* Инструкция HELP */
                     int  cState        (char *, Crowd_IFace *) ;       /* Инструкция STATE */
                     int  cPrepare      (char *, Crowd_IFace *) ;       /* Инструкция PREPARE */
                     int  cQueue        (char *, Crowd_IFace *) ;       /* Инструкция QUEUE */
                     int  cDebug        (char *, Crowd_IFace *) ;       /* Инструкция DEBUG */
                     int  cRun          (char *, Crowd_IFace *) ;       /* Инструкция RUN */

            Crowd_Object *FindObject    (char *) ;                      /* Поиск объекта по имени */ 
           Crowd_Message *FindMessage   (char *) ;                      /* Поиск сообщения по имени */ 

    public:
	                  Crowd_Module_Relay() ;               /* Конструктор */
	                 ~Crowd_Module_Relay() ;               /* Деструктор */
                                                             } ;

/*-------------------------------------------- Инструкции управления */

 struct Crowd_Module_Relay_instr {

           char                      *name_full ;                        /* Полное имя команды */
           char                      *name_shrt ;                        /* Короткое имя команды */
           char                      *help_row ;                         /* HELP - строка */
           char                      *help_full ;                        /* HELP - полный */
            int (Crowd_Module_Relay::*process)(char *, Crowd_IFace *) ;  /* Процедура выполнения команды */
                              }  ;
/*--------------------------------------------- Диалоговые процедуты */

/* Файл  T_Relay.cpp */

/* Файл  T_Relay_dialog.cpp */
    BOOL CALLBACK  Task_Relay_Help_dialog (HWND, UINT, WPARAM, LPARAM) ;
    BOOL CALLBACK  Task_Relay_Queue_dialog(HWND, UINT, WPARAM, LPARAM) ;
    BOOL CALLBACK  Task_Relay_Debug_dialog(HWND, UINT, WPARAM, LPARAM) ;
