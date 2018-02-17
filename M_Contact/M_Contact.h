/********************************************************************/
/*								    */
/*		МОДУЛЬ УПРАВЛЕНИЯ СООБЩЕНИЕМ "КОНТАКТ"              */
/*								    */
/********************************************************************/

#ifdef M_CONTACT_EXPORTS
#define M_CONTACT_API __declspec(dllexport)
#else
#define M_CONTACT_API __declspec(dllimport)
#endif

/*---------------------------------------------- Параметры генерации */

/*------------------ Описание класса управления сообщением "Контакт" */

  class M_CONTACT_API Crowd_Module_Contact : public Crowd_Kernel {

    public:

     static
             struct Crowd_Module_Contact_instr *mInstrList ;               /* Список команд */


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

                                  Crowd_Module_Contact() ;               /* Конструктор */
	                         ~Crowd_Module_Contact() ;               /* Деструктор */
                                                       } ;

/*-------------------------------------------- Инструкции управления */

 struct Crowd_Module_Contact_instr {

           char                        *name_full ;           /* Полное имя команды */
           char                        *name_shrt ;           /* Короткое имя команды */
           char                        *help_row ;            /* HELP - строка */
           char                        *help_full ;           /* HELP - полный */
            int (Crowd_Module_Contact::*process)(char *) ;  /* Процедура выполнения команды */
                                   }  ;

/*----------------------------- Описание класса сообщением "Контакт" */

  class M_CONTACT_API Crowd_Message_Contact : public Crowd_Message {

//    public:       
//                           char  Kind[64] ;                  /* Вид связи */

    public:
            virtual void  vReadSave     (char *, std::string *,        /* Считать данные из строки */
                                                        char * ) ;
            virtual void  vGetInfo      (std::string *) ;              /* Выдать информацию о свойстве */
            virtual  int  vParameter    (char *, char *, char *) ;     /* Работа с параметрами */  

                    void  Show          (void) ;                       /* Отобразить объект */

	                  Crowd_Message_Contact() ;                 /* Конструктор */
	                 ~Crowd_Message_Contact() ;                 /* Деструктор */
                                                         } ;

/*--------------------------------------------- Диалоговые процедуры */

/* Файл  M_Contact.cpp */

/* Файл  M_Contact_dialog.cpp */
  BOOL CALLBACK  Message_Contact_Help_dialog(HWND, UINT, WPARAM, LPARAM) ;
