// #include <stdio.h>
// #include <linux/types.h>

#define __NR_sched_setweight 380
#define __NR_sched_getweight 381
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>
#include <uapi/linux/sched.h>

int main(int argc, char* argv[])
{
	unsigned long long int i, t, h, g;
	int j, weight;
	double time, sum;
	pid_t id = 0;
	clock_t start, end;

	h = atoi(argv[1]);
	g = atoi(argv[2]);

	printf("start policy: %d \n", sched_getscheduler(0));

	for (weight = 20; weight > 0; weight -= g) {
		printf("getweight: %u\n", syscall(__NR_sched_getweight, 0));
		printf("set weight %d: %d\n", weight, syscall(__NR_sched_setweight, id, weight));
		printf("getweight: %u\n", syscall(__NR_sched_getweight, 0));
		sum = 0;
		for (j = 0; j < h; j++) {
			t = 1674919423;
			start = clock();
			while(t != 1){
				for(i = 2; i <= t; i++){
					if(t % i == 0){
						t = t / i;
						break;
					}
				}
			}
			end = clock();
			time = (double)(end - start)/1000;
			printf("%.7f\t", time);
			sum += time;
		}
		printf("avg: %.7f\n", sum/h);
	}
	return 0;
}
