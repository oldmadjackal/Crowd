
#ifndef  CRWOD_MODEL_H 

#define  CROWD_MODEL_H 

#include <string>

#ifdef CROWD_MODEL_EXPORTS
#define CROWD_MODEL_API __declspec(dllexport)
#else
#define CROWD_MODEL_API __declspec(dllimport)
#endif

/*---------------------------------------------- Параметры генерации */

#define    _MODEL_PARS_MAX    8
#define    _MODEL_SECT_MAX    8

/*--------------------------------------------- Транзитные структуры */

  typedef  struct {
                     char  text[32] ;        /* Подпись параметра */
                     char  value[32] ;       /* Значение параметра */
                  } Crowd_Model_parameter ;

  typedef  struct {
                            char  title[32] ;       /* Наименование секции */
                     std::string *decl ;            /* Данные секции */
                  } Crowd_Model_section ;

  typedef  struct {
                           char  name[32] ;                   /* Название обьекта */
                           char  lib_path[FILENAME_MAX] ;     /* Путь к разделу библиотеки */
                           char  path[FILENAME_MAX] ;         /* Путь к файлу */
                           char  object[32] ;                 /* Вид объекта */
                           char  model[32] ;                  /* Название модели */
                           char  picture[FILENAME_MAX] ;      /* Файл картинки */
          Crowd_Model_parameter  pars[_MODEL_PARS_MAX] ;      /* Параметры */
            Crowd_Model_section  sections[_MODEL_SECT_MAX] ;  /* Данные специальных секций */
                  } Crowd_Model_data ;

/*---------------------------------------------------- Прототипы п/п */

/* Crowd_model.cpp */
 CROWD_MODEL_API int  Crowd_Model_list    (char *, char *,       /* Формирование списка файлов модулей в разделе */
                                            int  , char * ) ;
 CROWD_MODEL_API int  Crowd_Model_ReadPars(Crowd_Model_data *) ; /* Считывание параметров модели */ 
 CROWD_MODEL_API int  Crowd_Model_ReadSect(Crowd_Model_data *) ; /* Считывание секций описаний модели */ 
 CROWD_MODEL_API int  Crowd_Model_Picture (Crowd_Model_data *,   /* Считывание картинки модели */ 
                                            HBITMAP *, RECT *) ;

#endif        // CROWD_MODEL_H
