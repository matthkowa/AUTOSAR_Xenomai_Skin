#include <linux/version.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <rtdm/driver.h>

#include <rtdm/uapi/autosar.h>
#define SUCCESS 0
#define nom_device_rtdm "mesure_periode_rtdm"




static rtdm_mutex_t mtx_statistiques;
static int open_gpio (struct rtdm_fd * fd, int flags);
static void close_gpio (struct rtdm_fd * fd);
int device_ioctl(struct rtdm_fd * fd,unsigned int request,void __user * arg); 

static void SetGPIO(int GPIO, int value)
{
        int err;
        if((err = gpio_request(GPIO,THIS_MODULE->name))!=0){
                return;
        }
        if((err=gpio_direction_output(GPIO,value)) != 0 ){
                gpio_free(GPIO);
                return;
        }

        gpio_free(GPIO);
	
}

static int GetGPIO(int GPIO)
{
        int err;
        int val;
        if((err = gpio_request(GPIO,THIS_MODULE->name))!=0){
                return err;
        }
        if((err=gpio_direction_input(GPIO)) != 0 ){
                gpio_free(GPIO);
                return err;
        }
        val = gpio_get_value(GPIO);
        gpio_free(GPIO);
        return val;
	
}

static struct rtdm_driver rtdev_mesure_periode = {
	.profile_info   = RTDM_PROFILE_INFO(	gpio,
						RTDM_CLASS_TESTING,
						1,
						1),	
	.device_flags   = RTDM_NAMED_DEVICE,
	.context_size   = 0,
	.device_count   = 1,
	.ops = {
		.open       = open_gpio, 
		.close 	    = close_gpio,
		.ioctl_rt      = device_ioctl,
	},
};

static struct rtdm_device gpio_device = {	
	.driver = &rtdev_mesure_periode,
	.label = "gpio",
};  
      


static int __init init_gpio (void)
{

	rtdm_mutex_init(& mtx_statistiques);
	rtdm_dev_register(&gpio_device);
	return 0;
}

static void __exit exit_gpio(void)
{
	rtdm_dev_unregister(&gpio_device);
	rtdm_mutex_destroy(& mtx_statistiques);
}

int device_ioctl(struct rtdm_fd * fd,unsigned int request,void __user * arg)
{
	struct dataIOCTL * data = arg;
	/* 
	 * Switch according to the ioctl called 
	 */
        
	if(data != NULL && data->Pin != 0){
	
		switch (request) {
			case IOCTL_WRITE_PIN:
				rtdm_mutex_lock(& mtx_statistiques);
				SetGPIO(data->Pin, data->Value);
				rtdm_mutex_unlock(& mtx_statistiques);
			break;
                        case IOCTL_READ_PIN:
				rtdm_mutex_lock(& mtx_statistiques);
				data->Value = GetGPIO(data->Pin);
				rtdm_mutex_unlock(& mtx_statistiques);
			break;
		}
		return SUCCESS;
	}else{
		return 1;	
	}
	
}

static int open_gpio(struct rtdm_fd * fd, int flags)
{
    	return 0;
}

static void close_gpio(struct rtdm_fd * fd)
{
    
}

module_init(init_gpio);
module_exit(exit_gpio);
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
MODULE_DESCRIPTION("Led blinker driver");   
MODULE_AUTHOR("Matthieu K"); 
