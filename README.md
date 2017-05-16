# Weighted Round Robin Scheduler

## 3 Big Challenges of this Project

1. Making new scheduler WRR work alongside well with the existing schedulers.  
	1) syscall implements
2. Running a Linux successfully with WRR as a basic(default) scheduler.
3. Implementing WRR policy and Load Balancing.
	1) WRR  
	2) cpu allocation  
	3) load balancing  

### 1. Implementing new scheduler

1) syscalls

Implement sched_setweight and sched_getweight syscall as syscall number 380, 381. By using these syscall, we can test WRR scheduler working even before not selecting them as a basic scheduler. The way how to implement syscall is same as proj1, proj2.

2) wrr_rq

Defined in 'kernel/sched/sched.h'. Work as list dummy head of sched_wrr_entities inside the task structure. It has member variable total_weight so that it can be used at load balancing. When accessing the list, lock must be acquired. And It uses spinlock because acccessing is writting rather than reading, and also it takes only short time.

3) sched_wrr_entity

Defined in 'include/linux/sched.h'. It has list_head, weight, remaining time as a member variable. It represents the actual task as a abstraction so it is most important part of scheduler.

4) wrr_sched_class

Implemented polymorphic functions of scheduler inside the wrr_sched_class. It makes WRR scheduler works well with the existing scheduling system, and also makes difference between other schedulers by polymorphism. If it is not implemented, it works as FIFO scheduler.
Main functions are enqueue_task, dequeue_task, pick_next_task, task_tick, task_fork, etc.. More details are in "3. Implementing Policy".

5) core.c

Many codes are implemented for rt or cfs, and assuming cfs as a basic scheduler. We had to find and fix all these parts. This job is done by searching 'fair', 'SCHED_NORMAL', 'rt_prio()', 'task_has_rt_policy()'.

### 2. Making WRR as a basic scheduler

Modify default_policy as SCHED_WRR in 'include/linux/init_task.h', and change SCHED_NORMAL with SCHED_WRR in 'kernel/kthread.c'. And also change the codes with the fair/cfs as a default scheduler in 'core.c'

### 3. Impelementing Policy

Proj3 was implementing Weighted Round Robin scheduler. We declaired data structures and functions for WRR in wrr.c. The abstract spec of those data structure and functions are following that of RT and CFS. And we also modifyed core.c and sched.h to use wrr in actual linux running. WRR is set to be default scheduler.

#### Functions

1. task_tick_wrr
It is called by every tick and do next things. It does 1) If current task has remaining time slice, it does -1 to time slice. 2) If time slice of current task became 0, it moves the task to tail by list_move_tail, and calls also resched_task().

2. task_fork_wrr
When WRR sched entity is fored, it inherits the parent's weight. So it is implemented in task_fork_wrr.

#### Load Balancing



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
