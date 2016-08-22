

/*****************************************************************
 * 						Generated by Xpand Eclipse
 *						File :	app_config.c
 *						Date : 
 *						Author : Matthieu Kowalewski
 *						License : GPLv2
 ****************************************************************/
#include "os_task.h"
#include "os_appmode.h"
#include "os_os.h"



#define OsAppModeDefault_bit  (1 << OsAppModeDefault_ID)

struct OsAppMode OsAppModeDefault_OsAppMode = {
	"OsAppModeDefault",		//	EcucStringParamDef OsAppModeName;
	OsAppModeDefault_bit	//	EcucIntegerParamDef OsAppModeValue;	
};

struct OsAppMode * OsAppMode_table[APP_COUNT] = {
	&OsAppModeDefault_OsAppMode
};

/*********************** Task OsTask_100ms ***********************/
extern void OsTask_100ms_function(void);

struct OsTask OsTask_100ms_OsTask = {
	1,	//	EcucIntegerParamDef OsTaskActivation;
	10,		//	EcucIntegerParamDef OsTaskPriority;
	TASK_FULL,	//	EcucEnumerationParamDef OsTaskSchedule; NON or FULL preempt
	//											//	struct OsApplication * OsTaskAccessingApplication;
	//											//	struct OsEvent * OsTaskEventRef;
	NULL,										//	struct OsResource * OsTaskResourceRef;
	NULL,										//	struct OsTaskAutostart * OsTaskAutostart;
	//											//	struct OsTaskTimingProtection * OsTaskTimingProtection;
	OsTask_100ms_function,			//	EcucFunctionNameDef handler;
	0,//OsTask_100ms_RESOURCE_COUNT,	//	EcucIntegerParamDef ResourceCount;	
	NULL										//	struct OsTaskXenomai * OsTaskXenomai;
};


/*********************** Task OsTask_5ms ***********************/
extern void OsTask_5ms_function(void);

struct OsTask OsTask_5ms_OsTask = {
	1,	//	EcucIntegerParamDef OsTaskActivation;
	15,		//	EcucIntegerParamDef OsTaskPriority;
	TASK_FULL,	//	EcucEnumerationParamDef OsTaskSchedule; NON or FULL preempt
	//											//	struct OsApplication * OsTaskAccessingApplication;
	//											//	struct OsEvent * OsTaskEventRef;
	NULL,										//	struct OsResource * OsTaskResourceRef;
	NULL,										//	struct OsTaskAutostart * OsTaskAutostart;
	//											//	struct OsTaskTimingProtection * OsTaskTimingProtection;
	OsTask_5ms_function,			//	EcucFunctionNameDef handler;
	0,//OsTask_5ms_RESOURCE_COUNT,	//	EcucIntegerParamDef ResourceCount;	
	NULL										//	struct OsTaskXenomai * OsTaskXenomai;
};


 
struct OsTask * OsTask_table[TASK_COUNT] = {
&OsTask_100ms_OsTask, 
&OsTask_5ms_OsTask, 
};

/*********************** Counter SystemCounter ***********************/
struct OsCounter SystemCounter_OsCounter = {
	65535,		//	EcucIntegerParamDef OsCounterMaxAllowedValue;
	1,		//	EcucIntegerParamDef OsCounterMinCycle;
	1,		//	EcucIntegerParamDef OsCounterTicksPerBase;
	COUNTER_HARDWARE,	//	OsCounterType OsCounterType;
	0.001,		//	EcucFloatParamDef OsSecondsPerTick;
	//		//	struct OsApplication * OsCounterAccessingApplication;
	//		//	struct OsDriver * OsDriver;
	//		//	struct OsTimerConstant * OsTimerConstant;
	NULL	//	IMPL+ : struct OsCounterXenomai * OsCounterXenomai;
};
 
struct OsCounter * OsCounter_table[COUNTER_COUNT] = {
&SystemCounter_OsCounter,
};

