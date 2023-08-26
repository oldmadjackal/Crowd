
/********************************************************************/
/*								    */
/*		������ ���������� �������� "�������"  		    */
/*								    */
/********************************************************************/

#ifdef O_HUMAN_EXPORTS
#define O_HUMAN_API __declspec(dllexport)
#else
#define O_HUMAN_API __declspec(dllimport)
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
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - ������ DOG */
   struct Dog {
                   double  energy ;
                   double  weariness ;
                   double  endurance ;
                 COLORREF *color ;
              } ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - -  ������ SPECTR */
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

/*-------------------------------- �������� ������ ������� "�������" */

  class O_HUMAN_API Crowd_Object_Human : public Crowd_Object {

    public:
                      char  model_path[FILENAME_MAX] ;            /* ���� ������ */

                      char  behavior_model[128] ;                 /* �������� ���������� ������ ��������� */
                      void *behavior_data ;                       /* ������ ���������� ������ ��������� */

    public:
               virtual void  vFree          (void) ;                                    /* ���������� ������� */
               virtual void  vWriteSave     (std::string *) ;                           /* �������� ������ � ������ */
               virtual  int  vEventStart    (void) ;                                    /* ���������� ��������� ������� */
               virtual  int  vEvent         (long, char *, void *, Crowd_Kernel *) ;    /* ��������� ������� */
                                                 
               virtual  int  vEventShow     (void) ;                                    /* ����������� ���������� ��������� ������� */

               virtual void  vCallBack      (Crowd_Object *, char *, char *) ;          /* ����� �������� ����� */

                        int  iBehaviorDog   (long, char *, void *, Crowd_Kernel *) ;    /* ������ ��������� - DOG */
                        int  iBehaviorSpectr(long, char *, void *, Crowd_Kernel *) ;    /* ������ ��������� - SPECTR */

	                     Crowd_Object_Human() ;           /* ����������� */
	                    ~Crowd_Object_Human() ;           /* ���������� */
                                                       } ;

/*-------------------- �������� ������ ���������� �������� "�������" */

  class O_HUMAN_API Crowd_Module_Human : public Crowd_Kernel {

    public:

     static
      struct Crowd_Module_Human_instr *mInstrList ;              /* ������ ������ */
				     
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

        Crowd_Object_Human *FindObject    (char *) ;             /* ����� ������� ���� HUMAN �� ����� */

    public:
	                  Crowd_Module_Human() ;               /* ����������� */
	                 ~Crowd_Module_Human() ;               /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_Human_instr {

           char                      *name_full ;         /* ������ ��� ������� */
           char                      *name_shrt ;         /* �������� ��� ������� */
           char                      *help_row ;          /* HELP - ������ */
           char                      *help_full ;         /* HELP - ������ */
            int (Crowd_Module_Human::*process)(char *) ;  /* ��������� ���������� ������� */
                                 }  ;

/*--------------------------------------------- ���������� ��������� */

/* ����  O_Human.cpp */

/* ����  O_Human_dialog.cpp */
  INT_PTR CALLBACK  Object_Human_Help_dialog   (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Human_Create_dialog (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Object_Human_Profile_dialog(HWND, UINT, WPARAM, LPARAM) ;
