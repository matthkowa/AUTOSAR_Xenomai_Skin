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
	AlarmBaseType Info;
	
	ret = GetAlarmBase(alarm1,&Info);
	checkError(ret, E_OK,1);
	
	ret = GetAlarmBase(-1,&Info);
	checkError(ret, E_OS_ID,2);
	
	ChainTask(OsTask2);
}


TASK(OsTask2){
	StatusType ret;
	TickType tick;
	
	ret = GetAlarm(alarm2,&tick);
	checkError(ret, E_OK,3);

	ret = GetAlarm(alarm3,&tick);
	checkError(ret, E_OS_NOFUNC,4);

	ret = GetAlarm(-1,&tick);
	checkError(ret, E_OS_ID,5);
	
	TerminateTask();
}

TASK(OsTask3){
	TerminateTask();
}



int main(int argc, char *const argv[]){
	StartOS(app1);
	return 0;
}
