/**
 *
 *	\file os_os.h
 *	\brief Autosar Os header 
 *	\author  Matthieu Kowalewski
 *	\version 1.0
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

/************************* STRUCTURE DEFINITION *************************/
/*
 *	\struct autosar_queue_wait 
 *	\brief Xenomai structur
 */
struct Os {
	struct OsAlarm ** OsAlarm; 		/*!< Array of all alarms */
	struct OsAppMode ** OsAppMode;		/*!< Array of all appmodes */
	struct OsCounter ** OsCounter;		/*!< Array of all counters */
	struct OsIsr ** OsIsr;
	struct OsResource ** OsResource;	/*!< Array of all resources*/
	struct OsTask ** OsTask;		/*!< Array of all tasks */
        //	struct OsEvent * OsEvent;
        //	struct OsIoc * OsIoc;
        //	struct OsScheduleTable * OsScheduleTable;
        //	struct OsSpinlock * OsSpinlock;
        //	struct OsApplication * OsApplication;
};

#endif
