#ifndef APP_CONFIG_H
#define APP_CONFIG_H
#include "app_define.h"

#define COUNTER1_ALARMS			2


int task_app_mode[TASKCOUNT+1];
struct AUTOSAR_TCB * autosar_task_table[TASKCOUNT+1];

int alarm_app_mode[ALARMCOUNT];
struct AUTOSAR_ACB * autosar_alarm_table[ALARMCOUNT];

struct AUTOSAR_CCB * autosar_counter_table[COUNTERCOUNT+1];

const TaskType t1;
const TaskType t2;



#endif
