/*
 *  chardev.h - the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the process calling ioctl (ioctl.c)
 */

#ifndef RTLEDBLINK_H
#define RTLEDBLINK_H

#include <linux/ioctl.h>

/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. 
 */
#define MAJOR_NUM 100

/* 
 * Set the message of the device driver 
 */
#define IOCTL_ENABLE_INTERRUPT _IOR(MAJOR_NUM, 0, char *)
#define IOCTL_DISABLE_INTERRUPT _IOR(MAJOR_NUM, 1, char *)
#define IOCTL_RESUME_INTERRUPT _IOR(MAJOR_NUM, 2, char *)
#define IOCTL_SUSPEND_INTERRUPT _IOR(MAJOR_NUM, 3, char *)
#define IOCTL_RESUME_OS_INTERRUPT _IOR(MAJOR_NUM, 4, char *)
#define IOCTL_SUSPEND_OS_INTERRUPT _IOR(MAJOR_NUM, 5, char *)
#define IOCTL_INIT_ISR _IOR(MAJOR_NUM, 6, char *)
#define IOCTL_STOP_ISR _IOR(MAJOR_NUM, 7, char *)
/*
 * _IOR means that we're creating an ioctl command 
 * number for passing information from a user process
 * to the kernel module. 
 *
 * The first arguments, MAJOR_NUM, is the major device 
 * number we're using.
 *
 * The second argument is the number of the command 
 * (there could be several with different meanings).
 *
 * The third argument is the type we want to get from 
 * the process to the kernel.
 */



#endif
