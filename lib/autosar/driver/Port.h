#ifndef PORT_H
#define PORT_H

struct Port_ConfigType {

};

typedef uint Port_PinType;

enum Port_PinDirection {
	PORT_PIN_IN, 
	PORT_PIN_OUT  
};

typedef enum Port_PinDirection Port_PinDirectionType;

typedef uint Port_PinModeType;

void Port_Init(Const Port_ConfigType * ConfigPtr);

void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction);

void Port_RefreshPortDirection(void);

void Port_GetVersionInfo( Std_VersionInfoType * versioninfo);

void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);

#endif
