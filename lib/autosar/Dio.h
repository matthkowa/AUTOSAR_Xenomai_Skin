/**
 *
 *	\file Dio.h
 *	\brief Autosar Digital Input Output header 
 *	\author  Matthieu Kowalewski
 *	\version 1.0
 *	\date 29 Juin 2016
*/
#ifndef _AUTOSAR_DIO_H
#define _AUTOSAR_DIO_H
#include "autosar/Std_Types.h"
#include <autosar/os.h>

/************************* STRUCTURE DEFINITION *************************/
/*
 *	\struct DIOXenomai
 *	\brief Inner structure to describe an Digital Input Output in Xenomai
 */
struct DIOXenomai {
        uint32 Pin;
};

/************************* FUNCTION DEFINITION *************************/

/**
 * \fn Dio_LevelType __Dio_ReadChannel(Dio_ChannelType ChannelId)
 * \brief Cf ::Dio_ReadChannel
 */
Dio_LevelType __Dio_ReadChannel(Dio_ChannelType ChannelId);

/**
 * \fn void __Dio_WriteChannel(Dio_ChannelType ChannelId,Dio_LevelType Level)
 * \brief Cf ::Dio_WriteChannel
 */
void __Dio_WriteChannel(Dio_ChannelType ChannelId,Dio_LevelType Level);

/**
 * \fn Dio_PortLevelType __Dio_ReadPort(Dio_PortType PortId)
 * \brief Cf ::Dio_ReadPort
 */
Dio_PortLevelType __Dio_ReadPort(Dio_PortType PortId);

/**
 * \fn void __Dio_WritePort(Dio_PortType PortId,Dio_PortLevelType Level)
 * \brief Cf ::Dio_WritePort
 */
void __Dio_WritePort(Dio_PortType PortId,Dio_PortLevelType Level);

/**
 * \fn Dio_PortLevelType __Dio_ReadChannelGroup(const Dio_ChannelGroupType * ChannelGroupIdPtr)
 * \brief Cf ::__Dio_ReadChannelGroup
 */
Dio_PortLevelType __Dio_ReadChannelGroup(const Dio_ChannelGroupType * ChannelGroupIdPtr);

/**
 * \fn void __Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level)
 * \brief Cf ::Dio_WriteChannelGroup
 */
void __Dio_WriteChannelGroup(const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level);

/**
 * \fn void __Dio_GetVersionInfo(Std_VersionInfoType * VersionInfo)
 * \brief Cf ::Dio_GetVersionInfo
 */
void __Dio_GetVersionInfo(Std_VersionInfoType * VersionInfo);

/**
 * \fn Dio_LevelType __Dio_FlipChannel(Dio_ChannelType ChannelId);
 * \brief Cf ::Dio_FlipChannel
 */
Dio_LevelType __Dio_FlipChannel(Dio_ChannelType ChannelId);

/**
 * \fn void __InitDio(void);
 * \brief Open Real Time Driver Module (RTDM) DIO
 */
void __InitDio(void);

/**
 * \fn void __StopDio(void);
 * \brief Close Real Time Driver Module (RTDM) DIO
 */
void __StopDio(void);
#endif /* _AUTOSAR_DIO_H */
