/**
 *
 *	\file os_isr.h
 *	\brief Xenomai Interrupt Service Routine header 
 *	\author  Matthieu Kowalewski.
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_ISR_H
#define _AUTOSAR_ISR_H
#include <linux/ioctl.h>
#include "os_types.h"
#include "os_resource.h"
#include "app/app_define.h"

/************************* STRUCTURE DEFINITION *************************/

/*
 *	\enum IsrCategory
 *      \brief Category of an ISR
 */
enum IsrCategory {
	ISR_CATEGORY_1, /*!< ISR_CATEGORY_1 Cannot use AUTOSAR functions */
	ISR_CATEGORY_2  /*!< ISR_CATEGORY_1 Can use AUTOSAR functions */
};

/*! Create a type name for IsrCategory */
typedef enum IsrCategory OsIsrCategory;

/*struct OsIsrResourceLock {
	float64 OsIsrResourceLockBudget;
	struct OsResource * OsIsrResourceLockResourceRef;
};

struct OsIsrTimingProtection {
	float64 OsIsrAllInterruptLockBugdet;
	float64 OsIsrExecutionBudget;
	float64 OsIsrInterruptLockBudget;
	float64 OsIsrTimeFrame;
	struct OsIsrResourceLock ** OsIsrResourceLock;

};*/

/*
 *	\struct OsIsr
 *	\brief Autosar structure to describe an ISR
 */
struct OsIsr {
	OsIsrCategory OsIsrCategory;            /*!<Category of the ISR*/
	struct OsResource ** OsIsrResourceRef;  /*!<Resource linked to this ISR to be locked*/
         char name[XNOBJECT_NAME_LEN];          /*!<Name of the ISR RTDM*/
       int fd;                                 /*!<File descriptor linked to the ISR  RTDM*/
        void (*handler)(void);                  /*!<Interrupt Service Routine (Only on ISR Category 2)*/
        //	struct OsIsrTimingProtection * OsIsrTimingProtection;
};




/************************* FUNCTION DEFINITION *************************/

/**
 * \fn ISRType __GetISRID(void)
 * \brief Cf ::GetISRID
 */
ISRType __GetISRID(void);

/**
 * \fn void __EnableAllInterrupts(void)
 * \brief Cf ::EnableAllInterrupts
 */
void __EnableAllInterrupts(void);

/**
 * \fn void __DisableAllInterrupts(void)
 * \brief Cf ::DisableAllInterrupts
 */
void __DisableAllInterrupts(void);

/**
 * \fn void __ResumeAllInterrupts(void)
 * \brief Cf ::ResumeAllInterrupts
 */
void __ResumeAllInterrupts(void);

/**
 * \fn void __SuspendAllInterrupts(void)
 * \brief Cf ::SuspendAllInterrupts
 */
void __SuspendAllInterrupts(void);

/**
 * \fn void __ResumeOSInterrupts(void)
 * \brief Cf ::ResumeOSInterrupts
 */
void __ResumeOSInterrupts(void);

/**
 * \fn void __SuspendOSInterrupts(void)
 * \brief Cf ::SuspendOSInterrupts
 */
void __SuspendOSInterrupts(void);

/**
 * \fn StatusType __InitIsr(ISRType __ISRID)
 * \brief Init AUTOSAR ISR<__ISRID>
 * \param ISRID Interrupt Service Routine reference.
 */
void __InitISR(ISRType __ISRID);

/**
 * \fn StatusType __StopIsr(ISRType __ISRID)
 * \brief Uninit and delete AUTOSAR ISR<__ISRID>
 * \param ISRID Interrupt Service Routine reference.
 */
void __StopIsr(ISRType __ISRID);
#endif 
