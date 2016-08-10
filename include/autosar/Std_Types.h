
#ifndef _XENOMAI_AUTOSAR_STD_TYPES_H
#define _XENOMAI_AUTOSAR_STD_TYPES_H

#include "Compiler.h"
#include "Platform_Types.h"


typedef uint8 Std_ReturnType;

#ifndef STATUSTYPEDEFINED 
#define STATUSTYPEDEFINED 
#define E_OK 					0x00u
#endif

#define E_NOT_OK 				0x01u

typedef struct {
	uint16 vendorID;
	uint16 moduleID;

	uint8 sw_major_version;    /**< Vendor numbers */
	uint8 sw_minor_version;    /**< Vendor numbers */
	uint8 sw_patch_version;    /**< Vendor numbers */

} Std_VersionInfoType;

#define STD_HIGH		0x01u
#define STD_LOW			0x00u

#define STD_ACTIVE		0x01u
#define STD_IDLE		0x00u

#define STD_ON			0x01u
#define STD_OFF			0x00u


#endif
