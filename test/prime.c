#include<stdio.h>

int prime_number_factorize (int x)
{
	if(x==1)
	{
		printf("\n");
		 return 0;
	}
	for(int i=2; i<=x; i++)
	{
		if(x%i==0)
		{
			printf("%d",i);
			if(x/i != 1) printf(" * ");
			prime_number_factorize(x/i);
			break;
		}
	}
}

int trial(int x)
{
	printf("trial-0: %d = ", x);
	prime_number_factorize(x);
}


int main()
{
	trial(10);
	trial(360);
	return 0;


}
