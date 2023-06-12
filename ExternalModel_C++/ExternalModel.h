/*********************************************************************/
/*                                                                   */
/*                    ������� �������� �������������                 */
/*                                                                   */
/*********************************************************************/

#ifdef  __MAIN__
#define  _EXTERNAL
#else
#define  _EXTERNAL  extern
#endif

/*---------------------------------------------- ��������� ��������� */

#define   _APPLICATION     "ExternalModel"
#define   _PROGRAM_TITLE   "ExternalModel"

#undef    _VERSION
#define   _VERSION   "10.06.2023"

/*--------------------------------------------------- ������� ������ */

  _EXTERNAL   int  __exit_flag ;             /* ���� ���������� ������ */

/*---------------------------------- ���� ���������������� ��������� */

#define    _USER_SECTION_ENABLE    1
#define    _USER_RELOAD            2

/*-------------------------------------------------- ���������� ���� */

  _EXTERNAL            UINT  TaskBar_Msg ;        /* ��������� ����������� ����� � TaskBar */
  _EXTERNAL  NOTIFYICONDATA  TbIcon ;             /* �������� TaskBar-������ */

  _EXTERNAL       HINSTANCE  hInst ;
  _EXTERNAL        WNDCLASS  FrameWindow ;
  _EXTERNAL            HWND  hFrameWindow ;

  _EXTERNAL            HWND  __dialog ;

  _EXTERNAL             int  __window_closed ;    /* ���� ������� ���� � ���� */

  _EXTERNAL             int  __console_process ;  /* ���� ������ � ���������� ������ */

#define                    _NO_ICON              0
#define               _WARNING_ICON              1
#define                 _ERROR_ICON              2

  _EXTERNAL  char  __title[FILENAME_MAX] ;        /* ��������� ���� */

/*------------------------------------------------- ����� ���������� */

  _EXTERNAL            char  __cwd[FILENAME_MAX] ;              /* ������� ������ */

  _EXTERNAL            char  __log_path[FILENAME_MAX] ;         /* ���� ����� ���� */

/*-------------------------------------------------- �������� ������ */

  typedef struct {
                    HWND  hWnd ;           /* ���������� ���� */
                    char  title[128] ;     /* ��������� */           
                 } Section ;

  _EXTERNAL      Section  __sections[10] ;       /* ������ ������ */
  _EXTERNAL          int  __sections_cnt ;

  _EXTERNAL          int  __sec_work ;           /* ������ ������� ������ */

  _EXTERNAL       time_t  __sec_change_time ;    /* ����� ���������� ������������ ������ */

/*---------------------------------------- ����� ����������� ������� */

  _EXTERNAL  HANDLE  hFilesIface_Tread ;                   /* ������� ��� ��������� ���������� */
  _EXTERNAL   DWORD  hFilesIface_PID ;

  _EXTERNAL  HANDLE  hTcpIface_Tread ;                     /* ������� ��� �������� ���������� */
  _EXTERNAL   DWORD  hTcpIface_PID ;

  _EXTERNAL    HWND  hConsoleDialog ;

  _EXTERNAL    char  __control_folder[FILENAME_MAX] ;      /* ����� ����������� ������ */
  _EXTERNAL    char  __control_object[FILENAME_MAX] ;      /* ������������� ������ - �� ��������� ��� * - ��� */
  _EXTERNAL    char  __targets_path[FILENAME_MAX] ;        /* ���� ������ ����� */

  _EXTERNAL    char  __port[FILENAME_MAX] ;                /* ���������� ���� �������� ���������� */

/*------------------------------------------ �������� �������� ����� */

  typedef struct {                                   /* ��������� ������� */
                      char  name[128] ;               /* ��� */
                      char  category[128] ;           /* ���������: message */
                      char  type[128] ;               /* ��� */
                      char  kind[128] ;               /* ��� */
                      char  sender[128] ;             /* ����������� */
                      char  receiver[128] ;           /* ���������� */
                      char  info[1024] ;              /* ���������� */

                 } Event ;
  
  typedef struct {                                   /* ��������� ������� */
                      char  name[128] ;               /* �������� */
                      char  type[128] ;               /* ��� */
                      long  t  ;                      /* ��� ������������� */
                    double  x, y, z ;                 /* ���������� ������� */
                     Event *events ;                  /* ������� */
                       int  events_cnt ;

                 } Object ;

#define        _TARGETS_MAX   100000

  _EXTERNAL  Object *__targets[_TARGETS_MAX] ;        /* ������ ������� ����� */
  _EXTERNAL     int  __targets_cnt ;
  _EXTERNAL    long  __targets_time ;

/*------------------------------------------------ ����� TCP-������� */

