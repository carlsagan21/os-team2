# os-team2

## PROJ3: WRR SCHEDULER

### 3 Big Challenges

1. 새로운 스케줄러가 기존 스케줄러들 사이에 잘 자리잡게 하기.  
	1) syscall implements
2. 기본 스케줄러로 동작하게 하기.
3. WRR / BALANCING 등 정책 구현하기.  
	1) WRR  
	2) cpu allocation  
	3) load balancing  


##### 1. 새로운 스케줄러 구현하기

1) syscalls

380, 381 번으로 sched_setweight / sched_getweight 시스템 콜을 구현합니다. 이 단계에서 구현해야 하는 이유는, 기본 스케줄러로 WRR 을 세팅하기 이전에도 스케줄러를 테스트해야하는데, sched_setweight 을 이용하여 WRR 스케줄러로 넣어줄 수 있기 때문입니다.
시스템콜 구현 방법은 프로젝트1 부터 이어져 온 방법과 동일합니다.

2) wrr_rq

`kernel/sched/sched.h` 에 정의됩니다. task 안의 sched_wrr_entity 의 리스트의 더미헤드로서 기능합니다. total_weight 도 보관하여 load balancing 에 참조됩니다. 리스트를 돌 때에 lock 을 두어 잡습니다. 읽기보다도 수정이 빈번하게 발생하므로, 그리고 짧은 시간동안만 발생하므로, rwlock 이나 mutex 보다 일반 spinlock 으로 잡습니다.

3) sched_wrr_entity
`include/linux/sched.h` 에 정의되고 list_head, weight, remaining time 을 가집니다. task_struct 내부에 들어가게 됩니다. 스케줄 리스트의 핵심입니다.

4) wrr_sched_class
enqueue_task 등의 스케줄러 다형 함수들을 내부적으로 구현해야 합니다. 정책 구현에 핵심적인 역할을 담당하며, 내부함수를 구현하지 않으면 스케줄러가 FIFO 로 동작함을 알 수 있습니다.
enqueue_task, dequeue_task, pick_next_task, task_tick, task_fork 등이 핵심 함수로서 각각 자료구조에 맞게 구현되어야 합니다. 자세한 사항은 "3. 정책구현하기" 에서 설명합니다.

5) core.c
많은 코드들이 rt or cfs 식으로 구현되어 있고, cfs 를 기본스케줄러라고 가정한 코드가 많습니다. 이 부분들을 다 찾아서 고쳐줘야 합니다. 파일에서 `fair`, `SCHED_NORMAL`, `rt_prio()`, `task_has_rt_policy()` 등을 검색하면 왠만한 경우는 다 잡아낼 수 있습니다.

##### 2. 기본 스케줄 화 하기
`include/linux/init_task.h` 에서 default policy 를 SCHED_WRR 로 설정하고, `kernel/kthread.c` 에서 SCHED_NORMAL 을 SCHED_WRR 로 바꿔줍니다.

##### 3. 정책 구현하기

## shell scripts
-   `./build`: build && flash && console
-   `./build $1`: build && arm-linux-gnueabi-gcc -I./include test/$1.c -o test/$1 && flash && console
-   `./lint`: scripts/checkpatch.pl --max-line-length=200 -f arch/arm/kernel/\*rot\*.c
-   `ARTIK$ ./sdb`: sdb 를 켭니다
-   `ARTIK$ ./dmes`: sdb 를 켜고 dmesg -w 를 실행합니다.

`./build` 를 실행하기 전에 artik에서 thordown 을 켜줍시다.  
`checkpatch` 는 4.11 버전으로 되어있습니다.


## test with checkpatch.pl
-   ./scripts/checkpatch.pl --max-line-length=200 -f arch/arm/kernel/*rot*.c (\*rot\* 입니다.)

-   pull /proc/sched_debug test/sched_debug
-   pull /proc/schedstat test/schedstat

## 참고 git

<https://github.com/MattMCloudy/CSE3320-Kernel-Project/blob/c52eb413c9eaa14103707adf5714686211e8681e/kernel/sched_wrr.c>

<https://github.com/slgu/OS/blob/d26d1aa40a8c44531932e0b056eb7eef0eff50ed/hw4/flo-kernel/kernel/sched/wrr.c>

<https://github.com/nBerg/OS-Programming-Assignments/blob/7d4fd96e31a114625493b601e44458012b1c46cc/hmwk4/android-tegra-3.1/kernel/sched_wrr.c>

<https://github.com/jervisfm/W4118_HW4/blob/c1d2e7984a1fabfb8e749eb5f4ab85f712762775/android-tegra-3.1/kernel/sched_wrr.c>

<https://github.com/sloanchoi1124/wrr-scheduler/blob/1a070fcd51c38a7e6216c7f21e72e64ae67296b5/kernel/kernel/sched/wrr.c>

<https://github.com/liangcht/linuxWRRScheduler/blob/1a070fcd51c38a7e6216c7f21e72e64ae67296b5/kernel/kernel/sched/wrr.c>

<https://github.com/deeprog/CS_4500_Project_4/blob/b69794fecef84894c4f721c5ea81eba85c5d9f5e/sched_wrr.c>

<https://github.com/hugotkk/CS_4500_Project_4/blob/b69794fecef84894c4f721c5ea81eba85c5d9f5e/sched_wrr.c>

<https://github.com/binwang0105/os_hmwk4_scheduler/blob/609bf7d40b3fd611890817d3eab1643d97a1a916/flo-kernel/kernel/sched/wrr.c>

<https://github.com/gs0510/Operating-Systems/blob/f631323e1dcaffadf60a885c685b508c99a7f37e/process%20scheduler/sched_wrr.c>

<https://github.com/at3103/Linux-Weighted-Round-robin-Scheduler/blob/8bf6116ab1ebac350eff11703b69a597d9dd627e/kernel/kernel/sched/wrr.c>

<https://github.com/keyu-lai/Multicore-Round-Robin-Scheduler/blob/47400c623e67ece87720cb63780e4c5389a5c318/flo-kernel/kernel/sched/wrr.c>

<https://github.com/xyaoinum/Linux_WeightedRR_Scheduler/blob/5e6e9b1794d4645b8968c9a069467cf7d247467e/flo-kernel/kernel/sched/wrr.c~>

<https://github.com/Keanesean/CS4500_OS/blob/8bd14d1387a149a4d597e44a0955903f8d452647/linux-2.6.32/kernel/sched_wrr.c>

<https://github.com/nick76567/comp4511_ass05/blob/f069e4f1d87b51fb1c84b7707231ee91f54a89c0/code/sched_wrr.c>
