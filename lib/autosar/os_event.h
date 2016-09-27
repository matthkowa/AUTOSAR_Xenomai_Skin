/**
 *
 *	\file os_event.h
 *	\brief Event header
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_EVENT_H
#define _AUTOSAR_EVENT_H
#include "os_types.h"

/************************* STRUCTURE DEFINITION *************************/

/*
 *	\struct OsEvent
 *	\brief AUTOSAR structure to describe an event
 */
struct OsEvent {
	uint64 OsEventMask;
};

/************************* FUNCTION DEFINITION *************************/

/**
 * \fn StatusType __SetEvent(TaskType TaskID, EventMaskType Mask)
 * \brief Cf ::SetEvent
 */
StatusType __SetEvent(TaskType TaskID, EventMaskType Mask);

/**
 * \fn StatusType __ClearEvent(EventMaskType Mask)
 * \brief Cf ::ClearEvent
 */
StatusType __ClearEvent(EventMaskType Mask);

/**
 * \fn StatusType __GetEvent(TaskType TaskID,EventMaskRefType Event)
 * \brief Cf ::GetEvent
 */
StatusType __GetEvent(TaskType TaskID,EventMaskRefType Event);

/**
 * \fn StatusType __WaitEvent(EventMaskType Mask)
 * \brief Cf ::WaitEvent
 */
StatusType __WaitEvent(EventMaskType Mask);


#endif
