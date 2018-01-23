
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>


#include "..\Crowd_feature\Crowd_feature.h"
#include "Crowd_object.h"

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
/*	         Компоненты класса "ТРАНЗИТ КОНТЕКСТА"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       Конструктор класса "ТРАНЗИТ КОНТЕКСТА"      	     */

     Crowd_Transit::Crowd_Transit(void)

{
   memset(action, 0, sizeof(action)) ;

          object=NULL ;
}


/*********************************************************************/
/*								     */
/*	        Деструктор класса "ТРАНЗИТ КОНТЕКСТА"      	     */

    Crowd_Transit::~Crowd_Transit(void)

{
}


/********************************************************************/
/*								    */
/*	              Исполнение действия                           */

    int  Crowd_Transit::vExecute(void)

{
   return(0) ;
}


/*********************************************************************/
/*								     */
/*		      Компоненты класса "ОБЪЕКТ"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*		       Конструктор класса "ОБЪЕКТ"      	     */

     Crowd_Object::Crowd_Object(void)

{
     Parameters    =NULL ;
     Parameters_cnt=  0 ;
       Features    =NULL ;
       Features_cnt=  0 ;

       battle_state= 0 ; 

        ErrorEnable= 1 ;
}


/*********************************************************************/
/*								     */
/*		        Деструктор класса "ОБЪЕКТ"      	     */

    Crowd_Object::~Crowd_Object(void)

{
    int  i ;

/*--------------------------------------------- Удаление компонентов */

   for(i=0 ; i<this->Units.List_cnt ; i++)
                  delete this->Units.List[i].object ;

/*-------------------------------------------------------------------*/

}


/********************************************************************/
/*								    */
/*                        Копировать объект		            */

    class Crowd_Object *Crowd_Object::vCopy(char *name)

{
   return(NULL) ;
}


/********************************************************************/
/*								    */
/*                    Сохранить состояние объекта                   */
/*                    Восстановить состояние объекта                */

    void  Crowd_Object::vPush(void)

{
     x_base_stack    =x_base ;
     y_base_stack    =y_base ;
     z_base_stack    =z_base ;

     a_azim_stack    =a_azim ;
     a_elev_stack    =a_elev ;
     a_roll_stack    =a_roll ;

     x_velocity_stack=x_velocity ;
     y_velocity_stack=y_velocity ;
     z_velocity_stack=z_velocity ;
}


    void  Crowd_Object::vPop(void)

{
     x_base    =x_base_stack ;
     y_base    =y_base_stack ;
     z_base    =z_base_stack ;

     a_azim    =a_azim_stack ;
     a_elev    =a_elev_stack ;
     a_roll    =a_roll_stack ;

     x_velocity=x_velocity_stack ;
     y_velocity=y_velocity_stack ;
     z_velocity=z_velocity_stack ;
}


/********************************************************************/
/*								    */
/*		  Вкл./Выкл. сообщений об ошибках		    */

    void  Crowd_Object::vErrorMessage(int  on_off)

{
         ErrorEnable=on_off ;
}


/********************************************************************/
/*								    */
/*		        Считать данные из строки		    */

    void  Crowd_Object::vReadSave(std::string *text)

{
}


/********************************************************************/
/*								    */
/*		        Записать данные в строку		    */

    void  Crowd_Object::vWriteSave(std::string *text)

{
     *text="" ;
}


/********************************************************************/
/*								    */
/*		      Освободить ресурсы        		    */

    void  Crowd_Object::vFree(void)

{
}


/********************************************************************/
/*								    */
/*		      Формировать описание      		    */

    void  Crowd_Object::vFormDecl(void)

{
   sprintf(Decl, "%s  %s", Name, Type) ;
}


/********************************************************************/
/*								    */
/*		      Работа с базовыми точками   		    */
/*								    */
/*   Return: число базовых точек                                    */

     int  Crowd_Object::vGetPosition(Crowd_Point *points)

