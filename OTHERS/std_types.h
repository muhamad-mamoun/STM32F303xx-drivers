/*
=======================================================================================================================
Author       : Mamoun
Module       : Standard Types
File Name    : std_types.h
Date Created : Oct 14, 2023
Description  : The most common standard types used in Embedded C.
=======================================================================================================================
*/


#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/*=====================================================================================================================
                                            < Definitions >
=====================================================================================================================*/

#ifndef NULL_PTR
#define NULL_PTR                   ((void*)(0))
#endif

#ifndef TRUE
#define TRUE                       (1U)
#endif

#ifndef FALSE
#define FALSE                      (0U)
#endif

#ifndef HIGH
#define HIGH                       (1U)
#endif

#ifndef LOW
#define LOW                        (0U)
#endif

/*=====================================================================================================================
                                       < User-defined Data Types >
=====================================================================================================================*/

typedef signed char                sint8;   /* Stores integer numbers with range: [-128 ~ 127].                      */
typedef unsigned char              uint8;   /* Stores integer numbers with range: [0 ~ 255].                         */
typedef signed short               sint16;  /* Stores integer numbers with range: [-32,768 ~ 32,767].                */
typedef unsigned short             uint16;  /* Stores integer numbers with range: [0 ~ 65535].                       */
typedef signed long                sint32;  /* Stores integer numbers with range: [-2,147,483,648 ~ -2,147,483,647]. */
typedef unsigned long              uint32;  /* Stores integer numbers with range: [0 ~ 4,294,967,295].               */
typedef signed long long           sint64;  /* Stores integer numbers.                                               */
typedef unsigned long long         uint64;  /* Stores integer numbers.                                               */
typedef float                      float32; /* Stores float numbers.                                                 */
typedef double                     float64; /* Stores float numbers.                                                 */

#endif /* STD_TYPES_H_ */
