/********************************************************************/
/*								    */
/*		������ ������ "������������� ���� ���������"        */
/*								    */
/********************************************************************/

#define  stricmp  _stricmp

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <malloc.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys\stat.h>

#include "..\Matrix\Matrix.h"

#include "..\Crowd_Feature\Crowd_Feature.h"
#include "..\Crowd_Message\Crowd_Message.h"
#include "..\Crowd_Object\Crowd_Object.h"
#include "..\Crowd_Kernel\Crowd_Kernel.h"

#include "T_Relay.h"


#pragma warning( disable : 4244 )
#pragma warning( disable : 4996 )


#define  _SECTION_FULL_NAME   "RELAY"
#define  _SECTION_SHRT_NAME   "RELAY"

#define  SEND_ERROR(text)    SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,        \
                                         (LPARAM) text)
#define  SEND_INFO(text)     SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_INFO_MESSAGE,         \
                                         (LPARAM) text)


#define  CREATE_DIALOG  CreateDialogIndirectParam


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
/*		    	����������� ������                          */

     static   Crowd_Module_Relay  ProgramModule ;


/********************************************************************/
/*								    */
/*		    	����������������� ����                      */

 T_RELAY_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 T_RELAY_API Crowd_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/********************************************************************/
/********************************************************************/
/**							           **/
/**	       �������� ������ ������ "����� �����������"          **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*                            ������ ������                         */

  struct Crowd_Module_Relay_instr  Crowd_Module_Relay_InstrList[]={

 { "help",       "?",  "#HELP (?) - ������ ��������� ������", 
                        NULL,
                       &Crowd_Module_Relay::cHelp   },
 { "state",      "p",  "#STATE (S) - ������� ������ �������� ��������� ��������", 
                        "STATE/M - �������� � ������\n "
                        "STATE/F <����> - �������� � ������ � ��������� �����"
                        "STATE/L <����> - �������� � ������ � ������ - ������ ��� ���������",
                       &Crowd_Module_Relay::cState    },
 { "prepare",    "p",  "#PREPARE (P) - ���������� ��������� �����", 
                       " PREPARE <��� ���������> <��������� ����> - ��������� ��������� � �������",
                       &Crowd_Module_Relay::cPrepare    },
 { "queue",      "q",  "#QUEUE (Q) - �������� ������� ���������", 
                        NULL,
                       &Crowd_Module_Relay::cQueue    },
 { "debug",      "d",  "#DEBUG (D) - ������� ���� �������", 
                        NULL,
                       &Crowd_Module_Relay::cDebug    },
 { "run",        "r",  "#RUN (R) - ��������� �������������", 
                        NULL,
                       &Crowd_Module_Relay::cRun    },
 { "gas",        "g",  "#GAS (G) - ���������� �������� ��������", 
                       " GAS <1|0> \n"
                       "   ��������/��������� ���� ������� �������� ��� �������������\n"
                       " GAS/P <��� ���������> <��������� �����������> <���������>\n"
                       "   ������� ��������� �������� ��������� ������������� ���� ��� �������� ��������� ������������\n",
                       &Crowd_Module_Relay::cGas    },
 {  NULL }
                                                                  } ;


/********************************************************************/
/*								    */
/*		     ����� ����� ������             		    */

  struct Crowd_Module_Relay_instr  *Crowd_Module_Relay::mInstrList=NULL ;

  struct             Crowd_MQueue   Crowd_Module_Relay::mQueue[_QUEUE_DEEP] ;

                             HWND   Crowd_Module_Relay::mQueueDlg ;
                             HWND   Crowd_Module_Relay::mDebugDlg ;

                              int   Crowd_Module_Relay::mStateRegime ;
                             char   Crowd_Module_Relay::mStateFolder[FILENAME_MAX] ;

                             long   Crowd_Module_Relay::mStep ;

                              int   Crowd_Module_Relay::mGasUse       =  0 ;
                       Crowd_MGas **Crowd_Module_Relay::mMsgPrices    =NULL ;
                              int   Crowd_Module_Relay::mMsgPrices_cnt=  0 ;


