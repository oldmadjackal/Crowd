/*********************************************************************/
/*                                                                   */
/*               СИСТЕМА ГРАФИЧЕСКОГО ИНТЕРФЕЙСА                     */
/*                                                                   */
/*********************************************************************/

#include "gl\gl.h"
#include "gl\glu.h"

/*--------------------------------------------- Контексты Рендеринга */

 typedef  struct {
                      char  name[256] ;
                      HWND  hWnd ;
                     HGLRC  hOpenGL ;               
                    double  Look_x ;
                    double  Look_y ;
                    double  Look_z ;
                    double  Look_azim ;
                    double  Look_elev ;
                    double  Look_roll ;
                    double  Range_strobe ;
                    double  Zoom ;
                      char  AtObject[256] ;

                 }  WndContext ;

/*-------------------------------------------------------- Интерфейс */

    int  CrowdRoot_show_init       (void) ;            /* Инициализация системы отображения */
    int  CrowdRoot_show_getlist    (int) ;             /* Резервирование номера дисплейного списка */
   void  CrowdRoot_show_releaselist(int) ;             /* Освобождение номера дисплейного списка */
    int  CrowdRoot_first_context   (char *) ;          /* Установить первый контекст */
    int  CrowdRoot_next_context    (char *) ;          /* Установить следующий контекст */
   void  CrowdRoot_show_context    (char *) ;          /* Отобразить текущий контекст */
 double  CrowdRoot_get_context     (char *) ;          /* Выдать параметр контекста */
   void  CrowdRoot_read_context    (std::string *) ;   /* Считать параметры контекста */
   void  CrowdRoot_write_context   (std::string *) ;   /* Записать параметры контекста */
    int  CrowdRoot_redraw          (char *) ;          /* Перерисовать окно */
    int  CrowdRoot_look            (char *, char *,    /* Работа с точкой зрения камеры */
                                    double *, double *, double *,
                                    double *, double *, double * ) ;
    int  CrowdRoot_zoom            (char *, char *,    /* Работа с полем зрения камеры */
                                          double * ) ;
    int  CrowdRoot_lookat          (char *, char *,    /* Работа с точкой наблюдения камеры */ 
                                            char * ) ;
    int  CrowdRoot_lookat_point    (WndContext *) ;    /* Обработка точки наблюдения */

/*------------------------------------------------------ Обработчики */

   LRESULT CALLBACK  CrowdRoot_show_prc(HWND, UINT, WPARAM, LPARAM) ;
