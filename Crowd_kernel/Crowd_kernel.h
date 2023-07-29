
#ifndef  CROWD_KERNEL_H 

#define  CROWD_KERNEL_H 

#include "..\Crowd_Model\Crowd_Model.h"
#include "..\Crowd_Feature\Crowd_Feature.h"
#include "..\Crowd_Message\Crowd_Message.h"
#include "..\Crowd_Object\Crowd_Object.h"

#include <string>

#ifdef CROWD_KERNEL_EXPORTS
#define CROWD_KERNEL_API __declspec(dllexport)
#else
#define CROWD_KERNEL_API __declspec(dllimport)
#endif

/*----------------------------- �������� "�����" ������������ ������ */

    struct Crowd_Module_Entry {
		       class Crowd_Kernel *entry ;     /* ���� */
				  HMODULE  module ;    /* ���������� DLL */
                              } ;

/*----------------------------------------- �������� "�����" ������� */

    struct Crowd_Resource {
		              char  name[256] ;      /* �������� */
		              void *ptr ;	     /* ��������� */
                          } ;

/*-------------------------------------- �������� ������ ����������� */

    struct Crowd_Parameter {
		               char  name[256] ;     /* �������� */
		             double  value ;	     /* �������� */
		             double *ptr ;	     /* ��������� */
                           } ;
/*---------------------------------------- �������� ��������� ������ */

   struct Crowd_Context {
                          char  name[64] ;    /* �������� ��������� */
                          void *data ;        /* ������ ������ */
            class Crowd_Kernel *module ;      /* ������ �� ������ */

                        }  ;
/*---------------------------------------- �������� ���������� ����� */

    struct Crowd_Name {
		              char  name[256] ;      /* ��� */
		              char  module[256] ;    /* ��������� ������ */
		      } ;
/*------------------------------ �������� ��������������� ���������� */

   struct Crowd_Redirect {
                           char *master ;     /* ������� ������ */
                           char *command ;    /* ������� */
             class Crowd_Kernel *module ;     /* ������ �� ������ */

                         }  ;
/*---------------------------------------- �������� ����� ���������� */

   struct Crowd_Result {
                         char  id[256] ;    /* ������������� */
                         void *result ;     /* ��������� */
                       }  ;
/*---------------------------------- ��������� � ������� ����������� */

    struct Crowd_Display {
		            int  (* GetList)        (int) ;     /* �������������� ����������� ������ */
		           void  (* ReleaseList)    (int) ;     /* ������������ ����������� ������ */
		            int  (* SetFirstContext)(char *) ;  /* ���������� ������ ����������� �������� */
		            int  (* SetNextContext) (char *) ;  /* ���������� ��������� ���������� �������� */
		           void  (* ShowContext)    (char *) ;  /* ���������� �������� */
		         double  (* GetContextPar)  (char *) ;  /* ������ �������� ��������� */
                         } ;
/*---------------------------------- �������� "�����" �������� ����� */

typedef  int (CALLBACK *Crowd_Kernel_CallBack)(int, void *)  ;

    struct Crowd_CB_Entry {
		                char  name[64] ;       /* �������� */
	       Crowd_Kernel_CallBack  proc ;	     /* ��������� */
			  } ;

#define  _CALLB_MAX  10

/*----------------------------------------------- ���������� ������� */

 typedef  struct {
                    char  entry[128] ;  /* �������� ����� */
                     int  idx1 ;        /* 1-�� ������ */
                     int  idx2 ;        /* 2-�� ������ */

                    void *ptr  ;        /* ��������� �� ���� ������ */
                    long  size ;        /* ������ ����a ������ */
                     int  used ;        /* ���� ������������� */
                  time_t  time ;        /* ����� �������� */

                 }  Crowd_Memory ;

/*----------------------------------------------- ����������� ������ */

 typedef  struct {
                    char  path[FILENAME_MAX] ;
                    char *data ;
                 }  Crowd_File ;

/*------------------------------------- ��������� ������������ ����� */

  class CROWD_KERNEL_API Crowd_IFace {

    public:
                                     char *std_iface ;

    public:
             virtual  void  vClear (void) ;              /* �������� ������ */
             virtual  void  vSignal(char *, void *) ;    /* ������� ������ */
             virtual  void  vPass  (Crowd_IFace *) ;     /* ������� ������� */
             virtual   int  vDecode(char *, void *) ;    /* ������ ������� */
             virtual   int  vCheck (char * ) ;           /* �������� ������� */
                                             
    public:
                            Crowd_IFace() ;              /* ����������� */
                           ~Crowd_IFace() ;              /* ���������� */

                                     } ; 

/*------------------------------------------------ �������� �������� */

   struct Crowd_Event {
                         Crowd_Object *source ;    /* �������� */
                         Crowd_Object *object ;    /* ������ */
                      }  ;
