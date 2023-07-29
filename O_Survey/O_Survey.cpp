/********************************************************************/
/*                                                                  */
/*          ������ ���������� �������� "�������� �������"           */
/*                                                                  */
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

#include "O_Survey.h"

#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
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
/*                                                                  */
/*                      ����������� ������                          */

     static   Crowd_Module_Survey  ProgramModule ;


/********************************************************************/
/*                                                                  */
/*                      ����������������� ����                      */

 O_SURVEY_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 O_SURVEY_API Crowd_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/********************************************************************/
/********************************************************************/
/**                                                                **/
/**	 �������� ������ ������ ���������� �������� "�������� �������" **/
/**                                                                **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*                            ������ ������                         */

  struct Crowd_Module_Survey_instr  Crowd_Module_Survey_InstrList[]={

 { "help",    "?",   "#HELP   - ������ ��������� ������", 
                    NULL,
                     &Crowd_Module_Survey::cHelp   },
 { "create",  "cr",  "#CREATE - ������� ������",
                     " CREATE <���> [<������> [<������ ����������>]]\n"
                     "   ������� ����������� ������ �� ����������������� ������",
                     &Crowd_Module_Survey::cCreate },
 { "info",    "i",   "#INFO - ������ ���������� �� �������",
                     " INFO <���> \n"
                     "   ������ �������� ��������� �� ������� � ������� ����\n"
                     " INFO/ <���> \n"
                     "   ������ ������ ���������� �� ������� � ��������� ����",
                     &Crowd_Module_Survey::cInfo },
 { "base",    "b",  "#BASE - ������ ������� ����� �������",
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
                     &Crowd_Module_Survey::cBase },
 { "color",    "c",  "#COLOR   - ���������� ���� �������", 
                     " COLOR <���> <�������� �����>\n"
                     "   ���������� ���� ������� �� ��������: RED, GREEN, BLUE\n"
                     " COLOR <���> RGB <R-������>:<G-������>:<B-������>\n"
                     "   ���������� ���� ������� �� RGB-�����������\n",
                     &Crowd_Module_Survey::cColor      },
 { "visible",  "v",  "#VISIBLE - ������� ������ ��������� �������",
                     " VISIBLE <���> \n"
                     "   �������� ��������� ��������� ������� �� ���������������",
                     &Crowd_Module_Survey::cVisible },
 { "behavior", "bh", "#BEHAVIOR - ������� ��������� ��������� �������",
                     " BEHAVIOR <���> $<��� ���������>\n"
                     "   ���������� ��������� ���������\n"
                     " PROGRAM <���> <���� � ����� ���������>\n"
                     "   ��������� ��������� ������� ��������� � �����",
                     &Crowd_Module_Survey::cBehavior },
 { "profile", "p",   "#PROFILE - ������� ���������� ��������� ��������� �������",
                     " PROFILE <���> \n"
                     "   �������/�������� � ���������� ������\n"
                     " PROFILE <���> <Key1>=<Value1>,...,<KeyN>=<ValueN> \n"
                     "   ������� ��������� �������",
                     &Crowd_Module_Survey::cProfile },
 {  NULL }
                                                            } ;

/********************************************************************/
/*                                                                  */
/*                       ����� ����� ������                         */

    struct Crowd_Module_Survey_instr *Crowd_Module_Survey::mInstrList=NULL ;

/********************************************************************/
/*                                                                  */
/*                        ����������� ������                        */

     Crowd_Module_Survey::Crowd_Module_Survey(void)

{
           keyword="Crowd" ;
    identification="Survey" ;
          category="Object" ;


        mInstrList=Crowd_Module_Survey_InstrList ;
}


/********************************************************************/
/*                                                                  */
/*                          ���������� ������                       */

    Crowd_Module_Survey::~Crowd_Module_Survey(void)

{
}


/********************************************************************/
/*                                                                  */
/*                        �������� �������                          */

  Crowd_Object *Crowd_Module_Survey::vCreateObject(Crowd_Model_data *data)

{
  Crowd_Object_Survey *object ;
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
              SEND_ERROR("������ SURVEY: �� ������ ��� �������") ;
                                return(NULL) ;
                         }

       for(i=0 ; i<OBJECTS_CNT ; i++)
         if(!stricmp(OBJECTS[i]->Name, data->name)) {
              SEND_ERROR("������ SURVEY: ������ � ����� ������ ��� ����������") ;
                                return(NULL) ;
                                                    }
