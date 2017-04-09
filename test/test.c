#define __NR_set_rotation 380
#include <stdio.h>
#include <unistd.h>

int main() {
	int res = syscall(__NR_set_rotation, 0, 0);

	if (res < 0) {
		printf("Fail to run syscall(__NR_set_rotation): %d\n", res);
		return -1;
	}

	return 0;
}
