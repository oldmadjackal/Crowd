
/********************************************************************/
/*								    */
/*		������ ������ "����� �����������"    		    */
/*								    */
/********************************************************************/

#ifdef T_VIEW_EXPORTS
#define T_VIEW_API __declspec(dllexport)
#else
#define T_VIEW_API __declspec(dllimport)
#endif


/*---------------------------------------------- ��������� ��������� */


/*----------------------- �������� ������ ������ "����� �����������" */

  class T_VIEW_API Crowd_Module_View : public Crowd_Kernel {

    public:

     static
        struct Crowd_Module_View_instr *mInstrList ;                /* ������ ������ */

    public:
     virtual         int  vGetParameter (char *, char *)   ;        /* �������� �������� */
     virtual         int  vExecuteCmd   (const char *) ;            /* ��������� ������� */
     virtual         int  vExecuteCmd   (const char *, Crowd_IFace *) ;

    public:
                     int  cHelp         (char *, Crowd_IFace *) ;   /* ���������� HELP */
                     int  cRing         (char *, Crowd_IFace *) ;   /* ���������� RING */

    public:
	                  Crowd_Module_View() ;               /* ����������� */
	                 ~Crowd_Module_View() ;               /* ���������� */
                                                             } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_View_instr {

           char                     *name_full ;                        /* ������ ��� ������� */
           char                     *name_shrt ;                        /* �������� ��� ������� */
           char                     *help_row ;                         /* HELP - ������ */
           char                     *help_full ;                        /* HELP - ������ */
            int (Crowd_Module_View::*process)(char *, Crowd_IFace *) ;  /* ��������� ���������� ������� */
                              }  ;
/*--------------------------------------------- ���������� ��������� */

/* ����  T_View.cpp */

/* ����  T_View_dialog.cpp */
   INT_PTR CALLBACK  Task_View_Help_dialog(HWND, UINT, WPARAM, LPARAM) ;
