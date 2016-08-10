#include "autosar/task.h"
#include <stdio.h>
#define validate(i) printf("Test %d validé\n",i)
#define error(i,err) printf("Error Test %d : %d\n",i,err)


TASK(t1){

	printf("Execution de la tâche en cours\n");
	

}

int main(void) {
	StatusType ret;
	TaskType taskId;
	TaskStateRefType state;
	int mode = 0;
	mode |= T_LOCK;

	ret = TaskInit(80,&taskId,t1,0);
	if(ret == E_OK)
		validate(1);
	else
		error(1,ret);
	ret = ActivateTask(taskId);
	if(ret == E_OK)
		validate(2);
	else
		error(2,ret);
	ret = TaskInit(80,&taskId,t1,mode);
	if(ret == E_OK)
		validate(3);
	else
		error(3,ret);
	ret = ActivateTask(taskId);
	if(ret == E_OK)
		validate(4);
	else
		error(4,ret);
	ret = GetTaskID(&taskId);
	if(ret == E_OK){
		validate(5);
		printf("ID %lu\n",taskId);	
	}
	else
		error(5,ret);

	ret = GetTaskState(taskId,state);
	if(ret == E_OK){
		validate(6);
		printf("state %lu\n",*state);	
	}
	else
		error(6,ret);

	return 0;
}



