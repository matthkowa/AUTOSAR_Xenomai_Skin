/**
 *
 *	\file os_alarm.c
 *	\brief Alarm implementation
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/

#include <stdint.h>
#include <stdio.h>
#include <copperplate/threadobj.h>
#include <copperplate/heapobj.h>
#include <copperplate/registry-obstack.h>
#include <copperplate/internal.h>
#include "os_alarm.h"
#include "os_task.h"
#include "os_timer.h"
#include "os_hook.h"
#include "os_counter.h"

extern struct Os os;

static DEFINE_NAME_GENERATOR(alarm_namegen, "alarm",
			     struct OsAlarmXenomai, name);

#ifdef CONFIG_XENO_REGISTRY

static int alarm_registry_open(struct fsobj *fsobj, void *priv)
{
	/*struct fsobstack *o = priv;
	struct OsAlarmXenomai *acb;
	struct itimerspec itmspec;
	unsigned long expiries;
	struct timespec delta;
	int ret;

	acb = container_of(fsobj, struct OsAlarmXenomai, fsobj);
	ret = timerobj_lock(&acb->tmobj);
	if (ret)
		return ret;
	itmspec = acb->itmspec;
	expiries = acb->expiries;
	timerobj_unlock(&acb->tmobj);

	fsobstack_init(o);

	fsobstack_grow_format(o, "%-12s%-12s%-12s\n",
			      "[EXPIRIES]", "[DISTANCE]", "[INTERVAL]");
	clockobj_get_distance(&autosar_clock, &itmspec, &delta);
	fsobstack_grow_format(o, "%8lu%10ld\"%ld%10ld\"%ld\n",
			      expiries,
			      delta.tv_sec,
			      delta.tv_nsec / 100000000,
			      itmspec.it_interval.tv_sec,
			      itmspec.it_interval.tv_nsec / 100000000);

	fsobstack_finish(o);
*/
	return 0;
}

static struct registry_operations registry_ops = {
	.open		= alarm_registry_open,
	.release	= fsobj_obstack_release,
	.read		= fsobj_obstack_read
};

#else /* !CONFIG_XENO_REGISTRY */

static struct registry_operations registry_ops;

#endif /* CONFIG_XENO_REGISTRY */

struct OsAlarm * get_alarm(AlarmType alarmID){
	struct OsAlarm * osAlarm = NULL;
	if(alarmID <= ALARM_COUNT && alarmID >= 1){
		osAlarm =  os.OsAlarm[alarmID-1];
	}	
	return osAlarm;	
}	


