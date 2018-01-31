
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>


#include "Crowd_Communication.h"


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

     Crowd_Communication::Crowd_Communication()

{
}


/********************************************************************/
/*								    */
/*		        Деструктор класса			    */

    Crowd_Communication::~Crowd_Communication()

{
}


/********************************************************************/
/*								    */
/*		        Считать данные из строки		    */

    void Crowd_Communication::vReadSave(char *title, std::string *text, char *reference)

{
}


/********************************************************************/
/*								    */
/*		        Записать данные в строку		    */

    void Crowd_Communication::vWriteSave(std::string *text)

{
     *text="" ;
}


/********************************************************************/
/*								    */
/*		      Освободить ресурсы        		    */

    void Crowd_Communication::vFree(void)

{
}


/********************************************************************/
/*								    */
/*                     Выдать информацию о свойстве   		    */

    void Crowd_Communication::vGetInfo(std::string *text)

{
        *text="" ;
}


/********************************************************************/
/*								    */
/*                    Проверка наличия связи                        */  

    int Crowd_Communication::vCheck(void *data, Crowd_Objects_List *checked)

{
   return(0) ;
}


