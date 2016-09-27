/**
 *
 *	\file os_timer.h
 *	\brief Xenomai timer header 
 *	\author  Matthieu Kowalewski.
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_TIMER_H
#define _AUTOSAR_TIMER_H

#include <copperplate/clockobj.h>
#include <autosar/os.h>
#include "os_types.h"

#define TM_INFINITE  0
#define TM_NOW       0
#define TM_NONBLOCK  (-1ULL)

#define autosar_abs_timeout(__t, __ts)					\
	({								\
		(__t) == TM_INFINITE ? NULL :				\
		(__t) == TM_NONBLOCK ?					\
		({ (__ts)->tv_sec = (__ts)->tv_nsec = 0; (__ts); }) :	\
		({ clockobj_ticks_to_timespec(&autosar_clock, (__t), (__ts)); \
			(__ts); });					\
	})

#define autosar_rel_timeout(__t, __ts)					\
	({								\
		(__t) == TM_INFINITE ? NULL :				\
		(__t) == TM_NONBLOCK ?					\
		({ (__ts)->tv_sec = (__ts)->tv_nsec = 0; (__ts); }) :	\
		({ clockobj_ticks_to_timeout(&autosar_clock, (__t), (__ts)); \
			(__ts); });					\
	})


extern struct clockobj autosar_clock;
struct OsTimeConstant {
	float64 OsTimeValue;
};




#endif /* _AUTOSAR_TIMER_H */