/*-------------------------------------- ���������� �������� ������� */
/*- - - - - - - - - - - - ���� ������ ������ ��������� � ����������� */
   if(data->path[0]==0) {

    if(data->model[0]==0) {                                         /* ���� ������ �� ������ */
              SEND_ERROR("������ SURVEY: �� ������ ������ �������") ;
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
              SEND_ERROR("������ SURVEY: ����������� ������ ����") ;
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

              SEND_ERROR("������ SURVEY: �������������� ����� ���������� ������") ;
                                return(NULL) ;
                                             }
/*------------------------------------------------- �������� ������� */

       object=new Crowd_Object_Survey ;
    if(object==NULL) {
              SEND_ERROR("������ SURVEY: ������������ ������ ��� �������� �������") ;
                        return(NULL) ;
                     }
/*------------------------------------- ���������� ������ ���������� */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������� ��������� */
     for(i=0 ; i<5 ; i++)
       if(data->pars[i].text[0]!=0) {

           PAR=(struct Crowd_Parameter *)
                 realloc(PAR, (PAR_CNT+1)*sizeof(*PAR)) ;
        if(PAR==NULL) {
                         SEND_ERROR("������ SURVEY: ������������ ������") ;
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
                         SEND_ERROR("������ SURVEY: ������������ ������") ;
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
                                         data->sections[i].decl, "Survey.Body") ;
                                             }

                                         data->sections[i].title[0]= 0 ;
                                        *data->sections[i].decl    ="" ;
                                               }
