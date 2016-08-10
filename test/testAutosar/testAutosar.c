#include <autosar/os.h>
#include <stdio.h>
long int i = 0;
/*
void ErrorHook(StatusType Error){
	printf("Error %u at service %u\n",Error,OSErrorGetServiceId());
	
	printf("Param 1 : %u\n",OSError_ActivateTask_TaskID());
}*/

TASK(OsTask_100ms){
	i++;
	printf("1 : %d\n",i);	
	
}

TASK(OsTask_5ms){
	i++;
	printf("2 : %d\n",i);	
	
}

int main(void) {
	StartOS(OsAppModeDefault);
	return 0;
}



