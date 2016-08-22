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

	start = 100;
	cycle = 1000;
	ret = SetAbsAlarm(alarm1,start,cycle);
	checkError(ret, E_OS_STATE,1);

	ret = CancelAlarm(alarm1);
	checkError(ret, E_OK,2);

	ret = SetAbsAlarm(alarm2,start,cycle);
	checkError(ret, E_OK,3);

	ret = SetAbsAlarm(-1,start,cycle);
	checkError(ret, E_OS_ID,4);

	start = -1;
	ret = SetAbsAlarm(alarm1,start,cycle);
	checkError(ret, E_OS_VALUE,5);

	start = 2000000;
	ret = SetAbsAlarm(alarm1,start,cycle);
	checkError(ret, E_OS_VALUE,6);

	start = 100;
	cycle = -1;
	ret = SetAbsAlarm(alarm1,start,cycle);
	checkError(ret, E_OS_VALUE,7);
	
	cycle = 300000;
	ret = SetAbsAlarm(alarm1,start,cycle);
	checkError(ret, E_OS_VALUE,8);
	
	TerminateTask();
	
}


TASK(OsTask2){
	TerminateTask();
}




int main(int argc, char *const argv[]){
	StartOS(app1);
	return 0;
}
