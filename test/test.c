#define __NR_set_rotation 380
#define __NR_rotlock_read 381
#define __NR_rotlock_write 382
#define __NR_rotunlock_read 383
#define __NR_rotunlock_write 386
#include <stdio.h>
#include <unistd.h>

int main() {
	int res;
	res = syscall(__NR_set_rotation, 0);

	if (res < 0) {
		printf("Fail to run syscall(__NR_set_rotation): %d\n", res);
		return -1;
	}

	res = syscall(__NR_rotlock_read, 0, 0);

	if (res < 0) {
		printf("Fail to run syscall(__NR_rotlock_read): %d\n", res);
		return -1;
	}

	res = syscall(__NR_rotlock_write, 0, 0);

	if (res < 0) {
		printf("Fail to run syscall(__NR_rotlock_write): %d\n", res);
		return -1;
	}

	res = syscall(__NR_rotunlock_read, 0, 0);

	if (res < 0) {
		printf("Fail to run syscall(__NR_rotunlock_read): %d\n", res);
		return -1;
	}

	res = syscall(__NR_rotunlock_write, 0, 0);

	if (res < 0) {
		printf("Fail to run syscall(__NR_rotunlock_write): %d\n", res);
		return -1;
	}

	printf("Test Success.\n");

	return 0;
}
