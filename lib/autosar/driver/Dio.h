#ifndef DIO_H
#define DIO_H

#include <autosar/os.h>


struct DioGeneral {
	EcucBooleanParamDef DioDevErrorDetect;
	EcucBooleanParamDef DioFlipChannelApi;
	EcucBooleanParamDef DioVersionInfoApi;
};



struct DioChannelGroupe {
	EcucStringParamDef DioChannelGroupIdentification;
	uint64 DioPortMask;
	uint8 DioPortOffset;
};

struct DioChannel {
	uint64 DioChannelId;	
};

struct DioPort {
	uint64 DioPortId;
	struct DioChannel ** DioChannel;
	struct DioChannelGroup ** DioChannelGroup;
};

struct DioConfig {
	struct DioPort ** DioPort;
};

struct Dio {
	struct DioConfig DioConfig;
	struct DioGeneral DioGeneral;
};



typedef uint8 Dio_LevelType;

typedef uint Dio_PortLevelType;

struct Dio_ConfigType {
	/* EMPTY */
};


Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);

void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);

Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);

void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);

Dio_PortLevelType Dio_ReadChannelGroup( const Dio_ChannelGroupeType * ChannelGroupIdPtr);

void Dio_WriteChannelGroup( const Dio_ChannelGroupType * ChannelGroupIdPtr, Dio_PortLevelType Level);

void Dio_GetVersionInfo( Std_VersionInfoType * VersionInfo);

Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);




#endif