/*---------------------------------- �������� ������� � ����� ������ */

       OBJECTS=(Crowd_Object **)
                 realloc(OBJECTS, (OBJECTS_CNT+1)*sizeof(*OBJECTS)) ;
    if(OBJECTS==NULL) {
              SEND_ERROR("������ SURVEY: ������������ ������") ;
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
/*                                                                  */
/*                         �������� ��������                        */

     int  Crowd_Module_Survey::vGetParameter(char *name, char *value)

{
/*-------------------------------------------------- �������� ������ */

    if(!stricmp(name, "$$MODULE_NAME")) {

         sprintf(value, "%-20.20s -  �������� ������� (����� ������)", identification) ;
                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                      ��������� �������                           */

  int  Crowd_Module_Survey::vExecuteCmd(const char *cmd)

{
  static  int  direct_command ;   /* ���� ������ ������ ������� */
         char  command[1024] ;
         char *instr ;
         char *end ;
          int  status ;
          int  i ;

#define  _SECTION_FULL_NAME   "SURVEY"
#define  _SECTION_SHRT_NAME   "SURVEY"

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
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"Object Survey:") ;
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
       if(status)  SEND_ERROR("������ SURVEY: ����������� �������") ;
                                            return(-1) ;
                                       }
 
     if(mInstrList[i].process!=NULL)                                /* ���������� ������� */
                status=(this->*mInstrList[i].process)(end) ;
     else       status=  0 ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*                                                                  */
/*                      ������� ������ �� ������                    */

    void  Crowd_Module_Survey::vReadSave(std::string *data)

{
                 char *buff ;
                  int  buff_size ;
     Crowd_Model_data  create_data ;
  Crowd_Object_Survey *object ;
                 char  name[128] ;
                 char *entry ;
                 char *end ;
                  int  i ;

/*----------------------------------------------- �������� ��������� */

   if(memicmp(data->c_str(), "#BEGIN MODULE SURVEY\n", 
                      strlen("#BEGIN MODULE SURVEY\n")) &&
      memicmp(data->c_str(), "#BEGIN OBJECT SURVEY\n", 
                      strlen("#BEGIN OBJECT SURVEY\n"))   )  return ;

/*------------------------------------------------ ���������� ������ */

              buff_size=data->size()+16 ;
              buff     =(char *)calloc(1, buff_size) ;

       strcpy(buff, data->c_str()) ;

/*------------------------------------------------- �������� ������� */

   if(!memicmp(buff, "#BEGIN OBJECT SURVEY\n", 
              strlen("#BEGIN OBJECT SURVEY\n"))) {                  /* IF.1 */
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
              object=(Crowd_Object_Survey *)vCreateObject(&create_data) ;
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
/*                                                                  */
/*                     �������� ������ � ������                     */

    void  Crowd_Module_Survey::vWriteSave(std::string *text)

{
  std::string  buff ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN MODULE SURVEY\n" ;

/*------------------------------------------------ �������� �������� */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� HELP                    */

  int  Crowd_Module_Survey::cHelp(char *cmd)

{ 
    DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_HELP", RT_DIALOG),
			   GetActiveWindow(), Object_Survey_Help_dialog) ;

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� CREATE                  */
/*                                                                  */
/*      CREATE <���> [<������> [<������ ����������>]]               */

  int  Crowd_Module_Survey::cCreate(char *cmd)

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
                                     Object_Survey_Create_dialog, 
                                    (LPARAM)&data               ) ;
   if(status)  return(status) ;

            this->kernel->vShow(NULL) ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� INFO                    */
/*                                                                  */
/*        INFO   <���>                                              */
/*        INFO/  <���>                                              */

  int  Crowd_Module_Survey::cInfo(char *cmd)

{
                 char  *name ;
  Crowd_Object_Survey  *object ;
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
/*                                                                  */
/*                    ���������� ���������� BASE                    */
/*                                                                  */
/*        BASE    <���> <X> <Y> <Z>                                 */
/*        BASE/X  <���> <X>                                         */
/*        BASE/+X <���> <X>                                         */
/*        BASE>   <���> <��� ���������> <���>                       */
/*        BASE>>  <���> <��� ���������> <���>                       */

  int  Crowd_Module_Survey::cBase(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

                 char  *pars[_PARS_MAX] ;
                 char  *name ;
                 char **xyz ;
               double   coord[_COORD_MAX] ;
                  int   coord_cnt ;
               double   inverse ;
  Crowd_Object_Survey  *object ;
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
     object->Features[i]->vBodyBasePoint("Survey.Body", object->x_base, 
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
/*                                                                  */
/*                   ���������� ���������� Color                    */
/*       COLOR  <���> <�������� �����>                              */
/*       COLOR  <���> RGB <R-������>:<G-������>:<B-������>          */

  int  Crowd_Module_Survey::cColor(char *cmd)

{ 
#define   _PARS_MAX   4
                  char *pars[_PARS_MAX] ;
                  char *name ;
   Crowd_Object_Survey  *object ;
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
/*                                                                  */
/*                    ���������� ���������� VISIBLE                 */
/*                                                                  */
/*        VISIBLE  <���>                                            */

  int  Crowd_Module_Survey::cVisible(char *cmd)

{
#define   _PARS_MAX   4

                  char *name ;
   Crowd_Object_Survey *object ;
                  char *end ;
                   int  i ;

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
/*                                                                  */
/*                   ���������� ���������� Behavior                 */
/*                                                                  */
/*       BEHAVIOR <���> $<��� ���������>                            */
/*       BEHAVIOR <���> <���� � ����� ���������>                    */

  int  Crowd_Module_Survey::cBehavior(char *cmd)

{ 
#define   _PARS_MAX   4
                  char *pars[_PARS_MAX] ;
                  char *name ;
                  char *path ;
   Crowd_Object_Survey  *object ;
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
                                 "��������: BEHAVIOR <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*----------------------------------------- �������� ����� ��������� */

    if(path==NULL) {
                       SEND_ERROR("�� ������ ��������� ���������. \n"
                                  "��������: BEHAVIOR <��� �������> <���� � ����� ���������>") ;
                                        return(-1) ;
                   } 
/*- - - - - - - - - - - - - - - - - - -  ���������� ������ ��������� */
    if(path[0]=='$') {

       if(!stricmp(path, "$DOG"   ) ||
          !stricmp(path, "$SPECTR")  )   strncpy(object->behavior_model, path, sizeof(object->behavior_model)-1) ;
       else                            {
                         sprintf(text, "����������� ��������� ��������� - %s", path) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                                        }
                     }
/*- - - - - - - - - - - - - - - - - - - - - ������� ������ ��������� */
    else             {

        data=this->FileCache(path, error) ;
     if(data==NULL) {
                         sprintf(text, "������ �������� ����� ��������� - %s", error) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                    }

                 object->Program=data ;

                     }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                   ���������� ���������� Profile                  */
/*                                                                  */
/*       PROFILE <���>                                              */
/*       PROFILE <���> <Key1>=<Value1>,...,<KeyN>=<ValueN>          */

  int  Crowd_Module_Survey::cProfile(char *cmd)

{ 
#define   _PARS_MAX   4
                  char *pars[_PARS_MAX] ;
                  char *name ;
                  char *list ;
   Crowd_Object_Survey *object ;
        struct Profile *profile ;
                  char *key ;
                  char *next ;
                  char *value ;
                double  value_d ;
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
                     list=pars[1] ;

/*------------------------------------------- ����� ������� �� ����� */ 

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: PROFILE <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*-------------------------------------- ���������� �������� ������� */

    if(object->behavior_model[0]!=0) {

      if(!stricmp(object->behavior_model, "$DOG"   ))   object->iBehaviorDefault(0, "GET_PROFILE", (void *)&profile, NULL) ;
      else                                            {

                            SEND_ERROR("����������� ������ ���������") ;
                                   return(-1) ;
                                                      }
                                     } 
    else                             {

                      SEND_ERROR("������� �������������� ������ ��� ���������� ������� ���������") ;
                                             return(-1) ;
                                     } 
/*--------------------------- ��������/��������� � ���������� ������ */

    if(list   ==NULL ||
       list[0]==  0    ) {

             DialogBoxIndirectParam( GetModuleHandle(NULL),
                                    (LPCDLGTEMPLATE)Resource("IDD_PROFILE", RT_DIALOG),
			             GetActiveWindow(), 
                                     Object_Survey_Profile_dialog, 
                                    (LPARAM)profile               ) ;

                                        return(0) ;
                         }
/*----------------------------------------- ������ ������ ���������� */

     for(key=list, next=list ; next!=NULL ; key=next+1) {
/*- - - - - - - - - - - - - - - - - - - - - - - ��������� ���������� */
             next=strchr(key, ',') ;
          if(next!=NULL)  *next=0 ;    

             value=strchr(key, '=') ;
          if(value==NULL) {
                               sprintf(error, "������������ ��������� ������ ���������� ����� %s", key) ;
                            SEND_ERROR(error) ;
                                  return(-1) ;
                          }

            *value=0 ;
             value++ ;
/*- - - - - - - - - - - - - - - - - - - - -  ������������� ��������� */
        for(i=0 ; profile[i].name[0]!=0 ; i++)
          if(!stricmp(key, profile[i].name))  break ;

          if(profile[i].name[0]==0) {
                                      sprintf(error, "����������� �������� %s", key) ;
                                   SEND_ERROR(error) ;
                                         return(-1) ;
                                    }
/*- - - - - - - - - - - - - - - - - - �������� � ���������� �������� */
          if(!stricmp(profile[i].type, "String" )) {

                                strcpy((char *)profile[i].value, value) ;

                                                   }
          else
          if(!stricmp(profile[i].type, "Digital")) {

                        value_d=strtod(value, &end) ;

             if(*end!=0) {
                               sprintf(error, "������������ �������� ��� ��������� %s = %s ", key, value) ;
                            SEND_ERROR(error) ;
                                  return(-1) ;
                         }

             if(profile[i].value_min!=profile[i].value_max)
                  if(value_d<profile[i].value_min ||
                     value_d>profile[i].value_max   ) {
                                   sprintf(text, "�������� ��������� %s ������ ���� � ��������� %lf ... %lf", key, profile[i].value_min, profile[i].value_max) ;
                                SEND_ERROR(text) ;
			            return(FALSE) ;
                                                      }

                      *((double *)profile[i].value)=value_d ;

                                                   }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                ����� ������� ���� SURVEY �� �����                */

  Crowd_Object_Survey *Crowd_Module_Survey::FindObject(char *name)

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

     if(strcmp(OBJECTS[i]->Type, "Survey")) {

           SEND_ERROR("������ �� �������� �������� ���� SURVEY") ;
                            return(NULL) ;
                                            }
/*-------------------------------------------------------------------*/ 

   return((Crowd_Object_Survey *)OBJECTS[i]) ;
  
#undef   OBJECTS
#undef   OBJECTS_CNT

}


/********************************************************************/
/********************************************************************/
/**                                                                **/
/**		  �������� ������ ������� "�������� �������"	           **/
/**                                                                **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*                       ����������� ������                         */

     Crowd_Object_Survey::Crowd_Object_Survey(void)

{
   strcpy(      Type, "Survey") ;
   strcpy(SenderType, "Survey") ;

   memset(behavior_model, 0, sizeof(behavior_model)) ;
          behavior_data=NULL ;
}


/********************************************************************/
/*                                                                  */
/*                         ���������� ������                        */

    Crowd_Object_Survey::~Crowd_Object_Survey(void)

{
}


/********************************************************************/
/*                                                                  */
/*                        ������������ ��������                     */

  void   Crowd_Object_Survey::vFree(void)

{
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++) {
               this->Features[i]->vBodyDelete(NULL) ;
          free(this->Features[i]) ;
                                         }

          free(this->Features) ;
}


/********************************************************************/
/*                                                                  */
/*                     �������� ������ � ������                     */

    void  Crowd_Object_Survey::vWriteSave(std::string *text)

{
  char  field[1024] ;
   int  i ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN OBJECT SURVEY\n" ;

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
/*                                                                  */
/*                  ���������� ��������� �������                    */

     int  Crowd_Object_Survey::vEventStart(void)
{
  return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                   ��������� �������                              */

     static         Crowd_Kernel *EventTask ;
     static  Crowd_Object_Survey *EventObject ;


     int  Crowd_Object_Survey::vEvent(long  t, char *type, void  *data, Crowd_Kernel *task)
{
   char  error[1024] ;


     if(!stricmp(this->behavior_model, "$DEFAULT"))   this->iBehaviorDefault(t, type, data, task) ;
     else                                           {
                  sprintf(error, "Section SURVEY: ����������� ���������� �������� ���������: %s", this->behavior_model) ;
               SEND_ERROR(error) ;
                     return(-1) ;
                                                    }
 
  return(0) ;
}


/********************************************************************/
/*                                                                  */
/*            ����������� ���������� ��������� �������              */

     int  Crowd_Object_Survey::vEventShow(void)
{
  int  i ;

/*------------------------------------------------ ��������� ������� */

   for(i=0 ; i<this->Features_cnt ; i++)
       this->Features[i]->vBodyBasePoint("Survey.Body", this->x_base, 
                                                        this->y_base, 
                                                        this->z_base ) ;

/*------------------------------------------------- ��������� ������ */

   for(i=0 ; i<this->Communications_cnt ; i++) {

                                               }
/*-------------------------------------------------------------------*/


  return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    ������ ��������� - DEFAULT                    */

     int  Crowd_Object_Survey::iBehaviorDefault(long  t, char *event_type, void *event_data, Crowd_Kernel *task)
{
   Crowd_Message  *message ;
  struct Default  *data ;
  struct Profile **profile_ext ;
             int   j ;

  static  struct Profile  profile[]={
            { "energy",    "������������",   "����� ���������� �� ����, ����������� �� ����",                "Digital", 0.,   1.0, NULL, NULL },
            { "weariness", "���������",      "����������� �������� '������������' �� ����",                  "Digital", 0.,   1.0, NULL, NULL },
            { "endurance", "������������",   "����� ������, � ������� �������� '������������' �� ���������", "Digital", 0., 999.0, NULL, NULL },
            { "recovery",  "��������������", "����������� ��������� '������������' �� ��������� ����",       "Digital", 0.,   1.0, NULL, NULL },
            { "" } 
                                    } ; 

/*==================================================== ������������� */

   if(behavior_data==NULL) {

            behavior_data=(void *)calloc(1, sizeof(struct Default)) ;
                     data=(struct Default *)behavior_data ;


    for(j=0 ; j<this->Features_cnt ; j++)                           /* ��������� ������ �� ���� */
      if(!stricmp(this->Features[j]->Type, "Show"))  
            data->color=&((Crowd_Feature_Show *)this->Features[j])->Color ;

                           }

                     data=(struct Default *)behavior_data ;

/*============================================ ������� - GET PROFILE */

  if(!stricmp(event_type, "GET_PROFILE")) {

/*
              profile[0].value=&(data->energy) ;
              profile[1].value=&(data->weariness) ;
              profile[2].value=&(data->endurance) ;
*/

                  profile_ext=(struct Profile **)event_data ;
                 *profile_ext=                   profile ;

                                                 return(0) ;
                                          }
/*================================================ ������� - MESSAGE */

  if(!stricmp(event_type, "MESSAGE")) {

                      message=(Crowd_Message *)event_data ;

/*------------------- ���������� ������ ���� ��� ��������� ��������� */

    if(message->Object_s==NULL) {
/*- - - - - - - - - - - -  ��������� ���������: ������������� � �.�. */
     if(!stricmp(message->Type, "system")) {

//                  data->energy = 0.8 ; 

                        return(0) ;
                                           }
/*- - - - - - - - - - - - - - - - - - - - ���������� ������� �� ���� */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                }
/*------------------------------------------------ ������� ��������� */

    else                        {
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������� �� ������ */
                *(data->color)=RGB(0, 255, 255) ;

/*
          this->x_base+=(message->Object_s->x_base-this->x_base)*data->energy ;
          this->y_base+=(message->Object_s->y_base-this->y_base)*data->energy ;

     if(t>data->endurance) data->energy = data->energy*(1.-data->weariness) ;
*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                }
/*-------------------------------------------------------------------*/
                                      }
/*===================================================================*/

  return(0) ;
}

