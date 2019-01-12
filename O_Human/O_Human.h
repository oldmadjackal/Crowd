
/********************************************************************/
/*								    */
/*		МОДУЛЬ УПРАВЛЕНИЯ ОБЪЕКТОМ "ЧЕЛОВЕК"  		    */
/*								    */
/********************************************************************/

#ifdef O_HUMAN_EXPORTS
#define O_HUMAN_API __declspec(dllexport)
#else
#define O_HUMAN_API __declspec(dllimport)
#endif


/*---------------------------------------------- Параметры генерации */

/*----------------------------------- Описание класса объекта "Тело" */

  class O_HUMAN_API Crowd_Object_Human : public Crowd_Object {

    public:
                      char  model_path[FILENAME_MAX] ;            /* Файл модели */

    public:
               virtual void  vFree      (void) ;                  /* Освободить ресурсы */
               virtual void  vWriteSave (std::string *) ;         /* Записать данные в строку */
               virtual  int  vEventStart(void) ;                  /* Подготовка обработки событий */
               virtual  int  vEvent     (long, char *,            /* Обработка события */
                                               void *, Crowd_Kernel *) ;  
               virtual  int  vEventShow (void) ;                  /* Отображение результата обработки события */

	                     Crowd_Object_Human() ;           /* Конструктор */
	                    ~Crowd_Object_Human() ;           /* Деструктор */
                                                       } ;

/*-------------------- Описание класса управления объектом "Человек" */

  class O_HUMAN_API Crowd_Module_Human : public Crowd_Kernel {

    public:

     static
      struct Crowd_Module_Human_instr *mInstrList ;              /* Список команд */
				     
    public:
     virtual  Crowd_Object *vCreateObject (Crowd_Model_data *) ; /* Создание объекта */ 
     virtual           int  vGetParameter (char *, char *)   ;   /* Получить параметр */
     virtual           int  vExecuteCmd   (const char *) ;       /* Выполнить команду */
     virtual          void  vReadSave     (std::string *) ;      /* Чтение данных из строки */
     virtual          void  vWriteSave    (std::string *) ;      /* Записать данные в строку */

    public:
                       int  cHelp         (char *) ;             /* Инструкция HELP */ 
                       int  cCreate       (char *) ;             /* Инструкция CREATE */ 
                       int  cInfo         (char *) ;             /* Инструкция INFO */ 
                       int  cBase         (char *) ;             /* Инструкция BASE */ 
                       int  cColor        (char *) ;             /* Инструкция COLOR */
                       int  cVisible      (char *) ;             /* Инструкция VISIBLE */
                       int  cProgram      (char *) ;             /* Инструкция PROGRAM */

        Crowd_Object_Human *FindObject    (char *) ;             /* Поиск обьекта типа HUMAN по имени */

    public:
	                  Crowd_Module_Human() ;               /* Конструктор */
	                 ~Crowd_Module_Human() ;               /* Деструктор */
                                                       } ;

/*-------------------------------------------- Инструкции управления */

 struct Crowd_Module_Human_instr {

           char                      *name_full ;         /* Полное имя команды */
           char                      *name_shrt ;         /* Короткое имя команды */
           char                      *help_row ;          /* HELP - строка */
           char                      *help_full ;         /* HELP - полный */
            int (Crowd_Module_Human::*process)(char *) ;  /* Процедура выполнения команды */
                                 }  ;

/*--------------------------------------------- Диалоговые процедуты */

/* Файл  O_Human.cpp */

/* Файл  O_Human_dialog.cpp */
  INT_PTR CALLBACK  Object_Human_Help_dialog  (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Human_Create_dialog(HWND, UINT, WPARAM, LPARAM) ;
