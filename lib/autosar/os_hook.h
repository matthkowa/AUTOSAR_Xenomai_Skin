/**
 *
 *	\file os_hook.h
 *	\brief Hook header
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_HOOK_H
#define _AUTOSAR_HOOK_H
#include "app/app_define.h"
#include <autosar/os.h>
#include <copperplate/threadobj.h>
#include <copperplate/cluster.h>
#include <copperplate/eventobj.h>
#include "os_types.h"

/************************** MACRO DEFINITION ****************************/
#define OS_STARTUP_HOOK_FLAG 			(1<<0)
#define OS_SHUTDOWN_HOOK_FLAG 			(1<<1)
#define OS_ERROR_HOOK_FLAG 			(1<<2)
#define OS_PRE_TASK_HOOK_FLAG 			(1<<3)
#define OS_POST_TASK_HOOK_FLAG 			(1<<4)

/************************* STRUCTURE DEFINITION *************************/

/*
 *	\struct OsHookXenomai
 *	\brief Xenomai structure to describe a hook
 */
struct OsHookXenomai {
	char name[XNOBJECT_NAME_LEN]; 		/*!< Name of the hook */
	struct threadobj thobj;			/*!< Thread object (Xenomai) */
	void (*entry)(void);			/*!< Hook routine to call */
	struct clusterobj cobj;			/*!< Cluster object (Xenomai) */
	struct eventobj evobj;                  /*!< evobj Event to activate the hook */
	StatusType Error;                       /*!< Error Null if no error detected, E_OS_ID,.... if an error occured */
	int flag;                               /*!< flag flag link to the hook */
};

struct OsHookXenomai * OsHook_table[5];


/************************* FUNCTION DEFINITION *************************/


OSServiceIdType _OSErrorGetServiceId;

/**
 * \fn StatusType __CreateHook(unsigned int flag)
 * \brief Create and Init the Hook identify by its flag
 * \param flag flag of the hook (i.e : OS_STARTUP_HOOK_FLAG)
 * \return E_OK if hook created, E_OS_ID if the flag is invalid 
 */
StatusType __CreateHook(unsigned int flag);

/**
 * \fn StatusType __ActivateOsHook(unsigned int flag,StatusType error)
 * \brief Activate the Hook identify by its flag
 * \param flag flag of the hook (i.e : OS_STARTUP_HOOK_FLAG)
 * \param error only if the flag is OS_ERROR_HOOK_FLAG, the error occured 
 * \return E_OK if hook activated, E_OS_ID if the flag is invalid 
 */
StatusType __ActivateOsHook(unsigned int flag,StatusType error);

/**
 * \fn void __StopHook(unsigned int __flag)
 * \brief Uninit and delete AUTOSAR hook identify by its flag
 * \param flag flag of the hook (i.e : OS_STARTUP_HOOK_FLAG)
 */
void __StopHook(unsigned int __flag);

#if OS_STARTUP_HOOK
extern void StartupHook(void);
#endif
#if OS_SHUTDOWN_HOOK
extern void ShutdownHook(StatusType Error);
#endif
#if OS_ERROR_HOOK

extern void ErrorHook(StatusType Error);

/**
 * \fn StatusType __CheckError(StatusType ret,OSServiceIdType ServiceId)
 * \brief check if an error occured and call errorHook if needed
 * \param ret value return by a service
 * \param service id
 */
void __CheckError(StatusType ret,OSServiceIdType ServiceId);

/* Setter of argument pass in a service */
TaskType _TaskID;
void __SetTaskID(TaskType TaskID);

TaskRefType _TaskRefID;
void __SetTaskRefID(TaskRefType taskRefID);

TaskStateRefType _State;
void __SetTaskState(TaskStateRefType State);

AlarmType _AlarmID;
void __SetAlarmID(AlarmType AlarmID);

AlarmBaseRefType _Info;
void __SetInfo(AlarmBaseRefType Info);

TickRefType _Tick;
void __SetTick(TickRefType Tick);

TickType _increment;
void __Setincrement(TickType increment);

TickType _cycle;
void __Setcycle(TickType cycle);

TickType _start;
void __Setstart(TickType start);

#endif
#if OS_PRE_TASK_HOOK
extern void PreTaskHook(void);
#endif
#if OS_POST_TASK_HOOK
extern void PostTaskHook(void);
#endif


#endif /* _AUTOSAR_HOOK_H */
