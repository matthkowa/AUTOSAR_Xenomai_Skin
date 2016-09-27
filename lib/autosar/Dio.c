/**
 *
 *	\file Dio.c
 *	\brief Digital Input Output implementation 
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#include "Dio.h"
#include <copperplate/internal.h>
#include <rtdm/rtdm.h>
#include <linux/ioctl.h>
#include <rtdm/uapi/autosar.h>
#include "HW/Config.h"
struct dataIOCTL * __data ;
int __fd;

static struct DIOXenomai * __get_Dio_Channel(Dio_ChannelType __ChannelId){
        struct DIOXenomai * __DioXenomai = NULL;
        if(__ChannelId >=  1 && __ChannelId <= DIO_COUNT){
                __DioXenomai = DIO_table[__ChannelId];
        }
        return __DioXenomai;
}

Dio_LevelType __Dio_ReadChannel(Dio_ChannelType __ChannelId){
        struct DIOXenomai * __DioXenomai = __get_Dio_Channel(__ChannelId);
        if(__DioXenomai == NULL)
                return 2;
        __data->Pin = __DioXenomai->Pin;
        __RT(ioctl(__fd, IOCTL_READ_PIN, (void *)__data));      
        return __data->Value;
}


void __Dio_WriteChannel(Dio_ChannelType __ChannelId,Dio_LevelType __Level){
        struct DIOXenomai * __DioXenomai = __get_Dio_Channel(__ChannelId);
        if(__DioXenomai == NULL)
                return ;        

         __data->Pin = __DioXenomai->Pin;
        __data->Value = __Level;
        
        __RT(ioctl(__fd, IOCTL_WRITE_PIN, (void *)__data));      
}

Dio_PortLevelType __Dio_ReadPort(Dio_PortType PortId){
        return 0;
}
void __Dio_WritePort(Dio_PortType PortId,Dio_PortLevelType Level){

}
Dio_PortLevelType __Dio_ReadChannelGroup(const Dio_ChannelGroupType * ChannelGroupIdPtr){
        return 0;
}
void __Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level){

}
void __Dio_GetVersionInfo(Std_VersionInfoType * VersionInfo){

}
Dio_LevelType __Dio_FlipChannel(Dio_ChannelType __ChannelId){
        Dio_LevelType __value = __Dio_ReadChannel(__ChannelId);
        __Dio_WriteChannel(__ChannelId,!__value);
        return !__value;
}


void __InitDio(void){
        __fd = __RT(open("/dev/rtdm/gpio",O_RDONLY));
        if(__fd < 0){
                warning("Stderr, fd %s",__fd);
                return;
        }
        __data = malloc(sizeof(struct dataIOCTL));
}

void __StopDio(void){
        free(__data);
        __RT(close(__fd));
}
