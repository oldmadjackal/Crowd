/********************************************************************/
/*								    */
/*                   МОДУЛЬ DCL-ВЫЧИСЛИТЕЛЯ        	            */
/*								    */
/********************************************************************/

#ifdef C_DCL_EXPORTS
#define C_DCL_API __declspec(dllexport)
#else
#define C_DCL_API __declspec(dllimport)
#endif


/*---------------------------------------------- Параметры генерации */

/*----------------------------------------------- Описание контекста */

 typedef struct {  
                    int  dummy ;
 
                }  Crowd_Calc_Dcl_context ;

/*------------------------- Описание класса стандартного вычислителя */

  class C_DCL_API Crowd_Calc_Dcl : public Crowd_Kernel {

    public:
     virtual         int  vCalculate   (char *, char *,         /* Вычислить выражение */                                       
                                        Crowd_Object *,
                                        struct Crowd_Event *,
                                        struct Crowd_Parameter *,
                                        struct Crowd_Parameter *,
                                        double *,
                                        void **, char *) ;

    public:

     virtual         int  vGetParameter(char *, char *) ;       /* Получить параметр */
     virtual        void  vStart       (void) ;                 /* Стартовая разводка */

                    void  iDebug       (char *, char *, int) ;  /* Отладочная печать в файл */ 
                    void  iDebug       (char *, char *) ;

	                  Crowd_Calc_Dcl() ;                     /* Конструктор */
	                 ~Crowd_Calc_Dcl() ;                     /* Деструктор */
                                                   } ;

/*---------------------------------------------------- Прототипы п/п */

  void  Crowd_Calc_Dcl_text_norm(char *) ; /* Нормализация строки выражения */
  char *Crowd_Calc_Dcl_get_text (void) ;   /* Процедура построчной выдачи текста компилятору */
  char *Crowd_Calc_Dcl_error    (int) ;    /* Выдача текста ошибки по коду */
  char *Crowd_Calc_Dcl_warning  (int) ;    /* Выдача текста предупреждения по коду */
  void  Crowd_Calc_Dcl_debug    (int) ;    /* Процедура отладки вычислителя */
