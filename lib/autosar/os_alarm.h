/**
 *
 *	\file os_alarm.h
 *	\brief Autosar Alarm header 
 *	\author  Matthieu Kowalewski
 *	\version 1.0
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

/************************** MACRO DEFINITION ****************************/
#define TM_INFINITE     0
#define alarm_magic	0x21122112

/************************* STRUCTURE DEFINITION *************************/
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
	struct OsCounter * OsAlarmCounterRef;		/*!< Counter link to the alarm  */
	struct OsAlarmAction * OsAlarmAction;		/*!< Reference to OsAlarmAction */
	struct OsAlarmAutostart * OsAlarmAutostart;	/*!< Reference to OsAlarmAutostart */
	struct OsAlarmXenomai * OsAlarmXenomai;		/*!< Reference to OsAlarmXenomai */
        //	struct OsApplication * OsAlarmAccesingApplication;
};

struct pvcluster xenomai_autosar_alarm_table;


/************************* FUNCTION DEFINITION *************************/

/**
 * \fn struct OsAlarm * get_alarm(AlarmType alarmID)
 * \brief Get the alarm<alarmID> structure 
 * \param alarmID ID of the alarm
 * \return OsAlarm structure if exist, NULL if not 
 */
struct OsAlarm * get_alarm(AlarmType alarmID);
 
/**
 * \fn StatusType __CreateAlarm(AlarmType alarmID)
 * \brief Create and Init the alarm<alarmID>
 * \param alarmID ID of the alarm
 * \return E_OK if alarm created, E_OS_ID if the alarmID is invalid 
 */
StatusType __CreateAlarm(AlarmType alarmID);

/**
 * \fn StatusType __ActivateAlarm(AlarmType alarmID)
 * \brief Activate the alarm<alarmID>
 * \param alarmID ID of the alarm
 * \return E_OK if alarm activated, E_OS_ID if the alarmID is invalid 
 */
StatusType __ActivateAlarm(AlarmType alarmID);

/**
 * \fn StatusType __GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
 * \brief Cf ::GetAlarmBase
 */
StatusType __GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info);

/**
 * \fn StatusType __GetAlarm(AlarmType AlarmID, TickRefType Tick)
 * \brief Cf ::GetAlarm
 */
StatusType __GetAlarm(AlarmType AlarmID, TickRefType Tick);

/**
 * \fn StatusType __SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle)
 * \brief Cf ::SetRelAlarm
 */
StatusType __SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle);

/**
 * \fn StatusType __SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle)
 * \brief Cf ::SetAbsAlarm
 */
StatusType __SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle);

/**
 * \fn StatusType __CancelAlarm(AlarmType AlarmID)
 * \brief Cf ::CancelAlarm
 */
StatusType __CancelAlarm(AlarmType AlarmID);

/**
 * \fn void __StopAlarm(AlarmType AlarmID);
 * \brief Uninit and delete AUTOSAR alarm<alarmID>
 * \param alarmID ID of the alarm
 */
void __StopAlarm(AlarmType AlarmID);
#endif
