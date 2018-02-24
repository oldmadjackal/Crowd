/********************************************************************/
/*								    */
/*		������ ���������� �������� "�������"  		    */
/*								    */
/********************************************************************/


#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <malloc.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include <sys\stat.h>

#include "..\Crowd_Feature\Crowd_Feature.h"
#include "..\Crowd_Object\Crowd_Object.h"
#include "..\Crowd_Kernel\Crowd_Kernel.h"
#include "..\F_Show\F_Show.h"

#include "..\DCL_kernel\dcl.h"

#include "O_Human.h"

#pragma warning(disable : 4996)

#define  SEND_ERROR(text)    SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,        \
                                         (LPARAM) text)

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

     static   Crowd_Module_Human  ProgramModule ;


/********************************************************************/
/*								    */
/*		    	����������������� ����                      */

 O_HUMAN_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 O_HUMAN_API Crowd_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/********************************************************************/
/********************************************************************/
/**							           **/
/**	  �������� ������ ������ ���������� �������� "�������"	   **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*                            ������ ������                         */

  struct Crowd_Module_Human_instr  Crowd_Module_Human_InstrList[]={

 { "help",   "?",  "#HELP   - ������ ��������� ������", 
                    NULL,
                   &Crowd_Module_Human::cHelp   },
 { "create", "cr", "#CREATE - ������� ������",
                   " CREATE <���> [<������> [<������ ����������>]]\n"
                   "   ������� ����������� ������ �� ����������������� ������",
                   &Crowd_Module_Human::cCreate },
 { "info",   "i",  "#INFO - ������ ���������� �� �������",
                   " INFO <���> \n"
                   "   ������ �������� ��������� �� ������� � ������� ����\n"
                   " INFO/ <���> \n"
                   "   ������ ������ ���������� �� ������� � ��������� ����",
                   &Crowd_Module_Human::cInfo },
 { "base",   "b", "#BASE - ������ ������� ����� �������",
                   " BASE <���> <x> <y> <z>\n"
                   "   ������ ������� ����� �������\n"
                   " BASE/x <���> <x>\n"
                   "   ������ ���������� X ������� ����� �������\n"
                   "       (���������� ��� Y � Z)\n"
                   " BASE/+x <���> <x>\n"
                   "   ������ ���������� ���������� X ������� ����� �������\n"
                   "       (���������� ��� Y � Z)\n"
                   " BASE> <���>\n"
                   "   ������ ������������ ���������� ������� ������ �������\n",
                   &Crowd_Module_Human::cBase },
 { "color",   "c", "#COLOR   - ���������� ���� �������", 
                   " COLOR <���> <�������� �����>\n"
                   "   ���������� ���� ������� �� ��������: RED, GREEN, BLUE\n"
                   " COLOR <���> RGB <R-������>:<G-������>:<B-������>\n"
                   "   ���������� ���� ������� �� RGB-�����������\n",
                   &Crowd_Module_Human::cColor      },
 { "visible", "v", "#VISIBLE - ������� ������ ��������� �������",
                   " VISIBLE <���> \n"
                   "   �������� ��������� ��������� ������� �� ���������������",
                   &Crowd_Module_Human::cVisible },
 { "program", "p", "#PROGRAM - ������� ��������� ��������� �������",
                   " PROGRAM <���> <���� � ����� ���������>\n"
                   "   ��������� ��������� ������� ��������� � �����",
                   &Crowd_Module_Human::cProgram },
 {  NULL }
                                                            } ;

/********************************************************************/
/*								    */
/*		     ����� ����� ������             		    */

    struct Crowd_Module_Human_instr *Crowd_Module_Human::mInstrList=NULL ;

/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     Crowd_Module_Human::Crowd_Module_Human(void)

{
	   keyword="Crowd" ;
    identification="Human" ;
          category="Object" ;


        mInstrList=Crowd_Module_Human_InstrList ;
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

    Crowd_Module_Human::~Crowd_Module_Human(void)

{
}


/********************************************************************/
/*								    */
/*		      �������� �������                              */

  Crowd_Object *Crowd_Module_Human::vCreateObject(Crowd_Model_data *data)

{
  Crowd_Object_Human *object ;
                char  models_list[4096] ;
                char *end ;
                 int  i ;
                 int  j ;

#define   OBJECTS       this->kernel->kernel_objects 
#define   OBJECTS_CNT   this->kernel->kernel_objects_cnt 

#define       PAR             object->Parameters
#define       PAR_CNT         object->Parameters_cnt
 
/*--------------------------------------------------- �������� ����� */

    if(data->name[0]==0) {                                           /* ���� ��� �� ������ */
              SEND_ERROR("������ HUMAN: �� ������ ��� �������") ;
                                return(NULL) ;
                         }

       for(i=0 ; i<OBJECTS_CNT ; i++)
         if(!stricmp(OBJECTS[i]->Name, data->name)) {
              SEND_ERROR("������ HUMAN: ������ � ����� ������ ��� ����������") ;
                                return(NULL) ;
                                                    }
/*-------------------------------------- ���������� �������� ������� */
/*- - - - - - - - - - - - ���� ������ ������ ��������� � ����������� */
   if(data->path[0]==0) {

    if(data->model[0]==0) {                                         /* ���� ������ �� ������ */
              SEND_ERROR("������ HUMAN: �� ������ ������ �������") ;
                                return(NULL) ;
                          }

        Crowd_Model_list(data->lib_path, models_list,               /* ������������ ������ ������� */
                                  sizeof(models_list)-1, "BODY" );

        for(end=models_list ; *end ; ) {                            /* ���� ������ �� ������ */
                        if(!stricmp(data->model, end))  break ;
                                         end+=strlen(end)+1 ;
                                         end+=strlen(end)+1 ;
                                       }

           if(*end==0) {
              SEND_ERROR("������ HUMAN: ����������� ������ ����") ;
                                return(NULL) ;
                       }

                                    end+=strlen(end)+1 ;            /* ��������� ��� ����� */

                      sprintf(data->path, "%s\\%s", data->lib_path, end) ;
         Crowd_Model_ReadPars(data) ;                               /* ��������� ��������� ������ */
                        }
/*- - - - - - - - - - - - - - - - -  ���� ������ ������ ������ ����� */
   else                 {
                            Crowd_Model_ReadPars(data) ;            /* ��������� ��������� ������ */
                        }
/*--------------------------------------- �������� ������ ���������� */

     for(i=0 ; i<5 ; i++)
       if((data->pars[i].text [0]==0 &&
           data->pars[i].value[0]!=0   ) ||
          (data->pars[i].text [0]!=0 &&
           data->pars[i].value[0]==0   )   ) {

              SEND_ERROR("������ HUMAN: �������������� ����� ���������� ������") ;
                                return(NULL) ;
                                             }
/*------------------------------------------------- �������� ������� */

       object=new Crowd_Object_Human ;
    if(object==NULL) {
              SEND_ERROR("������ HUMAN: ������������ ������ ��� �������� �������") ;
                        return(NULL) ;
                     }
/*------------------------------------- ���������� ������ ���������� */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������� ��������� */
     for(i=0 ; i<5 ; i++)
       if(data->pars[i].text[0]!=0) {

           PAR=(struct Crowd_Parameter *)
                 realloc(PAR, (PAR_CNT+1)*sizeof(*PAR)) ;
        if(PAR==NULL) {
                         SEND_ERROR("������ HUMAN: ������������ ������") ;
                                            return(NULL) ;
                      }

             memset(&PAR[PAR_CNT], 0, sizeof(PAR[PAR_CNT])) ;
            sprintf( PAR[PAR_CNT].name, "PAR%d", i+1) ;
                     PAR[PAR_CNT].value=strtod(data->pars[i].value, &end) ;
                     PAR[PAR_CNT].ptr  = NULL ;
                         PAR_CNT++ ;
                                    }
/*- - - - - - - - - - - - - - - - - - - - ����������� ������ ������� */
           PAR=(struct Crowd_Parameter *)
                 realloc(PAR, (PAR_CNT+1)*sizeof(*PAR)) ;
        if(PAR==NULL) {
                         SEND_ERROR("������ HUMAN: ������������ ������") ;
                                            return(NULL) ;
                      }

             memset(&PAR[PAR_CNT], 0, sizeof(PAR[PAR_CNT])) ;

/*---------------------------------- �������� ������ ������� ������� */

      object->Features_cnt=this->feature_modules_cnt ;
      object->Features    =(Crowd_Feature **)
                             calloc(this->feature_modules_cnt, 
                                     sizeof(object->Features[0])) ;

   for(i=0 ; i<this->feature_modules_cnt ; i++)
      object->Features[i]=this->feature_modules[i]->vCreateFeature(object, NULL) ;

/*-------------------------------------- ���������� �������� ������� */

           Crowd_Model_ReadSect(data) ;                             /* ��������� ������ �������� ������ */

   for(i=0 ; data->sections[i].title[0] ; i++) {

     for(j=0 ; j<object->Features_cnt ; j++) {

          object->Features[j]->vBodyPars(NULL, PAR) ;
          object->Features[j]->vReadSave(data->sections[i].title, 
                                         data->sections[i].decl, "Human.Body") ;
                                             }

                                         data->sections[i].title[0]= 0 ;
                                        *data->sections[i].decl    ="" ;
                                               }
/*---------------------------------- �������� ������� � ����� ������ */

       OBJECTS=(Crowd_Object **)
                 realloc(OBJECTS, (OBJECTS_CNT+1)*sizeof(*OBJECTS)) ;
    if(OBJECTS==NULL) {
              SEND_ERROR("������ HUMAN: ������������ ������") ;
                                return(NULL) ;
                      }

              OBJECTS[OBJECTS_CNT]=object ;
                      OBJECTS_CNT++ ;

       strcpy(object->Name,       data->name) ;
       strcpy(object->model_path, data->path) ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_DEFAULT_OBJECT, (LPARAM)data->name) ;

/*-------------------------------------------------------------------*/

#undef   OBJECTS
#undef   OBJECTS_CNT

#undef   PAR
#undef   PAR_CNT

  return(object) ;
}


