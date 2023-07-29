
#ifdef  __CROWD_ROOT_MAIN__
#define   _EXTERNAL
#else
#define   _EXTERNAL   extern
#endif

/*------------------------------------ ������� ���������� ���������� */

  _EXTERNAL  char  __cwd[FILENAME_MAX] ;    /* ������� ���������� */

  _EXTERNAL  char  __direct_command[128] ;  /* �������� ������ ������� ���������� */
  _EXTERNAL  char  __object_def[32] ;       /* ������ �� ��������� */

#define    _KEY_QUIT   1001

/*------------------------------------------ ������������� ��������� */

  _EXTERNAL  double  __robot_step_joint ;  /* ���� ��������� �������� ����������� �������� */
  _EXTERNAL  double  __robot_step_link ;

/*------------------------- �������� ������ ���������� ����� ������� */

  class Crowd_Module_Main : public Crowd_Kernel {

    public:

     static
      struct Crowd_Module_Main_instr *mInstrList ;          /* ������ ������ */

    public:
     virtual         int  vExecuteCmd   (const char *) ;        /* ��������� ������� */
     virtual         int  vGetParameter (char *, char *) ;      /* �������� �������� */
     virtual        void  vProcess      (void) ;                /* ��������� ������� ���������� */
     virtual        void  vShow         (char *) ;              /* ���������� ��������� ������ */
     virtual        void  vReadSave     (std::string *) ;       /* ������ ������ �� ������ */
     virtual        void  vWriteSave    (std::string *) ;       /* �������� ������ � ������ */

    public:
                     int  cHelp         (char *) ;              /* ���������� HELP */
                     int  cShowScene    (char *) ;              /* ���������� SHOWSCENE */
                     int  cKill         (char *) ;              /* ���������� KILL */
                     int  cAll          (char *) ;              /* ���������� ALL */
                     int  cMessages     (char *) ;              /* ���������� MESSAGES */
                     int  cLookInfo     (char *) ;              /* ���������� LOOKINFO */
                     int  cLookPoint    (char *) ;              /* ���������� EYE */ 
                     int  cLookDirection(char *) ;              /* ���������� LOOK */ 
                     int  cLookAt       (char *) ;              /* ���������� LOOKAT */ 
                     int  cLookZoom     (char *) ;              /* ���������� ZOOM */
                     int  cRead         (char *) ;              /* ���������� READ */
                     int  cWrite        (char *) ;              /* ���������� WRITE */
                     int  cThreads      (char *) ;              /* ���������� THREADS */
                     int  cStop         (char *) ;              /* ���������� STOP */
                     int  cNext         (char *) ;              /* ���������� NEXT */
                     int  cResume       (char *) ;              /* ���������� RESUME */
                     int  cModules      (char *) ;              /* ���������� MODULES */
                     int  cMemory       (char *) ;              /* ���������� MEMORY */
                     int  cSrand        (char *) ;              /* ���������� SRAND */
                     int  cTime         (char *) ;              /* ���������� TIME */
                     int  cReference    (char *) ;              /* ���������� REFERENCE */

                    void  ShowExecute   (char *) ;              /* ����������� ������ */

                    void  iDebug        (char *, char *, int) ; /* ���������� ������ � ���� */ 
                    void  iDebug        (char *, char *) ;

    public:
	                  Crowd_Module_Main() ;                   /* ����������� */
	                 ~Crowd_Module_Main() ;                   /* ���������� */
                                            } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_Main_instr {

           char                     *name_full ;         /* ������ ��� ������� */
           char                     *name_shrt ;         /* �������� ��� ������� */
           char                     *help_row ;          /* HELP - ������ */
           char                     *help_full ;         /* HELP - ������ */
            int (Crowd_Module_Main::*process)(char *) ;  /* ��������� ���������� ������� */
                                }  ;

/*-------------------------------------------------------- ��������� */

/* CrowdRoot.cpp */
      int  CrowdRoot_system           (void) ;                  /* ��������� ��������� ��������� */
      int  CrowdRoot_error            (char *, int) ;           /* ��������� ��������� �� ������� */
      int  CrowdRoot_command_start    (char *) ;                /* ������ ������ */
      int  CrowdRoot_command_processor(char *) ;                /* ��������� ������ */
      int  CrowdRoot_command_read     (char **) ;               /* ���������� ���������� ����� */
      int  CrowdRoot_command_write    (char *) ;                /* ������ ���������� ����� */
     void  CrowdRoot_command_normalise(char *) ;                /* ������������ ��������� ������ */

     void  iDebug                     (char *, char *, int) ;   /* ���������� ������ � ���� */ 
     void  iDebug                     (char *, char *) ;

/* CrowdRoot_dialog.cpp */
  INT_PTR CALLBACK  Main_Help_dialog        (HWND, UINT, WPARAM, LPARAM) ;   /* ���������� ���� HELP */
  INT_PTR CALLBACK  Main_ObjectsList_dialog (HWND, UINT, WPARAM, LPARAM) ;   /* ���������� ���� OBJECTS LIST */
  INT_PTR CALLBACK  Main_MessagesList_dialog(HWND, UINT, WPARAM, LPARAM) ;   /* ���������� ���� MESSAGES LIST */

/* CrowdRoot_monitor.cpp */
      int            CrowdRoot_threads  (char *, char *) ;               /* ���������� ������� ������� */ 
      int            CrowdRoot_modules  (char *) ;                       /* ����������� ������ �������������� ������� */ 
  INT_PTR  CALLBACK  Main_Threads_dialog(HWND, UINT, WPARAM, LPARAM) ;   /* ���������� ���� THREADS */
  INT_PTR  CALLBACK  Main_Modules_dialog(HWND, UINT, WPARAM, LPARAM) ;   /* ���������� ���� MODULES */
