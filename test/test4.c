#define __NR_sched_setweight 380
#define __NR_sched_getweight 381
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>
#include <uapi/linux/sched.h>
// #include <stdio.h>
// #include <stdio.h>
// #include <linux/types.h>

#define SET_WEIGHT 384
#define GET_WEIGHT 385
#define SET_SCHEDULER 156
#define GET_SCHEDULER 157
#define SCHED_WRR 6
#define SCHED_NORMAL 0

int main(int argc, char* argv[])
{
	struct sched_param param, new_param;
	printf("start policy = %d \n", sched_getscheduler(0));

	if (sched_setscheduler(0, SCHED_WRR, &param) != 0) {
		perror("sched_setscheduler6 failed \n");
	}

	printf("middle policy = %d \n", sched_getscheduler(0));

	syscall(__NR_sched_setweight, 0, 1);
	printf("getweight: %u\n", syscall(__NR_sched_getweight, 0));
	sleep(5);
	if (fork()) {
		//parent
		while(1) {
			printf("%d", 1);
		}
	} else {
		//child
		syscall(__NR_sched_setweight, 0, 20);
		while(1) {
			printf("%d", 2);
		}
	}

	return 0;
}