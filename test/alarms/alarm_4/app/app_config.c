#include "os_task.h"
#include "os_appmode.h"
#include "os_os.h"

/*********************** AppMode ***********************/
#define app1_ID 0
#define app1_bit  (1 << app1_ID)
const AppModeType app1 = app1_ID;
struct OsAppMode app1_OsAppMode = {
	"app1",		//	EcucStringParamDef OsAppModeName;
	app1_bit	//	EcucIntegerParamDef OsAppModeValue;	
};


/*********************** Task t2 ***********************/

#define t2_ID 1
const TaskType t2 = t2_ID; 
extern void t2_function(void);

struct AUTOSAR_TCB t2_tcb = {
	NULL, 		// 	struct OsXenomaiTask * xeno_task;
	t2_function,	// 	EcucFunctionParamDef handler;
	SUSPENDED,	// 	TaskStateType state;
	0,		//	EcucIntegerParamDef activation_number;
	t2_RESOURCE_COUNT//	EcucIntegerParamDef resourceCount link to task
};

struct OsTask t2_OsTask = {
	1,		//	EcucIntegerParamDef OsTaskActivation;
	90,		//	EcucIntegerParamDef OsTaskPriority;
	TASK_NON,		//	EcucEnumerationParamDef OsTaskSchedule; NON or FULL preempt
	NULL,		//	struct OsApplication * OsTaskAccessingApplication;
	NULL,		//	struct OsEvent * OsTaskEventRef;
	NULL,		//	struct OsResource * OsTaskResourceRef;
	NULL,		//	struct OsTaskAutostart * OsTaskAutostart;
	NULL,		//	struct OsTaskTimingProtection * OsTaskTimingProtection;
	&t2_tcb		//	IMPL+ : AutosarTCB 
};

/*********************** Task t1 ***********************/

#define t1_ID 0
const TaskType t1 = t1_ID;
extern void t1_function(void);

struct AUTOSAR_TCB t1_tcb = {
	NULL, 		// 	struct OsXenomaiTask * xeno_task;
	t1_function,	// 	EcucFunctionParamDef handler;
	SUSPENDED,	// 	TaskStateType state;
	0,		//	EcucIntegerParamDef activation_number;
	t1_RESOURCE_COUNT //	EcucIntegerParamDef resourceCount link to task
};

/*struct OsTaskAutostart t1_OsTaskAutostart = {
	&app1_OsAppMode
};*/


struct OsTask t1_OsTask = {
	1,		//	EcucIntegerParamDef OsTaskActivation;
	100,		//	EcucIntegerParamDef OsTaskPriority;
	TASK_NON,		//	EcucEnumerationParamDef OsTaskSchedule; NON or FULL preempt
	NULL,		//	struct OsApplication * OsTaskAccessingApplication;
	NULL,		//	struct OsEvent * OsTaskEventRef;
	NULL,		//	struct OsResource * OsTaskResourceRef;
	NULL,		//	struct OsTaskAutostart * OsTaskAutostart;
	NULL,		//	struct OsTaskTimingProtection * OsTaskTimingProtection;
	&t1_tcb		//	IMPL+ : AutosarTCB 
};

/************************ Tasks ************************/

struct OsTask * OsTask_table[TASK_COUNT] = {
	&t1_OsTask,
	&t2_OsTask
};

/********************* Counter 1 **********************/
#define counter1_ID 0
const CounterType counter1 = counter1_ID;
struct OsCounter counter1_OsCounter = {
	32000,		//	EcucIntegerParamDef OsCounterMaxAllowedValue;
	1,		//	EcucIntegerParamDef OsCounterMinCycle;
	1,		//	EcucIntegerParamDef OsCounterTicksPerBase;
	COUNTER_HARDWARE,	//	OsCounterType OsCounterType;
	0,		//	EcucFloatParamDef OsSecondsPerTick;
	NULL,		//	struct OsApplication * OsCounterAccessingApplication;
	//		//	struct OsDriver * OsDriver;
	NULL,		//	struct OsTimerConstant * OsTimerConstant;
	NULL		//	IMPL+ : struct AUTOSAR_CCB * ccb;
};


/********************** Counters **********************/
struct OsCounter * OsCounter_table[COUNTER_COUNT] = {
	&counter1_OsCounter
};

/********************** Alarm 3 ***********************/
#define alarm3_ID 2
const AlarmType alarm3 = alarm3_ID;

struct OsAlarmActivateTask alarm3_OsAlarmActivateTask = {
	&t1_OsTask
};

struct OsAlarmAction  alarm3_OsAlarmAction = {
	&alarm3_OsAlarmActivateTask,	//	struct OsAlarmActivateTask * OsAlarmActivateTask;
	NULL,		//	struct OsAlarmCallback * OsAlarmCallback;
	NULL,		//	struct OsAlarmIncrementCounter * OsAlarmIncrementCounter;
	NULL		//	struct OsAlarmSetEvent * OsAlarmSetEvent;

};

struct OsAlarmAutostart alarm3_OsAlarmAutostart = {
	1000,		//	EcucIntegerParamDef OsAlarmAlarmTime;
	ALARM_ABSOLUTE,	//	EcucEnumerationParamDef OsAlarmAutostartType;
	0,		//	EcucIntegerParamDef OsAlarmCycleTime;
	&app1_OsAppMode	//	struct OsAppMode * OsAlarmAppModeRef;
};

