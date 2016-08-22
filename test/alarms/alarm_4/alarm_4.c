#include <autosar/os.h>
#include <stdio.h>
static void checkError(StatusType func_ret,StatusType exp_ret,int id) {		
	if(func_ret == exp_ret) 				
		printf(" [%d] OK\n",id);		
	else						
		printf(" [%d] Failed : Ret = %d. Expected ret = %d\n",id,func_ret,exp_ret);	
}	
	

TASK(OsTask1){
	StatusType ret;
	TickType start, cycle;

	

	ret = CancelAlarm(alarm1);
	checkError(ret, E_OK,1);

	start = 100;
	cycle = 1000;
	ret = SetAbsAlarm(alarm1,start,cycle);
	checkError(ret, E_OK,2);

	ret = CancelAlarm(alarm1);
	checkError(ret, E_OK,3);

	ret = CancelAlarm(alarm1);
	checkError(ret, E_OS_NOFUNC,4);

	ret = CancelAlarm(alarm2);
	checkError(ret, E_OS_NOFUNC,5);

	ret = CancelAlarm(-1);
	checkError(ret, E_OS_ID,6);

	TerminateTask();
	
}


TASK(OsTask2){
	TerminateTask();
}




int main(int argc, char *const argv[]){
	StartOS(app1);
	return 0;
}
