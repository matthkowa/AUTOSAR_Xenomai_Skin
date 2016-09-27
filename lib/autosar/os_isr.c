/**
 *
 *	\file  os_isr.c
 *	\brief Interrupt Service Routine implementation
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/

#include "os_isr.h"
#include "app/app_config.h"
#include "app/isr.h"
#include "app/app_isr.h"
#include <copperplate/internal.h>
#include <rtdm/rtdm.h>
ISRType __GetISRID(){

	return 0;
}
/*
static struct OsIsr * __getISR(ISRType __ISRID){
        struct OsIsr * __osIsr = NULL;
	if(__ISRID <= ISR_COUNT_MANUEL && __ISRID >= 1){	
		__osIsr =  os.OsIsr[__ISRID-1];	
	}	
	return __osIsr;
} 

void __InitISR(ISRType __ISRID){
        int ret;
        warning("Init ISR");
        struct OsIsr * __osIsr = __getISR(__ISRID);
        if(__osIsr == NULL)
                goto out;
        __osIsr->fd = open(__osIsr->name,O_RDONLY);
        if(__osIsr->fd < 0){
                warning("Stderr, fd %s",__osIsr->fd);
                return;
        }
        warning("IOCTL ISR");
        if((ret = ioctl(__osIsr->fd,IOCTL_INIT_ISR,__osIsr->handler))<0){
                warning("IOCTL ret : %d",ret);
        }
        warning("IOCTL ISR");
        if((ret = ioctl(__osIsr->fd, IOCTL_ENABLE_INTERRUPT, NULL))<0){
                warning("IOCTL ret : %d",ret);
        }
out :
        return;
}


void __StopIsr(ISRType __ISRID){
        struct OsIsr * __osIsr = __getISR(__ISRID);
        if(__osIsr == NULL)
                goto out;
        ioctl(__osIsr->fd, IOCTL_DISABLE_INTERRUPT, NULL);
        ioctl(__osIsr->fd,IOCTL_STOP_ISR,__osIsr->handler);
        close(__osIsr->fd);
out :
        return;
}

void __EnableAllInterrupts(){
        int i;        
        for(i = 1;i<ISR_COUNT_MANUEL;i++){
                struct OsIsr * __osIsr = __getISR(i);
                if(__osIsr == NULL)
                        break;
                ioctl(__osIsr->fd, IOCTL_ENABLE_INTERRUPT, NULL);
        }
}

void __DisableAllInterrupts(){
	int i;        
        for(i = 1;i<ISR_COUNT_MANUEL;i++){
                struct OsIsr * __osIsr = __getISR(i);
                if(__osIsr == NULL)
                        break;
                ioctl(__osIsr->fd, IOCTL_DISABLE_INTERRUPT, NULL);
        }
}
void __ResumeAllInterrupts(){ 
	int i;        
        for(i = 1;i<ISR_COUNT_MANUEL;i++){
                struct OsIsr * __osIsr = __getISR(i);
                if(__osIsr == NULL)
                        break;
                ioctl(__osIsr->fd, IOCTL_RESUME_INTERRUPT, NULL);
        }
}
void __SuspendAllInterrupts(){
	int i;        
        for(i = 1;i<ISR_COUNT_MANUEL;i++){
                struct OsIsr * __osIsr = __getISR(i);
                if(__osIsr == NULL)
                        break;
                ioctl(__osIsr->fd, IOCTL_SUSPEND_INTERRUPT, NULL);
        }
}
void __ResumeOSInterrupts(){
	int i;        
        for(i = 1;i<ISR_COUNT_MANUEL;i++){
                struct OsIsr * __osIsr = __getISR(i);
                if(__osIsr == NULL)
                        break;
                ioctl(__osIsr->fd, IOCTL_RESUME_OS_INTERRUPT, NULL);
        }
}
void __SuspendOSInterrupts(){
	int i;        
        for(i = 1;i<ISR_COUNT_MANUEL;i++){
                struct OsIsr * __osIsr = __getISR(i);
                if(__osIsr == NULL)
                        break;
                ioctl(__osIsr->fd, IOCTL_SUSPEND_OS_INTERRUPT, NULL);
        }
}*/
