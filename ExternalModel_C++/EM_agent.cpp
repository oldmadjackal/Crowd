/*********************************************************************/
/*                                                                   */
/*                  ������� �������� �������������                   */
/*                                                                   */
/*                         ������ AGENT                              */
/*                                                                   */
/*********************************************************************/

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <tlhelp32.h>

#include <io.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#include <process.h>
#include <errno.h>
#include <sys\timeb.h>
#include <sys\types.h>
#include <sys\stat.h>


#include "controls.h"
#include "resource.h"


#include "ExternalModel.h"

#pragma warning(disable : 4996)
#pragma warning(disable : 4267)

#define   _GRD_TO_RAD   0.017453
#define   _RAD_TO_GRD  57.296


/********************************************************************/
/*                                                                  */
/*                          ������ ������                           */
/*                                                                  */
/*  ��������� ������:                                               */
/*     Step  -  ����������� ���� �����������                        */

     int  EM_model_agent(Object *data, char *command, char *error)

{
  Object *sender ;
  double  r ;  
  double  angle ;
     int  i ;
     int  j ;

/*----------------------------------- �������� ���������� ���������  */

         *error=0 ;

      if(__step==0.)   sprintf(error, "�� ����� �������� Step - ����������� ����") ;

      if(*error!=0)  return(-1) ;

/*------------------------------------------------------- ���������� */

               *command=0 ;

/*----------------------------- ��������� ������� ���� ������������� */

   if(data->t<=1) {

                  }
/*----------------------------------------------- �������� ��������� */

  for(i=0 ; i<data->events_cnt ; i++) {

       if(!stricmp(data->name, data->events[i].sender))  continue ; /* ����������� ��������� ���������� */

    for(j=0 ; j<__targets_cnt ; j++)                                /* �������������� ����������� �� ����� */ 
      if(!stricmp(__targets[j]->name, data->events[i].sender))  break ;   

      if(j>=__targets_cnt) {                                        /* ���� ����������� �� ������ */
                               continue ;
                           }

          sender=__targets[j] ;

              r = sqrt( (sender->x-data->x)*(sender->x-data->x) + (sender->y-data->y)*(sender->y-data->y) ) ;
          angle =atan2(data->y-sender->y, data->x-sender->x) ;
          angle+=__step/r ;

        data->x =sender->x+r*cos(angle) ;                           /* ��������� ������� */
        data->y =sender->y+r*sin(angle) ;
//      data->z+=__step ;

      EM_cmd_sendmessage(command, data, "Contact", "Info",          /* �������������� ��������� ����������� */
                           sender->name, data->events[i].info, 0) ;

                                      }
/*-------------------------------------------------------------------*/

    return(0) ;
}

