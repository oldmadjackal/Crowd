
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>


#include "..\Crowd_Object\Crowd_Object.h"
#include "Crowd_Message.h"


#pragma warning(disable : 4996)


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


/********************************************************************/
/*								    */
/*		       ����������� ����������			    */



/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     Crowd_Message::Crowd_Message()

{
    memset(Name, 0, sizeof(Name)) ;
    memset(Type, 0, sizeof(Type)) ;
    memset(Kind, 0, sizeof(Kind)) ;

           Info    =NULL ;
           Object_s=NULL ;
           Object_r=NULL ;
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

    Crowd_Message::~Crowd_Message()

{
}


/********************************************************************/
/*								    */
/*		        ������� ������ �� ������		    */

    void Crowd_Message::vReadSave(char *title, std::string *text, char *reference)

{
}


/********************************************************************/
/*								    */
/*		        �������� ������ � ������		    */

    void Crowd_Message::vWriteSave(std::string *text)

{
     *text="" ;
}


/********************************************************************/
/*								    */
/*		      ���������� �������        		    */

    void Crowd_Message::vFree(void)

{
}


/********************************************************************/
/*								    */
/*                     ������ ���������� � ���������  		    */

    void Crowd_Message::vGetInfo(std::string *text)

{
        *text="" ;
}


/********************************************************************/
/*								    */
/*		      ����������� ��������      		    */

    void  Crowd_Message::vFormDecl(char *text)

{
  char *name_s ;
  char *name_r ;

    if(Object_s!=NULL)  name_s=Object_s->Name ;
    else                name_s="Unknown" ;

    if(Object_r!=NULL)  name_r=Object_r->Name ;
    else                name_r="Broadcast" ;

   sprintf(text, "%s  %s->%s %s:%s", Name, name_s, name_r, Type, Kind) ;
}


/********************************************************************/
/*								    */
/*	      ����������� ��������� �� �����������     		    */

    int  Crowd_Message::vFormBySpec(char *text)

{
   return(0) ;
}


/********************************************************************/
/*								    */
/*                    �������� ������� �����                        */  

    int Crowd_Message::vCheck(class Crowd_Object *object)

{
   if(object==Object_r)  return(1) ;

     return(0) ;
}


