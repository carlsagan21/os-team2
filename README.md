# os-team2

## CHANGE LOG

Proj2 was implementing a 'rotation based lock'. Because ARTIK10 has no rotation sensor, we defined a structure 'rotation_t' which has variable 'degree' inside. And we changed that value by daemon which is given. We implemented rotation based locks that uses system call, and test function selector and trial which uses that system call. Below is the algorithm and data structure of our implement.

- All reading and writing processes 1 on 1 corresponds the structure '__rotlock_t'. And those are controlled by the functions which handles that structure.
- there are 4 lists that includes rotlocks - pending list, write waiting list(wwl), read waiting list(rwl), aquired list-.
- All locks which is called go to pending list first. and if the range of the lock includes the current rotation degree, it goes to the wwl or rwl according to the type of the rotlock. This process is done by 'refresh' functions. This functions are called when 1)new rotlock came in to pending list or 2)current rotation degree has been changed.
- Rotlocks in wwl or rwl can go to acquired list by the function wl_acquire and rl_acquire. When 1)any lock in the acquired list has deleted or 2) current rotation degree has been changed, wl_acquire is called first and rl_acquire is called later.(this is our policy and it prevents 'writer starvation' absolutely)
- wl_acquire and rl_acquire runs double iteration for all the elements in the waiting list and acquired list. wl_acquire checks certain waiting write_lock with 'every acquired locks' if they are overlaped or not. From the first element, if any element is not overlaped with any acquired locks, it sends that element to acquired list and it 'ends'. And only if no elements in the wwl is sended to acquired list, rl_acquire function is called.
- rl_acquire checks certatin waiting read_lock with only 'every acquired write locks' if they are overlaped or not. It sends every elements which is not overlaped with acquired write locks, and it ends when iteration runs to end of the list.
- when acquired lock is released or the process is killed, that elements are deleted from the acquired list.


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
