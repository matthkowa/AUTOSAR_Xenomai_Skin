#include <autosar/os.h>
#include <stdio.h>
static void checkError(StatusType func_ret,StatusType exp_ret,int id) {		
	if(func_ret == exp_ret) 				
		printf(" [%d] OK\n",id);		
	else						
		printf(" [%d] Failed : Ret = %d. Expected ret = %d\n",id,func_ret,exp_ret);	
}	
int i = 0;	
void ErrorHook(StatusType error){
	printf("[%d] : ErrorHook",i);
	printf("OSErrorGetServiceId : %d \nError : %d\n",OSErrorGetServiceId(),error);
	i++;
	ShutdownOS(error);
}

void PreTaskHook(void){
	printf("[%d] : PreTaskHook\n",i);
	i++;
}

void PostTaskHook(void){
	printf("[%d] : PostTaskHook\n",i);
	i++;
}

void StartupHook(void){
	printf("[%d] : StartupHook\n",i);
	i++;
}

void ShutdownHook(void){
	printf("[%d] : ShutdownHook\n",i);
	i++;
}

TASK(OsTask1){
	StatusType ret;
	
	printf("[%d] : %s\n",i,__FUNCTION__);
	
	ChainTask(OsTask2);
}


TASK(OsTask2){
	ActivateTask(-1);	
	TerminateTask();
}


int main(int argc, char *const argv[]){
	StartOS(app1);
	return 0;
}
