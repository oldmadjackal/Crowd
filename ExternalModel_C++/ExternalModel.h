/*********************************************************************/
/*                                                                   */
/*                    Утилита внешнего моделирования                 */
/*                                                                   */
/*********************************************************************/

#ifdef  __MAIN__
#define  _EXTERNAL
#else
#define  _EXTERNAL  extern
#endif

/*---------------------------------------------- Параметры генерации */

#define   _APPLICATION     "ExternalModel"
#define   _PROGRAM_TITLE   "ExternalModel"

#undef    _VERSION
#define   _VERSION   "10.06.2023"

/*--------------------------------------------------- Система команд */

  _EXTERNAL   int  __exit_flag ;             /* Флаг завершения работы */

/*---------------------------------- Коды пользовательских сообщений */

#define    _USER_SECTION_ENABLE    1
#define    _USER_RELOAD            2

/*-------------------------------------------------- Диалоговые окна */

  _EXTERNAL            UINT  TaskBar_Msg ;        /* Сообщение активизации иконы в TaskBar */
  _EXTERNAL  NOTIFYICONDATA  TbIcon ;             /* Описание TaskBar-иконки */

  _EXTERNAL       HINSTANCE  hInst ;
  _EXTERNAL        WNDCLASS  FrameWindow ;
  _EXTERNAL            HWND  hFrameWindow ;

  _EXTERNAL            HWND  __dialog ;

  _EXTERNAL             int  __window_closed ;    /* Флаг скрытия окна в трей */

  _EXTERNAL             int  __console_process ;  /* Флаг работы в консольном режиме */

#define                    _NO_ICON              0
#define               _WARNING_ICON              1
#define                 _ERROR_ICON              2

  _EXTERNAL  char  __title[FILENAME_MAX] ;        /* Заголовок окна */

/*------------------------------------------------- Общие переменные */

  _EXTERNAL            char  __cwd[FILENAME_MAX] ;              /* Рабочий раздел */

  _EXTERNAL            char  __log_path[FILENAME_MAX] ;         /* Путь файла лога */

/*-------------------------------------------------- Описание секций */

  typedef struct {
                    HWND  hWnd ;           /* Дескриптор окна */
                    char  title[128] ;     /* Заголовок */           
                 } Section ;

  _EXTERNAL      Section  __sections[10] ;       /* Список секций */
  _EXTERNAL          int  __sections_cnt ;

  _EXTERNAL          int  __sec_work ;           /* Индекс рабочий секции */

  _EXTERNAL       time_t  __sec_change_time ;    /* Время последнего переключения секций */

/*---------------------------------------- Общий управляющий процесс */

  _EXTERNAL  HANDLE  hFilesIface_Tread ;                   /* Процесс для файлового интерфейса */
  _EXTERNAL   DWORD  hFilesIface_PID ;

  _EXTERNAL    HWND  hConsoleDialog ;

  _EXTERNAL    char  __control_folder[FILENAME_MAX] ;      /* Папка управляющих файлов */
  _EXTERNAL    char  __control_object[FILENAME_MAX] ;      /* Отслеживаемый объект - по умолчанию или * - все */
  _EXTERNAL    char  __targets_path[FILENAME_MAX] ;        /* Файл списка целей */

/*------------------------------------------ Описание объектов сцены */

  typedef struct {                                   /* Параметры событию */
                      char  name[128] ;               /* Имя */
                      char  category[128] ;           /* Категория: message */
                      char  type[128] ;               /* Тип */
                      char  kind[128] ;               /* Вид */
                      char  sender[128] ;             /* Отправитель */
                      char  receiver[128] ;           /* Получатель */
                      char  info[1024] ;              /* Информация */

                 } Event ;
  
  typedef struct {                                   /* Параметры объекта */
                      char  name[128] ;               /* Название */
                      char  type[128] ;               /* Тип */
                      long  t  ;                      /* Шаг моделирования */
                    double  x, y, z ;                 /* Координаты объекта */
                     Event *events ;                  /* События */
                       int  events_cnt ;

                 } Object ;

#define        _TARGETS_MAX   100000

  _EXTERNAL  Object *__targets[_TARGETS_MAX] ;        /* Список агентов сцены */
  _EXTERNAL     int  __targets_cnt ;
  _EXTERNAL    long  __targets_time ;

/*------------------------------------------ Параметры моделирования */

                                             /* Модель AGENT */ 
  _EXTERNAL  double  __step ;                 /* Размерность шага */

/*-------------------------------------------------------- Прототипы */

/* ExternalModel.cpp */
     int  EM_system         (void) ;                         /* Обработка системных сообщений */
    void  EM_message        (char *) ;                       /* Система выдачи сообщений */
    void  EM_message        (char *, int) ;
     int  EM_log            (char *) ;                       /* Ведение лога */
     int  EM_read_config    (char *) ;                       /* Считывание файла конфигурации */
    void  EM_Change_Section (char *, int) ;                  /* Выбор секции */
    void  EM_text_trim      (char *) ;                       /* Отсечка начальных и конечных пробельных символов */
     int  EM_create_path    (char *) ;                       /* Формирование пути к разделу */
     int  EM_text_subst     (char *, char *, char *, int) ;  /* Подстановка полей данных */
     int  EM_json_subst     (char *, char *, char *) ;       /* Подстановка поля данных по ключу */
     int  EM_read_targets   (char *, int) ;                  /* Считывание файла целей */
     int  EM_read_request   (char *, Object *, int) ;        /* Считывание файла запроса */
     int  EM_read_request2  (char *, Object *, int) ;        /* Считывание файла запроса, сокращенная форма */
     int  EM_write_response (Object *, char *, char *) ;     /* Записать файл ответа */
     int  EM_process_model  (Object *, char *, char *) ;     /* Расчет модели */

     int  EM_cmd_sendmessage(char *, Object *,               /* Добавление команды "Отправить сообщение" */
                             char *, char *, char *, char *, int) ;

    DWORD WINAPI  FilesIface_Tread   (LPVOID) ;
    DWORD WINAPI  Model_Process_Tread(LPVOID) ;

/* EM_console.cpp */
INT_PTR CALLBACK  EM_console_dialog(HWND, UINT, WPARAM, LPARAM) ;

/* EM_agent.cpp */
             int  EM_model_agent(Object *, char *, char *) ;        /* Реализация модели агента */

