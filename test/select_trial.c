#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int selector(int x)
{
	int i = x;
	while(1)
	{
		lock.acquire();
		FILE *f = fopen("file.txt", "w");
		fprintf(f, "%d", i);
		fclose(f);
		lock.release();

		printf("selector: %d\n", i);
		i++;

//		if(i==100) break;
	}
}

int trial(int iden)
{
	char out[100] = "";
	int num;

	while(1)
	{
		lock.acquire();
		FILE *f = fopen("file.txt", "r");
		fgets(out, 100, f);
		fclose(f);
		lock.release();

		num = atoi(out);
		printf("trial-%d: ", iden);
		trial_imple(num);
//		printf("%d\n", num);
	}
}

int main()
{
	trial(10);
}
