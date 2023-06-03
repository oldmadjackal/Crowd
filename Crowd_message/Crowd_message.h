
#ifndef  CROWD_MESSAGE_H 

#define  CROWD_MESSAGE_H 

#include <string>

#ifdef CROWD_MESSAGE_EXPORTS
#define CROWD_MESSAGE_API __declspec(dllexport)
#else
#define CROWD_MESSAGE_API __declspec(dllimport)
#endif

/*-------------------------------------- �������� ������ "���������" */

    class CROWD_MESSAGE_API Crowd_Message {

    public:
                               char  Name[128] ;          /* ��� ��������� */
                               char  Type[128] ;          /* ��� ��������� */
                               char  Kind[128] ;          /* ��� ��������� */
                               char *Info ;               /* ������ ��������� */
                 class Crowd_Object *Object_s ;           /* ������-����������� */ 
                 class Crowd_Object *Object_r ;           /* ������-���������� */ 

    public:

               virtual void  vReadSave     (char *, std::string *,    /* ������� ������ �� ������ */
                                                           char * ) ;
               virtual void  vWriteSave    (std::string *) ;          /* �������� ������ � ������ */  
               virtual void  vFree         (void) ;                   /* ���������� ������� */  
               virtual void  vGetInfo      (std::string *) ;          /* ���������� � ��������� */  
               virtual void  vFormDecl     (char *) ;                 /* ����������� �������� */
               virtual  int  vFormBySpec   (char *) ;                 /* ����������� ��������� �� ������������ */

               virtual  int  vCheck        (class Crowd_Object *) ;   /* �������� �������-���������� */

			     Crowd_Message() ;                        /* ����������� */
			    ~Crowd_Message() ;                        /* ���������� */
                                      } ;

/*-------------------------------------------------------------------*/

#endif        // CROWD_MESSAGE_H 
