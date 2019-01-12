/********************************************************************/
/*								    */
/*		������ ���������� ���������� "�������"              */
/*								    */
/********************************************************************/

#ifdef M_CONTACT_EXPORTS
#define M_CONTACT_API __declspec(dllexport)
#else
#define M_CONTACT_API __declspec(dllimport)
#endif

/*---------------------------------------------- ��������� ��������� */

/*------------------ �������� ������ ���������� ���������� "�������" */

  class M_CONTACT_API Crowd_Module_Contact : public Crowd_Kernel {

    public:

     static
             struct Crowd_Module_Contact_instr *mInstrList ;               /* ������ ������ */


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

                                  Crowd_Module_Contact() ;               /* ����������� */
	                         ~Crowd_Module_Contact() ;               /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_Contact_instr {

           char                        *name_full ;           /* ������ ��� ������� */
           char                        *name_shrt ;           /* �������� ��� ������� */
           char                        *help_row ;            /* HELP - ������ */
           char                        *help_full ;           /* HELP - ������ */
            int (Crowd_Module_Contact::*process)(char *) ;  /* ��������� ���������� ������� */
                                   }  ;

/*----------------------------- �������� ������ ���������� "�������" */

  class M_CONTACT_API Crowd_Message_Contact : public Crowd_Message {

//    public:       
//                           char  Kind[64] ;                  /* ��� ����� */

    public:
            virtual void  vReadSave     (char *, std::string *,        /* ������� ������ �� ������ */
                                                        char * ) ;
            virtual void  vGetInfo      (std::string *) ;              /* ������ ���������� � �������� */
            virtual  int  vParameter    (char *, char *, char *) ;     /* ������ � ����������� */  
            virtual  int  vFormBySpec   (char *) ;                     /* ����������� ��������� �� ������������ */

                    void  Show          (void) ;                       /* ���������� ������ */

	                  Crowd_Message_Contact() ;                 /* ����������� */
	                 ~Crowd_Message_Contact() ;                 /* ���������� */
                                                         } ;

/*--------------------------------------------- ���������� ��������� */

/* ����  M_Contact.cpp */

/* ����  M_Contact_dialog.cpp */
  INT_PTR CALLBACK  Message_Contact_Help_dialog(HWND, UINT, WPARAM, LPARAM) ;
