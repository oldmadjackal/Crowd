/*********************************************************************/
/*                                                                   */
/*                    ������� �������� �������������                 */
/*                                                                   */
/*********************************************************************/

#include <windows.h>
#include <winsock.h>
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

#include "..\tcp_lib\exh.h"
#include "..\tcp_lib\tcp.h"

#include "controls.h"
#include "resource.h"


#define  __MAIN__

#include "ExternalModel.h"

#pragma warning(disable : 4996)
#pragma warning(disable : 4267)

/*----------------------------------------- ������������� ���������� */

  typedef struct {
                   int  elem ;
                   int  x ;
                   int  y ;
                   int  xs ;
                   int  ys ;
                 } Elem_pos_list ;

/*------------------------------------ ����������� ��������� ������� */

  union WndProc_par {
                        long            par ;
                     LRESULT (CALLBACK *call)(HWND, UINT, WPARAM, LPARAM) ; 
                    } ;

  static union WndProc_par  Tmp_WndProc ;

/*------------------------------------ ��������� ��������� ��������� */

   LRESULT CALLBACK  EM_window_processor(HWND, UINT, WPARAM, LPARAM) ;


/*********************************************************************/
/*                                                                   */
/*	                      MAIN                     	             */	

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
          HICON  hIcon ;
            MSG  SysMessage ;
           char  text[2048] ;
            int  status ;
            int  i ;

/*---------------------------------------------------- ������������� */

                 getcwd(__cwd, sizeof(__cwd)-1) ;

         status=EM_read_config(lpCmdLine) ;
      if(status)  exit(-1) ;

/*------------------------------- ����������� ������ ���������� ���� */

                            hInst=hInstance ;

	FrameWindow.lpszClassName="EM_Frame" ;
	FrameWindow.hInstance    = hInstance ;
	FrameWindow.lpfnWndProc  = EM_window_processor ;
	FrameWindow.hCursor      = LoadCursor(NULL, IDC_ARROW) ;
	FrameWindow.hIcon        =  NULL ;
	FrameWindow.lpszMenuName =  NULL ;
	FrameWindow.hbrBackground=(HBRUSH__ *)GetStockObject(LTGRAY_BRUSH) ;
	FrameWindow.style        =    0 ;
	FrameWindow.hIcon        =  NULL ;

    if(!RegisterClass(&FrameWindow)) {
              sprintf(text, "EM_Frame register error %d", GetLastError()) ;
           EM_message(text) ;
	                                return(-1) ;
				    }
/*------------------------- ����������� ������� ���������� ��������� */

/*----------------------------------------- �������� ���������� ���� */

       if(__title[0]==0)  strcpy(__title, "External objects model") ;

    hFrameWindow=CreateWindow("EM_Frame", 
                               __title, 
//                             WS_OVERLAPPEDWINDOW,
                               WS_OVERLAPPED  |
                               WS_CAPTION     |
                               WS_THICKFRAME  |
                               WS_SYSMENU     |
                               WS_MINIMIZEBOX |
                               WS_MAXIMIZEBOX   ,
			       CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT,
				        NULL, NULL, 
				   hInstance, NULL ) ;

/*---------------------------------- �������� ���������� ���� ������ */

   do {
/*- - - - - - - - - - - - - - - - - - - - ������������ ������ ������ */
                                              i=0 ;      
    strcpy(__sections[i].title, "CONSOLE") ;  i++ ;

                                 __sections_cnt=i ;
                                              i=0 ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - ���� ������� */
               hConsoleDialog=CreateDialog(hInst, "IDD_CONSOLE",
                                            hFrameWindow, EM_console_dialog) ;
           __sections[i].hWnd=hConsoleDialog ;
        if(__sections[i].hWnd==NULL) {
                   sprintf(text, "CONSOLE dialog load error %d", GetLastError()) ;
                EM_message(text) ;
	                               return(-1) ;
                                     }
                                          i++ ;    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      } while(0) ;

/*------------------------------------------ ������� ����� � TaskBar */

       TaskBar_Msg=RegisterWindowMessage("EM_Activate") ;           /* �������.��������� ����������� */

         hIcon=LoadIcon(hInstance, "IDI_TASKBAR_ICON") ;            /* ������ ������ */

                     TbIcon.cbSize          = sizeof(TbIcon) ;
                     TbIcon.hWnd            = hFrameWindow ;
                     TbIcon.uID             =    1 ;
                     TbIcon.uFlags          = NIF_TIP    |
                                              NIF_ICON   |
                                              NIF_MESSAGE ;
                     TbIcon.uCallbackMessage= TaskBar_Msg ;
                     TbIcon.hIcon           = hIcon ;
              strcpy(TbIcon.szTip, _PROGRAM_TITLE)  ;

    Shell_NotifyIcon(NIM_ADD, &TbIcon) ;

         SendMessage(hFrameWindow, WM_SETICON, ICON_BIG, (LPARAM) hIcon) ;

/*-------------------------------------------------- ���������� ���� */

                  ShowWindow(hFrameWindow, SW_SHOW) ;
		UpdateWindow(hFrameWindow) ;

  		  ShowWindow(__sections[0].hWnd, SW_SHOW) ;
/*
             for(i=1 ; i<__sections_cnt ; i++)
  		  ShowWindow(__sections[i].hWnd, SW_HIDE) ;
*/
                  SendMessage(__sections[0].hWnd, WM_USER,
                                (WPARAM)_USER_SECTION_ENABLE, NULL) ;

