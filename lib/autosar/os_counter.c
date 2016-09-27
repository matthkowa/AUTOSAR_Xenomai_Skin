/**
 *
 *	\file os_counter.c
 *	\brief Counter implementation
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/

#include "os_counter.h"
#include "os_timer.h"
#include "os_alarm.h"
#include "os_task.h"
#include <copperplate/internal.h>

extern struct Os os;
//TODO Check angular counter != clock

static void __incrementCounter(struct OsCounter * osCounter);

static struct OsCounter * get_counter(CounterType counterID){
	
	if(counterID <= COUNTER_COUNT && counterID >= 1){
		return os.OsCounter[counterID-1];
	}	
	return NULL;
}

static struct OsAlarm * get_alarm_link_to_counter(CounterType counterID,AlarmType alarmID){
	struct OsAlarm * osAlarm = NULL;	
	if(counterID > COUNTER_COUNT || counterID < 1)
		return NULL;
	if(alarmID >= ALARM_COUNT ||  alarmID < 0)
		return NULL;
	osAlarm = os.OsAlarm[alarmID];
	if(osAlarm->OsAlarmCounterRef != NULL && osAlarm->OsAlarmCounterRef->OsCounterXenomai != NULL && osAlarm->OsAlarmCounterRef->OsCounterXenomai->osCounterID == counterID)
		return osAlarm;
	else
		return NULL;	
}

static void activateTask(struct OsTask * osTask){
	__ActivateOsTask(osTask);
}

/*static void activateEvent(struct OsAlarmSetEvent * osAlarmSetEvent){
	warning("Activate Event");


}*/

static void updateAlarms(CounterType counterID,TickType value,TickType maxAllowedValue){

	TickType next;
	struct OsAlarm * osAlarm;
	struct OsAlarmXenomai * alarm;
	struct OsAlarmAction * osAlarmAction;
	int i;
	
	for(i=0;i<ALARM_COUNT;i++){ 
		
		osAlarm = get_alarm_link_to_counter(counterID,i);
		if(osAlarm == NULL){
			warning("Alarm not linked");
			continue;
		}
		alarm = osAlarm->OsAlarmXenomai;
		// Check if the alarm has been activated 
		if(alarm != NULL && alarm->isActive){ 
			next = alarm->next;
			// Check if the next expiries is now
			
			if(next  == value){
				osAlarmAction = osAlarm->OsAlarmAction;
				if(osAlarmAction->OsAlarmActivateTask != NULL)
					activateTask(osAlarmAction->OsAlarmActivateTask->OsAlarmActivateTaskRef);
				if(osAlarmAction->OsAlarmIncrementCounter != NULL)
					__incrementCounter(osAlarmAction->OsAlarmIncrementCounter->OsAlarmIncrementCounterRef);
/*				if(osAlarmAction->OsAlarmSetEvent != NULL)
					activateEvent(osAlarmAction->OsAlarmSetEvent);
*/
#if OS_SCALABILITY_CLASS == OSOS_SC1 //SWS_Os_00242
				if(osAlarmAction->OsAlarmCallback != NULL)
					(osAlarmAction->OsAlarmCallback)->OsAlarmCallback();
#endif
				if(alarm->cycle != 0){// Cycle != 0			
					alarm->next = next + alarm->cycle;
					if(alarm->next >maxAllowedValue)
						alarm->next -= (maxAllowedValue+1);
				}else{	       // Disable the alarm if Cycle = 0
					alarm->next = -1;
					alarm->isActive = FALSE;
				}
			}
		}
	}	


}

static void __incrementCounter(struct OsCounter * osCounter){
	
	struct OsCounterXenomai * counter = osCounter->OsCounterXenomai;
	TickType value,tick,maxAllowedValue,ticksPerBase;

	AlarmType counterID = counter->osCounterID;
	ticksPerBase = osCounter->OsCounterTicksPerBase;
	maxAllowedValue = osCounter->OsCounterMaxAllowedValue;
	value = counter->currentValue;
	tick = counter->currentTick;
	tick++;
	if(tick == ticksPerBase){
		value++;
		tick = 0;
		if(value > maxAllowedValue)
			value = 0;
		updateAlarms(counterID,value,maxAllowedValue);		
	}
	counter->currentTick = tick;
	counter->currentValue = value;	
}



