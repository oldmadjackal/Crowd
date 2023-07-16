
/********************************************************************/
/*                                                                  */
/*          МОДУЛЬ УПРАВЛЕНИЯ ОБЪЕКТОМ "ОПЕРАТОР ОПРОСОВ"           */
/*                                                                  */
/********************************************************************/

#ifdef O_SURVEY_EXPORTS
#define O_SURVEY_API __declspec(dllexport)
#else
#define O_SURVEY_API __declspec(dllimport)
#endif


/*---------------------------------------------- Параметры генерации */

/*------------------ Описание структуры данных для моделей поведения */

   struct Profile {
                        char  *name ;        /* Название параметра */
                        char  *title ;       /* Диалоговый заголовок параметра */
                        char  *describe ;    /* Детальное описание параметра */
                        char  *type ;        /* Тип: "Digital", "String", "DigitalArray" */
                      double   value_min ;   /* Диапазон допустимых значений для Digital */
                      double   value_max ;
                        char **value_list ;  /* Список допустимых значений для String */
                        void  *value ;       /* Указатель на данные */
                  } ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - Модель DEFAULT */
   struct Default {
                     COLORREF *color ;
                  } ;
/*----------------------- Описание класса объекта "Оператор опросов" */

  class O_SURVEY_API Crowd_Object_Survey : public Crowd_Object {

    public:
                      char  model_path[FILENAME_MAX] ;            /* Файл модели */

                      char  behavior_model[128] ;                 /* Название встроенной модели поведения */
                      void *behavior_data ;                       /* Данные встроенной модели поведения */

    public:
               virtual void  vFree           (void) ;                                    /* Освободить ресурсы */
               virtual void  vWriteSave      (std::string *) ;                           /* Записать данные в строку */
               virtual  int  vEventStart     (void) ;                                    /* Подготовка обработки событий */
               virtual  int  vEvent          (long, char *, void *, Crowd_Kernel *) ;    /* Обработка события */
                                                 
               virtual  int  vEventShow      (void) ;                                    /* Отображение результата обработки события */

                        int  iBehaviorDefault(long, char *, void *, Crowd_Kernel *) ;    /* Модель поведения - DEFAULT */

	                     Crowd_Object_Survey() ;           /* Конструктор */
	                    ~Crowd_Object_Survey() ;           /* Деструктор */
                                                       } ;

/*----------- Описание класса управления объектом "Оператор опросов" */

  class O_SURVEY_API Crowd_Module_Survey : public Crowd_Kernel {

    public:

     static
      struct Crowd_Module_Survey_instr *mInstrList ;              /* Список команд */
				     
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
                       int  cBehavior     (char *) ;             /* Инструкция BEHAVIOR */
                       int  cProfile      (char *) ;             /* Инструкция PROFILE */

       Crowd_Object_Survey *FindObject    (char *) ;             /* Поиск обьекта типа SURVEY по имени */

    public:
	                  Crowd_Module_Survey() ;               /* Конструктор */
	                 ~Crowd_Module_Survey() ;               /* Деструктор */
                                                       } ;

/*-------------------------------------------- Инструкции управления */

 struct Crowd_Module_Survey_instr {

           char                       *name_full ;         /* Полное имя команды */
           char                       *name_shrt ;         /* Короткое имя команды */
           char                       *help_row ;          /* HELP - строка */
           char                       *help_full ;         /* HELP - полный */
            int (Crowd_Module_Survey::*process)(char *) ;  /* Процедура выполнения команды */
                                  }  ;

/*--------------------------------------------- Диалоговые процедуты */

/* Файл  O_Survey.cpp */

/* Файл  O_Survey_dialog.cpp */
  INT_PTR CALLBACK  Object_Survey_Help_dialog   (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Survey_Create_dialog (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Survey_Profile_dialog(HWND, UINT, WPARAM, LPARAM) ;