/*------------------------------- ������ ��������� ������� ��������� */

          hFilesIface_Tread=CreateThread(NULL, 0, FilesIface_Tread,
                                         NULL, 0, &hFilesIface_PID) ;

/*------------------------------------------ ������� ���������� ���� */

   while(1) {

        if(GetMessage(&SysMessage, NULL, 0, 0)==0)  break ;
    
                TranslateMessage(&SysMessage) ;
                 DispatchMessage(&SysMessage) ;
            } 
/*------------------------------------------------ ���������� ������ */

//        DestroyWindow(FrameWindow_h) ;
        UnregisterClass("EM_Frame", GetModuleHandle(NULL)) ;

       Shell_NotifyIcon(NIM_DELETE, &TbIcon) ;                      /* �������� TaskBar-������ */

/*-------------------------------------------------------------------*/

  return(0) ;
}


/*********************************************************************/
/*								     */
/*		���������� ��������� "���������" ����		     */

 LRESULT CALLBACK  EM_window_processor(  HWND  hWnd,     UINT  Msg,
                                       WPARAM  wParam, LPARAM  lParam)
{
            HWND  hChild ;
            RECT  Rect ;
             int  i ;

/*----------------------------- ��������� ����������� TaskBar-������ */

    if(Msg==TaskBar_Msg) {

        if(lParam!=WM_LBUTTONDBLCLK)  return(0) ;

       SetWindowPos(               hFrameWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE) ;
       SetWindowPos(__sections[__sec_work].hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE) ;

         ShowWindow(               hFrameWindow, SW_RESTORE) ;
         ShowWindow(__sections[__sec_work].hWnd, SW_RESTORE) ;

       SetWindowPos(               hFrameWindow, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE) ;
       SetWindowPos(__sections[__sec_work].hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE) ;

                                              __window_closed=0 ;

                                    return(0) ;
                          }
/*---------------------------------------------- ��������� ��������� */

  switch(Msg) {

/*---------------------------------------------------- �������� ���� */

    case WM_CREATE: {
                       break ;
                    }
/*------------------------------------ ��������� ���������� �������� */

    case WM_USER:  {
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
  			     break ;
  		   }
/*------------------------------------------------ ������� ���������� */

    case WM_COMMAND:  break ;

/*----------------------------------------------- ��������� �������� */

    case WM_SIZE:   {

//      if(hWnd==hExportWindow ||
//         hWnd==hFileDialog     ) {
//                  return( DefWindowProc(hWnd, Msg, wParam, lParam) ) ;
//                                 }

                    hChild=GetWindow(hWnd, GW_CHILD) ;

        if(wParam==SIZE_RESTORED  ||
           wParam==SIZE_MAXIMIZED   )
         if(IsWindow(hChild)) {
/*- - - - - - - - - - - - - - - - - - - - - - - - - - -  ���� ������ */
            for(i=0 ; i<__sections_cnt ; i++) 
              if(hChild==__sections[i].hWnd)  hChild=__sections[0].hWnd ;
           
            if(hChild==__sections[0].hWnd) {

              for(i=0 ; i<__sections_cnt ; i++) {
                     GetWindowRect(__sections[i].hWnd, &Rect) ;
                        MoveWindow(__sections[i].hWnd,  0, 0,
                                              LOWORD(lParam),
                                              HIWORD(lParam), true) ;
                                                } 

                                                break ;
                                           } 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - -  ������ ���� */
                     GetWindowRect(hChild, &Rect) ;
                        MoveWindow(hChild,  0, 0,
                                              LOWORD(lParam),
                                              HIWORD(lParam), true) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                              } 

  			     break ;
  		    }
/*---------------------------------------------------- �������� ���� */

    case WM_CLOSE: {
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - ������� ���� */
      if(hWnd==hFrameWindow) {

                                PostQuitMessage(0) ;  
              			    break ;
                            }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		return( DefWindowProc(hWnd, Msg, wParam, lParam) ) ;
			    break ;
		     }
/*------------------------------------------------- ����������� ���� */

    case WM_DESTROY: {

               if(hWnd==hFrameWindow)  break ;

//   			PostQuitMessage(0) ;  
			     break ;
		     }
/*----------------------------------------------------------- ������ */

    default :        {
		return( DefWindowProc(hWnd, Msg, wParam, lParam) ) ;
			    break ;
		     }
	      }
/*-------------------------------------------------------------------*/

    return(0) ;
}


/*********************************************************************/
/*                                                                   */
/*                 ��������� ��������� ���������                     */

  int  EM_system(void)

