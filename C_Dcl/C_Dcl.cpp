/********************************************************************/
/*								    */
/*		������ ������������ �����������        	            */
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

#include "..\DCL_kernel\dcl.h"

#include "C_Dcl.h"

#pragma warning(disable : 4996)


#define  SEND_ERROR(text)    SendMessage(Crowd_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,        \
                                         (LPARAM) text) ;

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

     static   Crowd_Calc_Dcl  ProgramModule ;


/********************************************************************/
/*								    */
/*		    	����������������� ����                      */

 C_DCL_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 C_DCL_API Crowd_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/********************************************************************/
/*								    */
/*		    	����� ������                                */

   static  char *Crowd_Calc_Dcl_text    =NULL ;
   static  char *Crowd_Calc_Dcl_text_cur=NULL ;

/********************************************************************/
/********************************************************************/
/**							           **/
/**                  �������� ������ ������                        **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     Crowd_Calc_Dcl::Crowd_Calc_Dcl(void)

{
	   keyword="Crowd" ;
    identification="DCL_processor" ;
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

     Crowd_Calc_Dcl::~Crowd_Calc_Dcl(void)

{
}


/********************************************************************/
/*								    */
/*		        �������� ��������       		    */

     int  Crowd_Calc_Dcl::vGetParameter(char *name, char *value)