/*********************** Alarm OsAlarm_100ms ***********************/
struct OsAlarmAutostart OsAlarm_100ms_OsAlarmAutostart = {
	150, 								//uint64 OsAlarmAlarmTime; 		/*!< Start time of the alarm int tick */
	ALARM_ABSOLUTE,										//OsAlarmAutostartType OsAlarmAutostartType;	/*!< Alarm type : OsAlarmAutostartType */
	100, 								//uint64 OsAlarmCycleTime;		/*!< Cycle time of the alarm in tick */
	&OsAppModeDefault_OsAppMode		//struct OsAppMode * OsAlarmAppModeRef;		/*!< Appmode to autostart the alarm */
};


struct OsAlarmActivateTask OsAlarm_100ms_OsAlarmActivateTask = {
	&OsTask_100ms_OsTask			// struct OsTask * OsAlarmActivateTaskRef; /*!< Task to activate */
}; 


struct OsAlarmAction  OsAlarm_100ms_OsAlarmAction = {
	&OsAlarm_100ms_OsAlarmActivateTask,
	NULL,
	NULL
};

struct OsAlarm OsAlarm_100ms_OsAlarm = {
	//					//	struct OsApplication * OsAlarmAccesingApplication;
	&SystemCounter_OsCounter,		//	struct OsCounter * OsAlarmCounterRef;
	&OsAlarm_100ms_OsAlarmAction,		//	struct OsAlarmAction * OsAlarmAction;
	&OsAlarm_100ms_OsAlarmAutostart,
	NULL				//	IMPL+ :  struct OsAlarmXenomai * OsAlarmXenomai;
};
/*********************** Alarm OsAlarm_5ms ***********************/
struct OsAlarmAutostart OsAlarm_5ms_OsAlarmAutostart = {
	100, 								//uint64 OsAlarmAlarmTime; 		/*!< Start time of the alarm int tick */
	ALARM_ABSOLUTE,										//OsAlarmAutostartType OsAlarmAutostartType;	/*!< Alarm type : OsAlarmAutostartType */
	5, 								//uint64 OsAlarmCycleTime;		/*!< Cycle time of the alarm in tick */
	&OsAppModeDefault_OsAppMode		//struct OsAppMode * OsAlarmAppModeRef;		/*!< Appmode to autostart the alarm */
};


struct OsAlarmActivateTask OsAlarm_5ms_OsAlarmActivateTask = {
	&OsTask_5ms_OsTask			// struct OsTask * OsAlarmActivateTaskRef; /*!< Task to activate */
}; 


struct OsAlarmAction  OsAlarm_5ms_OsAlarmAction = {
	&OsAlarm_5ms_OsAlarmActivateTask,
	NULL,
	NULL
};

struct OsAlarm OsAlarm_5ms_OsAlarm = {
	//					//	struct OsApplication * OsAlarmAccesingApplication;
	&SystemCounter_OsCounter,		//	struct OsCounter * OsAlarmCounterRef;
	&OsAlarm_5ms_OsAlarmAction,		//	struct OsAlarmAction * OsAlarmAction;
	&OsAlarm_5ms_OsAlarmAutostart,
	NULL				//	IMPL+ :  struct OsAlarmXenomai * OsAlarmXenomai;
};
 
struct OsAlarm* OsAlarm_table[ALARM_COUNT] = {
&OsAlarm_100ms_OsAlarm,
&OsAlarm_5ms_OsAlarm,
};

struct Os os = {
	OsAlarm_table,	// 	struct OsAlarm ** OsAlarm;
	OsAppMode_table,//	struct OsAppMode ** OsAppMode;
	//	NULL,		//	struct OsApplication ** OsApplication;
	OsCounter_table,//	struct OsCounter ** OsCounter;
	//	NULL,		//	struct OsEvent ** OsEvent;
	//	NULL,		//	struct OsIoc * OsIoc;
	//	NULL,		//	struct OsIsr * OsIsr;
	NULL,			//	struct OsResource ** OsResource;
	//	NULL,		//	struct OsScheduleTable ** OsScheduleTable;
	//	NULL,		//	struct OsSpinlock * OsSpinlock;
	OsTask_table	// 	struct OsTask * OsTask;
};

