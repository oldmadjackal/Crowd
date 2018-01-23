
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>


#include "Crowd_Feature.h"


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
/*		       Статические переменные			    */



/********************************************************************/
/*								    */
/*		       Конструктор класса			    */

     Crowd_Feature::Crowd_Feature()

{
}


/********************************************************************/
/*								    */
/*		        Деструктор класса			    */

    Crowd_Feature::~Crowd_Feature()

{
}


/********************************************************************/
/*								    */
/*		        Считать данные из строки		    */

    void Crowd_Feature::vReadSave(char *title, std::string *text, char *reference)

{
}


/********************************************************************/
/*								    */
/*		        Записать данные в строку		    */

    void Crowd_Feature::vWriteSave(std::string *text)

{
     *text="" ;
}


/********************************************************************/
/*								    */
/*		      Освободить ресурсы        		    */

    void Crowd_Feature::vFree(void)

{
}


/********************************************************************/
/*								    */
/*                     Выдать информацию о свойстве   		    */

    void Crowd_Feature::vGetInfo(std::string *text)

{
        *text="" ;
}


/********************************************************************/
/*								    */
/*                     Работа с параметрами           		    */

     int Crowd_Feature::vParameter(char *name, char *action, char *value)

{
   return(-1) ;
}


/********************************************************************/
/*								    */
/*       Сброс контекста проверки непротиворечивости свойства       */  

    int Crowd_Feature::vResetCheck(void *data)

{
   return(0) ;
}


/********************************************************************/
/*								    */
/*       Подготовка к проверке непротиворечивости свойства          */  

    int Crowd_Feature::vPreCheck(void *data)

{
   return(0) ;
}


/********************************************************************/
/*								    */
/*                Проверка непротиворечивости свойства              */  

    int Crowd_Feature::vCheck(void *data, Crowd_Objects_List *checked)

{
   return(0) ;
}


/********************************************************************/
/*								    */
/*                          Добавить тело                           */

    void Crowd_Feature::vBodyAdd(char *body)
{
}


/********************************************************************/
/*								    */
/*                          Удалить тело                            */

    void Crowd_Feature::vBodyDelete(char *body)
{
}


/********************************************************************/
/*								    */
/*                   Задание базовой точки тела                     */

    void Crowd_Feature::vBodyBasePoint(   char *body,                   
                                        double  x, 
                                        double  y, 
                                        double  z )
{
}


/********************************************************************/
/*								    */
/*                     Задание ориентации тела                      */

    void Crowd_Feature::vBodyAngles(   char *body,                   
                                     double  azimuth, 
                                     double  tangage, 
                                     double  roll    )
{
}


/********************************************************************/
/*								    */
/*              Задание матрицы положения тела                      */

    void Crowd_Feature::vBodyMatrix(   char *body,
                                     double  matrix[4][4] )
{
}


/********************************************************************/
/*								    */
/*           Задание смещения положения и ориентации тела           */

    void Crowd_Feature::vBodyShifts(   char *body,                   
                                     double  x, 
                                     double  y, 
                                     double  z, 
                                     double  azimuth, 
                                     double  tangage, 
                                     double  roll    )
{
}


/********************************************************************/
/*								    */
/*              Задание списка параметров тела                      */

    void Crowd_Feature::vBodyPars(char *body, struct Crowd_Parameter *)
{
}