static int initAlarm(struct OsAlarm * osAlarm, const char *name)
{
	struct OsAlarmXenomai *alarm;	
	struct OsAlarmAutostart * osAlarmAutostart;
	struct service svc;
	int ret;
	CANCEL_DEFER(svc);
	
	
	alarm = pvmalloc(sizeof(*alarm));
	if (alarm == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	generate_name(alarm->name, name, &alarm_namegen);

	registry_init_file_obstack(&alarm->fsobj, &registry_ops);
	ret = __bt(registry_add_file(&alarm->fsobj, O_RDONLY,
				     "/autosar/alarms/%s", alarm->name));
	if (ret)
		warning("failed to export alarm %s to registry, %s",
			alarm->name, symerror(ret));
	
	if (pvcluster_addobj(&xenomai_autosar_alarm_table, alarm->name, &alarm->cobj)) {
		registry_destroy_file(&alarm->fsobj);
		ret = -EEXIST;
		goto fail;
	}
	osAlarmAutostart = osAlarm->OsAlarmAutostart;
	if( osAlarmAutostart != NULL){
		alarm->value = osAlarmAutostart->OsAlarmAlarmTime;
		alarm->cycle = osAlarmAutostart->OsAlarmCycleTime;
		alarm->next = alarm->value;
	}else{
		alarm->value = 0;
		alarm->cycle = 0;
		alarm->next = 0;
	}
	alarm->isActive = FALSE;	
	osAlarm->OsAlarmXenomai = alarm;

	CANCEL_RESTORE(svc);

	return 0;
fail:
	pvfree(alarm);
out:
	CANCEL_RESTORE(svc);

	return ret;
}




StatusType __CreateAlarm(AlarmType alarmID){
	struct OsAlarm * osAlarm;
	char name[XNOBJECT_NAME_LEN];
	StatusType ret = E_OK;
	osAlarm = get_alarm(alarmID);
	if(osAlarm == NULL)
		goto out;
	//TODO Change method to generate Name	
	sprintf(name,"Alarm %u",alarmID);
	ret = initAlarm(osAlarm,name);
	if(ret){
		warning("Failed to create alarm %lu",alarmID);
		goto out;
	}
	
out:
	return ret;
}

StatusType __GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info){
	struct OsAlarm * osAlarm;
	struct OsCounter * OsAlarmCounterRef;
	StatusType ret = E_OK;
	osAlarm = get_alarm(AlarmID);
	if(osAlarm == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	OsAlarmCounterRef = osAlarm->OsAlarmCounterRef;
	if(OsAlarmCounterRef == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	Info->maxallowedvalue = OsAlarmCounterRef->OsCounterMaxAllowedValue;
	Info->mincycle = OsAlarmCounterRef->OsCounterMinCycle;
	Info->ticksperbase = OsAlarmCounterRef->OsCounterTicksPerBase;
out:
#if OS_ERROR_HOOK
	if(ret){
		__SetAlarmID(AlarmID);
		__SetInfo(Info);
		__CheckError(ret,OSServiceId_GetAlarmBase);
	}
#endif
	return ret;
}


StatusType __GetAlarm(AlarmType AlarmID, TickRefType Tick){
	struct OsAlarm * osAlarm;
	struct OsAlarmXenomai * alarm;
	struct OsCounter * osCounter;	
	StatusType ret = E_OK;
	TickType now,next,maxAllowedValue,diff;
	osAlarm = get_alarm(AlarmID);
	if(osAlarm == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	alarm = osAlarm->OsAlarmXenomai;
	if(alarm == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	if(alarm->isActive == FALSE || alarm->next == -1){
		ret = E_OS_NOFUNC;
		goto out;
	}
	osCounter = osAlarm->OsAlarmCounterRef;
	maxAllowedValue = osCounter->OsCounterMaxAllowedValue;
	next = alarm->next;
	now = osCounter->OsCounterXenomai->currentValue;
	if(next >= now)
		diff = (next-now);
	else
		diff = next + maxAllowedValue - now;
	*Tick = diff;
out :
#if OS_ERROR_HOOK
	if(ret){
		__SetAlarmID(AlarmID);
		__SetTick(Tick);
		__CheckError(ret,OSServiceId_GetAlarm);
	}
#endif
	return ret;
}

StatusType __SetRelAlarm(AlarmType AlarmID, TickType increment, TickType cycle){
	struct OsCounter * osAlarmCounterRef;
	struct OsAlarm * osAlarm;
	struct OsAlarmXenomai * alarm;
	TickType maxAllowedValue,mincycle,now;
	StatusType ret = E_OK;
	osAlarm = get_alarm(AlarmID);
	if(osAlarm == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	alarm = osAlarm->OsAlarmXenomai;
	if(alarm == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	
	if(alarm->isActive == TRUE && alarm->next != -1){
		ret = E_OS_STATE;
		goto out;
	}
	osAlarmCounterRef = osAlarm->OsAlarmCounterRef;
	maxAllowedValue = osAlarmCounterRef->OsCounterMaxAllowedValue;

	if(increment < 0 || increment > maxAllowedValue){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_VALUE;
#endif
		goto out;
	}
	
	
	mincycle = osAlarmCounterRef->OsCounterMinCycle;
	if(cycle < mincycle || cycle > maxAllowedValue){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_VALUE;
#endif
		goto out;
	}
	now = osAlarmCounterRef->OsCounterXenomai->currentValue;
	alarm->cycle = cycle;
	alarm->value = now+increment;
	alarm->next = now + increment;
	alarm->isActive = TRUE;	
out:
#if OS_ERROR_HOOK
	if(ret){
		__SetAlarmID(AlarmID);
		__Setincrement(increment);
		__Setcycle(cycle);
		__CheckError(ret,OSServiceId_SetRelAlarm);
	}
#endif
	return ret;
}

StatusType __SetAbsAlarm(AlarmType AlarmID, TickType start, TickType cycle){
	struct OsCounter * osAlarmCounterRef;
	struct OsAlarm * osAlarm;
	struct OsAlarmXenomai * alarm;
	TickType maxAllowedValue,mincycle;
	StatusType ret = E_OK;
	osAlarm = get_alarm(AlarmID);
	if(osAlarm == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	alarm = osAlarm->OsAlarmXenomai;
	if(alarm == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	
	if(alarm->isActive == TRUE && alarm->next != -1){
		ret = E_OS_STATE;
		goto out;
	}
	
	
	osAlarmCounterRef = osAlarm->OsAlarmCounterRef;
	maxAllowedValue = osAlarmCounterRef->OsCounterMaxAllowedValue;

	if(start <= 0 || start > maxAllowedValue){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_VALUE;
#endif
		goto out;
	}

	
	mincycle = osAlarmCounterRef->OsCounterMinCycle;
	if(cycle < mincycle || cycle > maxAllowedValue){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_VALUE;
#endif
		goto out;
	}

	alarm->value = start;
	alarm->cycle = cycle;
	alarm->next = start;
	alarm->isActive = TRUE;
	

out:
#if OS_ERROR_HOOK
	if(ret){
		__SetAlarmID(AlarmID);
		__Setstart(start);
		__Setcycle(cycle);
		__CheckError(ret,OSServiceId_SetAbsAlarm);
	}
#endif
	return ret;
}

StatusType __CancelAlarm(AlarmType AlarmID){
	struct OsAlarm * osAlarm;	
	struct OsAlarmXenomai * alarm;
	StatusType ret = E_OK;
	osAlarm = get_alarm(AlarmID);
	if(osAlarm == NULL){		
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	alarm = osAlarm->OsAlarmXenomai;
	if(alarm == NULL){
#if OS_STATUS == OSOS_EXTENDED
		ret = E_OS_ID;
#endif
		goto out;
	}
	
	
	if(alarm->isActive == FALSE || alarm->next == 0){
		ret = E_OS_NOFUNC;
		goto out;
	}
	alarm->isActive = FALSE;
	alarm->next = -1;
	
out:
#if OS_ERROR_HOOK
	if(ret){
		__SetAlarmID(AlarmID);
		__CheckError(ret,OSServiceId_CancelAlarm);
	}
#endif
	return ret;
}

void __StopAlarm(AlarmType AlarmID){
	struct OsAlarmXenomai *alarm;
	struct OsAlarm * osAlarm;
	osAlarm = get_alarm(AlarmID);
	if(osAlarm == NULL){
		return;
	}
	alarm = osAlarm->OsAlarmXenomai;
	if(alarm == NULL)
		return;

	registry_destroy_file(&alarm->fsobj);
	pvcluster_delobj(&xenomai_autosar_alarm_table, &alarm->cobj);
	pvfree(alarm);
}



