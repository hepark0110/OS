#include <stdio.h>
#include <stdlib.h>

struct pcb {
	int pid;

	int priority;

	int arrival_time;

	int burst_time;

	int response_time;

	int waiting_time;

	int turnaround_time;

	struct pcb *next;

};

struct pcb *start = NULL;
struct pcb *new;
struct pcb *temp = NULL;

struct pcb *process[10]; //job queue

int total_burst = 0, lst_arrival = 0;

void swap(struct pcb *a, struct pcb *b)
{
	int temp = a->pid;
	a->pid = b->pid;
	b->pid = temp;

	temp = a->arrival_time;
	a->arrival_time = b->arrival_time;
	b->arrival_time = temp;

	temp = a->burst_time;
	a->burst_time = b->burst_time;
	b->burst_time = temp;

	temp = a->response_time;
	a->response_time = b->response_time;
	b->response_time = temp;

	temp = a->waiting_time;
	a->waiting_time = b->waiting_time;
	b->waiting_time = temp;

	temp = a->turnaround_time;
	a->turnaround_time = b->turnaround_time;
	b->turnaround_time = temp;
}

// ready queue with a linked list
void insert(struct pcb *process) {

	temp = start;
	new = (struct pcb *)malloc(sizeof(struct pcb));

	new->pid = process->pid;
	new->burst_time = process->burst_time;
	new->arrival_time = process->arrival_time;
	new->waiting_time = 0;
	new->turnaround_time = 0;

	new->next = NULL;

	if (start == NULL) // 아무 것도 없으면
	{
		start = new;
	}
	else if (new->arrival_time < temp->arrival_time) // 맨 앞으로 가야할 때
	{
		new->next = temp;
		start = new;
	}
	else if(new->arrival_time > start->arrival_time) // 맨 앞은 아니면
	{
		if (temp->next == NULL) // start만 있고 뒤에 없으면 (하나만 있다는 거)
		{
			temp->next = new;
		}
		else { // 두 개 이상 있다는 거
			
			while (temp->next != NULL)
			{
				temp = temp->next;

				if (new->arrival_time < temp->arrival_time) // temp보다 작으면 자리 바꾸고 next도 바꾸고
				{
					new->next = temp->next;
					temp->next = new;

					swap(new, temp);

					break;
				}
			}
			if (temp->next == NULL) // new가 제일 큰 경우
			{
				temp->next = new;
			}
		}
	}
	
}
void print()
{
	temp = start;

	while (temp != NULL) {
		printf("%d", temp->pid);
		temp = temp->next;
	}
}

// print output
void print_process() {

	float avgWT = 0, avgTT = 0;

	temp = start;
	printf("=========================================================== \n");
	while (temp != NULL)
	{
		avgWT += temp->waiting_time;
		avgTT += temp->turnaround_time;
		temp = temp->next;
	}
	
	printf("Average cpu usage : %f %%\n", avgWT / 10);
	printf("Average waiting time : %f \n", avgWT / 10);
	printf("Average response time : %f \n", avgTT / 10);
	printf("Average turnaround time : %f \n", avgTT / 10);
}

void fcfs()
{
	printf("Scheduling : FCFS \n ========================================\n");

	int time = 0;
		
	temp = start;

	

	while (temp != NULL)
	{			
		
		
		for (int i = 0; i < 10; i++) { // 도착한 애들 알려주기
			if (process[i]->arrival_time == time) 
				printf("<time %d> [new arrival] process %d\n", time, process[i]->pid);
		}

		if (time >= temp->arrival_time)
		{
			printf("<time %d> process %d is running\n", time, temp->pid);
			
			temp->waiting_time = time - (temp->arrival_time + temp->burst_time);
			temp->turnaround_time = time - (temp->arrival_time);
			temp = temp->next;

		}
		else
		{
			printf("<time %d> ---- system is idle ----\n", time);
		}

		time++;

	}

}

int main()
{

		FILE *fp = fopen("input.txt", "r");

		if (fp == 0) {
			printf("Could not open file.\n");
			exit(1);
		}

		else {
			
			for (int i = 0; i < 10; i++) {

				process[i] = (struct pcb*)malloc(sizeof(struct pcb));

				fscanf(fp, "%d %d %d %d", &process[i]->pid, &process[i]->priority, &process[i]->arrival_time, &process[i]->burst_time);
				insert(process[i]);

			}

			fclose(fp);

			print();
			//fcfs();
			//print_process();
			
		}
	
}
