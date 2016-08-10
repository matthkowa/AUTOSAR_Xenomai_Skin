/**
 *
 *	\file os_alarm.h
 *	\brief Autosar Alarm header 
 *	\author  Matthieu K.
 *	\version 0.1
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_ALARM_H
#define _AUTOSAR_ALARM_H
#include <autosar/os.h>
#include <copperplate/registry.h>
#include <copperplate/cluster.h>
#include <copperplate/registry.h>
#include <copperplate/threadobj.h>
#include "app/app_config.h"
#include "app/app_define.h"
#include "os_timer.h"
#include "os_appmode.h"


#define TM_INFINITE  0

#define alarm_magic	0x21122112

/*
 *	\struct OsAlarmXenomai
 *	\brief Inner structure to describe an alarm in Xenomai
 */

struct OsAlarmXenomai {
	char name[XNOBJECT_NAME_LEN]; 	/*!< Alarm name */
	struct pvclusterobj cobj;	/*!< Cluster object (Xenomai) */
	struct fsobj fsobj;		/*!< File system object (Xenomai) */
	TickType value;			/*!< Start value in tick */
	TickType cycle;			/*!< Cycle value in tick */
	TickType next;			/*!< Next wakeup in tick */
	int isActive;			/*!< Alarm status : TRUE : active, FALSE : inactive*/
};

/*
 *	\struct OsAlarmIncrementCounter
 *	\brief OsCounter reference to increment this alarm
 */
struct OsAlarmIncrementCounter{
	struct OsCounter * OsAlarmIncrementCounterRef; /*!< OsCounter reference*/
};
/*
 *	\struct OsAlarmCallback
 *	\brief OsAlarm Callback (!WARNING : Only defined in Scalability 1 !)
 */
struct OsAlarmCallback {
	EcucFunctionNameDef OsAlarmCallback;	/*!< Callback function */
};

/*struct OsAlarmSetEvent {
	struct OsEvent *  OsAlarmSetEventRef;
	struct OsTask * OsAlarmSetEventTaskRef;
};*/


/*
 *	\struct OsAlarmActivateTask
 *	\brief Task to activate by the alarm
 */
struct OsAlarmActivateTask {
	struct OsTask * OsAlarmActivateTaskRef; /*!< Task to activate */
};
/*
 *	\Enum AlarmAutostartType
 *	\brief Alarm autostart type 
 */
enum AlarmAutostartType {
	ALARM_ABSOLUTE, /*!< Absolute start */
	ALARM_RELATIVE  /*!< Relative start */
};
/*! Create a type name for AlarmAutostartType */
typedef enum AlarmAutostartType OsAlarmAutostartType;

/*
 *	\struct OsAlarmAction
 *	\brief List of action to execute when the alarm expire
 */
struct OsAlarmAction {
	struct OsAlarmActivateTask * OsAlarmActivateTask; 		/*!< Reference to OsAlarmActivateTask */
	struct OsAlarmCallback * OsAlarmCallback;			/*!< Reference to OsAlarmCallback */
	struct OsAlarmIncrementCounter * OsAlarmIncrementCounter;	/*!< Reference to OsAlarmIncrementCounter */
//	struct OsAlarmSetEvent * OsAlarmSetEvent;
};
/*
 *	\struct OsAlarmAutostart
 *	\brief Structure to autostart an alarm
 */
struct OsAlarmAutostart {
	uint64 OsAlarmAlarmTime; 		/*!< Start time of the alarm int tick */
	OsAlarmAutostartType OsAlarmAutostartType;	/*!< Alarm type : OsAlarmAutostartType */
	uint64 OsAlarmCycleTime;		/*!< Cycle time of the alarm in tick */
	struct OsAppMode * OsAlarmAppModeRef;		/*!< Appmode to autostart the alarm */
};
/*
 *	\struct OsAlarm
 *	\brief Autosar alarm structure to describe an alarm 
 */
struct OsAlarm {
//	struct OsApplication * OsAlarmAccesingApplication;
	struct OsCounter * OsAlarmCounterRef;		/*!< Counter link to the alarm  */
	struct OsAlarmAction * OsAlarmAction;		/*!< Reference to OsAlarmAction */
	struct OsAlarmAutostart * OsAlarmAutostart;	/*!< Reference to OsAlarmAutostart */
	struct OsAlarmXenomai * OsAlarmXenomai;		/*!< Reference to OsAlarmXenomai */
};


struct pvcluster xenomai_autosar_alarm_table;

struct OsAlarm * get_alarm(AlarmType alarmID);
 
StatusType __CreateAlarm(AlarmType alarmID);

StatusType __ActivateAlarm(AlarmType alarmID);

StatusType __GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);

StatusType __GetAlarm(AlarmType AlarmID, TickRefType Tick);

StatusType __SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);

StatusType __SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);

StatusType __CancelAlarm(AlarmType AlarmID);

void __StopAlarm(AlarmType AlarmID);
#endif
