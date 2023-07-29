/********************************************************************/
/*                                                                  */
/*         ������ ���������� �������� "������� ������"              */
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
#include "..\tcp_lib\exh.h"
#include "..\tcp_lib\tcp.h"

#include "O_External.h"

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
/*                       ����������� ������                         */

     static   Crowd_Module_External  ProgramModule ;


/********************************************************************/
/*                                                                  */
/*                      ����������������� ����                      */

 O_EXTERNAL_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 O_EXTERNAL_API Crowd_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/********************************************************************/
/********************************************************************/
/**                                                                **/
/**	  �������� ������ ������ ���������� �������� "������� ������"  **/
/**                                                                **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*                            ������ ������                         */

  struct Crowd_Module_External_instr  Crowd_Module_External_InstrList[]={

 { "help",    "?",   "#HELP   - ������ ��������� ������", 
                    NULL,
                     &Crowd_Module_External::cHelp   },
 { "create",  "cr",  "#CREATE - ������� ������",
                     " CREATE <���> [<������> [<������ ����������>]]\n"
                     "   ������� ����������� ������ �� ����������������� ������",
                     &Crowd_Module_External::cCreate },
 { "info",    "i",   "#INFO - ������ ���������� �� �������",
                     " INFO <���> \n"
                     "   ������ �������� ��������� �� ������� � ������� ����\n"
                     " INFO/ <���> \n"
                     "   ������ ������ ���������� �� ������� � ��������� ����",
                     &Crowd_Module_External::cInfo },
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
                     &Crowd_Module_External::cBase },
 { "color",    "c",  "#COLOR   - ���������� ���� �������", 
                     " COLOR <���> <�������� �����>\n"
                     "   ���������� ���� ������� �� ��������: RED, GREEN, BLUE\n"
                     " COLOR <���> RGB <R-������>:<G-������>:<B-������>\n"
                     "   ���������� ���� ������� �� RGB-�����������\n",
                     &Crowd_Module_External::cColor      },
 { "visible",  "v",  "#VISIBLE - ������� ������ ��������� �������",
                     " VISIBLE <���> \n"
                     "   �������� ��������� ��������� ������� �� ���������������",
                     &Crowd_Module_External::cVisible },
 { "itype",   "it", "#ITYPE - ������� ���� ���������� � ������� �������",
                    " ITYPE <���>\n"
                    "   ������ ��������� ���������� � ���������� ������\n"
                    " ITYPE <���> <iface_type> [<object_type>]\n"
                    "   ������ ��� ���������� (file, tcp-server) � ��� �������\n",
                    &Crowd_Module_External::cIType },
 { "ifile",   "if", "#IFILE - ��������� ��������� ���������� � ������� �������",
                    " IFILE/f <���> <path>\n"
                    "   ������ ���� ����� �������� ������\n"
                    " IFILE/t <���> <path>\n"
                    "   ������ ��� ����� ������ �������� �����\n"
                    " IFILE/c <���> <name>\n"
                    "   ������ ��� ��� ��������/��������� ������ ������\n",
                    &Crowd_Module_External::cIFile },
 { "itcp",    "ic", "#ITCP - ��������� �������� ���������� � ������� �������",
                    " ITCP/u <���> <URL>\n"
                    "   ������ URL �������� ������\n",
                    &Crowd_Module_External::cITcp },
 { "sender",  "s",  "#SENDER - ������ ��������� ����������� ���������",
                    " SENDER <���> <���������>\n"
                    "   ������ ��������� ����������� ���������\n",
                    &Crowd_Module_External::cSender },
 { "gas",     "g",  "#GAS - ���������� ���������",
                    " GAS <���> <����������>\n"
                    "   ������ ����� ������������� ��������\n"
                    " GAS/M <���> <����������>\n"
                    "   ������ ������������ ����� ������������� ��������\n"
                    " GAS/R <���> <����������>\n"
                    "   ������ ����� ��������, �������������� �� ��� �������������\n",
                    &Crowd_Module_External::cGas },

 {  NULL }
                                                            } ;

/********************************************************************/
/*                                                                  */
/*                       ����� ����� ������                         */

    struct Crowd_Module_External_instr *Crowd_Module_External::mInstrList=NULL ;

/********************************************************************/
/*                                                                  */
/*                      ����������� ������                          */

     Crowd_Module_External::Crowd_Module_External(void)

{
           keyword="Crowd" ;
    identification="External" ;
          category="Object" ;


        mInstrList=Crowd_Module_External_InstrList ;
}


/********************************************************************/
/*                                                                  */
/*                       ���������� ������                          */

    Crowd_Module_External::~Crowd_Module_External(void)

{
}


/********************************************************************/
/*                                                                  */
/*                        �������� �������                          */

  Crowd_Object *Crowd_Module_External::vCreateObject(Crowd_Model_data *data)

{
  Crowd_Object_External *object ;
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
              SEND_ERROR("������ EXTERNAL: �� ������ ��� �������") ;
                                return(NULL) ;
                         }

       for(i=0 ; i<OBJECTS_CNT ; i++)
         if(!stricmp(OBJECTS[i]->Name, data->name)) {
              SEND_ERROR("������ EXTERNAL: ������ � ����� ������ ��� ����������") ;
                                return(NULL) ;
                                                    }
/*-------------------------------------- ���������� �������� ������� */
/*- - - - - - - - - - - - ���� ������ ������ ��������� � ����������� */
   if(data->path[0]==0) {

    if(data->model[0]==0) {                                         /* ���� ������ �� ������ */
              SEND_ERROR("������ EXTERNAL: �� ������ ������ �������") ;
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
              SEND_ERROR("������ EXTERNAL: ����������� ������ ����") ;
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

              SEND_ERROR("������ EXTERNAL: �������������� ����� ���������� ������") ;
                                return(NULL) ;
                                             }
/*------------------------------------------------- �������� ������� */

       object=new Crowd_Object_External ;
    if(object==NULL) {
              SEND_ERROR("������ EXTERNAL: ������������ ������ ��� �������� �������") ;
                        return(NULL) ;
                     }
/*------------------------------------- ���������� ������ ���������� */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������� ��������� */
     for(i=0 ; i<5 ; i++)
       if(data->pars[i].text[0]!=0) {

           PAR=(struct Crowd_Parameter *)
                 realloc(PAR, (PAR_CNT+1)*sizeof(*PAR)) ;
        if(PAR==NULL) {
                         SEND_ERROR("������ EXTERNAL: ������������ ������") ;
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
                         SEND_ERROR("������ EXTERNAL: ������������ ������") ;
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
                                         data->sections[i].decl, "External.Body") ;
                                             }

                                         data->sections[i].title[0]= 0 ;
                                        *data->sections[i].decl    ="" ;
                                               }