/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     Crowd_Module_Relay::Crowd_Module_Relay(void)

{
                keyword="Crowd" ;
         identification="Relay" ;
               category="Task" ;

             mInstrList=Crowd_Module_Relay_InstrList ;
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

    Crowd_Module_Relay::~Crowd_Module_Relay(void)

{
}


/********************************************************************/
/*								    */
/*		        �������� ��������       		    */

     int  Crowd_Module_Relay::vGetParameter(char *name, char *value)

{
/*-------------------------------------------------- �������� ������ */

    if(!stricmp(name, "$$MODULE_NAME")) {

         sprintf(value, "%-20.20s -  ������������� ���� ���������", identification) ;
                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*	                ����������� ���������                       */

  int  Crowd_Module_Relay::vSpecial(char *action, void *object, char *details)

{
  Crowd_Object *obj ;
          char  path[FILENAME_MAX] ;
          FILE *file ;

/*------------------------------------------- ������ ��������� � ��� */

   if(!stricmp(action, "LOG")) {

         if(mDebugDlg)  SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)object) ;

                               }
/*------------------------------ ������/���������� ��������� ������� */

   if(!stricmp(action, "SAVE_STATE")) {

                       obj=(Crowd_Object *)object ;

     if(mStateRegime==_MEMORY_STATE ||
        mStateRegime==   _LOG_STATE   ) {
                  obj->State=(char *)realloc(obj->State, strlen(details)+8) ;
           strcpy(obj->State, details) ;
                                        }
     if(mStateRegime==  _FILE_STATE ||
        mStateRegime==   _LOG_STATE   ) {

                sprintf(path, "%s\\%s.state", mStateFolder, obj->Name) ;
             file=fopen(path, "wb") ;
          if(file==NULL)  return(-1) ;

                 fwrite(details, 1, strlen(details), file) ;
                 fclose(file) ;
                                        }                 
                                      }

   if(!stricmp(action, "READ_STATE")) {

                       obj=(Crowd_Object *)object ;

     if(mStateRegime==_MEMORY_STATE ||
        mStateRegime==   _LOG_STATE   ) {

           if(obj->State!=NULL)  strcpy(details, obj->State) ;
           else                  strcpy(details, "") ;
                                        }
     if(mStateRegime==  _FILE_STATE   ) {

                sprintf(path, "%s\\%s.state", mStateFolder, obj->Name) ;
             file=fopen(path, "rb") ;
          if(file==NULL)  return(-1) ;

                  fread(details, 1, 64000, file) ;
                 fclose(file) ;
                                        }                 

                                      }
/*------------------------------ ��������� ������ ���� ������������� */

   if(!stricmp(action, "GET_STEP")) {

                       (*(double *)object)=mStep ;

                                    }
/*-------------------------------------------------------------------*/


   return(0) ;
}


/********************************************************************/
/*								    */
/*		   ����������� ��������� � �������                  */

  int  Crowd_Module_Relay::vAddMessage(Crowd_Message *message, int  zone)

{
#define  QUEUE      this->mQueue[zone].messages
#define  QUEUE_CNT  this->mQueue[zone].cnt
#define  QUEUE_MAX  this->mQueue[zone].max

/*--------------------------------------------- ����� ��������� ���� */ 

   if(zone<    0        ||
      zone>=_QUEUE_DEEP   )  return(-1) ;

/*------------------------------------ ��������� ��������� � ������� */ 

   if(QUEUE_CNT==QUEUE_MAX) {

          QUEUE=(Crowd_Message **)realloc(QUEUE, (QUEUE_MAX+100)*sizeof(*QUEUE)) ;
       if(QUEUE==NULL)  return(-1) ;
                                                 QUEUE_MAX+=100 ;
                            }

          QUEUE[QUEUE_CNT]=message ;
                QUEUE_CNT++ ; 

/*-------------------------------------------------------------------*/ 

#undef  QUEUE
#undef  QUEUE_CNT
#undef  QUEUE_MAX

   return(0) ;
}


/********************************************************************/
/*								    */
/*		        ��������� �������       		    */

  int  Crowd_Module_Relay::vExecuteCmd(const char *cmd)

{
   return( vExecuteCmd(cmd, NULL) ) ;
}


  int  Crowd_Module_Relay::vExecuteCmd(const char *cmd, Crowd_IFace *iface)

{
  static  int  direct_command ;   /* ���� ������ ������ ������� */
         char  command[1024] ;
         char *instr ;
         char *end ;
          int  status ;
          int  i ;

/*--------------------------------------------- ������������� ������ */

             memset(command, 0, sizeof(command)) ;
            strncpy(command, cmd, sizeof(command)-1) ;
/*- - - - - - - - - - - - - - - - - - - -  ��������� �������� ������ */
        if(command[0]=='&') {
                                end=command+1 ;
                            }
/*- - - - - - - - - - - - - - - - - - - - -  ��������� ������ ������ */
   else if(!direct_command) {

         end=strchr(command, ' ') ;
      if(end!=NULL) {  *end=0 ;  end++ ;  }

      if(stricmp(command, _SECTION_FULL_NAME) &&
         stricmp(command, _SECTION_SHRT_NAME)   )  return(1) ;
                            }
/*- - - - - - - - - - - - - - - - - - - -  ��������� �������� ������ */
   else                     {
                                end=command ;
                            }
/*----------------------- ���������/���������� ������ ������ ������� */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - -  ��������� */
   if(end==NULL || end[0]==0) {
     if(!direct_command) {
                            direct_command=1 ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"Task Relay:") ;
        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_DIRECT_COMMAND, (LPARAM)identification) ;
                         }
                                    return(0) ;
                              }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - ���������� */
   if(end!=NULL && !strcmp(end, "..")) {

                            direct_command=0 ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"") ;
        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_DIRECT_COMMAND, (LPARAM)"") ;

                                           return(0) ;
                                       }
