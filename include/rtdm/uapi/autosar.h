/*
 *  chardev.h - the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the process calling ioctl (ioctl.c)
 */

#ifndef AUTOSAR_UAPI_GPIO_H
#define AUTOSAR_UAPI_GPIO_H


/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. 
 */
struct dataIOCTL {
	unsigned int Pin;
	unsigned int Value;
};

#define MAJOR_NUM 100

/* 
 * Set the message of the device driver 
 */
#define IOCTL_WRITE_PIN _IOWR(MAJOR_NUM, 0, struct dataIOCTL *)
#define IOCTL_READ_PIN _IOWR(MAJOR_NUM, 1, struct dataIOCTL *)
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

/* 
 * Get the message of the device driver 
 */



#endif
