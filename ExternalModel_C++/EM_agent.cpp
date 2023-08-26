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

     int  EM_model_agent(Object *data, char *command, char *callback, char *error)

{
    HWND  hDlg ;
  Object *sender ;
  double  r ;  
  double  angle ;
    char  text[1024] ;
     int  i ;
     int  j ;

/*---------------------------------------------------- ������������� */

                     hDlg=hConsoleDialog ;

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

             sprintf(text, "Event processing: name=%s  sender=%s  type=%s  kind=%s  info=%s", 
                                       data->events[i].name, data->events[i].sender,
                                       data->events[i].type, data->events[i].kind,
                                       data->events[i].info ) ;
          LB_INS_ROW(IDC_LOG, 0, text) ;

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

      EM_cmd_callback   (callback,                                  /* �������� ������ ����������� */
                           sender->name, data->events[i].name, "Processed") ;

                                      }
/*------------------------------------------------- �������� ������� */

  for(i=0 ; i<data->callback_cnt ; i++) {

             sprintf(text, "Callback processing: sender=%s  msg_id=%s  info=%s", 
                              data->callback[i].sender, data->callback[i].msg_id, data->callback[i].info) ;
          LB_INS_ROW(IDC_LOG, 0, text) ;

                                        }
/*-------------------------------------------------------------------*/

    return(0) ;
}

