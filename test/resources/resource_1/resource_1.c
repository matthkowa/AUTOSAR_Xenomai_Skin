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
	
	ret = GetResource(-1);
	checkError(ret, E_OS_ID,1);
	
	ret = GetResource(OsResource1);
	checkError(ret, E_OK,2);
	
	ret = ReleaseResource(OsResource1);
	checkError(ret, E_OK,3);
	
	ChainTask(OsTask2);
}


TASK(OsTask2){
	StatusType ret;
	int i;
	ret = GetResource(OsResource2);	
	checkError(ret, E_OK,4);

	ActivateTask(OsTask3); // Priority Ceiling Protocol 
	
	ret = ReleaseResource(OsResource2);
	checkError(ret, E_OK,5);

	TerminateTask();
}

TASK(OsTask3){
	StatusType ret;
	ret = GetResource(OsResource2);	
	checkError(ret, E_OK,6);
	
	ret = ReleaseResource(OsResource2);
	checkError(ret, E_OK,7);

	ret = GetResource(OsResource3);	
	checkError(ret, E_OK,8);
	
	ret = ReleaseResource(OsResource3);
	checkError(ret, E_OK,9);
	TerminateTask();
}



int main(int argc, char *const argv[]){
	StartOS(app1);
	return 0;
}
