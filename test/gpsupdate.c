#include <stdio.h>
// #include <sys/syscall.h>
#include <linux/gps.h>

#define __NR_set_gps_location 380
#define __NR_get_gps_location 381

int main(int argc, char* argv[])
{
	struct gps_location loc_st;
	struct gps_location *loc = &loc_st;
	loc->lat_integer = 10;
	loc->lat_fractional = 100;
	loc->lng_integer = -170;
	loc->lng_fractional = 999999;
	loc->accuracy = 10;

	int ret = syscall(__NR_set_gps_location, loc);

	printf("sys_set_gps_location: %d\n", ret);

	char str[255];
	scanf("%254s", str);
	ret = syscall(__NR_get_gps_location, str, loc);

	printf("sys_get_gps_location: %d, %d, %d, %d, %d, %d\n", ret, loc->lat_integer, loc->lat_fractional, loc->lng_integer, loc->lng_fractional, loc->accuracy);


	// int i, t, weight;
	// pid_t id = 0;
	// clock_t start, end;
	//
	// if (argc != 3) {
	// 	printf("Need two arguments");
	// 	return -1;
	// }
	//
	// printf("start policy: %d \n", sched_getscheduler(0));
	//
	// t = atoi(argv[1]);
	// weight = atoi(argv[2]);
	//
	// printf("getweight: %u\n", syscall(__NR_sched_getweight, 0));
	//
	// printf("setweight: %d\n", syscall(__NR_sched_setweight, id, weight));
	//
	// printf("getweight: %u\n", syscall(__NR_sched_getweight, 0));
	//
	// start = clock();
	// if (t == 1) printf("1\n");
	// while(t != 1){
	// 	for(i = 2; i <= t; i++){
	// 		if(t % i == 0){
	// 			printf("%d ",i);
	// 			t = t / i;
	// 			break;
	// 		}
	// 	}
	// }
	// printf("\n");
	// end = clock();
	// printf("execution time: %.7f\n", (double)(end-start)/1000);
	return 0;
}
