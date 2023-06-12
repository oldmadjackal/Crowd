/********************************************************************/
/*                                                                  */
/*        ������ ���������� ���������� "�������������"              */
/*                                                                  */
/********************************************************************/

#ifdef M_BROADCAST_EXPORTS
#define M_BROADCAST_API __declspec(dllexport)
#else
#define M_BROADCAST_API __declspec(dllimport)
#endif

/*---------------------------------------------- ��������� ��������� */

/*------------ �������� ������ ���������� ���������� "�������������" */

  class M_BROADCAST_API Crowd_Module_Broadcast : public Crowd_Kernel {

    public:

     static
             struct Crowd_Module_Broadcast_instr *mInstrList ;               /* ������ ������ */


    public:
     virtual                 int  vGetParameter       (char *, char *) ;         /* �������� �������� */
     virtual                void  vStart              (void) ;                   /* ��������� �������� */
     virtual       Crowd_Message *vCreateMessage      (Crowd_Object *,           /* ������� ��������� */
                                                       Crowd_Object *, Crowd_Message *) ;
     virtual                 int  vExecuteCmd         (const char *) ;           /* ��������� ������� */
     virtual                void  vWriteSave          (std::string *) ;          /* �������� ������ � ������ */

    public:
                             int  cHelp               (char *) ;                 /* ���������� Help */ 
                             int  cCreate             (char *) ;                 /* ���������� Create */

                    Crowd_Object *FindObject          (char *) ;                 /* ����� ������� �� ����� */ 
                   Crowd_Message *FindMessage         (char *) ;                 /* ����� ��������� �� ����� */ 

                                  Crowd_Module_Broadcast() ;               /* ����������� */
                                 ~Crowd_Module_Broadcast() ;               /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_Broadcast_instr {

           char                          *name_full ;           /* ������ ��� ������� */
           char                          *name_shrt ;           /* �������� ��� ������� */
           char                          *help_row ;            /* HELP - ������ */
           char                          *help_full ;           /* HELP - ������ */
            int (Crowd_Module_Broadcast::*process)(char *) ;  /* ��������� ���������� ������� */
                                     }  ;

/*----------------------- �������� ������ ���������� "�������������" */

  class M_BROADCAST_API Crowd_Message_Broadcast : public Crowd_Message {

//    public:       
//                           char  Kind[64] ;                  /* ��� ����� */

    public:
            virtual void  vReadSave     (char *, std::string *,        /* ������� ������ �� ������ */
                                                        char * ) ;
            virtual void  vFree         (void) ;                       /* ���������� ������� */  
            virtual void  vGetInfo      (std::string *) ;              /* ������ ���������� � �������� */
            virtual  int  vParameter    (char *, char *, char *) ;     /* ������ � ����������� */  
            virtual  int  vFormBySpec   (char *) ;                     /* ����������� ��������� �� ������������ */
            virtual  int  vCheck        (class Crowd_Object *) ;       /* �������� ������� ����� */ 

	                  Crowd_Message_Broadcast() ;                 /* ����������� */
	                 ~Crowd_Message_Broadcast() ;                 /* ���������� */
                                                         } ;

/*--------------------------------------------- ���������� ��������� */

/* ����  M_Broadcast.cpp */

/* ����  M_Broadcast_dialog.cpp */
  INT_PTR CALLBACK  Message_Broadcast_Help_dialog(HWND, UINT, WPARAM, LPARAM) ;
