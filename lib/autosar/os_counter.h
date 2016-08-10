/**
 *
 *	\file os_counter.h
 *	\brief Counter header
 *	\author  Matthieu K.
 *	\version 0.1
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_COUNTER_H
#define _AUTOSAR_COUNTER_H
#include <autosar/os.h>
#include <copperplate/timerobj.h>

#include "app/app_config.h"
#include "app/app_define.h"
#include "os_types.h"

/*
 *	\enum CounterType
 *	\brief Type of the Counter
 */
enum CounterType {
	COUNTER_HARDWARE,				/*!< Counter increment by Clock */
	COUNTER_SOFTWARE				/*!< Counter increment by software implementation */
};
/*! Create a type name for CounterType */
typedef enum CounterType OsCounterType;
/*
 *	\struct OsCounter
 *	\brief Structure to describe a counter
 */
struct OsCounter{
	uint64 OsCounterMaxAllowedValue;		/*!< Max allowed value for a Counter */
	uint64 OsCounterMinCycle;			/*!< Cycle minimun allowed for an alarm */
	uint32 OsCounterTicksPerBase;			/*!< Ticks per base of the counter */
	OsCounterType OsCounterType;			/*!< OsCounterType */
	float64 OsSecondsPerTick;			/*!< Time of one counter tick in seconds. */
//	struct OsApplication * OsCounterAccessingApplication;
//	struct OsDriver * OsDriver;
//	struct OsTimerConstant * OsTimerConstant;
	struct OsCounterXenomai * OsCounterXenomai;	/*!< OsCounterXenomai */
};

/*
 *	\struct OsCounterXenomai
 *	\brief Xenomai implementation for a counter 
 */
struct OsCounterXenomai {
	struct itimerspec itmspec;			/*!< Timer specification in Xenomai kernel for Software counter */
	struct timerobj tmobj;				/*!< Xenomai Timer object for Software counter */
	AlarmType osCounterID;				/*!< Identifiant of the OsCounter */
	TickType currentTick;				/*!< Current tick of the counter */
	TickType currentValue;				/*!< Current value of the counter */
};


StatusType __CreateCounter(CounterType counterID);
StatusType __IncrementCounter(CounterType counterID);
StatusType __GetCounterValue(CounterType CounterID,TickRefType Value);
StatusType __GetElapsedValue(CounterType CounterID,TickRefType Value,TickRefType ElapsedValue);
void __StopCounter(void);
#endif