{
        points->x=x_base ;
        points->y=y_base ;
        points->z=z_base ;

        points->azim=a_azim ;
        points->elev=a_elev ;
        points->roll=a_roll ;

    return(0) ;
}

    void  Crowd_Object::vSetPosition(Crowd_Point *points)

{
        x_base=points->x ;
        y_base=points->y ;
        z_base=points->z ;

        a_azim=points->azim ;
        a_elev=points->elev ;
        a_roll=points->roll ;
}


/********************************************************************/
/*								    */
/*                    Получение вектора скорости                    */

    int  Crowd_Object::vGetVelocity(Crowd_Vector *velocity)
{
        velocity->x=x_velocity ;
        velocity->y=y_velocity ;
        velocity->z=z_velocity ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		Сброс контекста проверки свойств                    */

     int  Crowd_Object::vResetFeatures(void *data)

{
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++)
                this->Features[i]->vResetCheck(data) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		Подготовка своиств к проверке корректности          */

     int  Crowd_Object::vPrepareFeatures(void *data)

{
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++)
                this->Features[i]->vPreCheck(data) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		Проверка непротиворечивости свойств                 */

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
/*		      Работа со свойствами                          */

    void  Crowd_Object::vSetFeature(Crowd_Feature *feature)

{
}

    void  Crowd_Object::vGetFeature(Crowd_Feature *feature)

{
}


/********************************************************************/
/*								    */
/*                        Специальные действия                      */

     int  Crowd_Object::vSpecial(char *oper, void *data)
{
  return(-1) ;
}


/********************************************************************/
/*								    */
/*             Подготовка расчета изменения состояния               */

     int  Crowd_Object::vCalculateStart(double  t)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*                   Расчет изменения состояния                     */

     int  Crowd_Object::vCalculate(double t1, double t2, char *callback, int callback_size)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*      Отображение результата расчета изменения состояния          */

     int  Crowd_Object::vCalculateShow(void)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*                        Обработка событий                         */

     int  Crowd_Object::vEvent(char *event_name, double  t)
{
  return(-1) ;
}


/********************************************************************/
/*								    */
/*                Получить список параметров управления             */

   int  Crowd_Object::vListControlPars(Crowd_ControlPar *list)

{
   return(0) ;
}


/********************************************************************/
/*								    */
/*               Установить значение параметра управления           */

   int  Crowd_Object::vSetControlPar(Crowd_ControlPar *par)   

{
   return(-1) ;
}


/*********************************************************************/
/*                                                                   */
/*               Проверка попадания угла в диапазон                  */

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
/*              Замена текстового фрагмента в строке                */

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
/*	      Компоненты класса "СПИСОК ОБЪЕКТОВ"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       Конструктор класса "СПИСОК ОБЪЕКТОВ"      	     */

     Crowd_Objects_List::Crowd_Objects_List(void)

{
     List    =NULL ;
     List_cnt=  0 ;
     List_max=  0 ;
}


/*********************************************************************/
/*								     */
/*	        Деструктор класса "СПИСОК ОБЪЕКТОВ"      	     */

    Crowd_Objects_List::~Crowd_Objects_List(void)

{
    if(List!=NULL)  free(List) ;
}


/********************************************************************/
/*								    */
/*		              Очистка списка		            */

    void  Crowd_Objects_List::Clear(void)

{
    List_cnt=0 ;
}


/********************************************************************/
/*								    */
/*		      Добавление записи в список	            */

    int  Crowd_Objects_List::Add(class Crowd_Object *object, char *relation)

{
   int  i ;

/*------------------------------------------------ Замена компонента */

   for(i=0 ; i<List_cnt ; i++)
     if(!stricmp(object->Name, List[i].object->Name))  break ;

      if(i<List_cnt) {
                               List[i].object->vFree() ;
                        delete List[i].object ;
                     }
/*------------------------------------- Добавление нового компонента */

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

