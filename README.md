# os-team2

build
flash
arm-linux-gnueabi-gcc -I./include test/test.c -o test/test
console
direct_set_debug.sh --sdb-set
ctl+a d
root
push test/test /root/test
console
./test/test
dmesg

---

## test already perfect
-	build && flash && arm-linux-gnueabi-gcc -I./include test/test.c -o test/test && console
-	root
-	tizen
-	./test/test && dmesg


## if you recompile test
-	build && flash && arm-linux-gnueabi-gcc -I./include test/test.c -o test/test && console

-	root
- tizen
-	direct_set_debug.sh --sdb-set
-	ctl+a d


-	root && push test/test /root/test && console


-	root
-	tizen
-	./test/test && dmesg