struct OsAlarm alarm3_OsAlarm = {
	NULL,				//	struct OsApplication * OsAlarmAccesingApplication;
	&counter1_OsCounter,		//	struct OsCounter * OsAlarmCounterRef;
	&alarm3_OsAlarmAction,		//	struct OsAlarmAction * OsAlarmAction;
	&alarm3_OsAlarmAutostart,				//	struct OsAlarmAutostart * OsAlarmAutostart;
	NULL				//	IMPL+ : struct AUTOSAR_ACB * acb;

};

/********************** Alarm 2 ***********************/
#define alarm2_ID 1
const AlarmType alarm2 = alarm2_ID;

struct OsAlarmActivateTask alarm2_OsAlarmActivateTask = {
	&t2_OsTask
};

struct OsAlarmAction  alarm2_OsAlarmAction = {
	&alarm2_OsAlarmActivateTask,	//	struct OsAlarmActivateTask * OsAlarmActivateTask;
	NULL,		//	struct OsAlarmCallback * OsAlarmCallback;
	NULL,		//	struct OsAlarmIncrementCounter * OsAlarmIncrementCounter;
	NULL		//	struct OsAlarmSetEvent * OsAlarmSetEvent;

};


struct OsAlarm alarm2_OsAlarm = {
	NULL,				//	struct OsApplication * OsAlarmAccesingApplication;
	&counter1_OsCounter,		//	struct OsCounter * OsAlarmCounterRef;
	&alarm2_OsAlarmAction,		//	struct OsAlarmAction * OsAlarmAction;
	NULL,	//	struct OsAlarmAutostart * OsAlarmAutostart;
	NULL				//	IMPL+ : struct AUTOSAR_ACB * acb;

};

/********************** Alarm 1 ***********************/
#define alarm1_ID 0
const AlarmType alarm1 = alarm1_ID;

struct OsAlarmActivateTask alarm1_OsAlarmActivateTask = {
	&t2_OsTask
};

struct OsAlarmAction  alarm1_OsAlarmAction = {
	&alarm1_OsAlarmActivateTask,	//	struct OsAlarmActivateTask * OsAlarmActivateTask;
	NULL,		//	struct OsAlarmCallback * OsAlarmCallback;
	NULL,		//	struct OsAlarmIncrementCounter * OsAlarmIncrementCounter;
	NULL		//	struct OsAlarmSetEvent * OsAlarmSetEvent;

};
struct OsAlarmAutostart alarm1_OsAlarmAutostart = {
	100,		//	EcucIntegerParamDef OsAlarmAlarmTime;
	ALARM_ABSOLUTE,	//	EcucEnumerationParamDef OsAlarmAutostartType;
	100,		//	EcucIntegerParamDef OsAlarmCycleTime;
	&app1_OsAppMode	//	struct OsAppMode * OsAlarmAppModeRef;
};

struct OsAlarm alarm1_OsAlarm = {
	NULL,				//	struct OsApplication * OsAlarmAccesingApplication;
	&counter1_OsCounter,		//	struct OsCounter * OsAlarmCounterRef;
	&alarm1_OsAlarmAction,		//	struct OsAlarmAction * OsAlarmAction;
	&alarm1_OsAlarmAutostart,	//	struct OsAlarmAutostart * OsAlarmAutostart;
	NULL				//	IMPL+ : struct AUTOSAR_ACB * acb;

};



/*********************** Alarms ***********************/


struct OsAlarm * OsAlarm_table[ALARM_COUNT] = {
	&alarm1_OsAlarm,
	&alarm2_OsAlarm,
	&alarm3_OsAlarm
};

/************************* OS *************************/
/*
struct OsHooks osHooks = {
	FALSE,		//	EcucBooleanParamDef OsErrorHook;
	FALSE,		//	EcucBooleanParamDef OsPostTaskHook;
	FALSE,		//	EcucBooleanParamDef OsPreTaskHook;
	FALSE,		//	EcucBooleanParamDef OsProtectionHook;
	FALSE,		//	EcucBooleanParamDef OsShutdownHook;
	FALSE,		//	EcucBooleanParamDef OsStartupHook;
};

struct OsOS osOS = {
	1, 		//	EcucIntegerParamDef OsNumberOfCores;
	OSOS_SC1,	//	EcucEnumerationParamDef OsScalabilityClass;
	FALSE,		//	EcucBooleanParamDef OsStackMonitoring;
	OSOS_EXTENDED,	//	EcucEnumerationParamDef OsStatus;
	FALSE,		//	EcucBooleanParamDef OsUseGetServiceId;
	FALSE,		//	EcucBooleanParamDef OsUseParameterAccess;
	FALSE,		//	EcucBooleanParamDef OsUseResScheduler;
	&osHooks	// 	struct OsHooks * OsHooks;
};
*/

struct Os os = {
	OsAlarm_table,	// 	struct OsAlarm * OsAlarm;
	NULL,		//	struct OsAppMode * OsAppMode;
	NULL,		//	struct OsApplication * OsApplication;
	OsCounter_table,//	struct OsCounter * OsCounter;
	NULL,		//	struct OsEvent * OsEvent;
			////	struct OsIoc * OsIoc;
	NULL,		//	struct OsIsr * OsIsr;
//	&osOS,		//	struct OsOS * OsOS;
	NULL,//	struct OsResource * OsResource;
	NULL,		//	struct OsScheduleTable * OsScheduleTable;
	NULL,		//	struct OsSpinlock * OsSpinlock;
	OsTask_table	// 	struct OsTask * OsTask;
};
