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

int total_wait;

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

	int total_burst = 0;

	temp = start;
	
	while (temp != NULL)
	{
		total_burst += temp->burst_time;

		temp = temp->next;
	}
	
	printf("Average waiting time : %.2f \n", (double)total_wait / (double)10);
	printf("Average response time : %.2f \n", (double)total_wait / (double)10);
	printf("Average turnaround time : %.2f \n", (double)(total_wait + total_burst) / (double)10);
}

void fcfs()
{
	printf("Scheduling : FCFS \n ========================================\n");

	int time = 0;
	int idle = 0;
		
	temp = start;

	for (time = 0; time < start->arrival_time; time++)
	{
		printf("<time %d> ----system is idle ----\n", time);

		idle++;
	}

	int num_arrival = 0; // 도착한 프로세스의 개수

	while (temp != NULL)
	{			
		for (int i = 0; i < temp->burst_time; i++)
		{
			for (int i = 0; i < 10; i++) { // 도착한 애들 알려주기
				if (process[i]->arrival_time == time)
				{
					printf("<time %d> [new arrival] process %d\n", time, process[i]->pid);

					num_arrival++; //도착했을 때마다 증가
				}
			}

			printf("<time %d> process %d is running\n", time, temp->pid);
			total_wait += (num_arrival - 1); // 도착한 애들 중에 실행 중인 애 하나 뺀 수가 지금 기다리고 있는 애들 수
			time++;
		}

		num_arrival--; // 한 프로세스가 종료되었다는 거니깐 도착한 애들에서 하나 빼기

		temp = temp->next;
		
		if ((num_arrival == 0) && (temp != NULL))
		{
			printf("<time %d> ----system is idle ----\n", time);

			time++;
			idle++;
		}
		else if(temp != NULL)
		{
			printf("------------------------------------(context-switch)\n");
		}

	}

	printf("<time %d> All process finish \n", time);

	printf("========================================\n");

	printf("Average cpu usage : %.2f %%\n", 100 * (double)(time - idle) / time);

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

			//print();
			fcfs();
			print_process();
			
		}
	
}