/*--------------------------------------------- ��������� ���������� */

       instr=end ;                                                  /* �������� ����� � ��������� ������� */

     for(end=instr ; *end!= 0  &&
                     *end!=' ' &&
                     *end!='>' &&
                     *end!='.' &&
                     *end!='/'    ; end++) ;

      if(*end!= 0 &&
         *end!=' '  )  memmove(end+1, end, strlen(end)+1) ;

      if(*end!=0) {  *end=0 ;  end++ ;  }
      else            end="" ;

   for(i=0 ; mInstrList[i].name_full!=NULL ; i++)                   /* ���� ������� � ������ */
     if(!stricmp(instr, mInstrList[i].name_full) ||
        !stricmp(instr, mInstrList[i].name_shrt)   )   break ;

     if(mInstrList[i].name_full==NULL) {                            /* ���� ����� ������� ���... */

          status=this->kernel->vExecuteCmd(cmd, iface) ;            /*  �������� �������� ������ ����... */
       if(status)  SEND_ERROR("������ RELAY: ����������� �������") ;
                                            return(-1) ;
                                       }
 
     if(mInstrList[i].process!=NULL)                                /* ���������� ������� */
                status=(this->*mInstrList[i].process)(end, iface) ;
     else       status=  0 ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� HELP                    */

  int  Crowd_Module_Relay::cHelp(char *cmd, Crowd_IFace *iface)

{ 
    DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_HELP", RT_DIALOG),
			   GetActiveWindow(), Task_Relay_Help_dialog) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� STATE                   */
/*								    */
/*      STATE/M                                                     */
/*      STATE/F <����>                                              */
/*      STATE/L <����>                                              */

  int  Crowd_Module_Relay::cState(char *cmd, Crowd_IFace *iface)

