#define __NR_sched_setweight 380
#define __NR_sched_getweight 381
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>
#include <uapi/linux/sched.h>
// #include <stdio.h>

int main()
{
	struct sched_param param, new_param;
	// int i = 0, j = 0;

  printf("start policy = %d \n", sched_getscheduler(0));

	if (sched_setscheduler(0, SCHED_WRR, &param) != 0) {
		perror("sched_setscheduler6 failed \n");
	}

	printf("middle policy = %d \n", sched_getscheduler(0));

	fork();
	fork();
	fork();
	fork();
	fork();
	while(1) {

	}

	return 0;
}
