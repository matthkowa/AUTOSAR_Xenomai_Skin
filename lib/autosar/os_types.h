/**
 *
 *	\file os_types.c
 *	\brief Types definition
 *	\author  Matthieu K.
 *	\version 0.1
 *	\date 29 Juin 2016
*/

#ifndef _AUTOSAR_TYPES_H
#define _AUTOSAR_TYPES_H
#include <stdint.h>
#include <autosar/os.h>

#define BITS	32
#define MAX_LENGTH 255
#if BITS == 32
typedef uint32_t EcucEnumerationParamDef;
#elif BITS == 64
typedef uint64_t EcucEnumerationParamDef;
#endif

typedef boolean EcucBooleanParamDef;


typedef char EcucAbstractStringParamDef[MAX_LENGTH];
typedef char EcucStringParamDef[MAX_LENGTH];
typedef char EcucMultilineStringParamDef[MAX_LENGTH];


typedef void (*EcucFunctionNameDef)(void);

typedef struct EcucCoreDefintion EcucCoreDefintion;

struct EcucCoreDefintion {
	// EcucCoreHwRef
	uint64 EcucCoreId;
};

typedef struct EcucPartition EcucPartition;

struct EcucPartition {
	// EcucPartitionBswModuleDistinguishedPartition
	EcucBooleanParamDef EcucPartitionBswModuleExecution;
	EcucBooleanParamDef EcucPartitionQmBswModuleExecution;
	// EcucPartitionSoftwareComponentInstanceRef;
	EcucBooleanParamDef PartitionCanBeRestarted;	
};


#endif