{
#define   _PARS_MAX  10

            char *pars[_PARS_MAX] ;
            char *end ;
             int  i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
       if(*cmd=='/') {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end!=NULL)  *end=0 ;
                
                if(strchr(cmd, 'm')!=NULL ||
                   strchr(cmd, 'M')!=NULL   ) mStateRegime=_MEMORY_STATE ;
                if(strchr(cmd, 'f')!=NULL ||
                   strchr(cmd, 'F')!=NULL   ) mStateRegime=  _FILE_STATE ;
                if(strchr(cmd, 'l')!=NULL ||
                   strchr(cmd, 'L')!=NULL   ) mStateRegime=   _LOG_STATE ;

                if(end!=NULL)  cmd=end+1 ;
                else           cmd= "" ;
                     }
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

      if(pars[0]!=NULL)
       if(*pars[0]!=0)  strcpy(mStateFolder, pars[0]) ;

/*---------------------------------------- �������� ����� ���������� */
  
    if(mStateRegime==_FILE_STATE ||
       mStateRegime== _LOG_STATE   ) {

     if(mStateFolder[0]==0) {
                      SEND_ERROR("�� ����� ���� ����� ���������� ��������� ��������") ;
                                     return(-1) ;
                            }

     if(access(mStateFolder, 0x00)) {
                      SEND_ERROR("��������� ����� ���������� ��������� �������� �� ����������") ;
                                     return(-1) ;
                                    }

                                     }
/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� PREPARE                 */
/*								    */
/*      PREPARE <��� ���������> <��������� ����>                    */

  int  Crowd_Module_Relay::cPrepare(char *cmd, Crowd_IFace *iface)

{
#define   _PARS_MAX  10

            char *pars[_PARS_MAX] ;
            char *name_msg ;
            char *zone_txt ;
             int  zone ;
   Crowd_Message *message ;
             int  status ;
            char *end ;
             int  i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name_msg=pars[0] ;
                     zone_txt=pars[1] ;

/*------------------------------------------- �������� ����� ������� */

    if(zone_txt==NULL) {                                            /* ���� ��� �� ������... */
                      SEND_ERROR("������ ���� ����� ������ ��������� ����: 0 ... 10000 \n"
                                 "��������: PREPARE <��� ���������> <��������� ����>") ;
                                     return(-1) ;
                       }

       message=FindMessage(name_msg) ;                              /* ���� ��������� �� ����� */
    if(message==NULL)  return(-1) ;

/*---------------------------------- ����������� ��������� � ������� */

                                   zone=strtol(zone_txt, &end, 10) ;
       status=vAddMessage(message, zone) ;
    if(status) {
                  SEND_ERROR("������ ����������� ��������� � �������") ;
                                     return(-1) ;
               }
/*----------------------------------------------- ����������� ������ */

    if(this->mQueueDlg)
        SendMessage(this->mQueueDlg, WM_USER, (WPARAM)_USER_REFRESH, NULL) ;

/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� QUEUE                   */

  int  Crowd_Module_Relay::cQueue(char *cmd, Crowd_IFace *iface)

{
/* 
  HWND  hDlg ;


    hDlg=CreateDialogIndirect(GetModuleHandle(NULL),
                               (LPCDLGTEMPLATE)Resource("IDD_QUEUE", RT_DIALOG),
                                  GetActiveWindow(), Task_Relay_Queue_dialog) ;
                   ShowWindow(hDlg, SW_SHOW) ;
*/

      DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_QUEUE", RT_DIALOG),
			   GetActiveWindow(), Task_Relay_Queue_dialog) ;
   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� DEBUG                   */

  int  Crowd_Module_Relay::cDebug(char *cmd, Crowd_IFace *iface)

{ 
      DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_DEBUG", RT_DIALOG),
			   GetActiveWindow(), Task_Relay_Debug_dialog) ;
   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� GAS                     */
/*                                                                  */
/*   GAS    <����� �����>                                           */
/*   GAS/P  <��� ���������> <��������� �����������> <���������>     */

  int  Crowd_Module_Relay::cGas(char *cmd, Crowd_IFace *iface)

{
#define   _PARS_MAX  10

                   char  *pars[_PARS_MAX] ;
                    int   p_flag ;
                   char  *kind ;
                   char  *sender ;
                 double   value ;
             Crowd_MGas  *price ;
                   char  *end ;
                    int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                      p_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, 'p')!=NULL ||
                   strchr(cmd, 'P')!=NULL   )  p_flag=1 ;
