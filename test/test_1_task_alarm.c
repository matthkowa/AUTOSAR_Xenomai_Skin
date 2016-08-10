#include "os.h"

int main(void){

	StartOs(OSDEFAULTAPPMODE);	
	return 0;
}


TASK(t1){

	printf("Test 1\n");


	TerminateTask();


}