/*---------------------------------- �������� ������� � ����� ������ */

       OBJECTS=(Crowd_Object **)
                 realloc(OBJECTS, (OBJECTS_CNT+1)*sizeof(*OBJECTS)) ;
    if(OBJECTS==NULL) {
              SEND_ERROR("������ EXTERNAL: ������������ ������") ;
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
/*                        �������� ��������                         */

     int  Crowd_Module_External::vGetParameter(char *name, char *value)

{
/*-------------------------------------------------- �������� ������ */

    if(!stricmp(name, "$$MODULE_NAME")) {

         sprintf(value, "%-20.20s -  ������� ������", identification) ;
                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                          ��������� �������                       */

  int  Crowd_Module_External::vExecuteCmd(const char *cmd)

{
  static  int  direct_command ;   /* ���� ������ ������ ������� */
         char  command[1024] ;
         char *instr ;
         char *end ;
          int  status ;
          int  i ;

#define  _SECTION_FULL_NAME   "EXTERNAL"
#define  _SECTION_SHRT_NAME   "EXT"

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
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"Object External:") ;
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
       if(status)  SEND_ERROR("������ EXTERNAL: ����������� �������") ;
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
/*                       ������� ������ �� ������                   */

    void  Crowd_Module_External::vReadSave(std::string *data)

{
                   char *buff ;
                    int  buff_size ;
       Crowd_Model_data  create_data ;
  Crowd_Object_External *object ;
                   char  name[128] ;
                   char *entry ;
                   char *end ;
                    int  i ;

/*----------------------------------------------- �������� ��������� */

   if(memicmp(data->c_str(), "#BEGIN MODULE EXTERNAL\n", 
                      strlen("#BEGIN MODULE EXTERNAL\n")) &&
      memicmp(data->c_str(), "#BEGIN OBJECT EXTERNAL\n", 
                      strlen("#BEGIN OBJECT EXTERNAL\n"))   )  return ;

/*------------------------------------------------ ���������� ������ */

              buff_size=data->size()+16 ;
              buff     =(char *)calloc(1, buff_size) ;

       strcpy(buff, data->c_str()) ;

/*------------------------------------------------- �������� ������� */

   if(!memicmp(buff, "#BEGIN OBJECT EXTERNAL\n", 
              strlen("#BEGIN OBJECT EXTERNAL\n"))) {                    /* IF.1 */
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
              object=(Crowd_Object_External *)vCreateObject(&create_data) ;
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
/*                      �������� ������ � ������                    */

    void  Crowd_Module_External::vWriteSave(std::string *text)

{
  std::string  buff ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN MODULE EXTERNAL\n" ;

/*------------------------------------------------ �������� �������� */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� HELP                    */

  int  Crowd_Module_External::cHelp(char *cmd)

{ 
    DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_HELP", RT_DIALOG),
			   GetActiveWindow(), Object_External_Help_dialog) ;

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� CREATE                  */
/*                                                                  */
/*      CREATE <���> [<������> [<������ ����������>]]               */

  int  Crowd_Module_External::cCreate(char *cmd)

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
                                     Object_External_Create_dialog, 
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

  int  Crowd_Module_External::cInfo(char *cmd)

{
                   char  *name ;
  Crowd_Object_External  *object ;
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

       object=FindObject(name, 1) ;                                 /* ���� ������ �� ����� */
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

  int  Crowd_Module_External::cBase(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

                   char  *pars[_PARS_MAX] ;
                   char  *name ;
                   char **xyz ;
                 double   coord[_COORD_MAX] ;
                    int   coord_cnt ;
                 double   inverse ;
  Crowd_Object_External  *object ;
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

       object=FindObject(name, 1) ;                                 /* ���� ������ �� ����� */
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
     object->Features[i]->vBodyBasePoint("External.Body", object->x_base, 
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

  int  Crowd_Module_External::cColor(char *cmd)

{ 
#define   _PARS_MAX   10
                    char *pars[_PARS_MAX] ;
                    char *name ;
   Crowd_Object_External  *object ;
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

       object=FindObject(name, 1) ;                                 /* ���� ������ �� ����� */
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
/*                   ���������� ���������� VISIBLE                  */
/*                                                                  */
/*        VISIBLE  <���>                                            */

  int  Crowd_Module_External::cVisible(char *cmd)

{
                    char  *name ;
   Crowd_Object_External  *object ;
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

       object=FindObject(name, 1) ;                                 /* ���� ������ �� ����� */
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
/*                 ���������� ���������� ITYPE                      */
/*                                                                  */
/*       ITYPE <���>                                                */
/*       ITYPE <���> <Iface type> [<Object type>]                   */

  int  Crowd_Module_External::cIType(char *cmd)

{
#define   _PARS_MAX  10

                    char *pars[_PARS_MAX] ;
                    char *name ;
   Crowd_Object_External *object ;
                 INT_PTR  status ;
                    char *end ;
                     int  i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name= pars[0] ;

/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: ITYPE <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=(Crowd_Object_External *)FindObject(name, 1) ;        /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*--------------------------------------------- ������� ����� ������ */

   if(pars[1]==NULL || *pars[1]==0) {

        status=DialogBoxIndirectParam( GetModuleHandle(NULL),
                                      (LPCDLGTEMPLATE)Resource("IDD_IFACE", RT_DIALOG),
                                       GetActiveWindow(), 
                                       Object_External_Iface_dialog, 
                                      (LPARAM)object               ) ;
         return((int)status) ;
                                    }
/*------------------------------------------ ������� ���� ���������� */

                     strupr(pars[1]) ;

   if(stricmp(pars[1], "FILE"      ) &&
      stricmp(pars[1], "TCP-SERVER")   ) {

              SEND_ERROR("��������� ��������� ���� ����������: FILE, TCP-SERVER") ;
                     return(-1) ;
                                         }

                strcpy(object->iface_type, pars[1]) ;

   if(pars[2]!=NULL && *pars[2]!=0)
                strcpy(object->object_type, pars[2]) ;

/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� IFILE                   */
/*                                                                  */
/*       IFILE/F <���> <Folder path>                                */
/*       IFILE/T <���> <Objects path>                               */
/*       IFILE/C <���> <Controls name>                              */

  int  Crowd_Module_External::cIFile(char *cmd)

{
#define   _PARS_MAX  10

                   char  *pars[_PARS_MAX] ;
                   char  *name ;
  Crowd_Object_External  *object ;
                    int   f_flag, t_flag, c_flag ;
                   char  *end ;
                    int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                      f_flag=0 ;
                      t_flag=0 ;
                      c_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, 'f')!=NULL ||
                   strchr(cmd, 'F')!=NULL   )  f_flag=1 ;
           else if(strchr(cmd, 't')!=NULL ||
                   strchr(cmd, 'T')!=NULL   )  t_flag=1 ;
           else if(strchr(cmd, 'c')!=NULL ||
                   strchr(cmd, 'C')!=NULL   )  c_flag=1 ;

                           cmd=end+1 ;
                        }

       if(f_flag+t_flag+c_flag==0) {
                      SEND_ERROR("������ ���� ����� ���� �� ���� �� ������: F, T ��� C. \n"
                                 "��������: IFILE/F <���_�������> ...") ;
                                           return(-1) ;
                                   }
       else
       if(f_flag+t_flag+c_flag!=1) {
                      SEND_ERROR("������ ���� ������ ���� �� ������: F, T ��� C. \n"
                                 "��������: IFILE/F <���_�������> ...") ;
                                           return(-1) ;
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

/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: IFILE <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=(Crowd_Object_External *)FindObject(name, 1) ;        /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*--------------------------------------------------- ������� ������ */

    if(pars[1]==NULL) {
                          SEND_ERROR("�� ������ �������� ���������") ;
                                             return(-1) ;
                      }

    if(f_flag) {

            if(access(pars[1], 0x00)!=0) {
                      SEND_ERROR("������� �������������� �����") ;
                                             return(-1) ;
                                         }

                  strcpy(object->iface_file_folder, pars[1]) ;
               }
    else 
    if(t_flag) {
                  strcpy(object->iface_targets, pars[1]) ;
               }
    else 
    if(c_flag) {
                  strcpy(object->iface_file_control, pars[1]) ;
               }
/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� ITCP                    */
/*                                                                  */
/*       ITCP/U <���> <URL>                                         */

  int  Crowd_Module_External::cITcp(char *cmd)

{
#define   _PARS_MAX  10

                    char  *pars[_PARS_MAX] ;
                    char  *name ;
   Crowd_Object_External  *object ;
                     int   u_flag ;
                    char  *end ;
                     int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                      u_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, 'u')!=NULL ||
                   strchr(cmd, 'U')!=NULL   )  u_flag=1 ;

                           cmd=end+1 ;
                        }

       if(u_flag/*+t_flag*/==0) {
                         SEND_ERROR("������ ���� ����� ���� �� ���� �� ������: U ... \n"
                                    "��������: ITCP/U <���_�������> ...") ;
                                           return(-1) ;
                                }
       else
       if(u_flag/*+t_flag*/!=1) {
                         SEND_ERROR("������ ���� ������ ���� �� ������: U ... \n"
                                    "��������: ITCP/U <���_�������> ...") ;
                                           return(-1) ;
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

/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: ITCP <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=(Crowd_Object_External *)FindObject(name, 1) ;        /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*--------------------------------------------------- ������� ������ */

    if(pars[1]==NULL) {
                          SEND_ERROR("�� ������ �������� ���������") ;
                                             return(-1) ;
                      }

    if(u_flag) {
                  strcpy(object->iface_tcp_connect, pars[1]) ;
                  strcpy(object->iface_targets,     pars[1]) ;
               }
/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� SENDER                  */
/*                                                                  */
/*       SENDER <���> <��������� �����������>                       */

  int  Crowd_Module_External::cSender(char *cmd)

{
#define   _PARS_MAX  10

                    char  *pars[_PARS_MAX] ;
                    char  *name ;
                    char  *sender ;
   Crowd_Object_External  *object ;
                     int   u_flag ;
                    char  *end ;
                     int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                      u_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, 'u')!=NULL ||
                   strchr(cmd, 'U')!=NULL   )  u_flag=1 ;

                           cmd=end+1 ;
                        }
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name  =pars[0] ;
                     sender=pars[1] ;

/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: SENDER <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=(Crowd_Object_External *)FindObject(name, 1) ;        /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*----------------------------------- �������� ��������� ����������� */

    if(sender==NULL) {
                          SEND_ERROR("�� ������ �������� ��������� �����������") ;
                                             return(-1) ;
                     }

    do {
/*- - - - - - - - - - - - - - - - - �������� �� ����������� "Sender" */
#define   NAMES       this->kernel->kernel_names 
#define   NAMES_CNT   this->kernel->kernel_names_cnt 

       for(i=0 ; i<NAMES_CNT ; i++)
         if(!stricmp(NAMES[i]->module, "Sender") &&
            !stricmp(NAMES[i]->name,    sender )   )  break ;

         if(i<NAMES_CNT)  break ;

#undef   NAMES
#undef   NAMES_CNT
/*- - - - - - - - - - - - - - - - - - - - �������� �� ����� �������� */
#define   MODULES       this->kernel->modules 
#define   MODULES_CNT   this->kernel->modules_cnt 


       for(i=0 ; i<MODULES_CNT ; i++) 
         if(MODULES[i].entry->category!=NULL)  
          if(!stricmp(MODULES[i].entry->category,      "Object") &&
             !stricmp(MODULES[i].entry->identification, sender )   )  break ;

         if(i<MODULES_CNT)  break ;

#undef    MODULES
#undef    MODULES_CNT
/*- - - - - - - - - - - - - - ���� ����������� ��������� ����������� */
             SEND_ERROR("K�������� ����������� ������ ���� �������������� ������ �� ����� ��������, "
                        "���� ���� ���������������� � ����������� SENDER (REFERENCE SENDER ...)"      ) ;
                                             return(-1) ;

       } while(0) ;

/*--------------------------------------------------- ������� ������ */

                  strcpy(object->SenderType, sender) ;

/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                    ���������� ���������� GAS                     */
/*                                                                  */
/*       GASE    <���> <����������>                                 */
/*       GASE/M  <���> <����������>                                 */
/*       GASE/R  <���> <����������>                                 */

  int  Crowd_Module_External::cGas(char *cmd)

{
#define   _PARS_MAX  10

                   char  *pars[_PARS_MAX] ;
                   char  *name ;
  Crowd_Object_External  *object ;
                    int   m_flag, r_flag ;
                 double   value ;
                   char  *end ;
                    int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                      m_flag=0 ;
                      r_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, 'm')!=NULL ||
                   strchr(cmd, 'M')!=NULL   )  m_flag=1 ;
           else if(strchr(cmd, 'r')!=NULL ||
                   strchr(cmd, 'R')!=NULL   )  r_flag=1 ;

                           cmd=end+1 ;
                        }

       if(m_flag+r_flag!=1) {
                      SEND_ERROR("����� ���� ������ ���� �� ������: M ��� R. \n"
                                 "��������: GAS/M <���_�������> ...") ;
                                           return(-1) ;
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

/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: GAS <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=(Crowd_Object_External *)FindObject(name, 1) ;        /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*--------------------------------------------------- ������� ������ */

    if(pars[1]==NULL) {
                          SEND_ERROR("�� ������ �������� ���������") ;
                                             return(-1) ;
                      }

        value=strtod(pars[1], &end) ;
    if(*end!=0) {
                  SEND_ERROR("������������ �������� ��������") ;
                        return(-1) ;
                }

         if(m_flag)  object->gas_max  =value ;
    else if(r_flag)  object->gas_renew=value ;
    else             object->gas      =value ;

/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*                                                                  */
/*              ����� ������� ���� EXTERNAL �� �����                */

  Crowd_Object_External *Crowd_Module_External::FindObject(char *name, int  check_type)

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

    if(check_type)
     if(strcmp(OBJECTS[i]->Type, "External")) {

           SEND_ERROR("������ �� �������� �������� ���� EXTERNAL") ;
                            return(NULL) ;
                                              }
/*-------------------------------------------------------------------*/ 

   return((Crowd_Object_External *)OBJECTS[i]) ;
  
#undef   OBJECTS
#undef   OBJECTS_CNT

}


/********************************************************************/
/********************************************************************/
/**                                                                **/
/**            �������� ������ ������� "������� ������"            **/
/**                                                                **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*                     ����������� ����������                       */

 O_EXTERNAL_API                              char *Crowd_Object_External::targets     =NULL ;
 O_EXTERNAL_API                              long  Crowd_Object_External::targets_time=  0. ;
 O_EXTERNAL_API                               int  Crowd_Object_External::targets_init=  0 ;
 O_EXTERNAL_API  struct Crowd_Object_ExternalLink *Crowd_Object_External::targets_links[_TARGETS_LINKS_MAX] ;
 O_EXTERNAL_API                               int  Crowd_Object_External::targets_links_cnt ;

/********************************************************************/
/*                                                                  */
/*                      ����������� ������                          */

     Crowd_Object_External::Crowd_Object_External(void)

{
    strcpy(Type, "External") ;

           start_flag=0 ;

    strcpy(object_type,        "") ;

    strcpy(iface_type,         "") ;
    strcpy(iface_file_folder,  "") ;
    strcpy(iface_file_control, "$NAME$") ;
    strcpy(iface_tcp_connect,  "") ;
    strcpy(iface_targets,      "") ;

        memory_t     =  0 ;
        memory_events=NULL ;
}


/********************************************************************/
/*                                                                  */
/*                         ���������� ������                        */

    Crowd_Object_External::~Crowd_Object_External(void)

{
}


/********************************************************************/
/*                                                                  */
/*                      ������������ ��������                       */

  void   Crowd_Object_External::vFree(void)

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
/*                      �������� ������ � ������                    */

    void  Crowd_Object_External::vWriteSave(std::string *text)

{
  char  field[1024] ;
   int  i ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN OBJECT EXTERNAL\n" ;

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

     int  Crowd_Object_External::vEventStart(void)
{
         char  name[FILENAME_MAX] ;
         char  flag[FILENAME_MAX] ;

/*--------------------------------- ������������� ���������� ������� */

                                 memory_t=0 ;

   if(memory_events!=NULL)  free(memory_events) ;

      memory_events=(char *)calloc(8, 1) ;

/*----------------------- ������� �������� ������ ��� ������� ������ */

/*- - - - - - - - - - - - - - - - - - -  ������� ��� ���������� FILE */
     if(!stricmp(this->iface_type, "FILE"      )) {

                                    strcpy(name, this->iface_file_control) ;
      if(!stricmp(name, "$NAME$"))  strcpy(name, this->Name) ;

            sprintf(flag, "%s/%s.out.flag", this->iface_file_folder, name) ;
             unlink(flag) ;
            sprintf(flag, "%s/%s.in.flag", this->iface_file_folder, name) ;
             unlink(flag) ;

                                                  }
/*- - - - - - - - - - - - - - - -  ������� ��� ���������� TCP-SERVER */
     else
     if(!stricmp(this->iface_type, "TCP-SERVER")) {


                                                  }
/*-------------------------------------------------------------------*/

  return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                       ��������� �������                          */

     static            Crowd_Kernel *EventTask ;
     static   Crowd_Object_External *EventObject ;


     int  Crowd_Object_External::vEvent(long  t, char *type, void  *data, Crowd_Kernel *task)
{
    Crowd_Message *message ;
             char *name_s ;
             char *name_r ;
             char *info ;
              int  size ;
              int  status ;

/*--------------------------------------- ����� �������� ����������� */


/*---------------------------------------- ������������� ������ ���� */

   if(t!=memory_t) {

         memory_t=t ;

     if(memory_events!=NULL)  free(memory_events) ;                 /* ������������� ������� ���������� ������� */
        memory_events=(char *)calloc(8, 1) ;

                message=new Crowd_Message ;
         strcpy(message->Type, "$$EXTERNAL") ;
                message->Object_s=this ; 
                message->Object_r=this ; 

         status=EventTask->vAddMessage(message, 0) ;
      if(status) {
                    SEND_ERROR("������ EXTERNAL: ���������� ��������� ����������� ��������� � �������") ;
                       return(-1) ;
                 }

                   }
/*-------------------------------------------------------- ��������� */

   if(!stricmp(type, "MESSAGE")) {

                  EventObject= this ;
                    EventTask= task ;
                      message=(Crowd_Message *)data ;

/*------------------------------------- ����� � ������� ������������ */

     if(!stricmp(message->Type, "$$EXTERNAL")) {

             status=SendObjectsList(t) ;
          if(status)  return(-1) ;

             status=SendRequest(t) ;
          if(status)  return(-1) ;

             status=WaitResponse(t) ;
          if(status)  return(-1) ;

                      return(0) ;
                                               }
/*-------------------------------------------- ����������� ��������� */

     else                                      {

       if(!stricmp(message->Type, "system")) {
                                                 name_s="" ;
                                                 name_r="" ;
                                                 info  ="" ;
                                             }
       else                                  {

                    if(message->Object_s!=NULL)  name_s=message->Object_s->Name ;
                    else                         name_s= "" ;

                    if(message->Object_r!=NULL)  name_r=message->Object_r->Name ;
                    else                         name_r= "" ;

                    if(message->Info    !=NULL)  info  =message->Info ;
                    else                         info  = "" ;
                                             }

                     size =256 ;
                     size+=strlen(name_s) ;
                     size+=strlen(name_r) ;
                     size+=strlen(message->Type) ;
                     size+=strlen(message->Kind) ;
                     size+=strlen(message->Name) ;
                     size+=strlen(info) ;

          memory_events=(char *)realloc(memory_events, strlen(memory_events)+size) ;

       if(memory_events[0]!=0)  strcat(memory_events, ",\r\n") ;

          sprintf(memory_events+strlen(memory_events), " { \"event\":\"message\"," 
                                                        " \"sender\":\"%s\","
                                                      " \"receiver\":\"%s\","
                                                          " \"type\":\"%s\","
                                                          " \"kind\":\"%s\","
                                                          " \"name\":\"%s\","
                                                           "\"info\":\"%s\" }", 
                                                              name_s,
                                                              name_r,
                                                              message->Type,
                                                              message->Kind,
                                                              message->Name,
                                                              info ) ;

                                               }
/*-------------------------------------------------------------------*/
                                 }
/*-------------------------------------------------------------------*/

 
  return(0) ;
}


/********************************************************************/
/*                                                                  */
/*            ����������� ���������� ��������� �������              */

     int  Crowd_Object_External::vEventShow(void)
{
  int  i ;

/*------------------------------------------------ ��������� ������� */

   for(i=0 ; i<this->Features_cnt ; i++)
       this->Features[i]->vBodyBasePoint("External.Body", this->x_base, 
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
/*            ������������ � �������� ������ ��������               */

     int  Crowd_Object_External::SendObjectsList(long t)
{
        FILE *file ;
        char  text[4096] ;
         int  status ;
         int  i ;

/*------------------------------------- ������������ ������ �������� */

#define   OBJECTS       this->Module->kernel_objects 
#define   OBJECTS_CNT   this->Module->kernel_objects_cnt 

    if(targets_init==1) {
                            targets_time=-1. ;
                            targets_init= 0 ;
                        }

    if(targets_time<t ) {

       if(targets==NULL)   targets=(char *)calloc(1, _TARGETS_MAX) ;

                          *targets=0 ;

        sprintf(text, "{  \"method\":\"targets\",\"t\":\"%ld\",\"objects\":[\r\n", t) ;
         strcat(targets, text) ;  

       for(i=0 ; i<OBJECTS_CNT ; i++) {

#define   O    OBJECTS[i]

         if(i>0)  strcat(targets, ",") ;

             sprintf(text, "{ \"name\":\"%s\","
                             "\"x\":\"%.2lf\",\"y\":\"%.2lf\",\"z\":\"%.2lf\","
                             "\"type\":\"%s\"}\r\n",
                                O->Name,
                                O->x_base_save, O->y_base_save, O->z_base_save,
                                O->Type ) ;

              strcat(targets, text) ;  

#undef    O
                                      }

              strcat(targets, "]\r\n}") ;  

                                  targets_time=t ;
                        } 

#undef   OBJECTS
#undef   OBJECTS_CNT

/*----------------------------------------- �������� ������ �������� */

   do {
/*- - - - - - - - - - - - - - - - - - -  �������� ��������� �������� */
         for(i=0 ; i<targets_links_cnt ; i++)
           if(!strcmp(targets_links[i]->link, this->iface_targets))  break ;

           if(i<targets_links_cnt) {
                  if(targets_links[i]->time_mark==t)  break ;      /* ���� ������ �������� �� ������ ������ ��� ������������ */ 
                                   }
           else                    {
                       targets_links[i]=(struct Crowd_Object_ExternalLink *)calloc(1, sizeof(struct Crowd_Object_ExternalLink)) ;
                strcpy(targets_links[i]->link, this->iface_targets) ;
                       targets_links_cnt++ ;
                                   }

                     targets_links[i]->time_mark=t ;
/*- - - - - - - - - - - - - -  �������� ������� ����� ��������� FILE */
   if(!stricmp(this->iface_type, "FILE")) {

         file=fopen(this->iface_targets, "wb") ;
      if(file==NULL) {
                          sprintf(text, "������ �������� ����� ��������: %s", this->iface_targets) ;
                       SEND_ERROR(text) ;
                            return(-1) ;
                     }

             fwrite(targets, 1, strlen(targets), file) ;
             fclose(file) ;

                                          }
/*- - - - - - - - - - -  �������� ������� ����� ��������� TCP-SERVER */
   if(!stricmp(this->iface_type, "TCP-SERVER")) {

         status=TCP_send(targets, strlen(targets)+1) ;
      if(status)  return(-1) ;

                                                }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
      } while(0) ;

/*-------------------------------------------------------------------*/

       return(0) ;
}


/********************************************************************/
/*                                                                  */
/*            ������������ � �������� ������� �� ���������          */

     int  Crowd_Object_External::SendRequest(long t)
{
          char  name[128] ;
          char  flag[FILENAME_MAX] ;
          char  path[FILENAME_MAX] ;
          FILE *file ;
          char  value[128] ;
           int  status ;
           int  i ; 

  static  char *text ;

#undef   _BUFF_MAX
#define  _BUFF_MAX  32000

/*---------------------------------------------------- ������������� */

    if(text==NULL)  text=(char *)calloc(_BUFF_MAX, 1) ;

/*--------------------------------------------- ������������ ������� */

         memset(text, 0, _BUFF_MAX) ;

         strcat(text, "{ \"method\":\"start\",");
        sprintf(value, "\"name\":\"%s\",\"t\":\"%ld\",", this->Name, t) ;
         strcat(text, value) ;  
        sprintf(value, "\"x\":\"%.2lf\",\"y\":\"%.2lf\",\"z\":\"%.2lf\",", this->x_base, this->y_base, this->z_base) ;
         strcat(text, value) ;  
        sprintf(value, "\"type\":\"%s\",\r\n", this->object_type) ;
         strcat(text, value) ; 

        sprintf(value, "\"links\":[\r\n") ;                           /* ������ ������ */
         strcat(text, value) ; 

    for(i=0 ; i<this->Communications_cnt ; i++) {  

      if(i>0)  strcat(text, ",") ; 

        sprintf(value, " { \"type\":\"%s\", ", this->Communications[i]->Type) ;
         strcat(text, value) ; 
        sprintf(value, "\"kind\":\"%s\", ", this->Communications[i]->Kind) ;
         strcat(text, value) ; 
        sprintf(value, "\"master\":\"%s\", ", this->Communications[i]->Object_m->Name==NULL ? "" : this->Communications[i]->Object_m->Name) ;
         strcat(text, value) ; 
        sprintf(value, "\"slave\":\"%s\" }\r\n", this->Communications[i]->Object_s->Name==NULL ? "" : this->Communications[i]->Object_s->Name) ;
         strcat(text, value) ; 
                                                } 

        sprintf(value, "],\r\n") ;
         strcat(text, value) ; 

        sprintf(value, "\"events\":[\r\n") ;                        /* ������ ������� */
         strcat(text, value) ; 
         strcat(text, this->memory_events) ; 
        sprintf(value, "\r\n]\r\n") ;
         strcat(text, value) ; 

         strcat(text, "}");
/*- - - - - - - - - - - - - -  �������� ������� ����� ��������� FILE */
   if(!stricmp(this->iface_type, "FILE")) {
                         
                                    strcpy(name, this->iface_file_control) ;
      if(!stricmp(name, "$NAME$"))  strcpy(name, this->Name) ;

            sprintf(path, "%s/%s.out",      this->iface_file_folder, name) ;
            sprintf(flag, "%s/%s.out.flag", this->iface_file_folder, name) ;

         file=fopen(path, "wb") ;
      if(file==NULL) {
                          sprintf(text, "������ %s - ������ �������� ����� �������: %s", this->Name, path) ;
                       SEND_ERROR(text) ;
                            return(-1) ;
                     }

             fwrite(text, 1, strlen(text), file) ;
             fclose(file) ;

         file=fopen(flag, "w") ;
      if(file==NULL) {
                          sprintf(text, "������ %s - ������ �������� ����-����� �������: %s", this->Name, path) ;
                       SEND_ERROR(text) ;
                            return(-1) ;
                     }

             fclose(file) ;
 
                                          }
/*---------------------- �������� ������� ����� ��������� TCP-SERVER */

   if(!stricmp(this->iface_type, "TCP-SERVER")) {

         status=TCP_send(text, _BUFF_MAX-1) ;
      if(status)  return(-1) ;

                                                }
/*-------------------------------------------------------------------*/

#undef  _BUFF_MAX

       return(0) ;
}


/********************************************************************/
/*                                                                  */
/*                   ����a��� ���������� ���������                  */

     int  Crowd_Object_External::WaitResponse(long t)
{
           char  name[128] ;
           char  flag[FILENAME_MAX] ;
           char  path[FILENAME_MAX] ;
           FILE *file ;
           char  em_name[128] ;
           long  em_t ;
           char  em_commands[4096] ;
            int  status ;
           char  command[128] ;
           char  value[1024] ;
           char  term ;
           char *cmd ;
           char *cmd_end ;
  Crowd_Message  cmd_message ;
  Crowd_Message *message ;
   Crowd_Kernel *msg_module ;
           char  msg_receiver[128] ;
           char  msg_info[1024] ;
            int  msg_delay ; 
           char *key ;
           char *end ;
            int  i ;

  static  char *keys[]={"\"name\":\"",
                        "\"t\":\"",
                        "\"x\":\"",
                        "\"y\":\"",
                        "\"z\":\"",
                        "\"commands\":[",
                            NULL} ;

//  {"command":"sendmessage","name":"ext1-1","type":"Contact","kind":"Info","recipient":"sender1","info":"spectr:1,0.7,-0.5","delay":"0"}
  static  char *sm_keys[]={"\"name\":\"",
                           "\"type\":\"",
                           "\"kind\":\"", 
                           "\"recipient\":\"",
                           "\"info\":\"",
                           "\"delay\":\"",
                            NULL} ;

  static  char *text ;

#undef   _BUFF_MAX
#define  _BUFF_MAX  32000

/*---------------------------------------------------- ������������� */

    if(text==NULL)  text=(char *)calloc(_BUFF_MAX, 1) ;

/*-------------------------------------- �������� ���������� ������� */

   do {
/*- - - - - - - - - - - - - - - �������� ������ ����� ��������� FILE */
   if(!stricmp(this->iface_type, "FILE")) {

                                    strcpy(name, this->iface_file_control) ;
      if(!stricmp(name, "$NAME$"))  strcpy(name, this->Name) ;

        sprintf(path, "%s/%s.in",      this->iface_file_folder, name) ;
        sprintf(flag, "%s/%s.in.flag", this->iface_file_folder, name) ;

      if(access(flag, 0x00)) continue ;

         file=fopen(path, "rt") ;
      if(file==NULL) {
                          sprintf(text, "������ %s - ������ �������� ����� ������: %s", this->Name, path) ;
                       SEND_ERROR(text) ;
                            return(-1) ;
                     }

             memset(text, 0, _BUFF_MAX) ;
              fread(text, 1, _BUFF_MAX-1, file) ;
             fclose(file) ;

                    status=unlink(flag) ;
      if(status) {
                            Sleep(100) ;
                    status=unlink(flag) ;
                 }
      if(status) {
                          sprintf(text, "ERROR - Response flag-file remove error %d : %s", errno, flag) ;
                       SEND_ERROR(text) ;
                            return(-1) ;
                 }

                      break ;
                                          }
/*- - - - - - - - - - -  �������� ������� ����� ��������� TCP-SERVER */
   if(!stricmp(this->iface_type, "TCP-SERVER")) {

         memset(text, 0, _BUFF_MAX) ;

         strcat(text, "{ \"method\":\"get\",");
        sprintf(value, "\"name\":\"%s\",\"t\":\"%ld\",\"type\":\"%s\"", this->Name, t, this->object_type) ;
         strcat(text, value) ;  
         strcat(text, " }");

         status=TCP_send(text, _BUFF_MAX-1) ;
      if(status)  return(-1) ;

      if(strstr(text, "\"result\":\"wait\"")==NULL)  break ;
                                                }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
             Sleep(10) ;

      } while(1) ;

/*-------------------------------------------- ������ ������ ������� */

   for(i=0 ; keys[i]!=NULL ; i++) {

        key=strstr(text, keys[i]) ;
     if(key==NULL) {
                         sprintf(text, "Parameter %s is missed", keys[i]) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                   } 

            memset(value, 0, sizeof(value)) ;
           strncpy(value, key+strlen(keys[i]), sizeof(value)-1) ;

        term=(keys[i])[strlen(keys[i])-1] ;
     if(term=='[')  term=']' ;
     if(term=='{')  term='}' ;
    
        end=strchr(value, term)  ;
     if(end==NULL) {
                         sprintf(text, "Invalid value for parameter %s", keys[i]) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                   } 

       *end=0 ;

     if(i== 0)  strcpy(em_name, value) ;  
     if(i== 1)         em_t            =strtoul(value, &end, 10) ;
     if(i== 2)         this->x_base    = strtod(value, &end) ;
     if(i== 3)         this->y_base    = strtod(value, &end) ;
     if(i== 4)         this->z_base    = strtod(value, &end) ;
     if(i== 5)  strcpy(em_commands, value) ;

                                  } 
/*----------------------------- �������� ������������ ������ ������� */

     if(stricmp(em_name, this->Name)) {
                         sprintf(text, "Invalid object name in response: %s <> %s", em_name, this->Name) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                                      }

     if(em_t!=t) {
                         sprintf(text, "Invalid time mark in response: %ld <> %ld", em_t, t) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                 }
/*------------------------------- ������������ ������ �������� ����� */

  while(em_commands[0]!=0) {

         cmd=strchr(em_commands, '{') ;
      if(cmd==NULL)   break ;

         cmd_end=strchr(cmd, '}') ;
      if(cmd_end==NULL)  {
                              sprintf(text, "Command element terminator ']' missed") ;
                           SEND_ERROR(text) ;
                                return(-1) ;
                         }             

        *cmd_end=0 ;
/*- - - - - - - - - - - - - - - - - - - - - - -  ����������� ������� */
         key=strstr(cmd, "\"command\":\"") ;
      if(key==NULL) {
                              sprintf(text, "Command spcification missed") ;
                           SEND_ERROR(text) ;
                                return(-1) ;
                    }             

             memset(command, 0, sizeof(command)) ;
            strncpy(command, key+strlen("\"command\":\""), sizeof(command)-1) ;

         cmd_end=strchr(command, '"') ;
      if(cmd_end==NULL)  {
                              sprintf(text, "Command tag terminator missed") ;
                           SEND_ERROR(text) ;
                                return(-1) ;
                         }             

        *cmd_end=0 ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - -  SendMessage */
//  "name":"...","type":"...","kind":"...","receiver":"...","info":"...","delay":"..."
   if(!stricmp(command, "SendMessage")) {

     for(i=0 ; sm_keys[i]!=NULL ; i++) {                            /* ������ ��������� ������� */

          key=strstr(cmd, sm_keys[i]) ;
       if(key==NULL) {
                           sprintf(text, "SendMessage - parameter %s is missed", sm_keys[i]) ;
                        SEND_ERROR(text) ;
                             return(-1) ;
                     } 

            memset(value, 0, sizeof(value)) ;
           strncpy(value, key+strlen(sm_keys[i]), sizeof(value)-1) ;

          end=strchr(value, '"')  ;
       if(end==NULL) {
                         sprintf(text, "SendMessage - invalid value for parameter %s", sm_keys[i]) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                   } 

         *end=0 ;

       if(i== 0)            strcpy(cmd_message.Name, value) ;  
       if(i== 1)            strcpy(cmd_message.Type, value) ;  
       if(i== 2)            strcpy(cmd_message.Kind, value) ;  
       if(i== 3)            strcpy(msg_receiver,     value) ;  
       if(i== 4)            strcpy(msg_info,         value) ;  
       if(i== 5)  msg_delay=strtol(value, &end, 10) ;

                                       } 

#define  KERNEL  ProgramModule.kernel

     for(i=0 ; i<KERNEL->modules_cnt ; i++)                         /* ����� ������, ����������� � �������� ����� ��������� */
       if(!stricmp(KERNEL->modules[i].entry->identification,
                                              cmd_message.Type)) {
                     msg_module=KERNEL->modules[i].entry ;
                            break ;
                                                                 }
 
       if(i>=KERNEL->modules_cnt) {                                 /* ���� ����� ������ �� ������... */
                         sprintf(text, "SendMessage - unwnown message type: %s", cmd_message.Type) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                                  }

                         cmd_message.Object_s= this ;
                         cmd_message.Object_r= NULL ;

       if(msg_receiver[0]!=0) {                                     /* ���������� ���������� ��������� */
            cmd_message.Object_r=ProgramModule.FindObject(msg_receiver, 0) ;
         if(cmd_message.Object_r==NULL) {
                         sprintf(text, "SendMessage - unwnown receiver: %s", msg_receiver) ;
                      SEND_ERROR(text) ;
                           return(-1) ;
                                        }
                              }
                                                                    /* �������� ��������� */ 
                 message=msg_module->vCreateMessage(cmd_message.Object_s, cmd_message.Object_r, NULL) ;
          strcpy(message->Name, cmd_message.Name) ;
          strcpy(message->Kind, cmd_message.Kind) ;

                 message->Info=(char *)calloc(1, strlen(msg_info)+1) ;
          strcpy(message->Info, msg_info) ;

          status=EventTask->vAddMessage(message, msg_delay+1) ;     /* ���������� ��������� � ������� */
       if(status) {
                         sprintf(text, "SendMessage - message queuing up error") ;
                      SEND_ERROR(text) ;
                  }

                                                }
/*- - - - - - - - - - - - - - - - - - - - - - -  ����������� ������� */
   else                                         {

            sprintf(text, "Unknown elements in command: %s", command) ;
         SEND_ERROR(text) ;
              return(-1) ;
                                                }
/*- - - - - - - - - - - - - - - - - - -  ������� � ��������� ������� */
            memmove(em_commands, cmd_end+1, strlen(cmd_end+1)+1) ;

                          } 
/*-------------------------------------------------------------------*/

     return(0) ;
}


/*********************************************************************/
/*                                                                   */
/*           ����� ������� � ����� �� �������� ���������             */

  int  Crowd_Object_External::TCP_send(char *text, int size)

{
               Tcp  Transport ;
              char  url[512] ;
              char *port ;
              char  error[512] ;
              char *result ;
              char *end ;
               int  status ;
               int  cnt ;
               int  i, j ;

      static  char *buff ;

#undef  _BUFF_MAX
#define _BUFF_MAX   128000

/*---------------------------------------------------- ������������� */

     if(buff==NULL)  buff=(char *)calloc(1, _BUFF_MAX) ;

/*-------------------------------------------- ����������� ��������� */

     for(i=0, j=0 ; text[i] ; i++) {

          text[j]=text[i] ;

       if(text[i]!='\r' &&
          text[i]!='\n'   )  j++ ;
                                   }

          text[j]=0 ;

/*-------------------------------------------- ��������� URL ������� */

             memset(url,    0, sizeof(url)  ) ;
            strncpy(url, this->iface_tcp_connect, sizeof(url)-1) ;
        port=strchr(url, ':')  ;
     if(port==NULL) {
                       sprintf(error, "������ URL: <host>:<port>") ;
                    SEND_ERROR(error) ;
                          return(-1) ;
                    }

       *port=0 ;
        port++ ;

/*-------------------------------------------- ���������� � �������� */

              Transport.mServer_name=url ;
              Transport.mServer_port=strtoul(port, &end, 10) ;
 
   if(*end!=0) {
                     sprintf(error, "������ URL - ������������ ����� �����") ;
                  SEND_ERROR(error) ;
                        return(-1) ;
               }

      status=Transport.LinkToServer() ;
   if(status) {
                     sprintf(error, "������ ���������� � �������� %s �� ����� %s : %d", url, port, status) ;
                  SEND_ERROR(error) ;
                        return(-1) ;
              } 
/*---------------------------------------- ������������ HTTP-������� */

        sprintf(buff, "POST / HTTP/1.0\r\n"
                      "Host: %s\r\n"
//                                "Accept: application/json\r\n"
                      "Content-Type: application/json; charset=UTF-8\r\n"
                      "Content-Length: %d\r\n"
                      "\r\n%s",
                         url, (int)strlen(text), text) ;

/*------------------------------------------------- �������� ������� */

      cnt=Transport.iSend(Transport.mSocket_cli,                    /* �������� ������ */ 
                               buff, strlen(buff), _WAIT_RECV) ;
   if(cnt==SOCKET_ERROR) {                                          /* ���� ������... */
                                  sprintf(error, "Request send error - %d", WSAGetLastError()) ;
                               SEND_ERROR(error) ;
                                     return(-1) ;
                         }
/*----------------------------------------------------- ����� ������ */

                        memset(buff, 0, _BUFF_MAX) ;                /* ������� �������� ����� */
                                    cnt=_BUFF_MAX ;

          Transport.mReceiveCallback=Object_External_Http_receive ; /* ������ ����������� ������� ������ */

      cnt=Transport.iReceive(Transport.mSocket_cli,                 /* ��������� ����� */ 
                                    buff, cnt-1, _WAIT_RECV) ;

          Transport.mReceiveCallback=NULL ;

   if(cnt==SOCKET_ERROR) {                                          /* ���� ������ */
                                  sprintf(error, "Reply receive error - %d", WSAGetLastError()) ;
                               SEND_ERROR(error) ;
                                     return(-1) ;
                         }
/*------------------------------------------- ������ ������ �� ����� */

                        result=strchr(buff, '{') ;
      if(result==NULL)  result=       buff ;

            strncpy(text, result, size-1) ;

/*-------------------------------------------- ���������� ���������� */

                    Transport.ClientClose() ;

/*-------------------------------------------------------------------*/

   return(0) ;
}


/*********************************************************************/
/*								     */
/*                ������� ���������� ������� ��� HTTP                */

   int  Object_External_Http_receive(char *data, int  data_size)

{
   char  work[8192] ;
   char *entry ;
   char *content ;
   char *end ;
    int  size ;
    int  cnt ;
    int  i ;

/*------------------------------------------ ��������� ������ ������ */

               memset(work, 0, sizeof(work)) ;                      /* �������� HTTP-��������� */

        if(data_size<sizeof(work))  size=data_size ;
        else                        size=sizeof(work)-1 ;

              strncpy(work, data, size) ;

       content=strstr(work, "\r\n\r\n") ;                           /* ���� ���� �������� */

           end=strchr(work, '\n') ;                                 /* �������� ������ ������ */
        if(end!=NULL)  *end=0 ;

/*---------------------------------------------------------- ���� OK */

   if(strstr(work, " 200 OK")!=NULL) {

        if(end!=NULL)  *end='\n' ;                                  /* ��������������� ������ */
      
        if(content==NULL)  return(0) ;                              /* ���� ��� ����������� ��������... */
 
          *content = 0 ;                                            /* �������� ��������� */
           content+= 4 ;                                            /* �������� �� ������� */

                 strupr(work) ;                                     /* ��������� ��������� � ������� ������� */
           entry=strstr(work, "CONTENT-LENGTH:") ;                  /* ���� ���� ��������� ����� ������ */
        if(entry==NULL) {                                           /* ��� ���������� ������ ������� ������ -        */
                                                                    /*  - ���������� �� ���������� �� json-��������� */
         if(memchr(data, '}', data_size)==NULL)  return(0) ;

          for(cnt=0, i=0 ; i<data_size ; i++)
            if(data[i]=='{')  cnt++ ;
            else 
            if(data[i]=='}')  cnt-- ;

                 if(cnt)  return(0) ;
                          return(1) ;          
                        }

                         entry+=strlen("CONTENT-LENGTH:") ;
            size=strtoul(entry, &end, 10) ;                         /* ��������� ����� ������ */

        if( (data_size-(content-work))>=size )  return(1) ;         /* ���� ��� ������ �������� - ������� */

                                     }
/*------------------------------------------------------ ���� ������ */

   else                              {

        if(content!=NULL)  return(1) ;                              /* ���� ���� ����������� ��������... */

                                     }
/*-------------------------------------------------------------------*/

   return(0) ;

}