//         else if(strchr(cmd, 'r')!=NULL ||
//                 strchr(cmd, 'R')!=NULL   )  r_flag=1 ;

                           cmd=end+1 ;
                        }

/*
       if(m_flag+r_flag!=1) {
                      SEND_ERROR("����� ���� ������ ���� �� ������: M ��� R. \n"
                                 "��������: GAS/M <���_�������> ...") ;
                                           return(-1) ;
                            }
*/
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }
/*------------------------------------------- ������� ���� ��������� */

   if(p_flag) {

     if(pars[2]==NULL) {
                           SEND_ERROR("������������ ������: GAS/P <��� ���������> <��������� �����������> <���������>") ;
                                 return(-1) ;
                       }

            kind=pars[0] ;
          sender=pars[1] ;
/*- - - - - - - - - - - - - - - - - - -  �������� �������� ��������� */
         value=strtod(pars[2], &end) ;
     if(*end!=0) {
                  SEND_ERROR("������������ �������� ��������") ;
                        return(-1) ;
                 }
/*- - - - - - - - - - - - - - - - - - - - - - - - ��������� �������� */
#define   PRICES       this->mMsgPrices 
#define   PRICES_CNT   this->mMsgPrices_cnt 

       for(i=0 ; i<PRICES_CNT ; i++)
         if(!stricmp(PRICES[i]->kind,   kind  ) &&
            !stricmp(PRICES[i]->sender, sender)   ) {

                        PRICES[i]->price=value ;
                                return(0) ;
                                                    }
/*- - - - - - - - - - - - - - - - - - �������� ��������� ����������� */
    do {

#define   NAMES       this->kernel->kernel_names         
#define   NAMES_CNT   this->kernel->kernel_names_cnt 

       for(i=0 ; i<NAMES_CNT ; i++)                                 /* �������� �� ����������� "Sender" */
         if(!stricmp(NAMES[i]->module, "Sender") &&
            !stricmp(NAMES[i]->name,    sender )   )  break ;

         if(i<NAMES_CNT)  break ;

#undef   NAMES
#undef   NAMES_CNT

#define   MODULES       this->kernel->modules 
#define   MODULES_CNT   this->kernel->modules_cnt 


       for(i=0 ; i<MODULES_CNT ; i++)                                  /* �������� �� ����� �������� */
         if(MODULES[i].entry->category!=NULL)  
          if(!stricmp(MODULES[i].entry->category,      "Object") &&
             !stricmp(MODULES[i].entry->identification, sender )   )  break ;

         if(i<MODULES_CNT)  break ;

#undef    MODULES
#undef    MODULES_CNT

             SEND_ERROR("K�������� ����������� ������ ���� �������������� ������ �� ����� ��������, "
                        "���� ���� ���������������� � ����������� SENDER (REFERENCE SENDER ...)"      ) ;
                                             return(-1) ;
       } while(0) ;
/*- - - - - - - - - - - - - - - - - - - - - - -  ���������� �������� */
        price=(Crowd_MGas *)calloc(1, sizeof(*price)) ;

       PRICES=(Crowd_MGas **)realloc(PRICES, (PRICES_CNT+1)*sizeof(*PRICES)) ;
    if(PRICES==NULL) {
                        SEND_ERROR("������������ ������") ;
                               return(-1) ;
                     }

                 PRICES[PRICES_CNT]=price ;
                        PRICES_CNT++ ;

          strcpy(price->kind,   kind) ;
          strcpy(price->sender, sender) ;
                 price->price=value ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
              }
