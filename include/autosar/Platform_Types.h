
#ifndef _XENOMAI_AUTOSAR_PLATFORM_TYPES_H
#define _XENOMAI_AUTOSAR_PLATFORM_TYPES_H
#include <stdint.h>

#define CPU_TYPE_8        	8
#define CPU_TYPE_16       	16
#define CPU_TYPE_32       	32
#define MSB_FIRST         	0
#define LSB_FIRST         	1
#define HIGH_BYTE_FIRST   	0
#define LOW_BYTE_FIRST    	1

#ifndef FALSE
#define FALSE			0
#endif
#ifndef TRUE
#define TRUE			1
#endif

#define CPU_TYPE            	CPU_TYPE_32
#define CPU_BIT_ORDER       	MSB_FIRST
#define CPU_BYTE_ORDER      	HIGH_BYTE_FIRST


typedef unsigned char       	boolean;
typedef signed char         	sint8;
typedef unsigned char       	uint8;
typedef signed short        	sint16;
typedef unsigned short      	uint16;
typedef signed long   	    	sint32;
typedef signed long long    	sint64;
typedef unsigned long       	uint32;
typedef unsigned long long  	uint64;
typedef signed char         	sint8_least;
typedef unsigned char       	uint8_least;
typedef signed short        	sint16_least;
typedef unsigned short      	uint16_least;
typedef signed long         	sint32_least;
typedef unsigned long       	uint32_least;
typedef float			float32;
typedef double			float64;


#endif
