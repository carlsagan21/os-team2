#include <stdio.h>
#include <linux/types.h>

#define __NR_sched_setweight 380
#define __NR_sched_getweight 381

int main(int argc, char* argv[])
{
	int i, t, weight;
	pid_t id = 0;
	clock_t start, end;

	if (argc != 3) {
		printf("Need two arguments");
		return -1;
	}

	printf("start policy: %d \n", sched_getscheduler(0));

	t = atoi(argv[1]);
	weight = atoi(argv[2]);

	printf("getweight: %u\n", syscall(__NR_sched_getweight, 0));

	printf("setweight: %d\n", syscall(__NR_sched_setweight, id, weight));

	printf("getweight: %u\n", syscall(__NR_sched_getweight, 0));

	start = clock();
	if (t == 1) printf("1\n");
	while(t != 1){
		for(i = 2; i <= t; i++){
			if(t % i == 0){
				printf("%d ",i);
				t = t / i;
				break;
			}
		}
	}
	printf("\n");
	end = clock();
	printf("execution time: %.7f\n", (double)(end-start)/1000);
	return 0;
}
