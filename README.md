# os-team2

## CHANGE LOG

# Weighted Round Robin Scheduler

Proj3 was implementing Weighted Round Robin scheduler. We declaired data structures and functions for WRR in wrr.c. The abstract spec of those data structure and functions are following that of RT and CFS. And we also modifyed core.c and sched.h to use wrr in actual linux running. WRR is set to be default scheduler.

### Data Structure
We implemented wrr_rq and sched_wrr_entity in sched.h. It works similar with other rq and sched_entity but it follows the policy only for WRR which we implemented. The first element of wrr_rq is the task which is now executing. And sched entity has weight and time_slice as a variable. Weight is the constant number which is set by syscall set_weight(), and time_slice is the variable which reduces as the task is executing through time.

### Functions
the main function that works for WRR policy is task_tick_wrr(). It is called by every tick and do next things.
1) If current task has remaining time slice, it does -1 to time slice.
2) If time slice of current task became 0, it moves the task to tail by list_move_tail, and calls also resched_task().


### Load Balancing

## shell scripts
-   `./build`: build && flash && console
-   `./build $1`: build && arm-linux-gnueabi-gcc -I./include test/$1.c -o test/$1 && flash && console
-   `./lint`: scripts/checkpatch.pl --max-line-length=200 -f arch/arm/kernel/\*rot\*.c

`./build` 를 실행하기 전에 artik에서 thordown 을 켜줍시다.  
`checkpatch` 는 4.11 버전으로 되어있습니다.

## test already perfect
-   thordown


-   build && flash && console
-   root
-   tizen
-   ./test/rotd
-   dmesg | grep degree


## if you recompile test
-   thordown


-   build && arm-linux-gnueabi-gcc -I./include test/test.c -o test/test && flash && console

-   root
-   tizen
-   direct_set_debug.sh --sdb-set
-   ctl+a d


-   root && push test/test /root/test && console


-   root
-   tizen
-   ./test/test && dmesg


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
