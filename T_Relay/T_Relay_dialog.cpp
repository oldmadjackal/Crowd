/********************************************************************/
/*								    */
/*		МОДУЛЬ ЗАДАЧИ "МОДЕЛИРОВАНИЕ СЕТИ СООБЩЕНИЙ"        */
/*								    */
/*                   Диалоговые процедуры                           */
/*                                                                  */
/********************************************************************/

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <math.h>

#include "..\Crowd_root\controls.h"

#include "..\Crowd_Feature\Crowd_Feature.h"
#include "..\Crowd_Object\Crowd_Object.h"
#include "..\Crowd_Kernel\Crowd_Kernel.h"

#include "T_Relay.h"

#pragma warning(disable : 4244)
#pragma warning(disable : 4996)


#define  SEND_ERROR(text)    SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,        \
                                         (LPARAM) text) ;

#define  CREATE_DIALOG  CreateDialogIndirectParam

#include "resource.h"


/*---------------------------------------------------- Прототипы п/п */


/*********************************************************************/
/*								     */
/* 	     Обработчик сообщений диалогового окна HELP	             */

  INT_PTR CALLBACK  Task_Relay_Help_dialog(  HWND hDlg,     UINT Msg, 
                                           WPARAM wParam, LPARAM lParam) 
{
  Crowd_Module_Relay  Module ;
                 int  elm ;         /* Идентификатор элемента диалога */
                 int  status ;
                 int  index ;
                 int  insert_flag ;
                char *help ;
                char  text[512] ;
                char *end ;
                 int  i ;

/*------------------------------------------------- Большая разводка */

  switch(Msg) {

/*---------------------------------------------------- Инициализация */

    case WM_INITDIALOG: {
/*- - - - - - - - - - - - - - - - - - - - -  Инициализация элементов */
   for(i=0 ; Module.mInstrList[i].name_full!=NULL ; i++) {

     if(Module.mInstrList[i].help_row==NULL)  continue ;

         index=LB_ADD_ROW (IDC_HELP_LIST, Module.mInstrList[i].help_row) ;
               LB_SET_ITEM(IDC_HELP_LIST, index, Module.mInstrList[i].help_full) ;
                                                         }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}

/*------------------------------------------------ Отработка событий */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - - - - -  Раскрытие справки */
       if(elm==IDC_HELP_LIST) {

        if(status==LBN_DBLCLK) {

             index=        LB_GET_ROW (elm) ;
              help=(char *)LB_GET_ITEM(elm) ;
                           LB_GET_TEXT(elm, index, text) ;
           if(text[0]!='#' || 
              help   ==NULL  )  return(FALSE) ;

               status=LB_GET_TEXT(elm, index+1, text) ;
           if( status==LB_ERR ||
              text[0]==  '#'    )  insert_flag=1 ;
           else                    insert_flag=0 ;

         do {
                   memset(text, 0, sizeof(text)) ;
                  strncpy(text, help, sizeof(text)-1) ;
               end=strchr(text, '\n') ;
           if( end!=NULL)  *end=0 ;

           if(insert_flag) {  LB_INS_ROW(elm, index+1, text) ;
                                              index++ ;        }
              help=strchr(help, '\n') ;
           if(help==NULL)  break ;
              help++ ;
              
            } while(1) ;
                               }

			            return(FALSE) ;
                              }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
			     break ;
			}
/*--------------------------------------------------------- Закрытие */

    case WM_CLOSE:      {
                            EndDialog(hDlg, 0) ;
  			       return(FALSE) ;
			              break ;
			}
/*----------------------------------------------------------- Прочее */

    default :        {
			  return(FALSE) ;
			    break ;
		     }
/*-------------------------------------------------------------------*/
	      }
/*-------------------------------------------------------------------*/

    return(TRUE) ;
}


