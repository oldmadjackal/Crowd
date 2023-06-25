
/********************************************************************/
/*                                                                  */
/*            МОДУЛЬ УПРАВЛЕНИЯ ОБЪЕКТОМ "ВНЕШНЯЯ МОДЕЛЬ"           */
/*                                                                  */
/********************************************************************/

#ifdef O_EXTERNAL_EXPORTS
#define O_EXTERNAL_API __declspec(dllexport)
#else
#define O_EXTERNAL_API __declspec(dllimport)
#endif


/*---------------------------------------------- Параметры генерации */

/*------------------ Описание структуры данных для моделей поведения */

/*------------------------- Описание класса объекта "Внешняя модель" */

struct Crowd_Object_ExternalLink {
                                       char  link[FILENAME_MAX] ;
                                     double  time_mark ;
                                 } ;

  class O_EXTERNAL_API Crowd_Object_External : public Crowd_Object {

    public:

#define                       _TARGETS_MAX     512000
#define                 _TARGETS_LINKS_MAX       1000

         static                              char *targets ;                           /* Описание целей */
         static                              long  targets_time ;                      /* Метка времени для описания целей */
         static                               int  targets_init ;                      /* Метка инициализации описания целей */

         static  struct Crowd_Object_ExternalLink *targets_links[_TARGETS_LINKS_MAX] ; /* Список адресов выгрузки описания целей */
         static                               int  targets_links_cnt ;

    public:
                       char  model_path[FILENAME_MAX] ;            /* Файл модели */

                        int  start_flag ;                       /* Признак первого шага моделирования */
                       char  object_type[FILENAME_MAX] ;        /* Тип объекта */
                       char  iface_type[FILENAME_MAX] ;         /* Вид интерфейса: FILE, TCP-SERVER */
                       char  iface_file_folder[FILENAME_MAX] ;  /* Папка для размещения обменных файлов */
                       char  iface_targets[FILENAME_MAX] ;      /* Имя файла целей или URL соединения для передачи списка целей */
                       char  iface_file_control[FILENAME_MAX] ; /* Имя для входного/выходного файлов данных, расширение - in и out */
                       char  iface_tcp_connect[FILENAME_MAX] ;  /* URL соединения */

                       long  memory_t ;
                       char *memory_events ;

    public:
               virtual void  vFree          (void) ;                                    /* Освободить ресурсы */
               virtual void  vWriteSave     (std::string *) ;                           /* Записать данные в строку */
               virtual  int  vEventStart    (void) ;                                    /* Подготовка обработки событий */
               virtual  int  vEvent         (long, char *, void *, Crowd_Kernel *) ;    /* Обработка события */
                                                 
               virtual  int  vEventShow     (void) ;                                    /* Отображение результата обработки события */

                        int  SendObjectsList(long) ;                                    /* Формирование и отправка списка объектов */ 
                        int  SendRequest    (long) ;                                    /* Формирование и отправка запроса на обработку */ 
                        int  WaitResponse   (long) ;                                    /* Ожидaние результата обработки */ 

                        int  TCP_send       (char *, int) ;                             /* Обмен данными с агентом по сетевому протоколу */

	                     Crowd_Object_External() ;           /* Конструктор */
	                    ~Crowd_Object_External() ;           /* Деструктор */
                                                       } ;

/*------------- Описание класса управления объектом "Внешняя модель" */

  class O_EXTERNAL_API Crowd_Module_External : public Crowd_Kernel {

    public:

     static
      struct Crowd_Module_External_instr *mInstrList ;           /* Список команд */
				     
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
                       int  cIType        (char *) ;             /* Инструкция ITYPE */
                       int  cIFile        (char *) ;             /* Инструкция IFILE */
                       int  cITcp         (char *) ;             /* Инструкция ITCP */

     Crowd_Object_External *FindObject    (char *, int) ;        /* Поиск обьекта типа EXTERNAL по имени */

    public:
	                  Crowd_Module_External() ;               /* Конструктор */
	                 ~Crowd_Module_External() ;               /* Деструктор */
                                                       } ;

/*-------------------------------------------- Инструкции управления */

 struct Crowd_Module_External_instr {

           char                         *name_full ;         /* Полное имя команды */
           char                         *name_shrt ;         /* Короткое имя команды */
           char                         *help_row ;          /* HELP - строка */
           char                         *help_full ;         /* HELP - полный */
            int (Crowd_Module_External::*process)(char *) ;  /* Процедура выполнения команды */
                                    }  ;

/*--------------------------------------------- Диалоговые процедуты */

/* Файл  O_External.cpp */
    int  Object_External_Http_receive(char *, int) ;     /* Функция управления приемом для HTTP */

/* Файл  O_External_dialog.cpp */
  INT_PTR CALLBACK  Object_External_Help_dialog   (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_External_Create_dialog (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_External_Iface_dialog  (HWND, UINT, WPARAM, LPARAM) ;
