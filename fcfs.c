#include <stdio.h>
#include <stdlib.h>

typedef struct process {
	int pid;

	int priority;

	int arrival_time;

	int burst_time;

	PCB *next;

} PCB;


/*
마지막에는 cpu usage랑 waiting time 평균이랑 response time 평균이랑 turnaround time 평균 구해야함
*/

int fcfs(int id, int arr_t, int bur_t)
{
	
}


int main(int argc, char *argv[])
{
	if (argc != 2)
		printf("usage: %s filename", argv[0]);

	else {

		FILE *fp = fopen(argv[1], "r");

		if (fp == 0)
			printf("Could not open file.\n");

		else {
            PCB* pcb = (PCB*)malloc(sizeof(PCB));
			int id, pri, arr_t, bur_t;
			
			while (fscanf(fp, "%d, %d, %d, %d", &id, &pri, &arr_t, &bur_t) != EOF)
				fcfs(id, arr_t, bur_t);
			
			fclose(fp);
		}
	}

}