{
      MSG  SysMessage ;

/*------------------------------------- ��������� ������� ���������� */

  if(__exit_flag) {
                        return(1) ;
                  }
/*------------------------------------ ��������� ��������� ��������� */

      while( PeekMessage(&SysMessage, NULL, 0, 0, PM_NOREMOVE) ) {

              if(SysMessage.message==WM_QUIT) {
                                                __exit_flag=1 ;
                                                       break ;
                                              }

             PeekMessage(&SysMessage, NULL, 0, 0, PM_REMOVE) ;
	TranslateMessage(&SysMessage) ;
	 DispatchMessage(&SysMessage) ;
						                 }
/*-------------------------------------------------------------------*/

  return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                       ������� ������ ���������                   */

  void  EM_message(char *text)
{
     EM_message(text, MB_ICONSTOP) ;
}

  void  EM_message(char *text, int  icon)
{
       EM_log(text) ;

     MessageBox(NULL, text, _PROGRAM_TITLE, MB_OK | MB_TASKMODAL | MB_TOPMOST | icon) ;
   
}


/*********************************************************************/
/*                                                                   */
/*	                         ������� ����                        */

  int  EM_log(char *text)

{
     time_t  time_abs ;
  struct tm *hhmmss ;
       FILE *file ;
       char  prefix[512] ;
       char  error[1024] ;
        int  status ;
 
/*------------------------------------------------- ������� �������� */

    if(__log_path[0]==0) {
//                          EM_message("�� ����� ���� ����� ����") ;
                                return(-1) ;
                         }
/*---------------------------------------------- �������� ����� ���� */

       file=fopen(__log_path, "at") ;
    if(file==NULL) {
                          sprintf(error, "������ �������� ����� ���� %d : %s", errno, __log_path) ;
                       MessageBox(NULL, error, _PROGRAM_TITLE, MB_OK | MB_TASKMODAL | MB_TOPMOST | MB_ICONSTOP) ;
                            return(-1) ;
                   }
/*-------------------------------------------- �������� ����� ������ */
   
        status=_heapchk() ;
     if(status!=_HEAPOK    && 
        status!=_HEAPEMPTY   ) {

          sprintf(prefix, "Heap crash!!!") ;
           fwrite(prefix, 1, strlen(prefix), file) ;

                               } 
/*------------------------------------------------------ ������ ���� */

    if(text!=NULL) {

               time_abs=     time( NULL) ;
                 hhmmss=localtime(&time_abs) ;

          sprintf(prefix, "%02d.%02d %02d:%02d:%02d ",
                                    hhmmss->tm_mday,
                                    hhmmss->tm_mon+1,
                                    hhmmss->tm_hour,
                                    hhmmss->tm_min,    
                                    hhmmss->tm_sec  ) ;

           fwrite(prefix, 1, strlen(prefix), file) ;
           fwrite(text,   1, strlen(text),   file) ;
           fwrite("\n",   1, strlen("\n"),   file) ;

                  }
/*---------------------------------------------- �������� ����� ���� */

           fclose(file) ;

/*-------------------------------------------------------------------*/
                       
   return(0) ;
}


/*********************************************************************/
/*                                                                   */
/*               ������ � ������ ���������� ���������                */

   int  EM_read_config(char *path)

{
       FILE *file ;
       char  text[1024] ;
       char  message[1024] ;
       char *end ;
        int  row ;
        int  i ;

 static  struct {
                  char *key ;
                  void *value ;     /* ��������!!! ��� ������, ���������� ���� ������ ����� ����� �� ����� FILENAME_MAX */
                  char  type ;
                   int  size ;
                }  pars[]={
                           { "Title=",              __title,              'C', sizeof(__title)           },

                           { "ControlFolder=",      __control_folder,     'C', sizeof(__control_folder)  },
                           { "Object=",             __control_object,     'C', sizeof(__control_object)  },
                           { "Targets=",            __targets_path,       'C', sizeof(__targets_path)    },

                           { "Step=",              &__step,               'D',    0                      },

                           {  NULL }
                          } ;

/*------------------------------------------------------- ���������� */

/*--------------------------------------------------- �������� ����� */

     if(access(path, 0x00)) {
                              sprintf(message, "Config file %s is missed", path) ;
                           EM_message(message) ;
                                 return(-1) ;
                            }

        file=fopen(path, "rb") ;
     if(file==NULL) {
                        sprintf(message, "Config file open error %d :%s", errno, path) ;
                     EM_message(message) ;
                          return(-1) ;
                    }
/*------------------------------------------------- ���������� ����� */

                    row=0 ;
     while(1) {                                                     /* CIRCLE.1 - ��������� ������ ���� */
                    row++ ;
/*- - - - - - - - - - - - - - - - - - -  ���������� ��������� ������ */
                      memset(text, 0, sizeof(text)) ;
                   end=fgets(text, sizeof(text)-1, file) ;          /* ��������� ������ */
                if(end==NULL)  break ;

         if(text[0]==';')  continue ;                               /* �������� ����������� */

            end=strchr(text, '\n') ;                                /* ������� ������ ����� ������ */
         if(end!=NULL)  *end=0 ;
            end=strchr(text, '\r') ;
         if(end!=NULL)  *end=0 ;

             EM_text_trim(text) ;                                   /* ��������� ������� ������� */
         if(text[0]== 0 )  continue ;                               /* ���������� ������ ������ */
/*- - - - - - - - - - - - - - - - - - - -  �������� ��������� ������ */
       for(i=0 ; pars[i].key!=NULL ; i++)                           /* �������.���� */
         if(!memcmp(text, pars[i].key,
                   strlen(pars[i].key ))) {

             if(pars[i].type=='I')  *(   int *)pars[i].value=atoi(text+strlen(pars[i].key)) ;
             else  
             if(pars[i].type=='D')  *(double *)pars[i].value=strtod(text+strlen(pars[i].key), &end) ;
             else              strncpy((char *)pars[i].value, text+strlen(pars[i].key), pars[i].size-1) ;

                                                break ;
                                          }

         if(pars[i].key!=NULL)  continue ;                          /* ���� ������ ���������������� - */
                                                                    /*     ��������� � ���������      */
               sprintf(message, "Context file %s - Unknown key in row %d", path, row) ;
            EM_message(message) ;
                 return(-1) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
              }                                                     /* CONTINUE.1 */
/*--------------------------------------------------- �������� ����� */

                   fclose(file) ;

/*----------------------------------- �������������� ������ �������� */
 
/*-------------------------------------------------------------------*/
  
  return(0) ;
}


/*********************************************************************/
/*                                                                   */
/*                        ����� ������                               */

  void  EM_Change_Section(char *name, int  indicate_flag)

{
  char  text[1024] ;
   int  idx ;

/*--------------------------------------- ����������� ������� ������ */

     for(idx=0 ; idx<__sections_cnt ; idx++)
        if(!strcmp(name, __sections[idx].title)) break ;

        if(idx>=__sections_cnt) {

          if(indicate_flag) {
                  sprintf(text, "������ <%s> �� �������� � ������������", name) ;
               EM_message(text) ;
                            }
               
                                        return ;
                                }
/*--------------------------------------- ������������ ������ ������ */

         if(idx!=__sec_work) {
                                 __sec_change_time=time(NULL) ;

                    ShowWindow(__sections[  idx     ].hWnd, SW_RESTORE) ;
                    ShowWindow(__sections[__sec_work].hWnd, SW_HIDE) ;
                                          __sec_work=idx ;
                   SendMessage(__sections[  idx     ].hWnd, WM_USER,
                                (WPARAM)_USER_SECTION_ENABLE, NULL) ;
                             }

               TabCtrl_SetCurSel(GetDlgItem(__sections[__sec_work].hWnd, 
                                                   IDC_SECTIONS_SWITCH), __sec_work) ;

/*-------------------------------------------------------------------*/     

}


/*********************************************************************/
/*                                                                   */
/*          ������� ��������� � �������� ���������� ��������         */

  void  EM_text_trim(char *text)

{
  char *beg ;
  char *end ;


    for(end=text+strlen(text)-1 ; 
        end>=text && (*end==' ' || *end=='\t'
                                || *end=='\n'
                                || *end=='\r') ; end--)  *end=0 ;

    for(beg=text ;    *beg==' ' || *beg=='\t'
                                || *beg=='\n'
                                || *beg=='\r'  ; beg++)  ;
  

        strcpy(text, beg) ;
}


/*********************************************************************/
/*                                                                   */
/*                 ������������ ���� � �������                       */
/*                                                                   */
/*  ����������: 0 ��� ��� ������                                     */

  int  EM_create_path(char *folder)

{
  char  path[FILENAME_MAX] ;
  char *cut ;
   int  status ;

/*------------------------------------------------------- ���������� */

               strncpy(path, folder, sizeof(path)-1) ;

/*-------------------------------------------------- ������ �������� */

    for(cut=path ; *cut ; cut++)  if(*cut=='/')  *cut='\\' ;

       if(path[1]==':') {                                           /* ������ �������� �������� */
         if(path[0]=='a' || path[0]=='�')  path[0]='A' ;
         if(path[0]=='�' || path[0]=='�')  path[0]='B' ;
         if(path[0]=='�' || path[0]=='�')  path[0]='C' ;
                        }
/*-------------------------------------- �������� ������������� ���� */

        status=access(path, 0x00) ;
     if(status==0)  return(0) ;

/*------------------------------------------------ ������������ ���� */

          if(path[0]=='\\' &&                                       /* ��� ������� �������� */
             path[1]=='\\'   )  cut=strchr(path+2, '\\')+1 ;
          else                  cut=path ;

        for( ; ; cut=cut+1) {
                        
              cut=strchr(cut, '\\') ;
           if(cut!=NULL) *cut=0 ;

                       status=access(path, 0x00) ;
           if(status)  status= mkdir(path) ;
           if(status) {
                           EM_log(path) ;
                           return(errno) ;
                      }

           if(cut!=NULL) *cut='\\' ;
           if(cut==NULL)  break ;
                            }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                     ����������� ����� ������                     */

   int  EM_text_subst(char *buff, char *name, char *value, int  cnt)

{
   char *entry ;
    int  shift ;
    int  i ;


             entry=buff ;
             shift=strlen(value)-strlen(name) ;

     for(i=0 ; i<10000 ; i++) {

             entry=strstr(entry, name) ;
          if(entry==NULL)  break ;
                         
               memmove(entry+strlen(value), entry+strlen(name), 
                                       strlen(entry+strlen(name))+1) ;
                memcpy(entry, value, strlen(value)) ;

             cnt-- ;
          if(cnt==0)  break ;
                           } ;

  return(i) ;
}


/********************************************************************/
/*                                                                  */
/*                 ����������� ���� ������ �� �����                 */

   int  EM_json_subst(char *buff, char *key, char *value)

{
   char  mask[128] ;
   char *entry ;
   char *end ;


              sprintf(mask, "\"%s\":\"", key) ;
         entry=strstr(buff, mask) ;
      if(entry==NULL)  return(0) ;

         entry+=strlen(mask) ;

         end=strchr(entry, '"') ;
      if(end==NULL)  return(-1) ;

         memmove(entry+strlen(value), end, strlen(end)+1) ;
         memmove(entry, value, strlen(value)) ;

  return(1) ;
}


/********************************************************************/
/*                                                                  */
/*   THREAD - ������� ����������� ����� ��� ��������� ����������    */

  DWORD WINAPI  FilesIface_Tread(LPVOID Pars)

{
           HWND  hDlg ;
         time_t  time_0 ;
      struct tm *hhmmss ;
           char  ctrl_mask[FILENAME_MAX] ;
       intptr_t  ctrl_group ;
    _finddata_t  ctrl_file ;
           char  flag[FILENAME_MAX] ;
           char  path[FILENAME_MAX] ;
 static  Object  data ;
           char  prefix[512] ;
           char  text[512] ;
           char  command[4096] ;
           char  callback[4096] ;
            int  rows_cnt ;
            int  status ;
           char *end ;
            int  i ;

#define    _TEST_LOG_MAX   500

/*---------------------------------------------------- ������������� */

                     hDlg=hConsoleDialog ;

/*------------------------------------------------------- ����� ���� */

   do {
          if(__exit_flag)  break ;                                  /* ���� ����� ���� */ 

                  Sleep(10) ;

/*--------------------------------------- ������������ �������� ���� */

                             time(&time_0) ;
                 hhmmss=localtime(&time_0) ;

          sprintf(prefix, "%02d.%02d %02d:%02d:%02d ",
                                    hhmmss->tm_mday,
                                    hhmmss->tm_mon+1,
                                    hhmmss->tm_hour,
                                    hhmmss->tm_min,    
                                    hhmmss->tm_sec  ) ;

/*------------------------------------------------ ������� ���� ���� */

            rows_cnt=LB_GET_COUNT(IDC_LOG) ;
         if(rows_cnt>_TEST_LOG_MAX) {

               for(i=_TEST_LOG_MAX ; i<rows_cnt ; i++)  LB_DEL_ROW(IDC_LOG, i) ;
                                    }
/*---------------------------------------- ����������� ����� ������� */

/*- - - - - - - - - - - - - - - ���� �������� �� ���������� �������� */
     if(__control_object[0]!= 0 &&
        __control_object[0]!='*'  ) {

            sprintf(path, "%s\\%s.out",      __control_folder, __control_object) ;
            sprintf(flag, "%s\\%s.out.flag", __control_folder, __control_object) ;

         if(!access(flag, 0x00)) {

              status=EM_read_request(path, &data, 1) ;
           if(status) {
                             sprintf(text, "ERROR - Control file read error %d : %s", status, path) ;
                          EM_message(text) ;
                                   break ;                           
                      }

              status=unlink(flag) ;
           if(status) {
                             sprintf(text, "ERROR - Control file remove error %d : %s", errno, path) ;
                          EM_message(text) ;
                                   break ;                           
                      }

                         sprintf(text, "%s - Object request detected: name=%s  type=%s  t=%ld", prefix, data.name, data.type, data.t) ;
                      LB_INS_ROW(IDC_LOG, 0, text) ;

          if(__targets_time!=data.t) {
              status=EM_read_targets(__targets_path, 1) ;
           if(status)  break ;
                                     }

                                                text[0]=0 ; 
                                             command[0]=0 ;
                                            callback[0]=0 ;

              status=EM_process_model(&data, command, callback, text) ;
           if(text[0]!=0) {
                                       LB_INS_ROW(IDC_LOG, 0, text) ;
                          } 
           if(status)        break ;

              status=EM_write_response(&data, command, callback, NULL) ;
           if(status) {
                             sprintf(text, "ERROR - Control file read error %d : %s", status, path) ;
                          EM_message(text) ;
                                   break ;                           
                      }

                                 }

                                    }
/*- - - - - - - - - - - - - - - - - - - - -  ���� �������� �� ������ */
     else                           {

              sprintf(ctrl_mask, "%s\\*.out.flag", __control_folder) ;

            ctrl_group=_findfirst(ctrl_mask, &ctrl_file);
         if(ctrl_group<0)  continue ; 

       while(1) {                                                   /* ��� ���� ctrl_file.name... */
                
                sprintf(flag, "%s\\%s", __control_folder, ctrl_file.name) ;
                sprintf(path, "%s\\%s", __control_folder, ctrl_file.name) ;
            end=strrchr(path, '.') ;
           *end=0 ;

              status=EM_read_request(path, &data, 1) ;
           if(status) {
                             sprintf(text, "ERROR - Control file read error %d : %s", status, path) ;
                          EM_message(text) ;
                                   break ;                           
                      }

              status=unlink(flag) ;
           if(status) {
                             sprintf(text, "ERROR - Control file remove error %d : %s", errno, path) ;
                          EM_message(text) ;
                                   break ;                           
                      }

                         sprintf(text, "%s - Object request detected: name=%s  type=%s  t=%ld", prefix, data.name, data.type, data.t) ;
                      LB_INS_ROW(IDC_LOG, 0, text) ;

          if(__targets_time!=data.t) {
              status=EM_read_targets(__targets_path, 1) ;
           if(status)  break ;
                                     }

                                                text[0]=0 ; 
                                             command[0]=0 ;
                                            callback[0]=0 ;

              status=EM_process_model(&data, command, callback, text) ;
           if(text[0]!=0) {
                                       LB_INS_ROW(IDC_LOG, 0, text) ;
                          } 
           if(status)        break ;

              status=EM_write_response(&data, command, callback, NULL) ;
           if(status) {
                             sprintf(text, "ERROR - Control file read error %d : %s", status, path) ;
                          EM_message(text) ;
                                   break ;                           
                      }

		status=_findnext(ctrl_group, &ctrl_file) ;          /* ����� ���������� ����� �� ����� */
	     if(status!=0)  break ;

	           }

	                  _findclose(ctrl_group) ;

                                    }
/*------------------------------------------------------- ����� ���� */


      } while(1) ;

/*-------------------------------------------------------------------*/
                                    
  return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                      ���������� ����� �����                      */

     int  EM_read_targets(char *path, int  file_flag)

{
    FILE *file ;
    char *buff ;
  Object  data ;
    char  text[1024] ;
    char *key ;
    char  value[128] ;
    char *end ;
     int  row ;
     int  i ;

  static  char *keys[]={"\"name\":\"", 
                        "\"x\":\"",    "\"y\":\"",    "\"z\":\"",
                        "\"type\":\"",
                         NULL} ;

/*--------------------------------------------------- �������� ����� */
/*- - - - - - - - - - - - - - - - - - -  ���� �������� ������ - ���� */
   if(file_flag) {

        file=fopen(path, "r") ;
     if(file==NULL) {
                           sprintf(text, "Targets file open error %d : %s", errno, path) ;
                        EM_message(text) ;
                             return(-1) ;
                    }  
                 }
/*- - - - - - - - - - - - - - - - - -  ���� �������� ������ - ������ */
   else          {
                          buff=path ;
                 }
/*-------------------------------------------- ���������� ���������� */

           __targets_cnt=0 ;

   for(row=0 ; row<_TARGETS_MAX ; row++) {

               memset(text, 0, sizeof(text)) ;

   if(file_flag) {

            end=fgets(text, sizeof(text)-1, file) ;
         if(end==NULL) {
                          fclose(file) ;
                         sprintf(text, "Targets file - File ended unexpectedly") ;
                      EM_message(text) ;
                           return(-1) ;
                       }

         if(text[0]==']')  break ;

                 }
   else          {

            end=strchr(buff, '\n') ;
         if(end==NULL) {
                         sprintf(text, "Targets file - Data ended unexpectedly") ;
                      EM_message(text) ;
                           return(-1) ;
                       }

                            *end=0 ;
               strncpy(text, buff, sizeof(text)-1) ;
                             buff=end+1 ;         
                 }
/*- - - - - - - - - - - - - - - - - - - - - - -  ��������� ��������� */
     if(row==0) {

#define  _KEY  "\"t\":\""

        key=strstr(text, _KEY) ;
     if(key==NULL) {
           if(file_flag)  fclose(file) ;
                         sprintf(text, "Targets file - Header attribute %s is missed", _KEY) ;
                      EM_message(text) ;
                           return(-1) ;
                   } 

            memset(value, 0, sizeof(value)) ;
           strncpy(value, key+strlen(_KEY), sizeof(value)-1) ;
        end=strchr(value, '"')  ;
     if(end==NULL) {
           if(file_flag)  fclose(file) ;
                         sprintf(text, "Targets file - Invalid value for parameter %s", _KEY) ;
                      EM_message(text) ;
                           return(-1) ;
                   } 

                      *end=0 ;
            __targets_time=strtol(value, &end, 10) ;

                    continue ;
#undef  _KEY
                }
/*- - - - - - - - - - - - - - - - - - - - - - - ��������� ����� ���� */
      for(i=0 ; keys[i]!=NULL ; i++) {

           key=strstr(text, keys[i]) ;
        if(key==NULL) {
              if(file_flag)  fclose(file) ;
                            sprintf(text, "Targets file, row %d - Parameter %s is missed", row+1, keys[i]) ;
                         EM_message(text) ;
                              return(-1) ;
                      } 

               memset(value, 0, sizeof(value)) ;
              strncpy(value, key+strlen(keys[i]), sizeof(value)-1) ;
           end=strchr(value, '"')  ;
        if(end==NULL) {
              if(file_flag)  fclose(file) ;
                            sprintf(text, "Targets file, row %d - Invalid value for parameter %s", row+1, keys[i]) ;
                         EM_message(text) ;
                              return(-1) ;
                      } 

          *end=0 ;

        if(i==0)  strcpy(data.name, value) ;  
        if(i==1)         data.x     =strtod(value, &end) ;
        if(i==2)         data.y     =strtod(value, &end) ;
        if(i==3)         data.z     =strtod(value, &end) ;
        if(i==4)  strcpy(data.type, value) ;  
                                     } 
/*- - - - - - - - - - - - - - - - - - - - - - - - - ����������� ���� */
       if(__targets[__targets_cnt]==NULL)
            __targets[__targets_cnt]=(Object *)calloc(1, sizeof(Object)) ;

           *__targets[__targets_cnt]=data ;
                      __targets_cnt++ ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

                                         }
/*--------------------------------------------------- �������� ����� */

      if(file_flag)  fclose(file) ;

/*-------------------------------------------------------------------*/

    return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                      ���������� ����� �������                    */

     int  EM_read_request(char *path, Object *data, int  file_flag)

{
          FILE *file ;
  static  char *text ;
          char *array ;
          char *array_end ;
          char *element ;
          char *element_end ;
          char *key ;
          char  value[1024] ;
          char *end ;
           int  i ;

  static  char *keys[]={"\"name\":\"", "\"t\":\"",
                        "\"x\":\"",    "\"y\":\"",       "\"z\":\"",
                        "\"gas\":\"",  "\"gas_max\":\"", "\"gas_renew\":\"",
                        "\"type\":\"",
                         NULL} ;

  static  char *event_keys[]={"\"event\":\"",  "\"name\":\"",     "\"type\":\"", "\"kind\":\"",
                              "\"sender\":\"", "\"receiver\":\"", "\"info\":\"",
                               NULL} ;

  static  char *callback_keys[]={"\"sender\":\"", "\"msgId\":\"", "\"info\":\"",
                               NULL} ;

#define  _BUFF_MAX   64000

/*---------------------------------------------------- ������������� */

     if(text==NULL)  text=(char *)calloc(1, _BUFF_MAX) ;

                      data->events_cnt  =0 ;
                      data->callback_cnt=0 ;

/*------------------------------------------------ ���������� ������ */

                       memset(text, 0, _BUFF_MAX) ;

   if(file_flag) {

        file=fopen(path, "r") ;
     if(file==NULL) {
                           sprintf(text, "Request file open error %d : %s", errno, path) ;
                        EM_message(text) ;
                             return(-1) ;
                    }

           fread(text, 1, _BUFF_MAX-1, file) ;
          fclose(file) ;

                 }
/*- - - - - - - - - - - - - - - - - -  ���� �������� ������ - ������ */
   else          {
                        strncpy(text, path, sizeof(text)-1) ;
                 }
/*------------------------------------------- ������ �������� ������ */

   for(i=0 ; keys[i]!=NULL ; i++) {

        key=strstr(text, keys[i]) ;
     if(key==NULL) {
                         sprintf(text, "Parameter %s is missed", keys[i]) ;
                      EM_message(text) ;
                           return(-1) ;
                   } 

            memset(value, 0, sizeof(value)) ;
           strncpy(value, key+strlen(keys[i]), sizeof(value)-1) ;
        end=strchr(value, '"')  ;
     if(end==NULL) {
                         sprintf(text, "Invalid value for parameter %s", keys[i]) ;
                      EM_message(text) ;
                           return(-1) ;
                   } 

       *end=0 ;

     if(i== 0)  strcpy(data->name, value) ;  
     if(i== 1)         data->t        =strtol(value, &end, 10) ;
     if(i== 2)         data->x        =strtod(value, &end) ;
     if(i== 3)         data->y        =strtod(value, &end) ;
     if(i== 4)         data->z        =strtod(value, &end) ;
     if(i== 5)         data->gas      =strtod(value, &end) ;
     if(i== 6)         data->gas_max  =strtod(value, &end) ;
     if(i== 7)         data->gas_renew=strtod(value, &end) ;
     if(i== 8)  strcpy(data->type, value) ;  

                                  } 
/*-------------------------------------------- ������ ������ ������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ������� ������� */
        array=strstr(text, "\"events\":[") ;
     if(array==NULL) {
                         sprintf(text, "Events section missed") ;
                      EM_message(text) ;
                           return(-1) ;
                     }

        array_end=strchr(array, ']') ;
     if(array_end==NULL) {
                             sprintf(text, "Events section terminator missed") ;
                          EM_message(text) ;
                               return(-1) ;
                         }

       *array_end= 0 ;

   for(element=array ; ; element=element_end+1) {
/*- - - - - - - - - - - - - - - - - - - - ��������� �������� ������� */
        element=strchr(element, '{') ;
     if(element==NULL)  break ;

        element_end=strchr(element, '}') ;
     if(element_end==NULL) {
                             sprintf(text, "Event specifcation terminator missed") ;
                          EM_message(text) ;
                               return(-1) ;
                           }

       *element_end=0 ;

        data->events=(Event *)realloc(data->events,
                                       sizeof(Event)*(data->events_cnt+1)) ;
/*- - - - - - - - - - - - - - - - - - - -  ������ ���������� ������� */
   for(i=0 ; event_keys[i]!=NULL ; i++) {

        key=strstr(element, event_keys[i]) ;
     if(key==NULL) {
                         sprintf(text, "Event attribute %s is missed", event_keys[i]) ;
                      EM_message(text) ;
                           return(-1) ;
                   } 

            memset(value, 0, sizeof(value)) ;
           strncpy(value, key+strlen(event_keys[i]), sizeof(value)-1) ;
        end=strchr(value, '"')  ;
     if(end==NULL) {
                         sprintf(text, "Invalid value for event attribute %s", event_keys[i]) ;
                      EM_message(text) ;
                           return(-1) ;
                   } 

       *end=0 ;

     if(i== 0)  strcpy(data->events[data->events_cnt].category, value) ;  
     if(i== 1)  strcpy(data->events[data->events_cnt].name,     value) ;  
     if(i== 2)  strcpy(data->events[data->events_cnt].type,     value) ;  
     if(i== 3)  strcpy(data->events[data->events_cnt].kind,     value) ;  
     if(i== 4)  strcpy(data->events[data->events_cnt].sender,   value) ;  
     if(i== 5)  strcpy(data->events[data->events_cnt].receiver, value) ;  
     if(i== 6)  strcpy(data->events[data->events_cnt].info,     value) ;  

                                        } 

                  data->events_cnt++ ;

                *element_end='}' ;
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ������� ������� */
                                                }

                  *array_end=']' ;

/*-------------------------------------------- ������ ������ ������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ������� ������� */
        array=strstr(text, "\"callback\":[") ;
     if(array==NULL) {
                         sprintf(text, "Callbak section missed") ;
                      EM_message(text) ;
                           return(-1) ;
                     }

        array_end=strchr(array, ']') ;
     if(array_end==NULL) {
                             sprintf(text, "Callback section terminator missed") ;
                          EM_message(text) ;
                               return(-1) ;
                         }

       *array_end= 0 ;

   for(element=array ; ; element=element_end+1) {
/*- - - - - - - - - - - - - - - - - - - -  ��������� �������� ������ */
        element=strchr(element, '{') ;
     if(element==NULL)  break ;

        element_end=strchr(element, '}') ;
     if(element_end==NULL) {
                             sprintf(text, "Callback specifcation terminator missed") ;
                          EM_message(text) ;
                               return(-1) ;
                           }

       *element_end=0 ;

        data->callback=(Callback *)realloc(data->callback,
                                       sizeof(Callback)*(data->callback_cnt+1)) ;
/*- - - - - - - - - - - - - - - - - - - - - ������ ���������� ������ */
   for(i=0 ; callback_keys[i]!=NULL ; i++) {

        key=strstr(element, callback_keys[i]) ;
     if(key==NULL) {
                         sprintf(text, "Callback attribute %s is missed", callback_keys[i]) ;
                      EM_message(text) ;
                           return(-1) ;
                   } 

            memset(value, 0, sizeof(value)) ;
           strncpy(value, key+strlen(callback_keys[i]), sizeof(value)-1) ;
        end=strchr(value, '"')  ;
     if(end==NULL) {
                         sprintf(text, "Invalid value for callback attribute %s", callback_keys[i]) ;
                      EM_message(text) ;
                           return(-1) ;
                   } 

       *end=0 ;

     if(i== 0)  strcpy(data->callback[data->callback_cnt].sender, value) ;  
     if(i== 1)  strcpy(data->callback[data->callback_cnt].msg_id, value) ;  
     if(i== 2)  strcpy(data->callback[data->callback_cnt].info,   value) ;  

                                           } 

                       data->callback_cnt++ ;

                *element_end='}' ;
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ������� ������� */
                                                }

                  *array_end=']' ;

/*-------------------------------------------------------------------*/

    return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                      ������ ����� ������                         */

     int  EM_write_response(Object *data, char *command, char *callback, char *buff)

{
  char  path[FILENAME_MAX] ;
  char  flag[FILENAME_MAX] ;
  FILE *file ;
  char  value[2048] ;
  char  text[8192] ;

/*---------------------------------------------- ������������ ������ */

         memset(text, 0, sizeof(text)) ;

        sprintf(value, "{ \"name\":\"%s\",\"t\":\"%ld\"\",", data->name, data->t) ;
         strcat(text, value) ;  
        sprintf(value, "\"x\":\"%.2lf\",\"y\":\"%.2lf\",\"z\":\"%.2lf\",\r\n", data->x, data->y, data->z) ;
         strcat(text, value) ;  
        sprintf(value, "\"commands\":[ %s ],\r\n", command) ;
         strcat(text, value) ;
        sprintf(value, "\"callback\":[ %s ]\r\n}\r\n", callback) ;
         strcat(text, value) ;

/*--------------------------------------------- ������ ����� ������ */
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ������ � ����� */
   if(buff!=NULL) {
                      strcpy(buff, text) ;
                  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - ������ � ���� */
   else           {

            sprintf(path, "%s/%s.in",      __control_folder, data->name) ;
            sprintf(flag, "%s/%s.in.flag", __control_folder, data->name) ;

         file=fopen(path, "w") ;
      if(file==NULL) {
                          sprintf(text, "������ %d �������� ����� ������: %s", errno, path) ;
                       EM_message(text) ;
                            return(-1) ;
                     }

             fwrite(text, 1, strlen(text), file) ;
             fclose(file) ;

         file=fopen(flag, "w") ;
      if(file==NULL) {
                          sprintf(text, "������ %d �������� ����-����� ������: %s", errno, path) ;
                       EM_message(text) ;
                            return(-1) ;
                     }

             fclose(file) ;

                  }
/*-------------------------------------------------------------------*/

    return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                          ������ ������                           */

     int  EM_process_model(Object *data, char *command, char *callback, char *error)

{
   int  status ;
  char  text[1024] ;


    if(!stricmp(data->type, "Agent"))   status=EM_model_agent(data, command, callback, error) ;
    else                              {

                sprintf(text, "����������� ������ %s ��� ������� %s", data->type, data->name) ;
             EM_message(text) ;
                  return(-1) ;
                                      }

    return(0) ;
}


/********************************************************************/
/*                                                                  */
/*           ���������� ������� "��������� ���������"               */

     int  EM_cmd_sendmessage(char *command, Object *sender, char *type, char *kind, char *recipient, char *info, int  delay)

{
  static  int  msg_cnt ;
         char  text[1024] ;


                    msg_cnt++ ;

          sprintf(text, "{\"command\":\"sendmessage\",\"name\":\"%s-%d\",\"type\":\"%s\",\"kind\":\"%s\",\"recipient\":\"%s\",\"info\":\"%s\",\"delay\":\"%d\"}",
                             sender->name, msg_cnt, type, kind, recipient, info, delay) ;

     if(command[0]!=0)  strcat(command, ",\r\n") ;
                        strcat(command, text) ;

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                           ���������� ������                      */

   int  EM_cmd_callback(char *callback, char *sender, char *msg_id, char *info)

{
  char  text[4096] ;


          sprintf(text, "{\"sender\":\"%s\",\"msgId\":\"%s\",\"info\":\"%s\"}",
                             sender, msg_id, info) ;

     if(callback[0]!=0)  strcat(callback, ",\r\n") ;
                         strcat(callback, text) ;

   return(0) ;
}
