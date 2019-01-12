
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>


#include "..\Crowd_feature\Crowd_feature.h"
#include "Crowd_object.h"

#pragma warning(disable : 4267)
#pragma warning(disable : 4996)


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


/*********************************************************************/
/*								     */
/*	         ���������� ������ "������� ���������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       ����������� ������ "������� ���������"      	     */

     Crowd_Transit::Crowd_Transit(void)

{
   memset(action, 0, sizeof(action)) ;

          object=NULL ;
}


/*********************************************************************/
/*								     */
/*	        ���������� ������ "������� ���������"      	     */

    Crowd_Transit::~Crowd_Transit(void)

{
}


/********************************************************************/
/*								    */
/*	              ���������� ��������                           */

    int  Crowd_Transit::vExecute(void)

{
   return(0) ;
}


/*********************************************************************/
/*								     */
/*		      ���������� ������ "������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*		       ����������� ������ "������"      	     */

     Crowd_Object::Crowd_Object(void)

{
     Parameters    =NULL ;
     Parameters_cnt=  0 ;
       Features    =NULL ;
       Features_cnt=  0 ;
 Communications    =NULL ;
 Communications_cnt=  0 ;

            Program=NULL ;
              State=NULL ;

        ErrorEnable= 1 ;

      x_base=0 ;
      y_base=0 ;
      z_base=0 ;

}


/*********************************************************************/
/*								     */
/*		        ���������� ������ "������"      	     */

    Crowd_Object::~Crowd_Object(void)

{
    int  i ;

/*---------------------------------------------- ������������ ������ */

       if(State!=NULL) {
                           free(State) ;
                                State=NULL ;
                       }
/*--------------------------------------------- �������� ����������� */

   for(i=0 ; i<this->Units.List_cnt ; i++)
                  delete this->Units.List[i].object ;

/*-------------------------------------------------------------------*/

}


/********************************************************************/
/*								    */
/*                        ���������� ������		            */

    class Crowd_Object *Crowd_Object::vCopy(char *name)

{
   return(NULL) ;
}


/********************************************************************/
/*								    */
/*                    ��������� ��������� �������                   */
/*                    ������������ ��������� �������                */

    void  Crowd_Object::vPush(void)

{
     x_base_stack    =x_base ;
     y_base_stack    =y_base ;
     z_base_stack    =z_base ;
}


    void  Crowd_Object::vPop(void)

{
     x_base    =x_base_stack ;
     y_base    =y_base_stack ;
     z_base    =z_base_stack ;
}


/********************************************************************/
/*								    */
/*		  ���./����. ��������� �� �������		    */

    void  Crowd_Object::vErrorMessage(int  on_off)

{
         ErrorEnable=on_off ;
}


/********************************************************************/
/*								    */
/*		        ������� ������ �� ������		    */

    void  Crowd_Object::vReadSave(std::string *text)

{
}


/********************************************************************/
/*								    */
/*		        �������� ������ � ������		    */

    void  Crowd_Object::vWriteSave(std::string *text)

{
     *text="" ;
}


/********************************************************************/
/*								    */
/*		      ���������� �������        		    */

    void  Crowd_Object::vFree(void)

{
}


/********************************************************************/
/*								    */
/*		      ����������� ��������      		    */

    void  Crowd_Object::vFormDecl(void)

{
   sprintf(Decl, "%s  %s", Name, Type) ;
}


/********************************************************************/
/*								    */
/*		      ������ � �������� �������   		    */
/*								    */
/*   Return: ����� ������� �����                                    */

     int  Crowd_Object::vGetPosition(Crowd_Point *points)

{
        points->x=x_base ;
        points->y=y_base ;
        points->z=z_base ;

    return(0) ;
}

    void  Crowd_Object::vSetPosition(Crowd_Point *points)

{
        x_base=points->x ;
        y_base=points->y ;
        z_base=points->z ;
}


/********************************************************************/
/*								    */
/*		����� ��������� �������� �������                    */

     int  Crowd_Object::vResetFeatures(void *data)

{
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++)
                this->Features[i]->vResetCheck(data) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		���������� ������� � �������� ������������          */

     int  Crowd_Object::vPrepareFeatures(void *data)

{
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++)
                this->Features[i]->vPreCheck(data) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		�������� ������������������ �������                 */

     int  Crowd_Object::vCheckFeatures(void *data, Crowd_Objects_List *checked)

