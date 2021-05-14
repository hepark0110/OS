#include <stdio.h>
#include <stdlib.h>

struct pcb {
	int pid;

	int priority;

	int arrival_time;

	int burst_time;

	int first_run; // 처음으로 실행하는 건지 판단하기 위해

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
void insert(struct pcb *process) { // 들어오면 linked list 맨 뒤로

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

int terminate = 0; // 종료된 프로세스의 개수를 세기 위해

void RR()
{
	printf("Scheduling : RR \n======================================== \n");

	while (terminate != 10)
	{
		if (start == NULL) // ready queue에 아무것도 없다는 것
		{
			for (int i = 0; i < 10; i++)
			{
				if (process[i]->arrival_time == time) // 누가 도착!
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

					if (start->first_run == 0) // 만약 이게 첫 실행이었으면 0을 1로 바꿔줌 (이제 다음부턴 첫 실행 아니니깐)
						start->first_run = 1;

					
					temp = start->next; // 실행 중인 프로세스 다음부터 

					while (temp != NULL)
					{
						total_wt++; // 실행 중인 프로세스 빼고 다 waiting time 증가

						if (temp->first_run == 0) total_rt++; // 아직 한 번도 실행 못한 프로세스들은 response time도 증가

						temp = temp->next;
					}

					for (int i = 0; i < 10; i++) // 이 와중에 또 누구 도착하면 알림
					{
						if (process[i]->arrival_time == time)
						{
							printf("<time %d> [new arrival] process %d \n", time, process[i]->pid);

							insert(process[i]);
						}
					}

					time++;
				}

				printf("<time %d> process %d is finished \n", time, start->pid); // 마지막 실행이었으니깐 프로세스 종료

				terminate++; // 종료 프로세스 개수 증가
				

				if (terminate < 10) printf("--------------------------------------(Context-Switch) \n"); // 아직 다 종료된 거 아니면 context switch

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
