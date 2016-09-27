/**
 *
 *	\file os_task.h
 *	\brief Autosar task header 
 *	\author  Matthieu Kowalewski.
 *	\version 1.0
 *	\date 29 Juin 2016
*/	


#ifndef _AUTOSAR_TASK_H
#define _AUTOSAR_TASK_H
#include <autosar/os.h>
#include <copperplate/threadobj.h>
#include <copperplate/registry.h>
#include <copperplate/cluster.h>
#include <copperplate/eventobj.h>
#include "os_types.h"
#include "os_resource.h"
#include "os_resource.h"
#include "os_appmode.h"

/************************** MACRO DEFINITION ****************************/
#define T_LOCK	__THREAD_M_LOCK
#define T_LOPRIO  0
#define T_HIPRIO  250
#define task_magic 0x12211221

/************************* STRUCTURE DEFINITION *************************/
extern struct syncluster xenomai_autosar_task_table;

/*
 *	\struct autosar_queue_wait 
 *	\brief Xenomai structure
 */
struct autosar_queue_wait {
	size_t size;
	dref_type(void *) ptr;
};

/*
 *	\struct OsTaskXenomai
 *	\brief Inner structure to describe an task in Xenomai
 */
struct OsTaskXenomai {
	char name[XNOBJECT_NAME_LEN]; 		/*!< Name of the task */
	struct threadobj thobj;			/*!< Thread object (Xenomai) */
	void (*entry)(void);			/*!< Function to call */
	struct fsobj fsobj;			/*!< File system object (Xenomai)*/
	struct clusterobj cobj;			/*!< Cluster object (Xenomai) */
	TaskType xenomaiID;			/*!< Xenomai identifiant */
	TaskType autosarID;			/*!< Autosar identifiant */
	int mode;				/*!< Mode : T_LOCK = NON preemptive */
	struct eventobj evobj;			/*!< Event object (Xenomai) */
	TaskStateType state;			/*!< Autosar state */
	uint32 activation_number;	/*!< Current number of activation */
	pthread_t thread;
};


/*struct OsTaskResourceLock {
	EcucFloatParamDef OsTaskResourceLockBudget;
	struct OsResource * OsTaskResourceLockResourceRef;
};

struct OsTaskTimingProtection {
	EcucFloatParamDef OsTaskAllInterruptLockBudget;
	EcucFloatParamDef OsTaskExecutionBudget;
	EcucFloatParamDef OsTaskOsInterruptLockBudget;
	EcucFloatParamDef OsTaskTimeFrame;
	struct OsTaskResourceLock * OsTaskResourceLock;
};*/

/*
 *	\struct OsTaskAutostart
 *	\brief Autosar structure to describe an autostart task
 */
struct OsTaskAutostart {
	struct OsAppMode * OsTaskAppModeRef; /*!< In AppMode, the task will autostart */
};

/*
 *	\enum TaskSchedule
 *	\brief FULL or NON Preemptive task
 */

enum TaskSchedule {
	TASK_FULL, /*!< FULL preemptive task */
	TASK_NON   /*!< NON preemptive task */
}; 
/*! Create a type name for TaskSchedule */
typedef enum TaskSchedule OsTaskSchedule;

/*
 *	\struct OsTaskAutostart
 *	\brief Autosar structure to describe a task
 */
struct OsTask {
	uint32 OsTaskActivation; 		/*!<Number of activation allowed */
	uint32 OsTaskPriority; 		/*!< Priority of the task*/
	OsTaskSchedule OsTaskSchedule;			/*!< Full or Non preemptive task*/       
	struct OsResource ** OsTaskResourceRef; 	/*!< Array of Task internal resource*/
	struct OsTaskAutostart * OsTaskAutostart;	/*!< Autostart structure of the task*/
	EcucFunctionNameDef handler;			/*!< Task function defined by TASK(task) */
	uint32 ResourceCount;		/*!< Number of Internal Resource*/	
	struct OsTaskXenomai * OsTaskXenomai;		/*!< Pointer to the OsTaskXenomai : always NULL */
        //	struct OsApplication * OsTaskAccessingApplication;
        //	struct OsEvent * OsTaskEventRef;
        //	struct OsTaskTimingProtection * OsTaskTimingProtection;
};


/************************* FUNCTION DEFINITION *************************/

/*
 *	\fn check_task_priority(uint32 prio)
 *	\brief Check if task priority is between T_LOPRIO and T_HIPRIO value
 *      \param prio Task priority.
 *      \return 0 if ok, -EINVAL if not                
 */
static inline int check_task_priority(uint32 prio)
{				
	return prio < T_LOPRIO || prio > T_HIPRIO ? -EINVAL : 0;
}

/**
 * \fn StatusType __CreateTask(TaskType taskID)
 * \brief Create and init Xenomai task with OsTask configuration 
 * \param taskID Task reference.
 * \return E_OK if no error, E_OS_ID if taskID doesn't exist.
 */
StatusType __CreateTask(TaskType taskID);

/**
 * \fn StatusType __ActivateOsTask(struct OsTask * osTask)
 * \brief Activate an Autosar task 
 * \param osTask task structure to activate.
 * \return E_OK if no error.
 */
StatusType __ActivateOsTask(struct OsTask * osTask);

/**
 * \fn StatusType __ActivateTask(TaskType taskID)
 * \brief Cf ::ActivateTask
 */
StatusType __ActivateTask(TaskType taskID);

/**
 * \fn StatusType __TerminateTask(void)
 * \brief Cf ::TerminateTask
 */
StatusType __TerminateTask(void);

/**
 * \fn StatusType __ChainTask(TaskType taskID)
 * \brief Cf ::ChainTask
 */
StatusType __ChainTask(TaskType taskID);

/**
 * \fn StatusType StatusType __Schedule(void)
 * \brief Cf ::Schedule
 */
StatusType __Schedule(void);

/**
 * \fn StatusType StatusType __GetTaskID(TaskRefType taskID)
 * \brief Cf ::GetTaskID
 */
StatusType __GetTaskID(TaskRefType taskID);

/**
 * \fn StatusType __GetTaskState(TaskType taskID, TaskStateRefType state)
 * \brief Cf ::GetTaskState
 */
StatusType __GetTaskState(TaskType taskID, TaskStateRefType state);

/**
 * \fn StatusType __StopTask(TaskType taskID)
 * \brief Uninit and delete AUTOSAR task identify by taskID
 * \param taskID Task reference.
 */
void __StopTask(TaskType taskID);

/**
 * \fn StatusType __JoinTask(TaskType taskID)
 * \brief Join the task<taskID)
 * \param taskID Task reference.
 */
StatusType __JoinTask(TaskType taskID);

#endif
