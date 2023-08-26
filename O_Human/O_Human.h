
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
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - Модель DOG */
   struct Dog {
                   double  energy ;
                   double  weariness ;
                   double  endurance ;
                 COLORREF *color ;
              } ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - -  Модель SPECTR */
#define    _SIGNAL_MAX   9
#define    _MEMORY_MAX  10

   struct SpectrMemory {
                          Crowd_Object *sender ; 
                                double  signal[_SIGNAL_MAX] ;
                                double  forget[_SIGNAL_MAX] ;
                                  long  distrust ;
                       } ;

   struct Spectr {
                     double  signal_sensitivity[_SIGNAL_MAX] ;
                     double  forget_period ;
                     double  penalty ;
                     double  shock  ;
                     double  distrust_period ;

        struct SpectrMemory  signal_memory[_MEMORY_MAX] ;
                   COLORREF *color ;
                       long  memory_t ;
                 } ;

/*-------------------------------- Описание класса объекта "Человек" */

  class O_HUMAN_API Crowd_Object_Human : public Crowd_Object {

    public:
                      char  model_path[FILENAME_MAX] ;            /* Файл модели */

                      char  behavior_model[128] ;                 /* Название встроенной модели поведения */
                      void *behavior_data ;                       /* Данные встроенной модели поведения */

    public:
               virtual void  vFree          (void) ;                                    /* Освободить ресурсы */
               virtual void  vWriteSave     (std::string *) ;                           /* Записать данные в строку */
               virtual  int  vEventStart    (void) ;                                    /* Подготовка обработки событий */
               virtual  int  vEvent         (long, char *, void *, Crowd_Kernel *) ;    /* Обработка события */
                                                 
               virtual  int  vEventShow     (void) ;                                    /* Отображение результата обработки события */

               virtual void  vCallBack      (Crowd_Object *, char *, char *) ;          /* Канал обратной связи */

                        int  iBehaviorDog   (long, char *, void *, Crowd_Kernel *) ;    /* Модель поведения - DOG */
                        int  iBehaviorSpectr(long, char *, void *, Crowd_Kernel *) ;    /* Модель поведения - SPECTR */

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
                       int  cBehavior     (char *) ;             /* Инструкция BEHAVIOR */
                       int  cProfile      (char *) ;             /* Инструкция PROFILE */

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
  INT_PTR CALLBACK  Object_Human_Help_dialog   (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Human_Create_dialog (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Human_Profile_dialog(HWND, UINT, WPARAM, LPARAM) ;