/*-------------------------------------------- ������� ������ ������ */
   else       {

      if( pars[0]==NULL) {
                            SEND_ERROR("�� ������ �������� ���������: GAS 1 ��� GAS 0") ;
                                  return(-1) ;
                         }
      else
      if(*pars[0]== '0') {
                             this->mGasUse=0 ;
                         }
      else
      if(*pars[0]== '1') {
                             this->mGasUse=1 ;
                         }
      else               {
                            SEND_ERROR("�� ������ �������� ���������: GAS 1 ��� GAS 0") ;
                                  return(-1) ;
                         }

              }   
/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� RUN                     */
/*								    */
/*       RUN                                                        */

  int  Crowd_Module_Relay::cRun(char *cmd, Crowd_IFace *iface)

{
            int  debug_flag ;
   Crowd_Object *object ;
  Crowd_Message  init_msg ;
            int  status ;
           char  decl[1024] ;
           char  text[1024] ;
            int  exit_flag ;
         double  msg_gas  ;
            int  n ;
            int  i ;

#define   OBJECTS       Crowd_Kernel::kernel->kernel_objects
#define   OBJECTS_CNT   Crowd_Kernel::kernel->kernel_objects_cnt

#define   QUEUE         this->mQueue[0].messages 
#define   QUEUE_CNT     this->mQueue[0].cnt 

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                        debug_flag=0 ;

       if(*cmd=='/') {
 
                if(*cmd=='/')  cmd++ ;

//                 end=strchr(cmd, ' ') ;
//              if(end==NULL) {
//                     SEND_ERROR("������������ ������ �������") ;
//                                     return(-1) ;
//                            }
//                *end=0 ;

                if(strchr(cmd, 'd')!=NULL ||
                   strchr(cmd, 'D')!=NULL   ) debug_flag=1 ;

//                           cmd=end+1 ;
                     }
/*------------------------------------------------------- ���������� */

     for(i=0 ; i<OBJECTS_CNT ; i++)  OBJECTS[i]->vEventStart() ;

                 init_msg.Object_s=NULL ;
          strcpy(init_msg.Type, "system") ;
          strcpy(init_msg.Kind, "initialisation") ;

     for(i=0 ; i<OBJECTS_CNT ; i++)  OBJECTS[i]->vEvent(0, "MESSAGE", (void *)&init_msg, this) ;

/*----------------------------------------------------- ������� ���� */

       if(debug_flag)  this->debug_next=_CROWD_KERNEL_WAIT_STEP ;
       else            this->debug_next=    0 ;

   for(mStep=0 ; ; ) {

           if(this->debug_stop)  break ;                            /* ���� ������� ����������... */

           if(this->debug_next==_CROWD_KERNEL_WAIT_STEP) {          /* ���������� ��������� ������� */
                                     Sleep(100) ;
                                      continue ;
                                                         }
           if(this->debug_next==_CROWD_KERNEL_NEXT_STEP) {
                      this->debug_next=_CROWD_KERNEL_WAIT_STEP ;
                                                         }

                        mStep++ ;

                          sprintf(text, "=== Step %d  -  %d messages", mStep, this->mQueue[0].cnt) ; 
       if(mDebugDlg)  SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;

/*----------------------------------------------- ��������� �������� */

    if(this->kernel->show_time_step!=0) {

                Sleep(1000*this->kernel->show_time_step) ;

                                        }
/*----------------------------------- ������������ ��������� ������� */

     for(i=0 ; i<OBJECTS_CNT ; i++)  OBJECTS[i]->vEventSaveState() ;

/*---------------------------------------------- ��������� ��������� */

     for(n=0 ; n<this->mQueue[0].cnt ; n++) {

         if(mDebugDlg) {
                                   QUEUE[n]->vFormDecl(decl) ;
                          sprintf(text, "--- Message : %4d  -  %s ", n, decl) ; 
                      SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;
                       }
/*- - - - - - - - - - - - - - - -  ����������� ������� - ����������� */
       for(i=0 ; i<OBJECTS_CNT ; i++) {
              if(QUEUE[n]->Object_r!=NULL    )  object=QUEUE[n]->Object_r ;
         else if(QUEUE[n]->vCheck(OBJECTS[i]))  object=OBJECTS[i] ; 
         else                                     continue ;
/*- - - - - - - - - - - - - - - - - - - - - - - -  �������� �������� */
                  status=0 ;

      if(this->mGasUse)
       if(QUEUE[n]->Object_s!=NULL) do {

            msg_gas=this->MessageGas(QUEUE[n]) ;

         if(QUEUE[n]->Object_s->gas<msg_gas) {
                          sprintf(text, " Sender gas over for fee -  %s ", QUEUE[n]->Object_s->Name) ; 
                      SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;
                                               status=-1 ;
                                                  break ;
                                             }

            QUEUE[n]->Object_s->gas-=msg_gas ;

         if(QUEUE[n]->Object_r==NULL ||                             /* ���� �� ����� ���������� ��� ��� ��������� ��������... */
            QUEUE[n]->Gas     <= 0.    )  break ;

         if(QUEUE[n]->Object_s->gas<QUEUE[n]->Gas) {
                          sprintf(text, " Sender gas over for transfer -  %s ", QUEUE[n]->Object_s->Name) ; 
                      SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;
                                                       break ;
                                                   }

            QUEUE[n]->Object_s->gas-=QUEUE[n]->Gas ;
            QUEUE[n]->Object_r->gas+=QUEUE[n]->Gas ;

                                       } while(0) ;

         if(status)  break ;                                        /* ���� �� �������� ���������� ��������� ��������� ����������... */
/*- - - - - - - - - - - - - - - - - - - - - - -  ��������� ��������� */
         if(mDebugDlg) {
                          sprintf(text, " Object : %s", object->Name) ; 
                      SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;
                       }

            status=object->vEvent(mStep, "MESSAGE", (void *)QUEUE[n], this) ;
         if(status)  break ;

                   object->vEventShow() ;

         if(QUEUE[n]->Object_r!=NULL)  break ;
                                      }
/*- - - - - - - - - - - - - - - - - - - - - - - - �������� ��������� */
         if(QUEUE[n]->Object_r==NULL)  delete QUEUE[n] ;            /* ���� ��������� �� ���������� - ������� ��� */
                                              QUEUE[n]=NULL ;

         if(mQueueDlg)  SendMessage(this->mQueueDlg, WM_USER, (WPARAM)_USER_REFRESH, (LPARAM)NULL) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                            }

         if(status)  break ;

/*----------------------------------------------- ����������� ������ */

                               this->kernel->vShow(NULL) ;

/*------------------------------------------ ����� ������� ��������� */

                  free(this->mQueue[0].messages) ;                  /* ����������� ���� ������� ����� ������� */

                                             exit_flag=1 ;

      for(i=1 ; i<_QUEUE_DEEP ; i++) {

                this->mQueue[i-1]=this->mQueue[i] ;
             if(this->mQueue[i].cnt) exit_flag=0 ;
                                     }

           memset(&this->mQueue[_QUEUE_DEEP-1], 0,                  /* ���������� ��������� ���� ������� */
                           sizeof(this->mQueue[0])) ;

            if(exit_flag)  break ;

/*---------------------------------------------- ���������� �������� */

     if(this->mGasUse) {

      for(i=0 ; i<OBJECTS_CNT ; i++) {

        if(OBJECTS[i]->gas_renew>0                   &&
           OBJECTS[i]->gas      < OBJECTS[i]->gas_max  ) {

                                                  OBJECTS[i]->gas+=OBJECTS[i]->gas_renew ;
        if(OBJECTS[i]->gas> OBJECTS[i]->gas_max)  OBJECTS[i]->gas =OBJECTS[i]->gas_max  ;
                                                         }
                                     }
                       }
/*----------------------------------------------------- ������� ���� */
                     }

         if(mDebugDlg) {
                          sprintf(text, "=== Exit") ; 
                      SendMessage(this->mDebugDlg, WM_USER, (WPARAM)_USER_LOG, (LPARAM)text) ;
                       }

            SEND_INFO("������������� ���������") ;

/*-------------------------------------------------------------------*/

#undef    QUEUE
#undef    QUEUE_CNT

#undef    OBJECTS
#undef    OBJECTS_CNT

   return(0) ;
}


