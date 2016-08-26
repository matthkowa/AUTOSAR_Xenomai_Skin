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
	ret = ActivateTask(OsTask2);
	checkError(ret, E_OK,1);
	
	ret = ActivateTask(-1);
	checkError(ret, E_OS_ID,3);
	
	ret = ChainTask(-1);
	checkError(ret, E_OS_ID,4);

	GetResource(OsResource1);
	ret = ChainTask(OsTask3);
	checkError(ret, E_OS_RESOURCE,5);

	ReleaseResource(OsResource1);
	ChainTask(OsTask3);
}


TASK(OsTask2){
	StatusType ret;
	GetResource(OsResource1);	
	ret = TerminateTask();
	checkError(ret, E_OS_RESOURCE,2);
	ReleaseResource(OsResource1);
	TerminateTask();
}

TASK(OsTask3){
	TerminateTask();
}



int main(int argc, char *const argv[]){
	StartOS(app1);
	return 0;
}
