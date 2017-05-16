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

int prime_number_factorize (int x)
{
	if(x==1)
	{
		// printf("\n");
		 return 0;
	}
	for(int i=2; i<=x; i++)
	{
		if(x%i==0)
		{
			// printf("%d",i);
			// if(x/i != 1) printf(" * ");
			prime_number_factorize(x/i);
			break;
		}
	}
}

int trial_imple(int x)
{
	// printf("%d = ", x);
	prime_number_factorize(x);
}

int main(int argc, char* argv[])
{
	struct sched_param param, new_param;
	printf("start policy = %d \n", sched_getscheduler(0));

	if (sched_setscheduler(0, SCHED_WRR, &param) != 0) {
		perror("sched_setscheduler6 failed \n");
	}

	printf("middle policy = %d \n", sched_getscheduler(0));

	fork();

	// while (1) {
	int i;
	i++;
	for (i = 0; i < 1e4 * 3; i++) {
		trial_imple(i);
	}
	// }

	return 0;
}
