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

struct pcb *process[10];

void insert(struct pcb *process) {
	temp = start;
	new = (struct pcb *)malloc(sizeof(struct pcb));
	new->pid = process->pid;
	new->burst_time = process->burst_time;
	new->arrival_time = process->arrival_time;

	new->waiting_time = 0;
	new->turnaround_time = 0;
	new->next = NULL;
	
	if (start == NULL)
		start = new;
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
}

int main(int argc, char *argv[])
{

	if (argc < 5)
		printf("usage: %s filename", argv[0]);

	else {

		FILE *fp = fopen(argv[1], "r");

		if (fp == 0) {
			printf("Could not open file.\n");
			exit(1);
		}

		else {

			struct pcb *start = NULL;

			for (int i = 0; i < 10; i++) {

				process[i] = (struct pcb*)malloc(sizeof(struct pcb));

				fscanf(fp, "%d %d %d %d", &process[i]->pid, &process[i]->priority, &process[i]->arrival_time, &process[i]->burst_time);
				insert(process[i]);

			}



			fclose(fp);

		}
	}

}