{
  int  status ;
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++) {
           status=this->Features[i]->vCheck(data, checked) ;
        if(status)  break ;
                                         }
   return(status) ;
}


/********************************************************************/
/*								    */
/*		      ������ �� ����������                          */

    void  Crowd_Object::vSetFeature(Crowd_Feature *feature)

{
}

    void  Crowd_Object::vGetFeature(Crowd_Feature *feature)

{
}


/********************************************************************/
/*								    */
/*                        ����������� ��������                      */

     int  Crowd_Object::vSpecial(char *oper, void *data)
{
  return(-1) ;
}


/********************************************************************/
/*								    */
/*                  ���������� ��������� �������                    */

     int  Crowd_Object::vEventStart(void)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*                   ��������� �������                              */

     int  Crowd_Object::vEvent(long  t, char *type, void  *data, Crowd_Kernel *task)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*            ����������� ���������� ��������� �������              */

     int  Crowd_Object::vEventShow(void)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*                �������� ������ ���������� ����������             */

   int  Crowd_Object::vListControlPars(Crowd_ControlPar *list)

{
   return(0) ;
}


/********************************************************************/
/*								    */
/*               ���������� �������� ��������� ����������           */

   int  Crowd_Object::vSetControlPar(Crowd_ControlPar *par)   

{
   return(-1) ;
}


/*********************************************************************/
/*                                                                   */
/*               �������� ��������� ���� � ��������                  */

  int  Crowd_Object::iAngleInCheck(double  value, 
                                   double  value_min, 
                                   double  value_max )
{
  int  i ;


  for(i=0 ; i<2 ; i++) {

    if(value_min<=value_max) {
	 if(value>=value_min &&
	    value<=value_max   ) return(0) ;
			     }
    else                     {
	 if(value<=value_min &&
	    value>=value_max   ) return(0) ;
			     }

    if(value>0)  value-=360. ;
    else         value+=360. ;

                       }

  return(1) ;
}


/********************************************************************/
/*								    */
/*              ������ ���������� ��������� � ������                */

  int  Crowd_Object::iReplaceText(char *buff, char *name, char *value, int  count)

{
   char *entry ;
    int  shift ;
    int  i ;


     if(count==0)  count=10000 ;

             entry=buff ;
             shift=strlen(value)-strlen(name) ;

     for(i=0 ; i<count ; i++) {

             entry=strstr(entry, name) ;
          if(entry==NULL)  break ;
                         
               memmove(entry+strlen(value), entry+strlen(name), 
                                       strlen(entry+strlen(name))+1) ;
                memcpy(entry, value, strlen(value)) ;

                              }

  return(i) ;
}


/*********************************************************************/
/*								     */
/*	      ���������� ������ "������ ��������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       ����������� ������ "������ ��������"      	     */

     Crowd_Objects_List::Crowd_Objects_List(void)

{
     List    =NULL ;
     List_cnt=  0 ;
     List_max=  0 ;
}


/*********************************************************************/
/*								     */
/*	        ���������� ������ "������ ��������"      	     */

    Crowd_Objects_List::~Crowd_Objects_List(void)

{
    if(List!=NULL)  free(List) ;
}


/********************************************************************/
/*								    */
/*		              ������� ������		            */

    void  Crowd_Objects_List::Clear(void)

{
    List_cnt=0 ;
}


/********************************************************************/
/*								    */
/*		      ���������� ������ � ������	            */

    int  Crowd_Objects_List::Add(class Crowd_Object *object, char *relation)

{
   int  i ;

/*------------------------------------------------ ������ ���������� */

   for(i=0 ; i<List_cnt ; i++)
     if(!stricmp(object->Name, List[i].object->Name))  break ;

      if(i<List_cnt) {
                               List[i].object->vFree() ;
                        delete List[i].object ;
                     }
/*------------------------------------- ���������� ������ ���������� */

   if(i>=List_max) {
                       List_max+=10 ;
                       List     =(Crowd_Objects_List_Elem *)
                                  realloc(List, List_max*sizeof(*List)) ;
                   }

   if(List==NULL)  return(-1) ;


               List[i].object=object ;
       strncpy(List[i].relation, relation, sizeof(List[0].relation)-1) ;

    if(i>=List_cnt)  List_cnt++ ;

/*-------------------------------------------------------------------*/

  return(0) ;
}

