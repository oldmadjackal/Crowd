/********************************************************************/
/*                                                                  */
/*           ������ ���������� �������� "�������� �������"          */
/*                                                                  */
/*                   ���������� ���������                           */
/*                                                                  */
/********************************************************************/

#include <windows.h>
#include <stdio.h>
#include <io.h>

#include "..\Crowd_root\controls.h"

#include "..\Crowd_Kernel\Crowd_Kernel.h"

#include "O_Survey.h"

#pragma warning(disable : 4244)
#pragma warning(disable : 4996)

#define  SEND_ERROR(text)    SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,        \
                                         (LPARAM) text) ;

#include "resource.h"


/*********************************************************************/
/*                                                                   */
/*         ���������� ��������� ����������� ���� HELP                */

  INT_PTR CALLBACK  Object_Survey_Help_dialog(  HWND hDlg,     UINT Msg, 
                                              WPARAM wParam, LPARAM lParam) 
{
  Crowd_Module_Survey  Module ;
                  int  elm ;         /* ������������� �������� ������� */
                  int  status ;
                  int  index ;
                  int  insert_flag ;
                 char *help ;
                 char  text[512] ;
                 char *end ;
                  int  i ;

/*------------------------------------------------- ������� �������� */

  switch(Msg) {

/*---------------------------------------------------- ������������� */

    case WM_INITDIALOG: {
/*- - - - - - - - - - - - - - - - - - - - -  ������������� ��������� */
   for(i=0 ; Module.mInstrList[i].name_full!=NULL ; i++) {

     if(Module.mInstrList[i].help_row==NULL)  continue ;

         index=LB_ADD_ROW (IDC_HELP_LIST, Module.mInstrList[i].help_row) ;
               LB_SET_ITEM(IDC_HELP_LIST, index, Module.mInstrList[i].help_full) ;
                                                         }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}

/*------------------------------------------------ ��������� ������� */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - - - - -  ��������� ������� */
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
/*--------------------------------------------------------- �������� */

    case WM_CLOSE:      {
                            EndDialog(hDlg, 0) ;
  			       return(FALSE) ;
			              break ;
			}
/*----------------------------------------------------------- ������ */

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
/*                                                                   */
/*           ���������� ��������� ����������� ���� CREATE            */

  INT_PTR CALLBACK  Object_Survey_Create_dialog(  HWND hDlg,     UINT Msg, 
                                                WPARAM wParam, LPARAM lParam) 
{
      Crowd_Module_Survey  Module ;
 static  Crowd_Model_data *data ;
             Crowd_Object *object ;
 static              char  models_list[4096] ;
 static              RECT  PictureFrame ;
                  HBITMAP  hBitmap ;
//                HBITMAP  hBitmap_prv ;
                      int  elm ;               /* ������������� �������� ������� */
                      int  status ;
                     char  library[FILENAME_MAX] ;
                     char  value[512] ;
                      int  assigned ;
                     char *end ;
                      int  i ;

/*------------------------------------------------- ������� �������� */

  switch(Msg) {

/*---------------------------------------------------- ������������� */

    case WM_INITDIALOG: {

                   GetWindowRect(ITEM(IDC_PICTURE), &PictureFrame) ;

              data=(Crowd_Model_data *)lParam ;
/*- - - - - - - - - - - - - - - - - - - ������������� ������ ������� */
         Crowd_Model_list(data->lib_path, models_list,
                                   sizeof(models_list)-1, "BODY") ;

                                          CB_CLEAR(IDC_MODEL) ;
     for(end=models_list ; *end ; ) {
                                      CB_ADD_LIST(IDC_MODEL, end) ;
                                         end+=strlen(end)+1 ;
                                         end+=strlen(end)+1 ;
                                    }
/*- - - - - - - - - - - - - - - - - - - - ���������� �������� ������ */
     while(data->model[0]!=0) {

        for(end=models_list ; *end ; ) {                            /* ���� ������ �� ������ */
                        if(!stricmp(data->model, end))  break ;
                                         end+=strlen(end)+1 ;
                                         end+=strlen(end)+1 ;
                                       }

           if(*end==0) {  data->model[0]=0 ;                        /* ���� ����� ������ ���... */
                             break ;          }

                      sprintf(data->path, "%s\\%s", data->lib_path, end) ;
         Crowd_Model_ReadPars(data) ;                               /* ��������� ��������� ������ */

                                  break ;
                              }
/*- - - - - - - - - - - - - - - - - - - - -   ������������� �������� */
     while(data->picture[0]!=0) {
    
          status=Crowd_Model_Picture(data, &hBitmap, &PictureFrame) ;
       if(status)  break ;

               SendMessage(ITEM(IDC_PICTURE), 
                              STM_SETIMAGE,
                               (WPARAM)IMAGE_BITMAP,
                                (LPARAM)hBitmap     ) ;

                                      break ;
                                }
/*- - - - - - - - - - - - - - - - -  ������������� ������ ���������� */
       for(i=0 ; i<5 ; i++) {
                                       SETs(IDC_PAR_NAME_1+i, data->pars[i].text) ;
         if(data->pars[i].text[0]!=0)  SHOW(IDC_PAR_VALUE_1+i) ;
         else                          HIDE(IDC_PAR_VALUE_1+i) ;
                            }
/*- - - - - - - - - - - - - - - - - - - - -  ������������� ��������� */
          DISABLE(IDC_CREATE) ;

            SETs(IDC_NAME,         data->name) ;
            SETs(IDC_LIBRARY_PATH, data->lib_path) ;
            SETc(IDC_MODEL,        data->model) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}

/*------------------------------------------------ ��������� ������� */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - ����� ���������� */
     if(elm==IDC_LIBRARY_PATH) {

        if(status!=EN_UPDATE)  return(FALSE) ;

              GETs(IDC_LIBRARY_PATH, library) ;

         Crowd_Model_list(library, models_list,
                            sizeof(models_list)-1, "BODY") ;

                                          CB_CLEAR(IDC_MODEL) ;
      for(end=models_list ; *end ; ) {
                                       CB_ADD_LIST(IDC_MODEL, end) ;
                                          end+=strlen(end)+1 ;
                                          end+=strlen(end)+1 ;
                                     }

                                 DISABLE(IDC_CREATE) ;

                                      return(FALSE) ;
                               }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - ����� ������ */
     if(elm==IDC_MODEL) {
                              if(status!=CBN_SELCHANGE)  break ;

                         GETs(IDC_LIBRARY_PATH, data->lib_path) ;
                         GETc(elm, data->model ) ;                  /* ��������� �������� ������ */

        for(end=models_list ; *end ; ) {                            /* ���� ������ �� ������ */
                        if(!stricmp(data->model, end))  break ;
                                         end+=strlen(end)+1 ;
                                         end+=strlen(end)+1 ;
                                       }

                                    end+=strlen(end)+1 ;            /* ��������� ��� ����� */

                      sprintf(data->path, "%s\\%s", data->lib_path, end) ;
         Crowd_Model_ReadPars(data) ;                               /* ��������� ��������� ������ */

        for(i=0 ; i<5 ; i++) {                                      /* ������ ������ ���������� ������ */
                                       SETs(IDC_PAR_NAME_1+i, data->pars[i].text) ;
         if(data->pars[i].text[0]!=0)  SHOW(IDC_PAR_VALUE_1+i) ;
         else                          HIDE(IDC_PAR_VALUE_1+i) ;
                             }

            status=Crowd_Model_Picture(data, &hBitmap, &PictureFrame) ;
//       if(status)  break ;

               SendMessage(ITEM(IDC_PICTURE), STM_SETIMAGE,
                               (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap) ;

         if(data->pars[0].text[0]!=0)  DISABLE(IDC_CREATE) ;
         else                           ENABLE(IDC_CREATE) ;

                                      return(FALSE) ;
                        }
/*- - - - - - - - - - - - - - - - - - - - -  ������� ���������� ���� */
     if(elm==IDC_PAR_VALUE_1 ||
        elm==IDC_PAR_VALUE_2 ||
        elm==IDC_PAR_VALUE_3 ||
        elm==IDC_PAR_VALUE_4 ||
        elm==IDC_PAR_VALUE_5   ) {

                    if(status!=EN_UPDATE)  return(FALSE) ;

           for(assigned=1, i=0 ; i<5 ; i++) {
               if(!VISIBLE(IDC_PAR_VALUE_1+i))  break ;

                  GETsl(IDC_PAR_VALUE_1+i, value, sizeof(value)-1) ;
               if(*value==0) {  assigned=0 ;  break ;  } 
                                            }

               if(assigned)  ENABLE(IDC_CREATE) ;

                                      return(FALSE) ;
                                }
/*- - - - - - - - - - - - - - - - - - - - - - - - - �������� ������� */
     if(elm==IDC_CREATE) {

                  GETs(IDC_NAME,         data->name) ;
                  GETs(IDC_LIBRARY_PATH, data->lib_path ) ;
                  GETc(IDC_MODEL,        data->model) ;

          for(i=0 ; i<5 ; i++)
                  GETs(IDC_PAR_VALUE_1+i, data->pars[i].value) ;  

                object=Module.vCreateObject(data) ;
             if(object!=NULL)  EndDialog(hDlg, 0) ;

                              return(FALSE) ;
                         }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
			     break ;
			}
/*--------------------------------------------------------- �������� */

    case WM_CLOSE:      {
                            EndDialog(hDlg, -1) ;
  			       return(FALSE) ;
			              break ;
			}
/*----------------------------------------------------------- ������ */

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
/*                                                                   */
/* 	     ���������� ��������� ����������� ���� PROFILE	             */

  INT_PTR CALLBACK  Object_Survey_Profile_dialog(  HWND hDlg,     UINT Msg, 
                                                 WPARAM wParam, LPARAM lParam) 
{
     Crowd_Module_Survey  Module ;
                   HFONT  font ;           /* ����� */
  static  struct Profile *profile ;
                     int  elm ;            /* ������������� �������� ������� */
                     int  status ;
  static             int  index ;
                    char  text[512] ;
                    char  value[128] ;
                  double  value_d ;
                    char *end ;
                     int  i ;

/*------------------------------------------------- ������� �������� */

  switch(Msg) {

/*---------------------------------------------------- ������������� */

    case WM_INITDIALOG: {

                 profile=(struct Profile *)lParam ;
/*- - - - - - - - - - - - - - - - - - - - -  ������������� ��������� */
             memset(value, 0, sizeof(value)) ;

   for(i=0 ; profile[i].name[0]!=0 ; i++) {

              if(!stricmp(profile[i].type, "String" ))  strncpy(value, (char   *)profile[i].value, sizeof(value)-1) ;
         else if(!stricmp(profile[i].type, "Digital"))  sprintf(value, "%lf", *(double *)profile[i].value) ;
         else                                            strcpy(value, "Unknown value type") ;

            sprintf(text, "%-20.20s %-40.40s %s", profile[i].name, profile[i].title, value) ;

         LB_ADD_ROW(IDC_LIST, text) ;
                                          }

                   DISABLE(IDC_VALUE) ;
                   DISABLE(IDC_SET_VALUE) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ������� ������� */
        font=CreateFont(14, 7, 0, 0, FW_THIN, 
                          false, false, false,
                           ANSI_CHARSET,
                            OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS,
                              DEFAULT_QUALITY,
                               VARIABLE_PITCH,
                                "Courier New Cyr") ;
            SendMessage(ITEM(IDC_LIST), WM_SETFONT, (WPARAM)font, 0) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}

/*------------------------------------------------ ��������� ������� */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - - - -  ��������� ��������� */
       if(elm==IDC_LIST) {

        if(status==LBN_DBLCLK) {

                index=LB_GET_ROW(elm) ;
                            SETs(IDC_NAME,     profile[index].name    ) ;
                            SETs(IDC_TITLE,    profile[index].title   ) ;
                            SETs(IDC_DESCRIBE, profile[index].describe) ;

                          ENABLE(IDC_VALUE) ;
                          ENABLE(IDC_SET_VALUE) ;

              if(!stricmp(profile[index].type, "String" ))   strncpy(value, (char *)profile[index].value, sizeof(value)-1) ;
         else if(!stricmp(profile[index].type, "Digital"))   sprintf(value, "%lf", *(double *)profile[index].value) ;
         else                                              {
                          strcpy(value, "Unknown value type") ;
                         DISABLE(IDC_VALUE) ;
                         DISABLE(IDC_SET_VALUE) ;
                                                           }

                            SETs(IDC_VALUE, value) ;

                               }

			            return(FALSE) ;
                        }
/*- - - - - - - - - - - - - - - - - - - ��������� �������� ��������� */
       if(elm==IDC_SET_VALUE) {

                           memset(value, 0, sizeof(value)) ;
                            GETsl(IDC_VALUE, value, sizeof(value)-1) ;

              if(!stricmp(profile[index].type, "String" )) {
                                strcpy((char *)profile[index].value, value) ;
                                                           }
         else if(!stricmp(profile[index].type, "Digital")) {

                        value_d=strtod(value, &end) ;

                 if(*end!=0) {
                                SEND_ERROR("������������ ��������") ;
			            return(FALSE) ;
                             }

                 if(profile[index].value_min!=profile[index].value_max)
                  if(value_d<profile[index].value_min ||
                     value_d>profile[index].value_max   ) {
                                   sprintf(text, "�������� ������ ���� � ��������� %lf ... %lf", profile[index].value_min, profile[index].value_max) ;
                                SEND_ERROR(text) ;
			            return(FALSE) ;
                                                          }

                      *((double *)profile[index].value)=value_d ;

                                                           }

               sprintf(text, "%-20.20s %-40.40s %s", profile[index].name, profile[index].title, value) ;

            LB_DEL_ROW(IDC_LIST, index) ;
            LB_INS_ROW(IDC_LIST, index, text) ;

			            return(FALSE) ;
                              }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
			     break ;
			}
/*--------------------------------------------------------- �������� */

    case WM_CLOSE:      {
                            EndDialog(hDlg, 0) ;
  			       return(FALSE) ;
			              break ;
			}
/*----------------------------------------------------------- ������ */

    default :        {
			  return(FALSE) ;
			    break ;
		     }
/*-------------------------------------------------------------------*/
	      }
/*-------------------------------------------------------------------*/

    return(TRUE) ;
}