/********************************************************************/
/*								    */
/*		        �������� ��������       		    */

     int  Crowd_Module_Human::vGetParameter(char *name, char *value)

{
/*-------------------------------------------------- �������� ������ */

    if(!stricmp(name, "$$MODULE_NAME")) {

         sprintf(value, "%-20.20s -  ������� (����� ������)", identification) ;
                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*		        ��������� �������       		    */

  int  Crowd_Module_Human::vExecuteCmd(const char *cmd)

{
  static  int  direct_command ;   /* ���� ������ ������ ������� */
         char  command[1024] ;
         char *instr ;
         char *end ;
          int  status ;
          int  i ;

#define  _SECTION_FULL_NAME   "HUMAN"
#define  _SECTION_SHRT_NAME   "HUMAN"

/*--------------------------------------------- ������������� ������ */

             memset(command, 0, sizeof(command)) ;
            strncpy(command, cmd, sizeof(command)-1) ;

   if(!direct_command) {

         end=strchr(command, ' ') ;
      if(end!=NULL) {  *end=0 ;  end++ ;  }

      if(stricmp(command, _SECTION_FULL_NAME) &&
         stricmp(command, _SECTION_SHRT_NAME)   )  return(1) ;
                       }
   else                {
                             end=command ;
                       }
/*----------------------- ���������/���������� ������ ������ ������� */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - -  ��������� */
   if(end==NULL || end[0]==0) {
     if(!direct_command) {
                            direct_command=1 ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"Object Human:") ;
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
                     *end!='/'    ; end++) ;

      if(*end!= 0 &&
         *end!=' '  )  memmove(end+1, end, strlen(end)+1) ;

      if(*end!=0) {  *end=0 ;  end++ ;  }
      else            end="" ;

   for(i=0 ; mInstrList[i].name_full!=NULL ; i++)                   /* ���� ������� � ������ */
     if(!stricmp(instr, mInstrList[i].name_full) ||
        !stricmp(instr, mInstrList[i].name_shrt)   )   break ;

     if(mInstrList[i].name_full==NULL) {                            /* ���� ����� ������� ���... */

          status=this->kernel->vExecuteCmd(cmd) ;                   /*  �������� �������� ������ ����... */
       if(status)  SEND_ERROR("������ HUMAN: ����������� �������") ;
                                            return(-1) ;
                                       }
 
     if(mInstrList[i].process!=NULL)                                /* ���������� ������� */
                status=(this->*mInstrList[i].process)(end) ;
     else       status=  0 ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*								    */
/*		        ������� ������ �� ������		    */

    void  Crowd_Module_Human::vReadSave(std::string *data)

{
                char *buff ;
                 int  buff_size ;
    Crowd_Model_data  create_data ;
  Crowd_Object_Human *object ;
                char  name[128] ;
                char *entry ;
                char *end ;
                 int  i ;

/*----------------------------------------------- �������� ��������� */

   if(memicmp(data->c_str(), "#BEGIN MODULE HUMAN\n", 
                      strlen("#BEGIN MODULE HUMAN\n")) &&
      memicmp(data->c_str(), "#BEGIN OBJECT HUMAN\n", 
                      strlen("#BEGIN OBJECT HUMAN\n"))   )  return ;

/*------------------------------------------------ ���������� ������ */

              buff_size=data->size()+16 ;
              buff     =(char *)calloc(1, buff_size) ;

       strcpy(buff, data->c_str()) ;

/*------------------------------------------------- �������� ������� */

   if(!memicmp(buff, "#BEGIN OBJECT HUMAN\n", 
              strlen("#BEGIN OBJECT HUMAN\n"))) {                    /* IF.1 */
/*- - - - - - - - - - - - - - - - - - - - - -  ���������� ���������� */
              memset(&create_data, 0, sizeof(create_data)) ;

                                     entry=strstr(buff, "NAME=") ;  /* ��������� ��� ������� */
           strncpy(create_data.name, entry+strlen("NAME="), 
                                       sizeof(create_data.name)-1) ;
        end=strchr(create_data.name, '\n') ;
       *end= 0 ;

                                     entry=strstr(buff, "MODEL=") ; /* ��������� ������ ������� */
           strncpy(create_data.path, entry+strlen("MODEL="),
                                       sizeof(create_data.path)-1) ;
        end=strchr(create_data.path, '\n') ;
       *end= 0 ;

    for(i=0 ; i<_MODEL_PARS_MAX ; i++) {
             sprintf(name, "PAR_%d=", i) ;
        entry=strstr(buff, name) ;
     if(entry!=NULL) {
           strncpy(create_data.pars[i].value, entry+strlen(name), 
                                        sizeof(create_data.pars[i].value)-1) ;
        end=strchr(create_data.pars[i].value, '\n') ;
       *end= 0 ;
                     }
                                       } 
/*- - - - - - - - - - - - - - - �������� ���������� �������� ������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - �������� ������� */
              object=(Crowd_Object_Human *)vCreateObject(&create_data) ;
           if(object==NULL)  return ;
/*- - - - - - - - - - - - ������� ������� ����� � ���������� ������� */
       entry=strstr(buff, "X_BASE=") ; object->x_base=atof(entry+strlen("X_BASE=")) ;
       entry=strstr(buff, "Y_BASE=") ; object->y_base=atof(entry+strlen("Y_BASE=")) ;
       entry=strstr(buff, "Z_BASE=") ; object->z_base=atof(entry+strlen("Z_BASE=")) ;

   for(i=0 ; i<object->Features_cnt ; i++) {
        object->Features[i]->vBodyBasePoint(NULL, object->x_base, 
                                                  object->y_base, 
                                                  object->z_base ) ;
                                           }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                               }                    /* END.1 */
/*-------------------------------------------- ������������ �������� */

                free(buff) ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*		        �������� ������ � ������		    */

    void  Crowd_Module_Human::vWriteSave(std::string *text)

{
  std::string  buff ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN MODULE HUMAN\n" ;

/*------------------------------------------------ �������� �������� */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� HELP                    */

  int  Crowd_Module_Human::cHelp(char *cmd)

{ 
    DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_HELP", RT_DIALOG),
			   GetActiveWindow(), Object_Human_Help_dialog) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� CREATE                  */
/*								    */
/*      CREATE <���> [<������> [<������ ����������>]]               */

  int  Crowd_Module_Human::cCreate(char *cmd)

{
 Crowd_Model_data  data ;
     Crowd_Object *object ;
             char *name ;
             char *model ;
             char *pars[10] ;
             char *end ;
             char  tmp[1024] ;
              int  status ;
              int  i ;

/*-------------------------------------- ���������� ��������� ������ */

                             model ="" ;
     for(i=0 ; i<10 ; i++)  pars[i]="" ;

   do {                                                             /* BLOCK.1 */
                  name=cmd ;                                        /* ��������� ��� ������� */
                   end=strchr(name, ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;

                 model=end+1 ;                                      /* ��������� �������� ������ */
                   end=strchr(model, ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
          
     for(i=0 ; i<10 ; i++) {                                        /* ��������� ��������� */
               pars[i]=end+1 ;            
                   end=strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                           }
      } while(0) ;                                                  /* BLOCK.1 */

/*--------------------------------- ���������� ����� ������ �������� */

         memset(&data, 0, sizeof(data)) ;

        strncpy(data.name,  name,  sizeof(data.name)) ;
        strncpy(data.model, model, sizeof(data.model)) ;

    for(i=0 ; *pars[i]!=0 ; i++) {
        strncpy(data.pars[i].value,  pars[i], sizeof(data.pars[i].value)) ;
         strcpy(data.pars[i].text, "") ;
                                 }

        sprintf(data.lib_path, "%s\\Body.lib", getcwd(tmp, sizeof(tmp))) ;

/*---------------------- �������� ������������� ��������� ���������� */

   if(data.name[0]!=0) {
                            object=vCreateObject(&data) ;
                         if(object!=NULL)  return(0) ;
                       }
/*----------------------------------------------- ���������� ������� */

      status=DialogBoxIndirectParam( GetModuleHandle(NULL),
                                    (LPCDLGTEMPLATE)Resource("IDD_CREATE", RT_DIALOG),
			             GetActiveWindow(), 
                                     Object_Human_Create_dialog, 
                                    (LPARAM)&data               ) ;
   if(status)  return(status) ;

            this->kernel->vShow(NULL) ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� INFO                    */
/*								    */
/*        INFO   <���>                                              */
/*        INFO/  <���>                                              */

  int  Crowd_Module_Human::cInfo(char *cmd)

{
                char  *name ;
  Crowd_Object_Human  *object ;
                 int   all_flag ;   /* ���� ������ ������ ���������� */
                char  *end ;
         std::string   info ;
         std::string   f_info ;
                char   text[4096] ;
                 int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                           all_flag=0 ;

       if(*cmd=='/') {
                           all_flag=1 ;
 
                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }

                           cmd=end+1 ;
                        }
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */
                  name=cmd ;
                   end=strchr(name, ' ') ;
                if(end!=NULL)  *end=0 ;

/*------------------------------------------- �������� ����� ������� */

    if(name   ==NULL ||
       name[0]==  0    ) {                                          /* ���� ��� �� ������... */
                           SEND_ERROR("�� ������ ��� �������. \n"
                                      "��������: INFO <���_�������> ...") ;
                                     return(-1) ;
                         }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*-------------------------------------------- ������������ �������� */

      sprintf(text, "%s\r\n%s\r\n"
                    "Base X % 7.3lf\r\n" 
                    "     Y % 7.3lf\r\n" 
                    "     Z % 7.3lf\r\n"
                    "\r\n",
                        object->Name, object->Type, 
                        object->x_base, object->y_base, object->z_base
                    ) ;

           info=text ;

/*----------------------------------------------- ������ �� �������� */

   for(i=0 ; i<object->Features_cnt ; i++) {

                      object->Features[i]->vGetInfo(&f_info) ;
                                               info+=f_info ;
                                               info+="\r\n" ;
                                           }
/*-------------------------------------------------- ������ �������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - � ��������� ���� */
     if(all_flag) {

                  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - � ������� ���� */
     else         {

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_SHOW_INFO, (LPARAM)info.c_str()) ;
                  }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� BASE                    */
/*								    */
/*        BASE    <���> <X> <Y> <Z>                                 */
/*        BASE/X  <���> <X>                                         */
/*        BASE/+X <���> <X>                                         */
/*        BASE>   <���> <��� ���������> <���>                       */
/*        BASE>>  <���> <��� ���������> <���>                       */

  int  Crowd_Module_Human::cBase(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

                char  *pars[_PARS_MAX] ;
                char  *name ;
                char **xyz ;
              double   coord[_COORD_MAX] ;
                 int   coord_cnt ;
              double   inverse ;
  Crowd_Object_Human  *object ;
                 int   xyz_flag ;          /* ���� ������ ����� ���������� */
                 int   delta_flag ;        /* ���� ������ ���������� */
                 int   arrow_flag ;        /* ���� ����������� ������ */
                char  *arrows ;
                char  *error ;
                char  *end ;
                 int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                        xyz_flag=0 ;
                      delta_flag=0 ;
                      arrow_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, '+')!=NULL   )  delta_flag= 1 ;

                if(strchr(cmd, 'x')!=NULL ||
                   strchr(cmd, 'X')!=NULL   )    xyz_flag='X' ;
           else if(strchr(cmd, 'y')!=NULL ||
                   strchr(cmd, 'Y')!=NULL   )    xyz_flag='Y' ;
           else if(strchr(cmd, 'z')!=NULL ||
                   strchr(cmd, 'Z')!=NULL   )    xyz_flag='Z' ;

                           cmd=end+1 ;
                        }

  else if(*cmd=='>') {
                           delta_flag=1 ;
                           arrow_flag=1 ;

                          cmd=strchr(cmd, ' ') ;
                       if(cmd==NULL)  return(0) ;
                          cmd++ ;
                     } 
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name= pars[0] ;
                      xyz=&pars[1] ;   
                  inverse=   1. ; 

/*------------------------------------- ��������� ����������� ������ */

    if(arrow_flag) {                        
                         arrows=pars[1] ;

      if(strstr(arrows, "left" )!=NULL   ) {  xyz_flag='X' ;  inverse=-1. ;  }
      if(strstr(arrows, "right")!=NULL   ) {  xyz_flag='X' ;  inverse= 1. ;  }  
      if(strstr(arrows, "up"   )!=NULL &&
         strstr(arrows, "ctrl" )==NULL   ) {  xyz_flag='Y' ;  inverse= 1. ;  }
      if(strstr(arrows, "down" )!=NULL &&
         strstr(arrows, "ctrl" )==NULL   ) {  xyz_flag='Y' ;  inverse=-1. ;  }
      if(strstr(arrows, "up"   )!=NULL &&
         strstr(arrows, "ctrl" )!=NULL   ) {  xyz_flag='Z' ;  inverse= 1. ;  }
      if(strstr(arrows, "down" )!=NULL &&
         strstr(arrows, "ctrl" )!=NULL   ) {  xyz_flag='Z' ;  inverse=-1. ;  }
           
                          xyz=&pars[2] ;
                   }
/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: BASE <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*------------------------------------------------- ������ ��������� */

    for(i=0 ; xyz[i]!=NULL && i<_COORD_MAX ; i++) {

             coord[i]=strtod(xyz[i], &end) ;
        if(*end!=0) {  
                       SEND_ERROR("������������ �������� ����������") ;
                                       return(-1) ;
                    }
                                                  }

                             coord_cnt=  i ;

                                 error= NULL ;
      if(xyz_flag) {
               if(coord_cnt==0)  error="�� ������� ���������� ��� �� ����������" ;
                   }
      else         {
               if(coord_cnt <3)  error="������ ���� ������� 3 ����������" ;
                   }

      if(error!=NULL) {  SEND_ERROR(error) ;
                               return(-1) ;   }

      if(arrow_flag && coord_cnt>1)  coord[0]=coord[coord_cnt-1] ;  /* ��� ����������� ������ ����� � �������� ���� */
                                                                    /*  ����� ��������� ��������                    */
/*------------------------------------------------ ������� ��������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - ���������� */
   if(delta_flag) {

          if(xyz_flag=='X')   object->x_base+=inverse*coord[0] ;
     else if(xyz_flag=='Y')   object->y_base+=inverse*coord[0] ;                 
     else if(xyz_flag=='Z')   object->z_base+=inverse*coord[0] ;
                  }
/*- - - - - - - - - - - - - - - - - - - - - - -  ���������� �������� */
   else           {

          if(xyz_flag=='X')   object->x_base=coord[0] ;
     else if(xyz_flag=='Y')   object->y_base=coord[0] ;                 
     else if(xyz_flag=='Z')   object->z_base=coord[0] ;
     else                   {
                              object->x_base=coord[0] ;
                              object->y_base=coord[1] ;
                              object->z_base=coord[2] ;
                            }
                  }
/*---------------------------------------------- ������� �� �������� */

   for(i=0 ; i<object->Features_cnt ; i++)
     object->Features[i]->vBodyBasePoint("Human.Body", object->x_base, 
                                                       object->y_base, 
                                                       object->z_base ) ;

/*------------------------------------------------------ ����������� */

                      this->kernel->vShow(NULL) ;

/*-------------------------------------------------------------------*/

#undef  _COORD_MAX   
#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*                   ���������� ���������� Color                    */
/*       COLOR  <���> <�������� �����>                              */
/*       COLOR  <���> RGB <R-������>:<G-������>:<B-������>          */

  int  Crowd_Module_Human::cColor(char *cmd)

{ 
#define   _PARS_MAX   4
                 char *pars[_PARS_MAX] ;
                 char *name ;
   Crowd_Object_Human  *object ;
             COLORREF  color ;
                  int  red, green, blue ;
                 char *end ;
                  int  i ;
                  int  j ;

#define   OBJECTS       this->kernel->kernel_objects 
#define   OBJECTS_CNT   this->kernel->kernel_objects_cnt 

/*------------------------------------------------ ������ ���������� */        

    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name=pars[0] ;

/*----------------------------------------------------- ������ ����� */

     if(pars[1]==NULL) {
                         SEND_ERROR("�� ������ ���� �������. \n"
                                    "��������: COLOR <���_�������> GREEN") ;
                                        return(-1) ;
                       } 

              if(!stricmp(pars[1], "RED"  ))  color=RGB(127,   0,   0) ;
         else if(!stricmp(pars[1], "GREEN"))  color=RGB(  0, 127,   0) ;
         else if(!stricmp(pars[1], "BLUE" ))  color=RGB(  0,   0, 127) ;
         else if(!stricmp(pars[1], "RGB"  )) {

                 if(pars[2]!=NULL) {
                                       green=0 ;
                                        blue=0 ;
                                         red=strtoul(pars[2], &end, 10) ;
                         if(*end==':') green=strtoul(end+1  , &end, 10) ;
                         if(*end==':')  blue=strtoul(end+1  , &end, 10) ;

                                       color=RGB(red, green, blue) ;                                          
                                   }
                                             } 
         else                                {
                         SEND_ERROR("����������� �������� �����") ;
                                        return(-1) ;
                                             }
/*------------------------------------------- ����� ������� �� ����� */ 

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: COLOR <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*---------------------------------- ���������� ����������� �������� */

    for(j=0 ; j<object->Features_cnt ; j++)
      if(!stricmp(object->Features[j]->Type, "Show"))  
            ((Crowd_Feature_Show *)object->Features[j])->Color=color ;

/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� VISIBLE                 */
/*								    */
/*        VISIBLE  <���>                                            */

  int  Crowd_Module_Human::cVisible(char *cmd)

{
#define   _PARS_MAX   4

                 char  *name ;
   Crowd_Object_Human  *object ;
                 char  *end ;
                  int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */
                  name=cmd ;
                   end=strchr(name, ' ') ;
                if(end!=NULL)  *end=0 ;

/*------------------------------------------- �������� ����� ������� */

    if(name   ==NULL ||
       name[0]==  0    ) {                                          /* ���� ��� �� ������... */
                           SEND_ERROR("�� ������ ��� �������. \n"
                                      "��������: VISIBLE <���_�������> ...") ;
                                     return(-1) ;
                         }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*------------------------------------ ��������� ��������� ��������� */

     for(i=0 ; i<this->feature_modules_cnt ; i++)
        if(!stricmp(object->Features[i]->Type, "Show")) {
             object->Features[i]->vParameter("VISIBLE", "INVERT", NULL) ;
                            break ;
                                                        }
/*------------------------------------------------------ ����������� */

                      this->kernel->vShow(NULL) ;

/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*                   ���������� ���������� Program                  */
/*       PROGRAM <���> <���� � ����� ���������>                     */

  int  Crowd_Module_Human::cProgram(char *cmd)

{ 
#define   _PARS_MAX   4
                 char *pars[_PARS_MAX] ;
                 char *name ;
                 char *path ;
   Crowd_Object_Human  *object ;
                 char *data ;
                 char *end ;
                 char  error[1024] ;
                 char  text[1024] ;
                  int  i ;

/*------------------------------------------------ ������ ���������� */        

    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name=pars[0] ;
                     path=pars[1] ;

/*------------------------------------------- ����� ������� �� ����� */ 

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: PROGRAM <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*----------------------------------------- �������� ����� ��������� */

     if(path==NULL) {
                       SEND_ERROR("�� ����� ���� ���������. \n"
                                  "��������: PROGRAM <��� �������> <���� � ����� ���������>") ;
                                        return(-1) ;
                    } 

        data=this->FileCache(path, error) ;
     if(data==NULL) {
                         sprintf(text, "������ �������� ����� ��������� - %s", error) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                    }

                 object->Program=data ;

/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*		   ����� ������� ���� HUMAN �� �����                */

  Crowd_Object_Human *Crowd_Module_Human::FindObject(char *name)

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

     if(strcmp(OBJECTS[i]->Type, "Human")) {

           SEND_ERROR("������ �� �������� �������� ���� HUMAN") ;
                            return(NULL) ;
                                          }
/*-------------------------------------------------------------------*/ 

   return((Crowd_Object_Human *)OBJECTS[i]) ;
  
#undef   OBJECTS
#undef   OBJECTS_CNT

}


/********************************************************************/
/********************************************************************/
/**							           **/
/**		  �������� ������ ������� "�������"	           **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     Crowd_Object_Human::Crowd_Object_Human(void)

{
   strcpy(Type, "Human") ;
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

    Crowd_Object_Human::~Crowd_Object_Human(void)

{
}


/********************************************************************/
/*								    */
/*		       ������������ ��������                        */

  void   Crowd_Object_Human::vFree(void)

{
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++) {
               this->Features[i]->vBodyDelete(NULL) ;
          free(this->Features[i]) ;
                                         }

          free(this->Features) ;
}





/********************************************************************/
/*								    */
/*		        �������� ������ � ������		    */

    void  Crowd_Object_Human::vWriteSave(std::string *text)

{
  char  field[1024] ;
   int  i ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN OBJECT HUMAN\n" ;

/*----------------------------------------------------------- ������ */

    sprintf(field, "NAME=%s\n",       this->Name      ) ;  *text+=field ;
    sprintf(field, "X_BASE=%.10lf\n", this->x_base    ) ;  *text+=field ;
    sprintf(field, "Y_BASE=%.10lf\n", this->y_base    ) ;  *text+=field ;
    sprintf(field, "Z_BASE=%.10lf\n", this->z_base    ) ;  *text+=field ;
    sprintf(field, "MODEL=%s\n",      this->model_path) ;  *text+=field ;

  for(i=0 ; i<this->Parameters_cnt ; i++) {
    sprintf(field, "PAR_%d=%.10lf\n", 
                          i, this->Parameters[i].value) ;  *text+=field ;
                                          }
/*------------------------------------------------ �������� �������� */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*                  ���������� ��������� �������                    */

     int  Crowd_Object_Human::vEventStart(void)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*                   ��������� �������                              */

     static         Crowd_Kernel *EventTask ;
     static   Crowd_Object_Human *EventObject ;


     int  Crowd_Object_Human::vEvent(long  t, char *type, void  *data, Crowd_Kernel *task)
{
     Crowd_Kernel *dcl ;
    Crowd_Message *message ;
  Crowd_Parameter  const_list[5] ;
             void *context ;
         COLORREF *color ;
              int  color_r, color_g, color_b ;
             char  error[1024] ;
              int  status ;
              int  i ;
              int  j ;

             char  msg_name[128] ;
             char  msg_type[128] ;
             char  msg_kind[128] ;
             char  msg_sender[128] ;
             char  obj_color[128] ;

    Dcl_decl *Human_dcl_Log      (Lang_DCL *,             Dcl_decl **, int) ;    /* ������ � ��� */
    Dcl_decl *Human_dcl_ObjectXYZ(Lang_DCL *,             Dcl_decl **, int) ;    /* ������ ��������� ������� */
    Dcl_decl *Human_dcl_GetLinks (Lang_DCL *, Dcl_decl *, Dcl_decl **, int) ;    /* ��������� ������ ������ */
    Dcl_decl *Human_dcl_SendMsg  (Lang_DCL *,             Dcl_decl **, int) ;    /* �������� ��������� */
    Dcl_decl *Human_dcl_Recall   (Lang_DCL *,             Dcl_decl **, int) ;    /* ��������� ������� */

    Dcl_decl  dcl_human_lib[]={
         {0, 0, 0, 0, "$PassiveData$", NULL, "human", 0, 0},
	 {_CHR_AREA, 0,          0, 0, "$MsgName",            msg_name,            NULL,   0, sizeof(msg_name  )},
	 {_CHR_AREA, 0,          0, 0, "$MsgType",            msg_type,            NULL,   0, sizeof(msg_type  )},
	 {_CHR_AREA, 0,          0, 0, "$MsgKind",            msg_kind,            NULL,   0, sizeof(msg_kind  )},
	 {_CHR_AREA, 0,          0, 0, "$MsgSender",          msg_sender,          NULL,   0, sizeof(msg_sender)},
	 {_CHR_AREA, 0,          0, 0, "$ThisColor",          obj_color,           NULL,   0, sizeof(obj_color )},
	 {_DGT_VAL,  0,          0, 0, "$ThisX",             &this->x_base,        NULL,   1,   1               },
	 {_DGT_VAL,  0,          0, 0, "$ThisY",             &this->y_base,        NULL,   1,   1               },
	 {_DGT_VAL,  0,          0, 0, "$ThisZ",             &this->z_base,        NULL,   1,   1               },
 	 {_CHR_PTR, _DCL_CALL,   0, 0, "Log",         (void *)Human_dcl_Log,       "s",    0,   0               },
 	 {_DGT_VAL, _DCL_CALL,   0, 0, "ObjectXYZ",   (void *)Human_dcl_ObjectXYZ, "ss",   0,   0               },
 	 {_DGT_VAL, _DCL_METHOD, 0, 0, "GetLinks",    (void *)Human_dcl_GetLinks,  "ssss", 0,   0               },
 	 {_DGT_VAL, _DCL_CALL,   0, 0, "SendMessage", (void *)Human_dcl_SendMsg,   "ssss", 0,   0               },
 	 {_DGT_VAL, _DCL_CALL,   0, 0, "Recall",      (void *)Human_dcl_Recall,    "s",    0,   0               },
	 {0, 0, 0, 0, "", NULL, NULL, 0, 0}
                              } ;

/*-------------------------------------- ����������� DCL-����������� */

#define  CALC_CNT   Crowd_Kernel::calculate_modules_cnt
#define  CALC       Crowd_Kernel::calculate_modules

                      dcl=NULL ;

         for(i=0 ; i<CALC_CNT ; i++) {

             status=CALC[i]->vCalculate("DCL", NULL, NULL, NULL, 
                                               NULL, NULL, NULL ) ;
         if(status==0)  dcl=CALC[i] ;
                                     }

          if(dcl==NULL) {
               SEND_ERROR("Section HUMAN: �� �������� ��������� DCL-���������") ;
                            return(-1) ;
                        }

#undef   CALC_CNT
#undef   CALC

/*------------------------------------------------ ���������� ������ */

        memset(const_list, 0, sizeof(const_list)) ;

        strcpy(const_list[0].name, "$LIBRARY") ;
               const_list[0].ptr=(double *)dcl_human_lib ;

        memset(error, 0, sizeof(error)) ; 

    for(j=0 ; j<this->Features_cnt ; j++)
      if(!stricmp(this->Features[j]->Type, "Show"))  
            color=&((Crowd_Feature_Show *)this->Features[j])->Color ;

/*---------------------------------------------- ��������� ��������� */

   if(!stricmp(type, "MESSAGE")) {
/*- - - - - - - - - - - - - - - - - - - - - - - -  ���������� ������ */
                  EventObject= this ;
                    EventTask= task ;
                      message=(Crowd_Message *)data ;

           memset(msg_name, 0, sizeof(msg_name)) ;                  /* $MsgName */ 
          strncpy(msg_name, message->Name, sizeof(msg_name)-1) ;

           memset(msg_type, 0, sizeof(msg_type)) ;                  /* $MsgType */ 
          strncpy(msg_type, message->Type, sizeof(msg_type)-1) ;

           memset(msg_kind, 0, sizeof(msg_kind)) ;                  /* $MsgKind */ 
          strncpy(msg_kind, message->Kind, sizeof(msg_kind)-1) ;

           memset(msg_sender, 0, sizeof(msg_sender)) ;              /* $MsgSender */ 
     if(message->Object_s!=NULL)
          strncpy(msg_sender, message->Object_s->Name, sizeof(msg_sender)-1) ;

           memset(obj_color, 0, sizeof(obj_color)) ;                /* $ObjectColor */ 
          sprintf(obj_color, "%03d:%03d:%03d", (int)GetRValue(*color), (int)GetGValue(*color), (int)GetBValue(*color)) ;

       for(i=0 ; dcl_human_lib[i].name[0]!=0 ; i++)
         if(dcl_human_lib[i].type     ==_CHR_AREA &&
            dcl_human_lib[i].func_flag==  0         )  
             dcl_human_lib[i].size=strlen((char *)dcl_human_lib[i].addr) ;
/*- - - - - - - - - - - - - - - - - - - - - - - ���������� ��������� */
                      context= NULL ;

           status=dcl->vCalculate("DCL", this->Program,
                                  const_list, NULL, NULL, &context, error) ;
                  dcl->vCalculate("CLEAR", NULL, NULL, NULL, NULL, &context, error) ;

        if(status) {
                      SEND_ERROR(error) ;
                            return(-1) ;
                   }
/*- - - - - - - - - - - - - - - - - - - ��������� ��������� �������� */
         sscanf(obj_color, "%d:%d:%d", &color_r, &color_g, &color_b) ;
     *color=RGB(color_r, color_g, color_b) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                 }
/*-------------------------------------------------------------------*/

 
  return(0) ;
}


/********************************************************************/
/*								    */
/*            ����������� ���������� ��������� �������              */

     int  Crowd_Object_Human::vEventShow(void)
{
  int  i ;

/*------------------------------------------------ ��������� ������� */

   for(i=0 ; i<this->Features_cnt ; i++)
       this->Features[i]->vBodyBasePoint("Human.Body", this->x_base, 
                                                       this->y_base, 
                                                       this->z_base ) ;

/*------------------------------------------------- ��������� ������ */

   for(i=0 ; i<this->Communications_cnt ; i++) {

                                               }
/*-------------------------------------------------------------------*/


  return(0) ;
}



/********************************************************************/
/*								    */
/*                   ��������� ������ � ���������� ���              */

  Dcl_decl *Human_dcl_Log( Lang_DCL  *dcl_kernel,
                           Dcl_decl **pars, 
                                int   pars_cnt)
{
   char  text[1024] ;

 static     char  chr_value[512] ;          /* ����� ������ */
 static Dcl_decl  chr_return={ _CHR_PTR, 0,0,0,"", chr_value, NULL, 0, 512} ;

/*---------------------------------------------------- ������������� */

                 memset(chr_value, 0, sizeof(chr_value)) ;
                        chr_return.size=0 ;

/*-------------------------------------------- ���������� ���������� */

       if(pars_cnt     !=1   ||                                     /* ��������� ����� ���������� */
	  pars[0]->addr==NULL  ) {
                                    dcl_kernel->mError_code=_DCLE_PROTOTYPE ;
                                      return(&chr_return) ; 
                                 }

                    memset(text, 0, sizeof(text)) ;                 /* ��������� ������ �� ���� */
        if(pars[0]->size>=sizeof(text))
                    memcpy(text, pars[0]->addr, sizeof(text)-1) ;
        else        memcpy(text, pars[0]->addr, pars[0]->size) ;

/*----------------------------------------------------- ������ � ��� */

     if(EventTask!=NULL) {

                EventTask->vSpecial("LOG", text, NULL) ;

                         }
/*-------------------------------------------------------------------*/

  return(&chr_return) ;
}


/*********************************************************************/
/*                                                                   */
/*                  ������ ��������� �������                         */

   Dcl_decl *Human_dcl_ObjectXYZ(Lang_DCL  *dcl_kernel,
                                 Dcl_decl **pars, 
                                      int   pars_cnt)

{
           char  name[128] ;
           char  elem[128] ;
            int  i ;

 static   double  dgt_value ;          /* ����� ��������� �������� */
 static Dcl_decl  dgt_return={ _DGT_VAL, 0,0,0,"", &dgt_value, NULL, 1, 1} ;

/*---------------------------------------------------- ������������� */

                              dgt_value=0. ;

/*-------------------------------------------- ���������� ���������� */

       if(pars_cnt     !=2    ||                                     /* ��������� ����� ���������� */
	  pars[0]->addr==NULL ||
	  pars[1]->addr==NULL   ) {
                                    dcl_kernel->mError_code=_DCLE_PROTOTYPE ;
                                      return(&dgt_return) ; 
                                  }

                    memset(name, 0, sizeof(name)) ;                 /* ��������� ��� ������� */
        if(pars[0]->size>=sizeof(name))
                    memcpy(name, pars[0]->addr, sizeof(name)-1) ;
        else        memcpy(name, pars[0]->addr, pars[0]->size) ;

                    memset(elem, 0, sizeof(elem)) ;                 /* ��������� ��� �������� */
        if(pars[1]->size>=sizeof(elem))
                    memcpy(elem, pars[1]->addr, sizeof(elem)-1) ;
        else        memcpy(elem, pars[1]->addr, pars[1]->size) ;

/*-------------------------------------------- ������ ������ ������� */

#define   OBJECTS       ProgramModule.kernel_objects 
#define   OBJECTS_CNT   ProgramModule.kernel_objects_cnt 

       for(i=0 ; i<OBJECTS_CNT ; i++)                               /* ���� ������ �� ����� */
         if(!stricmp(OBJECTS[i]->Name, name))  break ;

    if(i==OBJECTS_CNT) {                                            /* ���� ��� �� �������... */
                         dcl_kernel->mError_code=_DCLE_USER_DEFINED ;
                  strcpy(dcl_kernel->mError_details, "Unknown object") ;
                              return(&dgt_return) ;
                       }

         if(!stricmp(elem, "X"))  dgt_value=OBJECTS[i]->x_base ;
    else if(!stricmp(elem, "Y"))  dgt_value=OBJECTS[i]->y_base ;
    else if(!stricmp(elem, "Z"))  dgt_value=OBJECTS[i]->z_base ;
    else                         {
                         dcl_kernel->mError_code=_DCLE_USER_DEFINED ;
                  strcpy(dcl_kernel->mError_details, "Unknown object element") ;
                              return(&dgt_return) ;
                                 }

#undef   OBJECTS
#undef   OBJECTS_CNT

/*-------------------------------------------------------------------*/

  return(&dgt_return) ;

}


/*********************************************************************/
/*                                                                   */
/*                  ��������� ������ ������ �������                  */

   Dcl_decl *Human_dcl_GetLinks(Lang_DCL  *dcl_kernel,
                                Dcl_decl  *source, 
                                Dcl_decl **pars, 
                                     int   pars_cnt)

{
   char  type[128] ;
   char  kind[128] ;
   char  name[128] ;
   char  role[128] ;
   char *name_m ;
   char *name_s ;
   char  link_type[128] ;
   char  link_kind[128] ;
   char  link_m   [128] ;
   char  link_s   [128] ;
   char  link_p   [128] ;
    int  status ;
    int  i ;

          Dcl_decl  rec_data[5] ={
                                  {_CHR_AREA, 0, 0, 0, "type",             0,  link_type, 128, 128},
                                  {_CHR_AREA, 0, 0, 0, "kind",    (void *)128, link_kind, 128, 128},
                                  {_CHR_AREA, 0, 0, 0, "master",  (void *)256, link_m,    128, 128},
                                  {_CHR_AREA, 0, 0, 0, "slave",   (void *)384, link_s,    128, 128},
                                  {_CHR_AREA, 0, 0, 0, "partner", (void *)512, link_p,    128, 128}
                                 } ;
  Dcl_complex_type  rec_template={ "link", 640, rec_data, 5} ;

 static   double  dgt_value ;          /* ����� ��������� �������� */
 static Dcl_decl  dgt_return={ _DGT_VAL, 0,0,0,"", &dgt_value, NULL, 1, 1} ;

/*---------------------------------------------------- ������������� */

                              dgt_value=0 ;

/*-------------------------------------------- ���������� ���������� */

       if(pars_cnt     !=4    ||                                    /* ��������� ����� ���������� */
	  pars[0]->addr==NULL ||
	  pars[1]->addr==NULL ||
	  pars[2]->addr==NULL ||
	  pars[3]->addr==NULL   ) {
                                    dcl_kernel->mError_code=_DCLE_PROTOTYPE ;
                                      return(&dgt_return) ; 
                                  }

                    memset(type, 0, sizeof(type)) ;                 /* ��������� ������ �� ���� */
        if(pars[0]->size>=sizeof(type))
                    memcpy(type, pars[0]->addr, sizeof(type)-1) ;
        else        memcpy(type, pars[0]->addr, pars[0]->size) ;

                    memset(kind, 0, sizeof(kind)) ;                 /* ��������� ������ �� ���� */
        if(pars[1]->size>=sizeof(kind))
                    memcpy(kind, pars[1]->addr, sizeof(kind)-1) ;
        else        memcpy(kind, pars[1]->addr, pars[1]->size) ;

                    memset(name, 0, sizeof(name)) ;                 /* ��������� ������ �� ����� */
        if(pars[2]->size>=sizeof(name))
                    memcpy(name, pars[2]->addr, sizeof(name)-1) ;
        else        memcpy(name, pars[2]->addr, pars[2]->size) ;

                    memset(role, 0, sizeof(role)) ;                 /* ��������� ������ �� ���� */
        if(pars[3]->size>=sizeof(role))
                    memcpy(role, pars[3]->addr, sizeof(role)-1) ;
        else        memcpy(role, pars[3]->addr, pars[3]->size) ;

        if(!stricmp(name, "this"))  strcpy(name, EventObject->Name) ;

/*------------------------------------------------ ������ ���������� */

#define  LINK  EventObject->Communications[i]

    for(i=0 ; i<EventObject->Communications_cnt ; i++) {

         if(type[0]!=0)
          if(stricmp(type, LINK->Type))  continue ;

         if(kind[0]!=0)
          if(stricmp(kind, LINK->Kind))  continue ;

                                   name_m= "" ;
         if(LINK->Object_m!=NULL)  name_m=LINK->Object_m->Name ;
                                   name_s= "" ;
         if(LINK->Object_s!=NULL)  name_s=LINK->Object_s->Name ;

        if( stricmp(name, ""      )) {
         if(!stricmp(role, ""      ))
           if( stricmp(name, name_m) &&
               stricmp(name, name_s)   )  continue ;
         if(!stricmp(role, "master")   )
           if( stricmp(name, name_m))     continue ;
         if(!stricmp(role, "slave" ))
           if( stricmp(name, name_s)   )  continue ;
         if(!stricmp(role, "none"  )   )
           if(!stricmp(name, name_m) ||
              !stricmp(name, name_s)   )  continue ;
                                     }
 
                                          strcpy(link_type, LINK->Type) ;
                                          strcpy(link_kind, LINK->Kind) ;
                                          strcpy(link_m   , name_m) ;
                                          strcpy(link_s   , name_s) ;
         if(LINK->Object_m==EventObject)  strcpy(link_p   , link_s) ;
         else                             strcpy(link_p   , link_m) ;

                    rec_data[0].size=strlen(rec_data[0].prototype) ;
                    rec_data[1].size=strlen(rec_data[1].prototype) ;
                    rec_data[2].size=strlen(rec_data[2].prototype) ;
                    rec_data[3].size=strlen(rec_data[3].prototype) ;
                    rec_data[4].size=strlen(rec_data[4].prototype) ;

          status=dcl_kernel->iXobject_add(source, &rec_template) ;
       if(status) break ;
                                                       }
#undef  LINK

/*-------------------------------------------------------------------*/

  return(&dgt_return) ;

}


/*********************************************************************/
/*                                                                   */
/*                      �������� ���������                           */

   Dcl_decl *Human_dcl_SendMsg(Lang_DCL  *dcl_kernel,
                               Dcl_decl **pars, 
                                    int   pars_cnt)
{
          char  name[128] ;
          char  type[128] ;
          char  kind[128] ;
          char  spec[1024] ;
        double  wait ;
  Crowd_Kernel *msg_module ;
 Crowd_Message *message ;
  Crowd_Object *receiver ;
           int  status ;
           int  i ;

 static   double  dgt_value ;          /* ����� ��������� �������� */
 static Dcl_decl  dgt_return={ _DGT_VAL, 0,0,0,"", &dgt_value, NULL, 1, 1} ;

/*---------------------------------------------------- ������������� */

                              dgt_value=0 ;

/*-------------------------------------------- ���������� ���������� */

       if(pars_cnt     < 4    ||                                    /* ��������� ����� ���������� */
	  pars[0]->addr==NULL ||
	  pars[1]->addr==NULL ||
	  pars[2]->addr==NULL ||
	  pars[3]->addr==NULL   ) {
                                    dcl_kernel->mError_code=_DCLE_PROTOTYPE ;
                                      return(&dgt_return) ; 
                                  }

                    memset(name, 0, sizeof(name)) ;                 /* ��������� ��� ���������� */
        if(pars[0]->size>=sizeof(name))
                    memcpy(name, pars[0]->addr, sizeof(name)-1) ;
        else        memcpy(name, pars[0]->addr, pars[0]->size) ;

                    memset(type, 0, sizeof(type)) ;                 /* ��������� ��� ��������� */
        if(pars[1]->size>=sizeof(type))
                    memcpy(type, pars[1]->addr, sizeof(type)-1) ;
        else        memcpy(type, pars[1]->addr, pars[1]->size) ;

                    memset(kind, 0, sizeof(kind)) ;                 /* ��������� ��� ��������� */
        if(pars[2]->size>=sizeof(kind))
                    memcpy(kind, pars[2]->addr, sizeof(kind)-1) ;
        else        memcpy(kind, pars[2]->addr, pars[2]->size) ;

                    memset(spec, 0, sizeof(spec)) ;                 /* ��������� ������������ ��������� */
        if(pars[3]->size>=sizeof(spec))
                    memcpy(spec, pars[3]->addr, sizeof(spec)-1) ;
        else        memcpy(spec, pars[3]->addr, pars[3]->size) ;

                                  wait= 0 ;
        if(pars_cnt==5)
         if(pars[4]->addr!=NULL)  wait=dcl_kernel->iDgt_get(pars[4]->addr, pars[4]->type ) ;
 
/*----------------------------------------------- �������� ��������� */

#define  KERNEL  ProgramModule.kernel

     for(i=0 ; i<KERNEL->modules_cnt ; i++)                         /* ����� ������, ����������� � �������� ����� ��������� */
       if(!stricmp(KERNEL->modules[i].entry->identification, type)) {
                     msg_module=KERNEL->modules[i].entry ;
                            break ;
                                                                    }
 
       if(i>=KERNEL->modules_cnt) {                                 /* ���� ����� ������ �� ������... */
                         dcl_kernel->mError_code=_DCLE_USER_DEFINED ;
                  strcpy(dcl_kernel->mError_details, "Unknown message type") ;
                                       return(&dgt_return) ;
                                  }

                         receiver= NULL ;

       if(name[0]!=0) {

              receiver=ProgramModule.FindObject(name) ;
           if(receiver==NULL) {
                                 dcl_kernel->mError_code=_DCLE_USER_DEFINED ;
                          strcpy(dcl_kernel->mError_details, "Unknown receiver") ;
                                       return(&dgt_return) ;
                              }
                      }

                 message=msg_module->vCreateMessage(EventObject, receiver, NULL) ;
          strcpy(message->Kind, kind) ;

          status=message->vFormBySpec(spec) ;
       if(status) {
                         dcl_kernel->mError_code=_DCLE_USER_DEFINED ;
                  strcpy(dcl_kernel->mError_details, "Message forming error") ;
                              return(&dgt_return) ;
                  }

/*----------------------------------- ���������� ��������� � ������� */

          status=EventTask->vAddMessage(message, wait+1) ;
       if(status) {
                         dcl_kernel->mError_code=_DCLE_USER_DEFINED ;
                  strcpy(dcl_kernel->mError_details, "Message queuing up error") ;
                              return(&dgt_return) ;
                  }
/*-------------------------------------------------------------------*/

  return(&dgt_return) ;

}


/*********************************************************************/
/*                                                                   */
/*                      ��������� �������                            */

   Dcl_decl *Human_dcl_Recall(Lang_DCL  *dcl_kernel,
                              Dcl_decl **pars, 
                                   int   pars_cnt)
{
          char  kind[128] ;
        double  wait ;
  Crowd_Kernel *msg_module ;
 Crowd_Message *message ;
  Crowd_Object *receiver ;
           int  status ;
           int  i ;

 static   double  dgt_value ;          /* ����� ��������� �������� */
 static Dcl_decl  dgt_return={ _DGT_VAL, 0,0,0,"", &dgt_value, NULL, 1, 1} ;

/*---------------------------------------------------- ������������� */

                              dgt_value=0 ;

/*-------------------------------------------- ���������� ���������� */

       if(pars_cnt     < 1    ||                                    /* ��������� ����� ���������� */
	  pars[0]->addr==NULL   ) {
                                    dcl_kernel->mError_code=_DCLE_PROTOTYPE ;
                                      return(&dgt_return) ; 
                                  }

                    memset(kind, 0, sizeof(kind)) ;                 /* ��������� ��� ��������� */
        if(pars[0]->size>=sizeof(kind))
                    memcpy(kind, pars[0]->addr, sizeof(kind)-1) ;
        else        memcpy(kind, pars[0]->addr, pars[0]->size) ;

                                  wait= 0 ;
        if(pars_cnt==2)
         if(pars[1]->addr!=NULL)  wait=dcl_kernel->iDgt_get(pars[1]->addr, pars[1]->type ) ;
 
/*----------------------------------------------- �������� ��������� */

                 message=new Crowd_Message ;
          strcpy(message->Type, "Recall") ;
          strcpy(message->Kind, kind) ;
                 message->Object_s=NULL ;
                 message->Object_r=EventObject ; 

/*----------------------------------- ���������� ��������� � ������� */

          status=EventTask->vAddMessage(message, wait+1) ;
       if(status) {
                         dcl_kernel->mError_code=_DCLE_USER_DEFINED ;
                  strcpy(dcl_kernel->mError_details, "Message queuing up error") ;
                              return(&dgt_return) ;
                  }
/*-------------------------------------------------------------------*/

  return(&dgt_return) ;

}


