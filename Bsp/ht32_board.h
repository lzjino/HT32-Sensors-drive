
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_BOARD_H
#define __HT32_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
//#include "ht32_config.h" //È¥µô
#ifdef USE_HT32F52352_SK
 // #include "HT32_board/ht32f52352_sk.h"
#else
 #error "Please select board by define USE_HT32FXXXX_XXX in project."
#endif

#ifdef __cplusplus
}
#endif

#endif
