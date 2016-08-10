#include <autosar/os.h>
#include <stdio.h>
/*static void checkError(StatusType func_ret,StatusType exp_ret,int id) {		
	if(func_ret == exp_ret) 				
		printf(" [%d] OK\n",id);		
	else						
		printf(" [%d] Failed : Ret = %d. Expected ret = %d\n",id,func_ret,exp_ret);	
}*/	
	

TASK(t1){
	printf("Task 1 before the resource\n");
	GetResource(res1);
	printf("Task 1 got the resource\n");
	
	while(1){
	}
	ReleaseResource(res1);
	TerminateTask();
}


TASK(t2){
	printf("Task 2 Before the resource\n");
	GetResource(res1);
	printf("Task 2 got the resource ret ");
	ReleaseResource(res1);
	
	TerminateTask();
}





int main(int argc, char *const argv[]){
	StartOS(0);
	return 0;
}
