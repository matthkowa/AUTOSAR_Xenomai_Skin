/**
 *
 *	\file os_scheduletable.h
 *	\brief Autosar Schedule Table header 
 *	\author  Matthieu Kowalewski.
 *	\version 1.0
 *	\date 29 Juin 2016
*/	
#ifndef _AUTOSAR_SCHEDULETABLE_H
#define _AUTOSAR_SCHEDULETABLE_H
#include "os_types.h"
#include "os_task.h"
#include "os_event.h"

/************************* STRUCTURE DEFINITION *************************/
struct OsScheduleTableSync {
	uint64 OsScheduleTblExplicitPrecision;
	EcucEnumerationParamDef OsScheduleTblSyncStrategy;
};

struct OsScheduleTblAdjustableExpPoint {
	uint64 OsScheduleTableMaxLengthen;
	uint64 OsScheduleTableMaxShorten;
};

struct OsScheduleTableTaskAction {
	struct OsTask * OsScheduleTableActivateTaskRef;
};

struct OsScheduleTableExpiryPoint {
	uint64 OsScheduleTblExpPointOffset;
	struct OsScheduleTableEventSetting * OsScheduleTableEventSetting;
	struct OsScheduleTableTaskActivation * OsScheduleTableTaskActivation;
	struct OsScheduleTblAdjustableExpPoint * OsScheduleTblAdjustableExpPoint;
};

struct OsScheduleTableEventSetting {
	struct OsEvent * OsScheduleTableSetEventRef;
	struct OsTask * OsScheduleTableSetEventTaskRef;
};

struct OsScheduleTableAutostart {
	EcucEnumerationParamDef OsScheduleTableAutostartType;
	uint64 OsScheduleTableStartValue;
        //	struct OsAppMode * OsScheduleTableAppModeRef;
};
	
struct OsScheduleTable{
	uint64 OsScheduleTableDuration;
	EcucBooleanParamDef OsScheduleTableRepeating;
	struct OsApplication * OsSchTblAccessingApplication;
	struct OsCounter * OsScheduleTableCounterRef;
	struct OsScheduleTableAutostart * OsScheduleTableAutostart;
	struct OsScheduleTableExpiryPoint * OsScheduleTableExpiryPoint;
	struct OsScheduleTableSync * OsScheduleTableSync;
};


#endif
