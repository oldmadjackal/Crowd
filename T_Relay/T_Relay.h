
/********************************************************************/
/*								    */
/*		������ ������ "������������� ���� ���������"        */
/*								    */
/********************************************************************/

#ifdef T_RELAY_EXPORTS
#define T_RELAY_API __declspec(dllexport)
#else
#define T_RELAY_API __declspec(dllimport)
#endif


/*---------------------------------------------- ��������� ��������� */


/*------------------------------------ �������� �������� � ��������� */

     struct  Crowd_MQueue {
                       class Crowd_Message **messages ;
                                       int   cnt ;
                                       int   max ;
                          } ;

#define  _QUEUE_DEEP   10000

     struct  Crowd_MGas {
                              char  kind[128] ;    /* ��� ��������� */ 
                              char  sender[128] ;  /* ��������� ����������� */ 
                            double  price ;        /* ��������� ��������� */ 
                        } ;

/*----------------------- �������� ������ ������ "����� �����������" */

  class T_RELAY_API Crowd_Module_Relay : public Crowd_Kernel {

    public:

     static
        struct Crowd_Module_Relay_instr  *mInstrList ;                  /* ������ ������ */

     static               Crowd_MQueue   mQueue[_QUEUE_DEEP] ;          /* ������� ��������� */

     static                       HWND   mQueueDlg ;                    /* ���� ����������� ��������� ������� */
     static                       HWND   mDebugDlg ;                    /* ���� ������� */

     static                        int   mStateRegime ;                 /* ������ ���������� ��������� �������� */
#define                                    _MEMORY_STATE   0             /* ���������� � ������ */
#define                                      _FILE_STATE   1             /* ���������� � ������ */
#define                                       _LOG_STATE   2             /* ���������� � ������ � � ������ - ������ �� �������� */
     static                       char   mStateFolder[FILENAME_MAX] ;   /* ����� ������ ��������� �������� */

     static                       long   mStep ;                        /* ����� ���� ������������� */

     static                        int   mGasUse ;                      /* ����� ����� ������� �������� ��� �������������  */
     static                 Crowd_MGas **mMsgPrices ;                   /* ������� ��������� �������� ��������� */
     static                        int   mMsgPrices_cnt ;

    public:
     virtual         int  vGetParameter (char *, char *)   ;            /* �������� �������� */
     virtual         int  vExecuteCmd   (const char *) ;                /* ��������� ������� */
     virtual         int  vExecuteCmd   (const char *, Crowd_IFace *) ;
     virtual         int  vSpecial      (char *, void *, char *) ;      /* ����������� ��������� */
     virtual         int  vAddMessage   (Crowd_Message *, int) ;        /* ����������� ��������� � ������� */

    public:
                     int  cHelp         (char *, Crowd_IFace *) ;       /* ���������� HELP */
                     int  cState        (char *, Crowd_IFace *) ;       /* ���������� STATE */
                     int  cPrepare      (char *, Crowd_IFace *) ;       /* ���������� PREPARE */
                     int  cQueue        (char *, Crowd_IFace *) ;       /* ���������� QUEUE */
                     int  cDebug        (char *, Crowd_IFace *) ;       /* ���������� DEBUG */
                     int  cRun          (char *, Crowd_IFace *) ;       /* ���������� RUN */
                     int  cGas          (char *, Crowd_IFace *) ;       /* ���������� GAS */

            Crowd_Object *FindObject    (char *) ;                      /* ����� ������� �� ����� */ 
           Crowd_Message *FindMessage   (char *) ;                      /* ����� ��������� �� ����� */ 
                  double  MessageGas    (Crowd_Message *) ;             /* ����������� ������� �������� �� ��������� */ 

    public:
	                  Crowd_Module_Relay() ;               /* ����������� */
	                 ~Crowd_Module_Relay() ;               /* ���������� */
                                                             } ;

/*-------------------------------------------- ���������� ���������� */

 struct Crowd_Module_Relay_instr {

           char                      *name_full ;                        /* ������ ��� ������� */
           char                      *name_shrt ;                        /* �������� ��� ������� */
           char                      *help_row ;                         /* HELP - ������ */
           char                      *help_full ;                        /* HELP - ������ */
            int (Crowd_Module_Relay::*process)(char *, Crowd_IFace *) ;  /* ��������� ���������� ������� */
                              }  ;
/*--------------------------------------------- ���������� ��������� */

/* ����  T_Relay.cpp */

/* ����  T_Relay_dialog.cpp */
  INT_PTR CALLBACK  Task_Relay_Help_dialog (HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Task_Relay_Queue_dialog(HWND, UINT, WPARAM, LPARAM) ;
  INT_PTR CALLBACK  Task_Relay_Debug_dialog(HWND, UINT, WPARAM, LPARAM) ;
