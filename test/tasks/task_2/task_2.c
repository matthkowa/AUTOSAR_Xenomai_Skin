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
	TaskType taskID;
	TaskStateType state;
	ret = ActivateTask(OsTask2);
	checkError(ret, E_OK,1);
	
	ret = GetTaskID(&taskID);
	checkError(ret, E_OK,5);
	
	ret = GetTaskState(-1,&state);
	checkError(ret, E_OS_ID,6);

	ret = GetTaskState(OsTask1,&state);
	checkError(ret, E_OK,7);

	
	TerminateTask();
}


TASK(OsTask2){
	StatusType ret;
	TaskStateType state;
	GetResource(OsResource1);	
	ret = Schedule();
	checkError(ret, E_OS_RESOURCE,2);

	ret = GetTaskState(OsTask1,&state);
	checkError(ret, E_OK,3);
	
	ReleaseResource(OsResource1);
	ret = Schedule();
	checkError(ret, E_OK,4);

	TerminateTask();
}

TASK(OsTask3){
	TerminateTask();
}



int main(int argc, char *const argv[]){
	StartOS(app1);
	return 0;
}