/*------------------------------ �������� ������ ������������ ������ */

  class CROWD_KERNEL_API Crowd_Kernel {

   public : 
    static   Crowd_Kernel  *kernel ;	            /* ������-���� */
    static           HWND   kernel_wnd ;            /* ���� ���� */
    static           HWND   active_wnd ;            /* �������� o��� */
    static      HINSTANCE   kernel_inst ;           /* ������������� ������ ���� */

    static            int   debug_stop ;            /* ���� ��������� ���������� */
    static            int   debug_next ;            /* ���� ���������� ���������� ���� */
#define                    _CROWD_KERNEL_NEXT_STEP  1
#define                    _CROWD_KERNEL_WAIT_STEP  2

    static   Crowd_Kernel  *priority_entry ;	    /* ������ ��� ������������� ������ */
    static   Crowd_Kernel **events_entry ;            /* ������ �������, ������� ��������� ��������� ������� */
    static            int   events_entry_cnt ;

    static   Crowd_Object **kernel_objects ;          /* ������ �������� */
    static            int   kernel_objects_cnt ;

    static  Crowd_Message **kernel_messages ;         /* ������ ��������� */
    static            int   kernel_messages_cnt ;

    static     Crowd_Name **kernel_names ;            /* ������ ��������� ���� */
    static            int   kernel_names_cnt ;

    static   Crowd_Kernel **feature_modules ;         /* ������ �������-������� */
    static            int   feature_modules_cnt ;

    static   Crowd_Kernel **calculate_modules ;       /* ������ ������������ */
    static            int   calculate_modules_cnt ;

    static     Crowd_File  *files ;                   /* ������ ������������ ������ */
    static            int   files_cnt ;

    static   Crowd_Result **results ;                 /* ���� ����������� ������ ������� */
    static            int   results_cnt ;

    static Crowd_CB_Entry   callbacks[_CALLB_MAX] ;   /* ������� �������� ����� */

    static  Crowd_Display   display ;                 /* ��������� � ������� ����������� */

    static            int   srand_fixed ;             /* ���� ������������� �������������� ���������� ��������� ����� */

    static         double   calc_time_step ;          /* ����� ��������� ������������� �� ������� */ 
    static         double   show_time_step ;          /* ������� ����������� ����� �� ������� */ 

    static            int   debug_flag ;
    static           char   debug_list[1024] ;        /* ������ ������������ ������� */ 
   
   public : 
		  HMODULE   DLL_module ;              /* ���������� DLL, ���������� ������ */
       Crowd_Module_Entry  *modules ;	              /* ������ ����������� ������� */
		      int   modules_cnt ;
	   Crowd_Resource  *resources ;	              /* ������ ������������ �������� */
		      int   resources_cnt ;
           Crowd_Redirect  *command_redirect ;        /* ������ ��������������� ������ */
                      int   command_redirect_cnt ;

   public :
                   char  *keyword ;	            /* �������� ���� - ������������� ������� */
                   char  *identification ;          /* ������������� ������ */
                   char  *category ;                /* ��������� ������������ ������ */
                   char  *lego_type ;               /* ��� Lego-�������� */
                   char  *lego_set ;                /* ����� Lego-��������� */
                    int   priority ;                /* ��������� ��������� ���������� */
                    int   events_processing ;       /* ���� ����������� ��������� ������� */
                    
	  	    int   attempt ;		    /* ������ "������" */

   public:
		   void	  SetKernel(const HWND,         /* ���������� ���� */
				    const HINSTANCE) ; 
  Crowd_Kernel_CallBack   SetCallB (char *,             /* ������� ������� �������� ����� */
				    Crowd_Kernel_CallBack) ;
  Crowd_Kernel_CallBack   GetCallB (char *) ;           /* ������ ������� �������� ����� */
                   void  *Resource (const char *,       /* ����������� ������� */
				    const char * ) ; 
		    int	  Load     (const char *) ;     /* �������� ������������ ������� */
                   char  *FileCache(char *, char *) ;   /* �������� ����� � ������ */
		    int	  Free     (void) ;	        /* ������������ ������������ ������� */
		    
   public:
	    virtual int   vKernelEvents (void) ;           /* ��������� ������� */

   public:
     virtual                void  vStart              (void) ;                     /* ��������� �������� */
     virtual                void  vInit               (void) ;                     /* ������������� ������ */
     virtual                void  vReadSave           (std::string *) ;            /* ������� ������ �� ������ */
     virtual                void  vWriteSave          (std::string *) ;            /* �������� ������ � ������ */
     virtual        Crowd_Object *vCreateObject       (Crowd_Model_data *) ;       /* ������� ������ */
     virtual       Crowd_Feature *vCreateFeature      (Crowd_Object *,             /* ������� �������� */
                                                       Crowd_Feature * ) ;
     virtual Crowd_Communication *vCreateCommunication(Crowd_Object *,             /* ������� ����� */
                                                       Crowd_Object *,
                                                       Crowd_Communication *) ;
     virtual       Crowd_Message *vCreateMessage      (Crowd_Object *,             /* ������� ��������� */
                                                       Crowd_Object *,
                                                       Crowd_Message *) ;
     virtual                 int  vAddMessage         (Crowd_Message *, int) ;     /* ����������� ��������� � ������� */

     virtual                 int  vExecuteCmd         (const char *) ;             /* ��������� ������� */
     virtual                 int  vExecuteCmd         (const char *,               /* ��������� ������� � ������� ���������� �� ������ */
                                                        Crowd_IFace *) ;
     virtual                 int  vCalculate          (char *, char *,             /* ��������� ��������� */
                                                        struct Crowd_Parameter *,
                                                        struct Crowd_Parameter *,
                                                        double *, void **, char *) ;
     virtual                 int  vGetParameter       (char *, char *) ;           /* �������� �������� */
     virtual                void  vSetParameter       (char *, char *) ;           /* ���������� �������� */
     virtual                void  vProcess            (void) ;                     /* ��������� ������� ���������� */
     virtual                void  vShow               (char *) ;                   /* ���������� ��������� ������ */
     virtual                void  vChangeContext      (void)  ;                    /* ��������� �������� � ��������� ������ */

     virtual                 int  vSpecial            (char *, void *, char *) ;   /* ����������� ��������� */

     virtual       Crowd_Context *vAddData            (Crowd_Context ***) ;        /* ���� ��������� � ������ ���������� */
     virtual                 int  vReadData           (Crowd_Context ***,          /* ������� ������ ��������� ������ �� ������ */
                                                       std::string * ) ;
     virtual                void  vWriteData          (Crowd_Context *,            /* �������� ������ ��������� ������ � ������ */
                                                       std::string * ) ;
     virtual                void  vReleaseData        (Crowd_Context *) ;          /* ���������� ������� ������ ��������� ������ */

     virtual              double  vGetTime            (void) ;                     /* ��������� ����� */

    public:

                          double  gGaussianValue      (double, double) ;           /* ���������� ������������� */

    public:

     static          int   Crowd_Kernel::memchk_regime ;
#define                     _MEMCHECK_OFF      0
#define                     _MEMCHECK_ACTUAL   1
#define                     _MEMCHECK_HISTORY  2
     static Crowd_Memory  *Crowd_Kernel::memchk_list ;
     static          int   Crowd_Kernel::memchk_list_max ; 
     static          int   Crowd_Kernel::memchk_list_idx ; 
     static          int   Crowd_Kernel::memchk_rep_num ; 

		    void  gMemOnOff     (int) ;
		    void  gMemList      (char *) ;
		    void *gMemCalloc    (size_t, size_t, char *, int, int) ;  
		    void *gMemRealloc   (void *, size_t, char *, int, int) ;  
		    void  gMemFree      (void *) ;
		     int  gMemCheck     (void) ;
                    void  iMemAddList   (void *, int, char *, int, int) ;
                    void  iMemDelList   (void *) ;

    public:
		    void  iErrorMsg     (const char *) ;   /* ������������ ��������� �� ������� */
		    void  iLogFile      (const char *) ;   /* ������ ��������� � ��� ���� */

    public:
			  Crowd_Kernel    () ;               /* ����������� */
			 ~Crowd_Kernel    () ;               /* ���������� */
		                  } ;

