// #define __NR_set_rotation 380
#define __NR_rotlock_read 381
#define __NR_rotlock_write 382
#define __NR_rotunlock_read 383
#define __NR_rotunlock_write 386

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prime.c"

int selector(int x)
{
	int i = x;
	while(1)
	{
		syscall(__NR_rotlock_write, 90, 90);
		FILE *f = fopen("/root/test/file.txt", "w");
		fprintf(f, "%d", i);
		fclose(f);

		printf("selector: %d\n", i);
		i++;

		syscall(__NR_rotunlock_write, 90, 90);
	}
}

int main(int argc, char* argv[])
{
	if (argc == 1) {
		printf("에러! 옵션을 입력하지 않으셨군요...\n");
		return -1;
	}

	selector(atoi(argv[1]));

	return 0;
}
