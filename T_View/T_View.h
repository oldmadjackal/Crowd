
/********************************************************************/
/*								    */
/*		МОДУЛЬ ЗАДАЧИ "СХЕМА ОТОБРАЖЕНИЯ"    		    */
/*								    */
/********************************************************************/

#ifdef T_VIEW_EXPORTS
#define T_VIEW_API __declspec(dllexport)
#else
#define T_VIEW_API __declspec(dllimport)
#endif


/*---------------------------------------------- Параметры генерации */


/*----------------------- Описание класса задачи "Схема отображения" */

  class T_VIEW_API Crowd_Module_View : public Crowd_Kernel {

    public:

     static
        struct Crowd_Module_View_instr *mInstrList ;                /* Список команд */

    public:
     virtual         int  vGetParameter (char *, char *)   ;        /* Получить параметр */
     virtual         int  vExecuteCmd   (const char *) ;            /* Выполнить команду */
     virtual         int  vExecuteCmd   (const char *, Crowd_IFace *) ;

    public:
                     int  cHelp         (char *, Crowd_IFace *) ;   /* Инструкция HELP */
                     int  cRing         (char *, Crowd_IFace *) ;   /* Инструкция CLEAR */

    public:
	                  Crowd_Module_View() ;               /* Конструктор */
	                 ~Crowd_Module_View() ;               /* Деструктор */
                                                             } ;

/*-------------------------------------------- Инструкции управления */

 struct Crowd_Module_View_instr {

           char                     *name_full ;                        /* Полное имя команды */
           char                     *name_shrt ;                        /* Короткое имя команды */
           char                     *help_row ;                         /* HELP - строка */
           char                     *help_full ;                        /* HELP - полный */
            int (Crowd_Module_View::*process)(char *, Crowd_IFace *) ;  /* Процедура выполнения команды */
                              }  ;
/*--------------------------------------------- Диалоговые процедуты */

/* Файл  T_View.cpp */

/* Файл  T_View_dialog.cpp */
    BOOL CALLBACK  Task_View_Help_dialog(HWND, UINT, WPARAM, LPARAM) ;
