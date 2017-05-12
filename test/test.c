#define __NR_sched_setweight 380
#define __NR_sched_getweight 381
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sched.h>
#include <unistd.h>
#include <uapi/linux/sched.h>
// #include <stdio.h>


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


int main(){
	struct sched_param param, new_param;
  printf("start policy = %d \n", sched_getscheduler(0));

	if (sched_setscheduler(0, SCHED_WRR, &param) != 0) {
		perror("sched_setscheduler6 failed \n");
	}
	printf("start policy = %d \n", sched_getscheduler(0));

	int i;
	for (i = 0; i < 1e4; i++) {
		trial_imple(i);
	}

	printf("end!\n");

  /* 이 부분에 타임 크리티컬한 연산을 넣는다. */
	// Trial

	//
  // sched_yield();  /*다른 실시간 쓰레드, 프로세스가 수행될 수 있도록 CPU를 양도한다.
  //                   이것이 필요한 이유는 이 프로세스가 실시간 선점형 프로세스이기때문에
  //                   다른 일반 프로세스, 쓰레드들을 항상 선점하기 때문이다.(기아현상 방지)*/
	//
  // param.sched_priority = sched_get_priority_max(SCHED_FIFO);
  // if(sched_setparam(0, &param) != 0){
  //         perror("sched_setparam failed \n");
  //         return ;
  // }
	//
  // sched_getparam(0, &new_param);
	//
  // printf("I am running at priority %d \n",
  //                 new_param.sched_priority);

  return 0;
}
