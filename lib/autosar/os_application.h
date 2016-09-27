/**
 *
 *	\file os_application.h
 *	\brief Application header
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_APPLICATION_H
#define _AUTOSAR_APPLICATION_H
/*
#include "os_types.h"
#include "os_alarm.h"
#include "os_counter.h"
#include "os_isr.h"
#include "os_scheduletable.h"
#include "os_task.h"
#include "os_appmode.h"

struct OsApplicationTrustedFunction{
	EcucFunctionNameDef OsTrustedFunctionName;
};

struct OsApplicationHooks {
	EcucBooleanParamDef OsAppErrorHook;
	EcucBooleanParamDef OsAppShutdownHook;
	EcucBooleanParamDef OsAppStartupHook;
};


struct OsApplication{
	EcucBooleanParamDef OsTrusted;
	EcucBooleanParamDef OsTrustedApplicationDelayTimingViolationCall;
	EcucBooleanParamDef OsTrustedApplicationWithProtection;
	struct OsAlarm * OsAppAlarmRef;
	struct OsCounter * OsAppCounterRef;
	EcucPartition OsAppEcucPartitionRef;
	struct OsIsr * OsAppIsrRef;
	struct OsScheduleTable * OsAppScheduleTableRef;
	struct OsTask * OsAppTaskRef;
	EcucCoreDefintion * OsApplicationCoreRef;
	struct OsTask * OsRestartTask;
	struct OsApplicationHooks * OsApplicationHooks;
	struct OsApplicationTrustedFunction * OsApplicationTrustedFunction;
};
*/



#endif
