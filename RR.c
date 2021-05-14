#include <stdio.h>
#include <stdlib.h>

struct pcb {
	int pid;

	int priority;

	int arrival_time;

	int burst_time;

	int first_run;

	struct pcb *next;

};

struct pcb *start = NULL;
struct pcb *new;
struct pcb *temp = NULL;

struct pcb *process[10]; //job queue

int quantum;

int time = 0;
int idle = 0;

int total_rt = 0;
int total_wt = 0;

// ready queue with a linked list
void insert(struct pcb *process) {

	temp = start;
	new = (struct pcb *)malloc(sizeof(struct pcb));

	new->pid = process->pid;
	new->burst_time = process->burst_time;
	new->arrival_time = process->arrival_time;
	new->first_run = process->first_run;

	new->next = NULL;

	if (start == NULL) // 아무 것도 없으면
	{
		start = new;
	}
	else
	{
		while (temp->next != NULL) temp = temp->next;
		
		if (temp->next == NULL) temp->next = new;
	}

}

int terminate = 0;

void RR()
{
	printf("Scheduling : RR \n======================================== \n");

	while (terminate != 10)
	{
		if (start == NULL) // ready queue에 아무것도 없다는 것
		{
			for (int i = 0; i < 10; i++)
			{
				if (process[i]->arrival_time == time)
				{
					printf("<time %d> [new arrival] process %d \n", time, process[i]->pid);

					insert(process[i]);

					break;
				}
			}

			if (start == NULL)
			{
				printf("<time %d> ---- system is idle ---- \n", time);
				idle++;
			}
			
			time++;
		}
		else // 뭐라도 하나 있음
		{
			if (start->burst_time <= quantum) // 마지막 running
			{
				for (int i = 0; i < start->burst_time; i++)
				{
					printf("<time %d> process %d is running \n", time, start->pid);

					if (start->first_run == 0)
						start->first_run = 1;

					temp = start->next;

					while (temp != NULL)
					{
						total_wt++;

						if (temp->first_run == 0) total_rt++;

						temp = temp->next;
					}

					for (int i = 0; i < 10; i++)
					{
						if (process[i]->arrival_time == time)
						{
							printf("<time %d> [new arrival] process %d \n", time, process[i]->pid);

							insert(process[i]);
						}
					}

					time++;
				}

				printf("<time %d> process %d is finished \n", time, start->pid);

				terminate++;
				

				if (terminate < 10) printf("--------------------------------------(Context-Switch) \n");

				start = start->next;

				
			}
			else // 아직 running 더 남음
			{
				for (int i = 0; i < quantum; i++)
				{
					printf("<time %d> process %d is running \n", time, start->pid);

					if (start->first_run == 0)
						start->first_run = 1;

					temp = start->next;

					while (temp != NULL)
					{
						total_wt++;

						if (temp->first_run == 0) total_rt++;

						temp = temp->next;
					}

					for (int i = 0; i < 10; i++)
					{
						if (process[i]->arrival_time == time)
						{
							printf("<time %d> [new arrival] process %d \n", time, process[i]->pid);

							insert(process[i]);
						}
					}
										
					time++;

					start->burst_time--;
				}

				printf("--------------------------------------(Context-Switch) \n");

				temp = start;
				start = start->next;
				insert(temp);
			}
		}

	}

	printf("<time %d> all processes finish \n===================================== \n", time);
}

void print_process()
{

	int total_burst = 0;

	for (int i = 0; i < 10; i++)
	{
		total_burst += process[i]->burst_time;
	}

	
	printf("Average cpu usage : %.2f %% \n", 100 * (double)(time - idle) / time);
	printf("Average waiting time : %.2f \n", (double)total_wt / (double)10);
	printf("Average response time : %.2f \n", (double)total_rt / (double)10);
	printf("Average turnaround time : %.2f \n", (double)(total_wt + total_burst) / (double)10);
	
}


int main()
{

	FILE *fp = fopen("input.txt", "r");

	if (fp == 0) {
		printf("Could not open file.\n");
		exit(1);
	}

	else {

		//quantum = argv[3];
		quantum = 5;

		for (int i = 0; i < 10; i++) {

			process[i] = (struct pcb*)malloc(sizeof(struct pcb));

			fscanf(fp, "%d %d %d %d", &process[i]->pid, &process[i]->priority, &process[i]->arrival_time, &process[i]->burst_time);

			process[i]->first_run = 0;

		}

		fclose(fp);

		RR();
		print_process();

	}

}
