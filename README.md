# os-team2

## CHANGE LOG



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