/*********************************************************************/
/*								     */
/* 	     Обработчик сообщений диалогового окна QUEUE             */

  INT_PTR CALLBACK  Task_Relay_Queue_dialog(  HWND hDlg,     UINT Msg, 
                                            WPARAM wParam, LPARAM lParam) 
{
  Crowd_Module_Relay  Module ;
       static  HFONT  font ;         /* Шрифт */
                 int  elm ;          /* Идентификатор элемента диалога */
                 int  status ;
                char  decl[1024] ;
                char  text[1024] ;
                 int  cnt ;
                 int  i ;
                 int  j ;

/*------------------------------------------------- Большая разводка */

  switch(Msg) {

/*---------------------------------------------------- Инициализация */

    case WM_INITDIALOG: {
                            Module.mQueueDlg=hDlg ;                 /* Фиксируем ссылку на диалог */
/*- - - - - - - - - - - - - - - - - - - - - - - - -  Пропись шрифтов */
        if(font==NULL)
           font=CreateFont(14, 0, 0, 0, FW_THIN, 
                                 false, false, false,
                                  ANSI_CHARSET,
                                   OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY,
                                      VARIABLE_PITCH,
                                       "Courier New Cyr") ;
         SendMessage(ITEM(IDC_QUEUE), WM_SETFONT, (WPARAM)font, 0) ;
/*- - - - - - - - - - - - - - - - - - - - -  Инициализация элементов */
                     cnt=0 ;
 
   for(i=0 ; i<_QUEUE_DEEP ; i++) {

        if(Module.mQueue[i].cnt==0)  continue ;

                  sprintf(text, "------------- Zone %d : %d messages", i, Module.mQueue[i].cnt) ;
               LB_ADD_ROW(IDC_QUEUE, text) ;

      for(j=0 ; j<Module.mQueue[i].cnt ; j++)
        if(Module.mQueue[i].messages[j]!=NULL) {
                       Module.mQueue[i].messages[j]->vFormDecl(decl) ;
                  sprintf(text, "%04d.%04d  %s", i, j, decl) ;
               LB_ADD_ROW(IDC_QUEUE, text) ;

                 cnt++ ;
              if(cnt>5000)  break ;
                                               }

              if(cnt>5000) {
                              SEND_ERROR("Слишком много сообщений - вывод прерван") ;
                                break ;
                           }
                                  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}

/*------------------------------------ Отработка внутренних сообений */

    case WM_USER:  {
/*- - - - - - - - - - - - - - - - - - - - - - - -  Обновление данных */
     if(wParam==_USER_REFRESH) {
 
                                      LB_CLEAR(IDC_QUEUE) ;

                     cnt=0 ;
 
   for(i=0 ; i<_QUEUE_DEEP ; i++) {

        if(Module.mQueue[i].cnt==0)  continue ;

                  sprintf(text, "------------- Zone %d : %d messages", i, Module.mQueue[i].cnt) ;
               LB_ADD_ROW(IDC_QUEUE, text) ;

      for(j=0 ; j<Module.mQueue[i].cnt ; j++)
        if(Module.mQueue[i].messages[j]!=NULL) {
                       Module.mQueue[i].messages[j]->vFormDecl(decl) ;
                  sprintf(text, "%04d.%04d  %s", i, j, decl) ;
               LB_ADD_ROW(IDC_QUEUE, text) ;

                 cnt++ ;
              if(cnt>5000)  break ;
                                               }

              if(cnt>5000) {
                              SEND_ERROR("Слишком много сообщений - вывод прерван") ;
                                break ;
                           }
                                  }

                                     return(FALSE) ;
                               }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
  			     break ;
  		   }
/*------------------------------------------------ Отработка событий */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  ... */
//     if(elm==IDC_HELP_LIST) {
//                                return(FALSE) ;
//                            }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
			     break ;
			}
/*--------------------------------------------------------- Закрытие */

    case WM_CLOSE:      {
                            Module.mQueueDlg=NULL ;                 /* Очищаем ссылку на диалог */

                            EndDialog(hDlg, 0) ;
  			       return(FALSE) ;
			              break ;
			}
/*----------------------------------------------------------- Прочее */

    default :        {
			  return(FALSE) ;
			    break ;
		     }
/*-------------------------------------------------------------------*/
	      }
/*-------------------------------------------------------------------*/

    return(TRUE) ;
}


/*********************************************************************/
/*								     */
/* 	     Обработчик сообщений диалогового окна DEBUG             */

  INT_PTR CALLBACK  Task_Relay_Debug_dialog(  HWND hDlg,     UINT Msg, 
                                            WPARAM wParam, LPARAM lParam) 
{
  Crowd_Module_Relay  Module ;
       static  HFONT  font ;         /* Шрифт */
                 int  elm ;         /* Идентификатор элемента диалога */
                 int  status ;
                 int  rows_cnt ;
                char  text[1024] ;
                 int  i ;

#define   _LOG_MAX   500

/*------------------------------------------------- Большая разводка */

  switch(Msg) {

/*---------------------------------------------------- Инициализация */

    case WM_INITDIALOG: {
                            Module.mDebugDlg=hDlg ;                 /* Фиксируем ссылку на диалог */
/*- - - - - - - - - - - - - - - - - - - - - - - - -  Пропись шрифтов */
        if(font==NULL)
           font=CreateFont(14, 0, 0, 0, FW_THIN, 
                                 false, false, false,
                                  ANSI_CHARSET,
                                   OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY,
                                      VARIABLE_PITCH,
                                       "Courier New Cyr") ;
         SendMessage(ITEM(IDC_LOG), WM_SETFONT, (WPARAM)font, 0) ;
/*- - - - - - - - - - - - - - - - - - - - -  Инициализация элементов */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}
/*------------------------------------ Отработка внутренних сообений */

    case WM_USER:  {
/*- - - - - - - - - - - - - - - - - - - - - - Занесение записи в лог */
     if(wParam==_USER_LOG) {

            rows_cnt=LB_GET_COUNT(IDC_LOG) ;                        /* Очистка лога */
         if(rows_cnt>_LOG_MAX) {
               for(i=0 ; i<rows_cnt-_LOG_MAX ; i++)  LB_DEL_ROW(IDC_LOG, i) ;
                               }

              sprintf(text, "%s", (char *)lParam) ;
           LB_ADD_ROW(IDC_LOG, text) ;
           LB_TOP_ROW(IDC_LOG, LB_GET_COUNT(IDC_LOG)-1) ;

                                     return(FALSE) ;
                           }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
  			     break ;
  		   }

/*------------------------------------------------ Отработка событий */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - - - - -  Раскрытие справки */
//     if(elm==IDC_HELP_LIST) {
//                                return(FALSE) ;
//                            }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
			     break ;
			}
/*--------------------------------------------------------- Закрытие */

    case WM_CLOSE:      {
                            Module.mDebugDlg=NULL ;                 /* Очищаем ссылку на диалог */

                            EndDialog(hDlg, 0) ;
  			       return(FALSE) ;
			              break ;
			}
/*----------------------------------------------------------- Прочее */

    default :        {
			  return(FALSE) ;
			    break ;
		     }
/*-------------------------------------------------------------------*/
	      }
/*-------------------------------------------------------------------*/

    return(TRUE) ;
}