/********************************************************************/
/*								    */
/*		   ����� ������� �� �����                           */

  Crowd_Object *Crowd_Module_Relay::FindObject(char *name)

{
     char   text[1024] ;
      int   i ;

#define   OBJECTS       this->kernel->kernel_objects 
#define   OBJECTS_CNT   this->kernel->kernel_objects_cnt 

/*------------------------------------------- ����� ������� �� ����� */ 

       for(i=0 ; i<OBJECTS_CNT ; i++)                               /* ���� ������ �� ����� */
         if(!stricmp(OBJECTS[i]->Name, name))  break ;

    if(i==OBJECTS_CNT) {                                            /* ���� ��� �� �������... */
                           sprintf(text, "������� � ������ '%s' "
                                         "�� ����������", name) ;
                        SEND_ERROR(text) ;
                            return(NULL) ;
                       }
/*-------------------------------------------- �������� ���� ������� */ 

/*-------------------------------------------------------------------*/ 

   return((Crowd_Object *)OBJECTS[i]) ;
  
#undef   OBJECTS
#undef   OBJECTS_CNT

}


/********************************************************************/
/*								    */
/*		   ����� ��������� �� �����                         */

  Crowd_Message *Crowd_Module_Relay::FindMessage(char *name)

{
     char   text[1024] ;
      int   i ;

#define   MESSAGES       this->kernel->kernel_messages 
#define   MESSAGES_CNT   this->kernel->kernel_messages_cnt 

/*----------------------------------------- ����� ��������� �� ����� */ 

       for(i=0 ; i<MESSAGES_CNT ; i++)                              /* ���� ������ �� ����� */
         if(!stricmp(MESSAGES[i]->Name, name))  break ;

   if(i==MESSAGES_CNT) {                                            /* ���� ��� �� �������... */
                           sprintf(text, "��������� � ������ '%s' "
                                         "�� ����������", name) ;
                        SEND_ERROR(text) ;
                            return(NULL) ;
                       }
/*------------------------------------------ �������� ���� ��������� */ 

/*-------------------------------------------------------------------*/ 

   return((Crowd_Message *)MESSAGES[i]) ;
  
#undef   MESSAGES
#undef   MESSAGES_CNT

}


/********************************************************************/
/*								    */
/*              ����������� ������� �������� �� ���������           */

  double  Crowd_Module_Relay::MessageGas(Crowd_Message *msg)

{
  int  i ;

#define   PRICES       this->mMsgPrices 
#define   PRICES_CNT   this->mMsgPrices_cnt 


    if(msg->Object_s==NULL)  return(0.) ;

  for(i=0 ; i<PRICES_CNT ; i++)
    if(!stricmp(PRICES[i]->kind,   msg->Kind                ) &&
       !stricmp(PRICES[i]->sender, msg->Object_s->SenderType)   ) {

                  return(PRICES[i]->price) ;
                                                                  }

#undef   PRICES
#undef   PRICES_CNT


    return(1.) ;
}
