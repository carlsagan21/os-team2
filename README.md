# os-team2

## Project 4: Geo-tagged File System

## CHANGE LOG



## shell scripts
-   `./build`: build && flash && console
-   `./build $1`: build && arm-linux-gnueabi-gcc -I./include test/$1.c -o test/$1 && flash && console
-   `./lint`: scripts/checkpatch.pl --max-line-length=200 -f arch/arm/kernel/\*rot\*.c
-   `./sdb`: direct_set_debug.sh --sdb-set
-   `./dmes`: direct_set_debug.sh --sdb-set && dmesg -w

`./build` 를 실행하기 전에 artik에서 thordown 을 켜줍시다.  
현재 3.11의 `checkpatch`에 오류가 있습니다. `lint`를 에러메세지 없이 쓰고싶으면 다른 버젼의 `checkpatch` 를 사용합시다. 현재 최신은 4.11 버전입니다.

## if tests are already perfect
-   thordown


-   build && flash && console
-   root
-   tizen
-   ./test/rotd
-   dmesg | grep degree


## if you recompile tests
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


## testing with checkpatch.pl
-   ./scripts/checkpatch.pl --max-line-length=200 -f arch/arm/kernel/*rot*.c (\*rot\* 입니다.)

-   pull /proc/sched_debug test/sched_debug
-   pull /proc/schedstat test/schedstat

## 참고 git
