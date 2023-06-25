
/********************************************************************/
/*                                                                  */
/*            ������ ���������� �������� "������� ������"           */
/*                                                                  */
/********************************************************************/

#ifdef O_EXTERNAL_EXPORTS
#define O_EXTERNAL_API __declspec(dllexport)
#else
#define O_EXTERNAL_API __declspec(dllimport)
#endif


/*---------------------------------------------- ��������� ��������� */

/*------------------ �������� ��������� ������ ��� ������� ��������� */

/*------------------------- �������� ������ ������� "������� ������" */

struct Crowd_Object_ExternalLink {
                                       char  link[FILENAME_MAX] ;
                                     double  time_mark ;
                                 } ;

  class O_EXTERNAL_API Crowd_Object_External : public Crowd_Object {

    public:

#define                       _TARGETS_MAX     512000
#define                 _TARGETS_LINKS_MAX       1000

         static                              char *targets ;                           /* �������� ����� */
         static                              long  targets_time ;                      /* ����� ������� ��� �������� ����� */
         static                               int  targets_init ;                      /* ����� ������������� �������� ����� */

         static  struct Crowd_Object_ExternalLink *targets_links[_TARGETS_LINKS_MAX] ; /* ������ ������� �������� �������� ����� */
         static                               int  targets_links_cnt ;

    public:
                       char  model_path[FILENAME_MAX] ;            /* ���� ������ */

                        int  start_flag ;                       /* ������� ������� ���� ������������� */
                       char  object_type[FILENAME_MAX] ;        /* ��� ������� */
                       char  iface_type[FILENAME_MAX] ;         /* ��� ����������: FILE, TCP-SERVER */
                       char  iface_file_folder[FILENAME_MAX] ;  /* ����� ��� ���������� �������� ������ */
                       char  iface_targets[FILENAME_MAX] ;      /* ��� ����� ����� ��� URL ���������� ��� �������� ������ ����� */
                       char  iface_file_control[FILENAME_MAX] ; /* ��� ��� ��������/��������� ������ ������, ���������� - in � out */
                       char  iface_tcp_connect[FILENAME_MAX] ;  /* URL ���������� */

                       long  memory_t ;
                       char *memory_events ;

    public:
               virtual void  vFree          (void) ;                                    /* ���������� ������� */
               virtual void  vWriteSave     (std::string *) ;                           /* �������� ������ � ������ */
               virtual  int  vEventStart    (void) ;                                    /* ���������� ��������� ������� */
               virtual  int  vEvent         (long, char *, void *, Crowd_Kernel *) ;    /* ��������� ������� */
                                                 
               virtual  int  vEventShow     (void) ;                                    /* ����������� ���������� ��������� ������� */

                        int  SendObjectsList(long) ;                                    /* ������������ � �������� ������ �������� */ 
                        int  SendRequest    (long) ;                                    /* ������������ � �������� ������� �� ��������� */ 
                        int  WaitResponse   (long) ;                                    /* ����a��� ���������� ��������� */ 

                        int  TCP_send       (char *, int) ;                             /* ����� ������� � ������� �� �������� ��������� */

	                     Crowd_Object_External() ;           /* ����������� */
	                    ~Crowd_Object_External() ;           /* ���������� */
                                                       } ;

/*------------- �������� ������ ���������� �������� "������� ������" */

  class O_EXTERNAL_API Crowd_Module_External : public Crowd_Kernel {

    public:

     static
      struct Crowd_Module_External_instr *mInstrList ;           /* ������ ������ */
				     
    public:
     virtual  Crowd_Object *vCreateObject (Crowd_Model_data *) ; /* �������� ������� */ 
     virtual           int  vGetParameter (char *, char *)   ;   /* �������� �������� */
     virtual           int  vExecuteCmd   (const char *) ;       /* ��������� ������� */
     virtual          void  vReadSave     (std::string *) ;      /* ������ ������ �� ������ */
     virtual          void  vWriteSave    (std::string *) ;      /* �������� ������ � ������ */

    public:
                       int  cHelp         (char *) ;             /* ���������� HELP */ 
                       int  cCreate       (char *) ;             /* ���������� CREATE */ 
                       int  cInfo         (char *) ;             /* ���������� INFO */ 
                       int  cBase         (char *) ;             /* ���������� BASE */ 
                       int  cColor        (char *) ;             /* ���������� COLOR */
                       int  cVisible      (char *) ;             /* ���������� VISIBLE */
                       int  cIType        (char *) ;             /* ���������� ITYPE */
                       int  cIFile        (char *) ;             /* ���������� IFILE */
                       int  cITcp         (char *) ;             /* ���������� ITCP */

     Crowd_Object_External *FindObject    (char *, int) ;        /* ����� ������� ���� EXTERNAL �� ����� */

    public:
	                  Crowd_Module_External() ;               /* ����������� */
	                 ~Crowd_Module_External() ;               /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_External_instr {

           char                         *name_full ;         /* ������ ��� ������� */
           char                         *name_shrt ;         /* �������� ��� ������� */
           char                         *help_row ;          /* HELP - ������ */
           char                         *help_full ;         /* HELP - ������ */
            int (Crowd_Module_External::*process)(char *) ;  /* ��������� ���������� ������� */
                                    }  ;

/*--------------------------------------------- ���������� ��������� */

/* ����  O_External.cpp */
    int  Object_External_Http_receive(char *, int) ;     /* ������� ���������� ������� ��� HTTP */

/* ����  O_External_dialog.cpp */
  INT_PTR CALLBACK  Object_External_Help_dialog   (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_External_Create_dialog (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_External_Iface_dialog  (HWND, UINT, WPARAM, LPARAM) ;
