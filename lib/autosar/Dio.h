#ifndef _AUTOSAR_DIO_H
#define _AUTOSAR_DIO_H
#include "autosar/Std_Types.h"
#include <autosar/os.h>


struct DIOXenomai {
        uint32 Pin;
};




Dio_LevelType __Dio_ReadChannel(Dio_ChannelType ChannelId);
void __Dio_WriteChannel(Dio_ChannelType ChannelId,Dio_LevelType Level);
Dio_PortLevelType __Dio_ReadPort(Dio_PortType PortId);
void __Dio_WritePort(Dio_PortType PortId,Dio_PortLevelType Level);
Dio_PortLevelType __Dio_ReadChannelGroup(const Dio_ChannelGroupType * ChannelGroupIdPtr);
void __Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level);
void __Dio_GetVersionInfo(Std_VersionInfoType * VersionInfo);
Dio_LevelType __Dio_FlipChannel(Dio_ChannelType ChannelId);
void __InitDio(void);
void __StopDio(void);
#endif /* _AUTOSAR_DIO_H */
