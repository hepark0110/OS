#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 3

int num;
int *list;
int avg, min, max;

void *average()
{
	int sum=0;
	
	for(int i=0;i<num;i++) sum+=list[i];

	avg = sum/num;

	pthread_exit(0);	
}

void *minimum()
{
	for(int i=1;i<num;i++){
		if(min>list[i]) min=list[i];
	}

	pthread_exit(0);
}

void *maximum()
{
	for(int i=1;i<num;i++){
		if(max<list[i])	max=list[i];
	}
	
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	printf("How many numbers do you enter? : ");
	scanf("%d", &num);

	list = (int*)malloc(sizeof(int)*num);
	pthread_t workers[NUM_THREADS];
	pthread_attr_t attr;

	for(int i=0;i<num;i++) scanf("%d", &list[i]);

	pthread_attr_init(&attr);

	min=list[0];
	max=list[0];

	pthread_create(&workers[0], &attr, average, argv[1]);
	pthread_create(&workers[1], &attr, minimum, argv[1]);
	pthread_create(&workers[2], &attr, maximum, argv[1]);

	for(int i=0;i<NUM_THREADS;i++) pthread_join(workers[i], NULL);

	printf("The average value is %d \n", avg);
	printf("The minimum value is %d \n", min);
	printf("The maximum value is %d \n", max); 

	return 0;
}
