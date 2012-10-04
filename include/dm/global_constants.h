/*
 * demolition man
 * global_constants.h
 *
 * written by James Cardona
 *
 */
#include <freewpc.h>




#ifndef _DM_GLOBAL_CONSTANTS_H
#define _DM_GLOBAL_CONSTANTS_H

//max width is 128
#define DMD_MIDDLE_X 	64
#define DMD_LEFT_X 		10
#define DMD_RIGHT_X 	118

//max height is 32 - for fonts larger than 5 but less than 14
#define DMD_BIG_CY_Top 	8
#define DMD_BIG_CY_Bot 	24

//for large fonts of height > 16 but < 30
#define DMD_BIG_CY_Cent 16


//4, 11, 19, 26
//max height is 32 - these are for fonts of height 5 and number is center of font
#define DMD_SMALL_CY_1 	4
#define DMD_SMALL_CY_2 	10
#define DMD_SMALL_CY_3 	16
#define DMD_SMALL_CY_4 	22
#define DMD_SMALL_CY_5 	28

#endif /* _DM_GLOBAL_CONSTANTS_H */