#ifdef  __MAIN__

  typedef  struct {                        /* �������� ������� */ 
                      int   use_flag ;
                      int   close_flag ;     /* ���������� �� �������� */
                   time_t   close_time ;     /* ����� �������� */
                     char   descr[64] ;      /* ������ ���������� �������� */

               Tcp_client  *TCP_client ;     /* ������ �� TCP-������� */
                     char   ip[64] ;         /* IP-����� ������� */
                      int   socket ;         /* ����� ������ */

                   HANDLE   thread ;         /* �������������� ������ ��������� */
                    DWORD   thread_id ;
                      int   thread_exit ;    /* ���� ���������� ������ ��������� */
                   HANDLE   hEvent ;         /* ������ �� ������� */
                  Tcp_job  *job ;            /* ������ ������ */

                   Object   object ;         /* ������ ������������� ������� */
                      int   processed ;      /* ���� ���������� ������������� */
                      int   error ;          /* ���� ������ � �������� ������������� */
                     char   result[8192] ;   /* ��������� ������� */

                  } SRV_client ;

  _EXTERNAL   SRV_client **__clients ;
  _EXTERNAL          int   __clients_cnt ;

  _EXTERNAL unsigned int   __process_idx ;

  class  Server_TCP : public Tcp {

      public :

       virtual int  vExternControl     (void) ;                        /* ���������� �������� ���������� */
       virtual int  vExternal          (SOCKET) ;                      /* ��������� ������� ������� */ 
       virtual int  vCloseConnect      (void *) ;                      /* �������� ������ ����� */
       virtual int  vProcessData       (Tcp_job *, void *) ;           /* ��������� ���������� ������ */
       virtual int  vMessagesLog       (Tcp_client *,                  /* ������� ���� ������ */
                                            std::string *, char *) ;
                                                                    
      public :
                    Server_TCP() ;          /* ����������� */
                   ~Server_TCP() ;          /* ���������� */
                                      } ;

  _EXTERNAL     int  __IP_port ;             /* ���������� ���� */

#endif

/*------------------------------------------ ��������� ������������� */

                                             /* ������ AGENT */ 
  _EXTERNAL  double  __step ;                 /* ����������� ���� */

/*---------------------------------------------- ��������� ��������� */

  typedef struct {                                   /* �������� ������� */
                      char  name[128] ;               /* �������� */
                    double  t1 ;                      /* ������ ������������� */
                    double  x, y, z ;                 /* ���������� ������� */
                    double  x_t, y_t, z_t ;           /* ���������� ���� */
                       int  missed ;                  /* ������� ������� */
                 } Context ;

#define        _CONTEXTS_MAX   100

  _EXTERNAL  Context *__contexts[_CONTEXTS_MAX] ;        /* ������ ���������� �������� */
  _EXTERNAL      int  __contexts_cnt ;

/*-------------------------------------------------------- ��������� */

/* ExternalModel.cpp */
     int  EM_system         (void) ;                         /* ��������� ��������� ��������� */
    void  EM_message        (char *) ;                       /* ������� ������ ��������� */
    void  EM_message        (char *, int) ;
     int  EM_log            (char *) ;                       /* ������� ���� */
     int  EM_read_config    (char *) ;                       /* ���������� ����� ������������ */
    void  EM_Change_Section (char *, int) ;                  /* ����� ������ */
    void  EM_text_trim      (char *) ;                       /* ������� ��������� � �������� ���������� �������� */
     int  EM_create_path    (char *) ;                       /* ������������ ���� � ������� */
     int  EM_text_subst     (char *, char *, char *, int) ;  /* ����������� ����� ������ */
     int  EM_json_subst     (char *, char *, char *) ;       /* ����������� ���� ������ �� ����� */
     int  EM_read_targets   (char *, int) ;                  /* ���������� ����� ����� */
     int  EM_read_request   (char *, Object *, int) ;        /* ���������� ����� ������� */
     int  EM_read_request2  (char *, Object *, int) ;        /* ���������� ����� �������, ����������� ����� */
     int  EM_write_response (Object *, char *, char *) ;     /* �������� ���� ������ */
     int  EM_process_model  (Object *, char *, char *) ;     /* ������ ������ */

     int  EM_cmd_sendmessage(char *, Object *,               /* ���������� ������� "��������� ���������" */
                             char *, char *, char *, char *, int) ;

    DWORD WINAPI  FilesIface_Tread   (LPVOID) ;
    DWORD WINAPI  TcpIface_Tread     (LPVOID) ;
    DWORD WINAPI  Model_Process_Tread(LPVOID) ;

/* EM_console.cpp */
INT_PTR CALLBACK  EM_console_dialog(HWND, UINT, WPARAM, LPARAM) ;

/* EM_agent.cpp */
             int  EM_model_agent(Object *, char *, char *) ;        /* ���������� ������ ������ */

