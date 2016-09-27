/**
 *
 *	\file os_types.h
 *	\brief Types definition header
 *	\author  Matthieu Kowalewski.
 *	\version 1.0
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
	uint64 EcucCoreId;        
	// EcucCoreHwRef
};

typedef struct EcucPartition EcucPartition;

struct EcucPartition {
	EcucBooleanParamDef EcucPartitionBswModuleExecution;
	EcucBooleanParamDef EcucPartitionQmBswModuleExecution;
	EcucBooleanParamDef PartitionCanBeRestarted;
	// EcucPartitionSoftwareComponentInstanceRef;
	// EcucPartitionBswModuleDistinguishedPartition	
};


#endif
