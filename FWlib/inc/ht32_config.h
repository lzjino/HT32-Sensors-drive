/*********************************************************************************************************//**
 * @file    ht32_config.h
 * @version $Rev:: 4270         $
 * @date    $Date:: 2019-10-16 #$
 * @brief   Configuration file of HT32.
 *************************************************************************************************************
*/
//编译器已额外定义了USE_HT32F52352_SK,不需要引用该文件
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_CONFIG_H
#define __HT32_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Settings ------------------------------------------------------------------------------------------------*/
#ifdef USE_HT32F59999_SK
  #define USE_HT32F52352_SK
#endif
#ifdef USE_HT32F59999
  #define USE_HT32F52342_52
#endif
#ifdef USE_MEM_HT32F59999
  #define USE_MEM_HT32F52352
#endif

#ifdef USE_HT32F52142_SK
  #define USE_HT32F0008_SK
#endif
#ifdef USE_HT32F52142
  #define USE_HT32F0008
#endif
#ifdef USE_MEM_HT32F52142
  #define USE_MEM_HT32F0008
#endif

#ifdef USE_HT32F61352_DVB
  #define USE_HT32F0006_DVB
#endif
#ifdef USE_HT32F61352
  #define USE_HT32F0006
#endif
#ifdef USE_MEM_HT32F61352
  #define USE_MEM_HT32F0006
#endif

#ifdef USE_HT50F32002_SK
  #define USE_HT32F50230_SK
#endif
#ifdef USE_HT50F32002
  #define USE_HT32F50220_30
#endif
#ifdef USE_MEM_HT50F32002
  #define USE_MEM_HT32F50230
#endif

#ifdef USE_HT50F32003_SK
  #define USE_HT32F52352_SK
#endif
#ifdef USE_HT50F32003
  #define USE_HT32F52342_52
#endif
#ifdef USE_MEM_HT50F32003
  #define USE_MEM_HT32F52352
#endif

#ifdef USE_HT32F59041_SK
  #define USE_HT32F50241_SK
#endif
#ifdef USE_HT32F59041
  #define USE_HT32F50231_41
#endif
#ifdef USE_MEM_HT32F59041
  #define USE_MEM_HT32F50241
#endif

#ifdef USE_HT32F59741_SK
  #define USE_HT32F57341_SK
#endif
#ifdef USE_HT32F59741
  #define USE_HT32F57331_41
#endif
#ifdef USE_MEM_HT32F59741
  #define USE_MEM_HT32F57341
#endif

#ifdef USE_HF5032_SK
  #define USE_HT32F50230_SK
#endif
#ifdef USE_HF5032
  #define USE_HT32F50220_30
#endif
#ifdef USE_MEM_HF5032
  #define USE_MEM_HT32F50230
#endif


#ifdef __cplusplus
}
#endif

#endif
