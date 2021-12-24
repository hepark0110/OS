#include <stdio.h>
#include <stdlib.h>

FILE *output;

typedef enum proc_state {
	init = 0, ready, running, terminated
} ProcState;

struct pcb {
	int pid;

	float priority;

	int arrival_time;

	int burst_time;

	float waiting_time;

	int response_time;
	
	int first_run;

	ProcState state;

	struct pcb *next;

};

int total_bt = 0;

struct pcb *start = NULL;
struct pcb *new;
struct pcb *temp = NULL;

struct pcb *process[10]; //job queue

float alpha;

int time = 0;
int idle = 0;

int terminate = 0;

struct pcb *Running = NULL;
struct pcb *Ready = NULL;

void insert(struct pcb *ptr) 
{
	temp = Ready;

	if (Ready == NULL) 
	{
		Ready = ptr;
		Ready->next = NULL;
	}
	else 
	{
		while (temp->next != NULL) 
		{
			temp = temp->next;
		}

		temp->next = ptr;
	}
}

void swap(struct pcb *a, struct pcb *b) 
{
	struct pcb *temp;

	temp = a;
	a = b;
	b = temp;
}

void bubbleSort(struct pcb *start) 
{
	int swapped, i;
	struct pcb *ptr1;
	struct pcb *lptr = NULL;

	if (start == NULL)
		return;

	do
	{
		swapped = 0;
		ptr1 = start;

		while (ptr1->next != lptr)
		{
			if (ptr1->priority > ptr1->next->priority)
			{
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;

		}
		lptr = ptr1;
	} while (swapped);
}

void PPSA()
{
	fprintf(output, "Scheduling : Preemptive Priority Scheduling with Aging \n======================================== \n");

	struct pcb *proc_run;

	while (1) 
	{
		for (int i = 0; i < 10; i++) 
		{
			if ((process[i]->arrival_time) == time) 
			{
				fprintf(output, "<time %d> [new arrival] process %d \n", time, process[i]->pid);

				if (Running != NULL && (Running->priority < process[i]->priority)) 
				{
					temp = Running;
					Running = NULL;
					insert(temp);

					bubbleSort(Ready);

					Running = process[i];
					process[i]->state = running;

					fprintf(output, "------------------------------------(Context-Switch)\n");
				}
				else 
				{
					insert(process[i]);

					bubbleSort(Ready);
					process[i]->state = ready;
				}

			}
		}

		if ((Running == NULL) && (Ready != NULL)) 
		{
			temp = Ready;
			Running = temp;
			Running->state = running;

			if (Running->first_run == 0)
				Running->first_run = 1;

			Ready = temp->next;
		}

		if (Running == NULL) 
		{
			fprintf(output, "<time %d> ---- system is idle ---- \n", time);
			idle++;
		}
		else {

			temp = Ready;
			while (temp != NULL) 
			{
				if (temp->first_run == 0) 
				{
					temp->response_time++;
				}

				temp->waiting_time++;

				temp = temp->next;
			}

			proc_run = Running;
			proc_run->burst_time -= 1;

			if (proc_run->burst_time == 0) 
			{
				fprintf(output, "<time %d> process %d is finished\n", time, Running->pid);
				Running->state = terminated;

				temp = Running;
				Running = NULL;

				terminate++;

				if (Ready != NULL)
					fprintf(output, "------------------------------------(Context-Switch) \n");
			}
			else
				fprintf(output, "<time %d> process %d is running \n", time, Running->pid);
			
			temp = Ready;
			while (temp != NULL) 
			{
				temp->priority = (temp->priority) + (alpha * temp->waiting_time);
				temp = temp->next;
			}
		}

		if (terminate == 10) 
		{
			fprintf(output, "<time %d> all processes finish \n===================================== \n", time);
			break;
		}
		time++;

	}
}

void print_process()
{

	int total_wt = 0;
	int total_rt = 0;

	for (int i = 0; i < 10; i++)
	{
		total_wt += process[i]->waiting_time;
		total_rt += process[i]->response_time;
	}


	fprintf(output, "Average cpu usage : %.2f %% \n", 100 * (float)(time - idle) / time);
	fprintf(output, "Average waiting time : %.2f \n", (float)total_wt / (float)10);
	fprintf(output, "Average response time : %.2f \n", (float)total_rt / (float)10);
	fprintf(output, "Average turnaround time : %.2f \n", (float)(total_wt + total_bt) / (float)10);

}


int main(int argc, char *argv[])
{
	if (argc != 5)
		printf("Invalid input");
	else
	{
		FILE *fp = fopen(argv[1], "r");
		output = fopen(argv[2], "w");

		if (fp == 0) {
			printf("Could not open file.\n");
			exit(1);
		}

		else {

			alpha = atoi(argv[3]);

			for (int i = 0; i < 10; i++) {

				process[i] = (struct pcb*)malloc(sizeof(struct pcb));

				fscanf(fp, "%d %f %d %d", &process[i]->pid, &process[i]->priority, &process[i]->arrival_time, &process[i]->burst_time);

				process[i]->state = 0;
				process[i]->response_time = 0;
				process[i]->waiting_time = 0;
				process[i]->first_run = 0;

				process[i]->next = NULL;

				total_bt += process[i]->burst_time;
			}

			fclose(fp);

			PPSA();
			print_process();

		}
	}

}