{
/*-------------------------------------------------- �������� ������ */

    if(!stricmp(name, "$$MODULE_NAME")) {

         sprintf(value, "%-20.20s -  ��������� ���������", identification) ;
                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*		        ��������� ��������                          */

    void  Crowd_Calc_Dcl::vStart(void)

{
   calculate_modules=(Crowd_Kernel **)
                      realloc(calculate_modules, 
                              (calculate_modules_cnt+1)*sizeof(calculate_modules[0])) ;

      calculate_modules[calculate_modules_cnt]=&ProgramModule ;
                        calculate_modules_cnt++ ;
}


/********************************************************************/
/*								    */
/*		        ��������� ���������     		    */

     extern double  dcl_errno ;            /* ��������� ��������� ������ -> DCL_SLIB.CPP */
     extern   char  dcl_err_details[512] ; /* ��������� ��������� ������������ �������� ������ -> DCL_SLIB.CPP */

//  Dcl_decl *ERPC_dcl_GetDocs       (Lang_DCL *, Dcl_decl *, Dcl_decl **, int) ;    /* ��������� ������ ��������� ���������� */
//  Dcl_decl *ERPC_dcl_ExtractDoc    (Lang_DCL *,             Dcl_decl **, int) ;    /* ���������� ��������� �� ��������� */

    Dcl_decl  dcl_crowd_lib[]={

         {0, 0, 0, 0, "$PassiveData$", NULL, "erpc", 0, 0},
//	 {_DGT_VAL, _DCL_METHOD, 0, 0, "GetDocs",          (void *)ERPC_dcl_GetDocs,        "s",    0, 0},
//	 {_CHR_PTR, _DCL_CALL,   0, 0, "ExtractDoc",       (void *)ERPC_dcl_ExtractDoc,     "s",    0, 0},
	 {0, 0, 0, 0, "", NULL, NULL, 0, 0}
                             } ;


             int ERPC_dcl_debug(void) ;


  int  Crowd_Calc_Dcl::vCalculate(            char  *program_type, 
                                              char  *program, 
                            struct Crowd_Parameter  *const_list,
                            struct Crowd_Parameter  *var_list,
                                            double  *result,
                                              void **context_ptr, 
                                              char  *error       )

{
                Lang_DCL  DCL ;
  Crowd_Calc_Dcl_context *context ;
                     int  i ;

#define  _SYSVARS_MAX    100

     Dcl_decl *vars[10] ;
         char *libs[10] ;
     Dcl_decl  sys_vars[_SYSVARS_MAX] ;
         char  crn_path[512] ;

 extern Dcl_decl  dcl_debug_lib[] ;  
 extern Dcl_decl  dcl_std_lib[] ;
 extern Dcl_decl  dcl_file_lib[] ;   

#define        _BUFF_SIZE   64000

/*---------------------------------------- ������������� ����������� */

   if(stricmp(program_type, "DCL"  ) &&
      stricmp(program_type, "CLEAR")   )  return(1) ;

   if(context_ptr==NULL)  return(0) ;

#ifdef REMARK

//                                   iDebug("ExpressionType:", NULL) ;
//      if(expression_type!=NULL)    iDebug(expression_type, NULL) ;
//      else                         iDebug("NULL", NULL) ;
//                                   iDebug("Expression:", NULL) ;
//      if(expression!=NULL)         iDebug(expression, NULL) ;
//      else                    {
//                                   iDebug("NULL", NULL) ;
//                              }
//
//            lng_debug     = 1 ;
//            lng_user_debug=RSS_Calc_Std_debug ;

/*------------------------------------------------ ������� ��������� */

   if(!stricmp(program_type, "CLEAR")) {

          context=(Crowd_Calc_Dcl_context *)*context_ptr ;
       if(context==NULL)  return(0) ;

                              free(context) ;

                        *context_ptr=NULL ;

                           return(0) ;
                                       }
/*-------------------------------------------- ����������� ��������� */

       memset(vars, 0, sizeof(vars)) ;

	   vars[0]=sys_vars ;                                       /* ������ ���������� � ������� */
	   vars[1]=dcl_debug_lib ;
	   vars[2]=dcl_std_lib ;
	   vars[3]=dcl_file_lib ;
	   vars[4]=dcl_crowd_lib ;

       memset(libs, 0, sizeof(libs)) ;

           libs[0]=crn_path ;                                       /* ������ ������������ �������� */
            getcwd(crn_path, sizeof(crn_path)-1) ;

//          DCL.mDebug=Crowd_dcl_debug ;

/*--------------------------------------------- ���������� ��������� */

          context=(Crowd_Calc_Dcl_context *)*context_ptr ;
       if(context==NULL) {
                    
            context=(Crowd_Calc_Dcl_context *)
                         calloc(1, sizeof(*context)) ;
         if(context==NULL) {
                               SEND_ERROR("�����������> ������������ ������ ��� ���������") ;
                                                  return(-1) ;
                           }
                         }
/*----------------------------------------------- ������� ���������� */

       memset(sys_vars, 0, sizeof(sys_vars)) ;

                        i=0 ;
/*
       strcpy( sys_vars[i].name, "$contract") ;
               sys_vars[i].type=_DCL_CHAR_AREA ;
               sys_vars[i].addr= contract->contract ;
               sys_vars[i].size=strlen(contract->contract) ;
               sys_vars[i].buff= -1 ;
                        i++ ;
*/
/*
      if(const_list!=NULL) {
         for(j=0 ; const_list[j].name[0] ; j++) {

           if(const_list[j].ptr!=NULL && const_flag==1)  continue ;

                     variables[i].name     =(unsigned char *)const_list[j].name ;
                     variables[i].func_flag=  0 ;
                     variables[i].type     =_CCL_DOUBLE ;

           if(const_list[j].ptr==NULL)
                     variables[i].addr=&const_list[j].value ;
           else      variables[i].addr= const_list[j].ptr  ;
                               i++ ;
                                                }
                           }

*/
/*-------------------------------- ���������� ������������ ��������� */

                     DCL.mWorkDir    =crn_path ;
                     DCL.mLibDirs    =libs ;
                     DCL.mVars       =vars ;

                     DCL.mProgramMem =program ;
                     DCL.mProgramFile= NULL ;

                     DCL.vProcess() ;

/*------------------------------------------------- ��������� ������ */

      if(DCL.mError_code) {

            strcpy(error, DCL.vDecodeError(DCL.mError_code)) ;
//       CharToOem(error, error) ;

           sprintf(error, "File   :%s  Row:%d  Bad:<%s>\r\n"
                          "Error  :%d  %s\r\n"
                          "Details:%s\r\n",
                         (DCL.mError_file!=NULL)?DCL.mError_file:"NULL", 
                          DCL.mRow, 
                         (DCL.mError_position!=NULL)?DCL.mError_position:"NULL", 
                          DCL.mError_code, error, DCL.mError_details) ;

                                  return(-1) ;
                          }
/*---------------------------------------------- ��������� ��������� */

                               *context_ptr=context ;

/*-------------------------------------------------------------------*/
#endif

      return( 0) ;  
}


/********************************************************************/
/*								    */
/*                   ���������� ������ � �����                      */

   void  Crowd_Calc_Dcl::iDebug(char *text, char *path,  int  condition)
{
    if(condition)  iDebug(text, path) ;
}


   void  Crowd_Calc_Dcl::iDebug(char *text, char *path)
{
   static int  init_flag ;
         FILE *file ;         

//return ;

    if(path==NULL) {
                             path="dcl.log" ;
      if(!init_flag)  unlink(path) ;
          init_flag=1 ;
                   }

       file=fopen(path, "at") ;
    if(file==NULL)  return ;

           fwrite(text, 1, strlen(text), file) ;
           fwrite("\n", 1, strlen("\n"), file) ;
           fclose(file) ;
}


/*********************************************************************/
/*                                                                   */
/*                     ��������� ������� �����������                 */

  void  Crowd_Calc_Dcl_debug(int  frame)

{
   Crowd_Calc_Dcl  module ;
             char  text[1024] ;


             sprintf(text, "Frame %d", frame) ;
       module.iDebug(text, NULL) ;

     if(_heapchk()!=_HEAPOK) module.iDebug("Frame Heap crash!!!", NULL) ;
     else                    module.iDebug("Frame Heap OK!!!", NULL) ;

}
