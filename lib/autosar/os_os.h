/**
 *
 *	\file os_os.h
 *	\brief Autosar Os header 
 *	\author  Matthieu K.
 *	\version 0.1
 *	\date 29 Juin 2016
*/	
#ifndef _AUTOSAR_OS_H
#define _AUTOSAR_OS_H
#include "app/app_define.h"
#include "os_types.h"
#include "os_alarm.h"
#include "os_counter.h"
#include "os_resource.h"
#include "os_task.h"
#include "os_appmode.h"

/*
 *	\struct autosar_queue_wait 
 *	\brief Xenomai structur
 */
struct Os {
	struct OsAlarm ** OsAlarm; 		/*!< Array of all alarms */
	struct OsAppMode ** OsAppMode;		/*!< Array of all appmodes */
//	struct OsApplication * OsApplication;
	struct OsCounter ** OsCounter;		/*!< Array of all counters */
//	struct OsEvent * OsEvent;
//	struct OsIoc * OsIoc;
//	struct OsIsr * OsIsr;
	struct OsResource ** OsResource;	/*!< Array of all resources*/
//	struct OsScheduleTable * OsScheduleTable;
//	struct OsSpinlock * OsSpinlock;
	struct OsTask ** OsTask;		/*!< Array of all tasks */
};

#endif
