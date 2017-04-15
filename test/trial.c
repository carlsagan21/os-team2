// #define __NR_set_rotation 380
#define __NR_rotlock_read 381
#define __NR_rotlock_write 382
#define __NR_rotunlock_read 383
#define __NR_rotunlock_write 386

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prime.c"

int trial(int iden)
{
	char out[100] = ""; // interger is max 10 length
	int num;

	while(1)
	{
		syscall(__NR_rotlock_read, 90, 90);
		FILE *f = fopen("file.txt", "r");
		fgets(out, 100, f);
		fclose(f);
		syscall(__NR_rotunlock_read, 90, 90);

		num = atoi(out);
		printf("trial-%d: ", iden);
		trial_imple(num);
	}
}

int main(int argc, char* argv[])
{
	if (argc == 1) {
		printf("에러! 옵션을 입력하지 않으셨군요...\n");
		return -1;
	}

	trial(atoi(argv[1]));

	return 0;
}
