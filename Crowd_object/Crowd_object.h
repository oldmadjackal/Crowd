
#ifndef  CROWD_OBJECT_H 

#define  CROWD_OBJECT_H 

#include "..\Crowd_Feature\Crowd_Feature.h"
#include "..\Crowd_Communication\Crowd_Communication.h"

#include <string>

#ifdef CROWD_OBJECT_EXPORTS
#define CROWD_OBJECT_API __declspec(dllexport)
#else
#define CROWD_OBJECT_API __declspec(dllimport)
#endif

/*---------------------------------------------------- ���� �������� */

#define   _CROWD_OBJECT_UNKNOWN_ERROR      -999    /* ����������� ������ */

/*------------------------------------- ���� ����� ������� ��������� */

#define   _CROWD_ABSOLUTE_TARGET  0   /* �������� ����������� ������������ */ 

/*----------------------------------- ��������� �������������� ����� */

#define   _GRD_TO_RAD   0.017453
#define   _RAD_TO_GRD  57.296
#define   _PI           3.1415926

/*---------------------------------------- �������� �������� "�����" */

   typedef struct {
                     double  x ;
                     double  y ;
                     double  z ;

                     double  azim ;
                     double  elev ;
                     double  roll ;

                        int  mark ;

                  } Crowd_Point ;

/*--------------------------------------- �������� �������� "������" */

   typedef struct {
                     double  x ;
                     double  y ;
                     double  z ;

                        int  mark ;

                  } Crowd_Vector ;

/*------------------------- �������� �������� "����������� ��������" */

   typedef struct {
                     char  link[32] ;            /* �����-������ �� �������� */
                     char  section_name[32] ;    /* �������� ������ ���������� */
                     char  parameter_name[32] ;  /* �������� ��������� */
                     char  type[8] ;             /* ��� ��������� */  
                     void *value ;               /* ��������������� �������� */
                  } Crowd_ControlPar ;

/*------------------------------------ ��������� �������� ���������� */

  class CROWD_OBJECT_API Crowd_Transit {

    public:
                      char  action[1024] ;
              Crowd_Object *object ;

    public:
             virtual   int  vExecute(void) ;             /* ���������� �������� */
                                             
    public:
                            Crowd_Transit() ;            /* ����������� */
                           ~Crowd_Transit() ;            /* ���������� */

                                   } ; 

/*-------------------------------- �������� ������ "������ ��������" */

  struct Crowd_Objects_List_Elem {
                               class Crowd_Object *object ;
                                             char  relation[128] ;
                                 }  ;

  class CROWD_OBJECT_API Crowd_Objects_List {

       public:
                Crowd_Objects_List_Elem *List ;
                                    int   List_cnt ;
                                    int   List_max ;

       public:

                        int  Add  (class Crowd_Object *, char *) ;    /* ���������� � ������ */
                       void  Clear(void) ;                            /* ������� ������ */

			     Crowd_Objects_List() ;                   /* ����������� */
			    ~Crowd_Objects_List() ;                   /* ���������� */

                                  } ;
/*----------------------------------------- �������� ������ "������" */

  class CROWD_OBJECT_API Crowd_Object {

       public:
                       char  Name[128] ;           /* ��� ������� */
                       char  Type[128] ;           /* ��� ������� */
                       char  Decl[1024] ;          /* �������� ������� */
                       char *Program ;             /* ��������� ��������� */
                       char *State ;               /* ����� ��������� */

                        int  message_cnt ;         /* ������� ������������ ��������� */

                       char  SenderType[128] ;     /* ��� ��������� ������� */
                     double  gas ;                 /* ����� ������������� �������� */
                     double  gas_max ;             /* ������������ ��������� ����� ������������� �������� */
                     double  gas_renew ;           /* ����� ��������, ����������������� �� ��� */

                     double  x_base ;              /* ���������� ������� ����� */
                     double  y_base ;
                     double  z_base ;
                     double  x_base_stack ;
                     double  y_base_stack ;
                     double  z_base_stack ;

                     double  x_base_save ;         /* ���������� ������� ����� ����� ����������� ���� */
                     double  y_base_save ;
                     double  z_base_save ;

     struct Crowd_Parameter  *Parameters ;         /* ������ ���������� */
                        int   Parameters_cnt ;

              Crowd_Feature **Features ;           /* ������ ������� */
                        int   Features_cnt ;

        Crowd_Communication **Communications ;     /* ������ ������� */
                        int   Communications_cnt ;

   class Crowd_Objects_List   Units ;              /* ������ ��������-��������� ������ */

              Crowd_Transit  *Context ;            /* ��������� �������� ���������� */
        class  Crowd_Kernel  *Module ;             /* ����������� ������ ������� */

                        int   ErrorEnable ;        /* ���� ������ ��������� �� ������� */

       public:

   virtual class Crowd_Object *vCopy           (char *) ;                /* ���������� ������ */
   virtual               void  vPush           (void)  ;                 /* ��������� ��������� ������� */
   virtual               void  vPop            (void)  ;                 /* ������������ ��������� ������� */

   virtual               void  vErrorMessage   (int) ;                   /* ���./����. ��������� �� ������� */

   virtual               void  vReadSave       (std::string *) ;         /* ������� ������ �� ������ */
   virtual               void  vWriteSave      (std::string *) ;         /* �������� ������ � ������ */
   virtual               void  vFree           (void) ;                  /* ���������� ������� */

   virtual               void  vFormDecl       (void) ;                  /* ����������� �������� */

   virtual                int  vListControlPars(Crowd_ControlPar *) ;    /* �������� ������ ���������� ���������� */
   virtual                int  vSetControlPar  (Crowd_ControlPar *) ;    /* ���������� �������� ��������� ���������� */

   virtual                int  vGetPosition    (Crowd_Point *) ;         /* ������ � ���������� ������� */
   virtual               void  vSetPosition    (Crowd_Point *) ;

   virtual                int  vSpecial        (char *, void *) ;        /* ����������� �������� */

   virtual                int  vEventStart     (void) ;                  /* ���������� ��������� ������� */
   virtual                int  vEventSaveState (void) ;                  /* ���������� �������� ���������  */
   virtual                int  vEvent          (long, char *,            /* ��������� ������� */
                                                      void *, Crowd_Kernel *) ;
   virtual                int  vEventShow      (void) ;                  /* ����������� ���������� ��������� ������� */

   virtual               void  vCallBack       (Crowd_Object *,          /* ����� �������� ����� */
                                                        char *, char *) ;

   virtual                int  vResetFeatures  (void *) ;                /* ����� ��������� �������� ������� */
   virtual                int  vPrepareFeatures(void *) ;                /* ����������� �������� � �������� ������������ */
   virtual                int  vCheckFeatures  (void *,                  /* ��������� ������������ ������� */
                                                Crowd_Objects_List *) ;
   virtual               void  vSetFeature     (Crowd_Feature *) ;       /* ������ �� ���������� */
   virtual               void  vGetFeature     (Crowd_Feature *) ;

                          int  iAngleInCheck   (double,                  /* �������� ��������� ���� � �������� */
                                                double, double) ;
                          int  iReplaceText    (char *, char *,          /* ������ ���������� ��������� � ������ */
                                                        char *, int) ;

			     Crowd_Object      () ;                   /* ����������� */
			    ~Crowd_Object      () ;                   /* ���������� */

                                  } ;
/*-------------------------------------------------------------------*/

#endif  // CROWD_OBJECT_H
