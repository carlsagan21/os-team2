#include<stdio.h>

int selector(int x)
{
	int i = x;
	while(true)
	{
		lock.acquire();
		FILE *f = fopen("file.txt", "w");
		fprintf(f, "%d", i);
		fclose(f);
		lock.release();

		printf("selector: %d", i);
		i++;
	}
}

int trial(int iden)
{
	char out[100] = "";
	int num;

	while(true)
	{
		lock.acquire();
		FILE *f = fopen("file.txt", "r");
		fgets(out, 100, f);
		fclose(f);
		lock.release();

		num = atoi(out);
		printf("trial-%d: ", iden);
		trial_imple(num);
	}
}
