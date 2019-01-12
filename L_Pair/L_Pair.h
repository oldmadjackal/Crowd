/********************************************************************/
/*								    */
/*		������ ���������� ������ "������ ������"            */
/*								    */
/********************************************************************/

#ifdef L_PAIR_EXPORTS
#define L_PAIR_API __declspec(dllexport)
#else
#define L_PAIR_API __declspec(dllimport)
#endif

/*---------------------------------------------- ��������� ��������� */

/*-------------------------------------------------------- ��� ����� */

 struct Crowd_Communication_Pair_Kind {

                      char  name[64] ;                  /* �������� ���� */
                  COLORREF  color ;                     /* ���� */

                                      }  ;

#define   _PAIR_KIND_MAX  50

/*---------------- �������� ������ ���������� ������ "������ ������" */

  class L_PAIR_API Crowd_Module_Pair : public Crowd_Kernel {

    public:

     static
             struct Crowd_Module_Pair_instr *mInstrList ;               /* ������ ������ */

     static
       struct Crowd_Communication_Pair_Kind  mKinds[_PAIR_KIND_MAX] ;   /* ������ ����� ����� */
     static                             int  mKinds_cnt ;

                                        int  mShow_dlist ;

    public:
     virtual                 int  vGetParameter       (char *, char *) ;         /* �������� �������� */
     virtual                void  vStart              (void) ;                   /* ��������� �������� */
     virtual Crowd_Communication *vCreateCommunication(Crowd_Object *,           /* ������� ����� */
                                                       Crowd_Object *,
                                                       Crowd_Communication *) ;
     virtual                void  vShow               (char *) ;                 /* ���������� ��������� ������ */
     virtual                 int  vExecuteCmd         (const char *) ;           /* ��������� ������� */
     virtual                void  vWriteSave          (std::string *) ;          /* �������� ������ � ������ */

    public:
                             int  cHelp               (char *) ;                 /* ���������� Help */ 
                             int  cKind               (char *) ;                 /* ���������� Kind */
                             int  cLink               (char *) ;                 /* ���������� Link */
                             int  cColor              (char *) ;                 /* ���������� Color */

                    Crowd_Object *FindObject          (char *) ;                 /* ����� ������� �� ����� */ 
                             int  iShowScene          (void) ;                   /* ����������� ����� */

                                  Crowd_Module_Pair() ;               /* ����������� */
	                         ~Crowd_Module_Pair() ;               /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_Pair_instr {

           char                   *name_full ;           /* ������ ��� ������� */
           char                   *name_shrt ;           /* �������� ��� ������� */
           char                   *help_row ;            /* HELP - ������ */
           char                   *help_full ;           /* HELP - ������ */
            int (Crowd_Module_Pair::*process)(char *) ;  /* ��������� ���������� ������� */
                                }  ;

/*---------------------------- �������� ������ ����� "������ ������" */

  class L_PAIR_API Crowd_Communication_Pair : public Crowd_Communication {

//    public:       
//                           char  Kind[64] ;                  /* ��� ����� */

    public:
            virtual void  vReadSave     (char *, std::string *,        /* ������� ������ �� ������ */
                                                        char * ) ;
            virtual void  vGetInfo      (std::string *) ;              /* ������ ���������� � �������� */
            virtual  int  vParameter    (char *, char *, char *) ;     /* ������ � ����������� */  

                    void  Show          (void) ;                       /* ���������� ������ */

	                  Crowd_Communication_Pair() ;                 /* ����������� */
	                 ~Crowd_Communication_Pair() ;                 /* ���������� */
                                                         } ;

/*--------------------------------------------- ���������� ��������� */

/* ����  F_Pair.cpp */

/* ����  F_Pair_dialog.cpp */
  INT_PTR CALLBACK  Communication_Pair_Help_dialog(HWND, UINT, WPARAM, LPARAM) ;
