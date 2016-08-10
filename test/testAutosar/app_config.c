#include "task.h"
#include "alarm.h"
#include "counter.h"
#include "app_config.h"

struct AUTOSAR_CCB counter1_ccb;

/*********************** Task t1 ***********************/ 

int task_app_mode[TASKCOUNT+1] = {
	0,  /* t1 */
	0   /* t2 */	
};

/* Task t1 identifier */
#define t1_id 0

extern void t1_function(void);

const TaskType t1 = t1_id;

struct AUTOSAR_TCB t1_tcb = { 
	80,
	t1_function,
	0,
	NULL,
	SUSPENDED,
	0
};

/* Task t2 identifier */
#define t2_id 1

extern void t2_function(void);

const TaskType t2 = t2_id;

struct AUTOSAR_TCB t2_tcb = { 
	80,		// Priority
	t2_function,	// Task function
	0,		// mode -> useless
	NULL,		// task pointer
	SUSPENDED, 	// State
	0 		// Number of activation
};


/* Task Table */

struct AUTOSAR_TCB * autosar_task_table[TASKCOUNT+1] = {
	&t1_tcb,
	&t2_tcb
};


/*********************** Alarm a1 **********************/

#define a1_id 0
const AlarmType a1 = a1_id;

struct AUTOSAR_ACB a1_tcb = {
	2000ULL, // value
	10ULL, // cycle
	0ULL, // next wakeup
	NO, // isActive
	alarm_handler_activate_task, // handler
	t1_id, // elementID
	NULL, // 
	&counter1_ccb // Counter
};

/*********************** Alarm a2 **********************/

#define a2_id 1
const AlarmType a2 = a2_id;



struct AUTOSAR_ACB a2_tcb = {
	200ULL,
	10ULL,
	0ULL, // next wakeup
	NO, // isActive 
	alarm_handler_activate_task,
	t2_id,
	NULL,
	&counter1_ccb
};

struct AUTOSAR_ACB * autosar_alarm_table[ALARMCOUNT] = {
	&a1_tcb,
	&a2_tcb
};



int alarm_app_mode[ALARMCOUNT] = {
	1,  /* alarm_1 */
	1   /* alarm_2 */
};



/*********************** Counter ***********************/
#define counter1_id 1
const AlarmType counter1 = counter1_id;

AlarmType counter1_activate_alarm[COUNTER1_ALARMS] = {
	a1_id,
	a2_id
};

struct autosar_counter_action counter1_action = {
	0,
	COUNTER1_ALARMS,
	NULL,
	counter1_activate_alarm
};

struct AlarmBaseType counter1_base = {
	32767U,
	2U,
	1U

};

struct AUTOSAR_CCB counter1_ccb = {
	&counter1_base,
	0U,
	0U,
	&counter1_action,
	NULL
};



#define system_counter_id 0
const AlarmType system_counter = system_counter_id;

AlarmType system_counter_activate_counter[COUNTERCOUNT] = {
	counter1_id
};

struct autosar_counter_action system_counter_action = {
	COUNTERCOUNT,
	0,
	system_counter_activate_counter,
	NULL
};

struct AlarmBaseType system_counter_base = {
	32767U,
	1U,
	1U
};

struct AUTOSAR_CCB system_counter_ccb = {
	&system_counter_base,
	0U,
	0U,
	&system_counter_action,
	NULL
};



struct AUTOSAR_CCB * autosar_counter_table[COUNTERCOUNT+1] = {
	&system_counter_ccb,
	&counter1_ccb
};