/*-------------------------------------------------------- ��������� */

#define  _USER_EXECUTE_COMMAND    101    /* ��������� ������� */
#define  _USER_COMMAND_PREFIX     102    /* ��������� �������� ��������� ������ */
#define  _USER_DEFAULT_OBJECT     103    /* ��������� ������� �� ��������� */

#define  _USER_DIRECT_COMMAND     201    /* ������� ������ ������� ���������� */

#define  _USER_SHOW               301    /* ����������� */
#define  _USER_CHANGE_CONTEXT     302    /* ��������� � ��������� ������ ��������� */

#define  _USER_THREAD_ADD         401    /* ���������������� ������� ����� */
#define  _USER_THREAD_DELETE      402    /* ��������� ������� ����� */

#define  _USER_ERROR_MESSAGE      901    /* ��������� �� ������ */
#define  _USER_SHOW_COMMAND       902    /* �������� ������� */
#define  _USER_SHOW_INFO          903    /* �������� ���������� �� ������� */
#define  _USER_INFO_MESSAGE       904    /* ��������� �� �� ������ */
#define  _USER_CHECK_MESSAGE      905    /* ��������� ����������� ������� �����: �����������, ����������� � ������ */
#define  _USER_THREAD_MESSAGE     906    /* ��������� �� ���������� ������ */

#define  _USER_REFRESH              1
#define  _USER_LOG                  2 


/*-------------------------------------------------------------------*/

#endif        // CROWD_KERNEL_H 
