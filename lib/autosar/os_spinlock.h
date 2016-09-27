/**
 *
 *	\file os_spinlock.h
 *	\brief Autosar Spinlock header 
 *	\author  Matthieu Kowalewski.
 *	\version 1.0
 *	\date 29 Juin 2016
*/	
#ifndef _AUTOSAR_SPINLOCK_H
#define _AUTOSAR_SPINLOCK_H
#include "os_types.h"
#include "os_application.h"

/************************* STRUCTURE DEFINITION *************************/
/*
 *	\struct OsSpinlock 
 *	\brief Spinlock structure from AUTOSAR XML
 */
typedef struct OsSpinlock OsSpinlock;

struct OsSpinlock {
	EcucEnumerationParamDef OsSpinlockLockMethode;
	struct OsApplication * OsSpinlockAccessingApplication;
	struct OsSpinlock * OsSpinlockSuccessor;	
};


#endif
