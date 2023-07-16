
/********************************************************************/
/*                                                                  */
/*          ������ ���������� �������� "�������� �������"           */
/*                                                                  */
/********************************************************************/

#ifdef O_SURVEY_EXPORTS
#define O_SURVEY_API __declspec(dllexport)
#else
#define O_SURVEY_API __declspec(dllimport)
#endif


/*---------------------------------------------- ��������� ��������� */

/*------------------ �������� ��������� ������ ��� ������� ��������� */

   struct Profile {
                        char  *name ;        /* �������� ��������� */
                        char  *title ;       /* ���������� ��������� ��������� */
                        char  *describe ;    /* ��������� �������� ��������� */
                        char  *type ;        /* ���: "Digital", "String", "DigitalArray" */
                      double   value_min ;   /* �������� ���������� �������� ��� Digital */
                      double   value_max ;
                        char **value_list ;  /* ������ ���������� �������� ��� String */
                        void  *value ;       /* ��������� �� ������ */
                  } ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - ������ DEFAULT */
   struct Default {
                     COLORREF *color ;
                  } ;
/*----------------------- �������� ������ ������� "�������� �������" */

  class O_SURVEY_API Crowd_Object_Survey : public Crowd_Object {

    public:
                      char  model_path[FILENAME_MAX] ;            /* ���� ������ */

                      char  behavior_model[128] ;                 /* �������� ���������� ������ ��������� */
                      void *behavior_data ;                       /* ������ ���������� ������ ��������� */

    public:
               virtual void  vFree           (void) ;                                    /* ���������� ������� */
               virtual void  vWriteSave      (std::string *) ;                           /* �������� ������ � ������ */
               virtual  int  vEventStart     (void) ;                                    /* ���������� ��������� ������� */
               virtual  int  vEvent          (long, char *, void *, Crowd_Kernel *) ;    /* ��������� ������� */
                                                 
               virtual  int  vEventShow      (void) ;                                    /* ����������� ���������� ��������� ������� */

                        int  iBehaviorDefault(long, char *, void *, Crowd_Kernel *) ;    /* ������ ��������� - DEFAULT */

	                     Crowd_Object_Survey() ;           /* ����������� */
	                    ~Crowd_Object_Survey() ;           /* ���������� */
                                                       } ;

/*----------- �������� ������ ���������� �������� "�������� �������" */

  class O_SURVEY_API Crowd_Module_Survey : public Crowd_Kernel {

    public:

     static
      struct Crowd_Module_Survey_instr *mInstrList ;              /* ������ ������ */
				     
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
                       int  cBehavior     (char *) ;             /* ���������� BEHAVIOR */
                       int  cProfile      (char *) ;             /* ���������� PROFILE */

       Crowd_Object_Survey *FindObject    (char *) ;             /* ����� ������� ���� SURVEY �� ����� */

    public:
	                  Crowd_Module_Survey() ;               /* ����������� */
	                 ~Crowd_Module_Survey() ;               /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_Survey_instr {

           char                       *name_full ;         /* ������ ��� ������� */
           char                       *name_shrt ;         /* �������� ��� ������� */
           char                       *help_row ;          /* HELP - ������ */
           char                       *help_full ;         /* HELP - ������ */
            int (Crowd_Module_Survey::*process)(char *) ;  /* ��������� ���������� ������� */
                                  }  ;

/*--------------------------------------------- ���������� ��������� */

/* ����  O_Survey.cpp */

/* ����  O_Survey_dialog.cpp */
  INT_PTR CALLBACK  Object_Survey_Help_dialog   (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Survey_Create_dialog (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Survey_Profile_dialog(HWND, UINT, WPARAM, LPARAM) ;