static void counter_handler(struct timerobj *tmobj)
{
	struct OsCounterXenomai *counter;
	struct OsCounter * osCounter;
	counter = container_of(tmobj, struct OsCounterXenomai, tmobj);
	osCounter = get_counter(counter->osCounterID);
	if(osCounter == NULL){
		return;
	}
	__incrementCounter(osCounter);
}


StatusType __CreateCounter(CounterType counterID){
	StatusType ret = E_OK;
	struct OsCounter * osCounter;
	struct OsCounterXenomai * counter;
	int timer_ret;
	osCounter = get_counter(counterID);

	if(osCounter == NULL){
		ret = E_OS_ID;
		goto out;
	}
		

	counter = pvmalloc(sizeof(*counter));	
	if (counter == NULL) {
		ret = E_OS_ID;
		goto out;
	}
	counter->currentValue = 0;
	counter->currentTick = 0;
	if(osCounter->OsCounterType == COUNTER_HARDWARE){
		timer_ret = timerobj_init(&counter->tmobj);
	if (timer_ret){
		pvfree(counter);
		ret = E_OS_ID;
		goto out;
	}
	clockobj_ticks_to_timeout(&autosar_clock,0, &(counter->itmspec.it_value));

	clockobj_ticks_to_timespec(&autosar_clock, osCounter->OsCounterTicksPerBase, &(counter->itmspec.it_interval));
	counter->osCounterID = counterID;
	osCounter->OsCounterXenomai = counter;
	timer_ret = timerobj_start(&counter->tmobj, counter_handler , &counter->itmspec);
	if (timer_ret){
		pvfree(counter);
		ret = E_OS_ID;
		goto out;
	}
}
	
out :
	return ret;
}

StatusType __IncrementCounter(CounterType counterID){
	StatusType ret = E_OK;
	struct OsCounter * osCounter;
	osCounter = get_counter(counterID);
	if(osCounter == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	__incrementCounter(osCounter);
out : 
	return ret;
}

StatusType __GetCounterValue(CounterType CounterID,TickRefType Value){
	StatusType ret = E_OK;
	struct OsCounter * osCounter;
	osCounter = get_counter(CounterID);
	if(osCounter == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	*Value = osCounter->OsCounterXenomai->currentValue;
out : 
	return ret;

}

StatusType __GetElapsedValue(CounterType CounterID,TickRefType Value,TickRefType ElapsedValue){
	StatusType ret = E_OK;
	struct OsCounter * osCounter;
	struct OsCounterXenomai * counter;
	TickType val,currentValue,diff;
	osCounter = get_counter(CounterID);
	if(osCounter == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	counter = osCounter->OsCounterXenomai;
	if(counter == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	val = *Value;
	currentValue = counter->currentValue;
	if(val > osCounter->OsCounterMaxAllowedValue || val < 0){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_VALUE;
#endif
		goto out;	
	}
	if(val > currentValue)
		diff = currentValue + osCounter->OsCounterMaxAllowedValue - val;
	else
		diff = currentValue - val;
	*Value = currentValue;
	*ElapsedValue = diff;	
out : 
	return ret;

}


void __StopCounter(void){
	int i;
	struct OsCounter * osCounter;
	struct OsCounterXenomai * counter ;
	for(i = 0;i<COUNTER_COUNT;i++){
		osCounter = get_counter(i);
		if(osCounter == NULL)
			continue;
		counter = osCounter->OsCounterXenomai;
		if (counter == NULL) 
			continue;

		timerobj_stop(&counter->tmobj);	
		timerobj_destroy(&counter->tmobj);
		pvfree(counter);
	}
}




