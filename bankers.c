#include <stdio.h>
#include <stdlib.h>

FILE *output;

typedef enum proc_state {
	init = 0, true, false
} State;

struct pcb {
	int pid;

	int allocation[3];

	int max[3];

	int need[3];

	State finish;

	struct pcb *next;
};

struct pcb *process[5];

int num_of_resource;
int num_of_process;

int available[3];

int request_pid;
int request_resource[3];

struct pcb *start = NULL;
struct pcb *new;
struct pcb *temp = NULL;


void insert(struct pcb *process) {

	temp = start;
	new = (struct pcb *)malloc(sizeof(struct pcb));

	new->pid = process->pid;

	new->next = NULL;

	if (start == NULL) 
	{
		start = new;
	}
	else
	{
		while (temp->next != NULL) temp = temp->next;

		if (temp->next == NULL) temp->next = new;
	}

}

void print_proc()
{
	for (int i = 0; i < num_of_process; i++)
	{
		fprintf(output, "The process pid : %d \n\n", process[i]->pid);
		fprintf(output, "The process allocation : %d \t %d \t %d \n\n", process[i]->allocation[0], process[i]->allocation[1], process[i]->allocation[2]);
		fprintf(output, "The process max : %d \t %d \t %d \n\n\n", process[i]->max[0], process[i]->max[1], process[i]->max[2]);
	}
}

void print_state()
{
	temp = start;

	while (temp != NULL)
	{
		fprintf(output, "p%d ", temp->pid);

		if (temp->next != NULL)
			fprintf(output, "-> ");

		temp = temp->next;
	}

	fprintf(output, "\n");
}

void need_arr()
{
	for (int i = 0; i < num_of_process; i++)
	{
		process[i]->need[0] = process[i]->max[0] - process[i]->allocation[0];
		process[i]->need[1] = process[i]->max[1] - process[i]->allocation[1];
		process[i]->need[2] = process[i]->max[2] - process[i]->allocation[2];
	}
}

int request()
{
	for (int i = 0; i < 3; i++)
	{
		available[i] -= request_resource[i];

		process[request_pid - 1]->allocation[i] += request_resource[i];
	}

	return 0;
}

int compare(struct pcb *process)
{
	for (int i = 0; i < 3; i++)
	{
		if (available[i] < process->need[i])
			return 1;
	}
	return 0;
}

void bankers()
{
	int check; //Determining whether allocable or not
	int count = 0; // Variables that count processes that fail to allocate
	int finish_proc = 0;
	int i = 0;

	while (1)
	{
		if (process[i % 5]->finish == false)
		{
			check = compare(process[i % 5]);

			if (check == 1) // failed to allocate
			{
				count++;
				
				if (count == (5 - finish_proc))
				{
					fprintf(output, "UNSAFE \n");

					break;
				}
			}
			else if (check == 0) // success to allocate
			{
				process[i % 5]->finish = true;
				
				finish_proc++;

				for (int j = 0; j < 3; j++)
				{
					available[j] += process[i % 5]->allocation[j];
				}

				insert(process[i % 5]);
			}
		}
		else if ((process[i % 5]->finish == true) && (finish_proc == 5))
		{
			print_state();
			break;
		}

		i++;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 5)
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

			num_of_resource = atoi(argv[3]);
			num_of_process = atoi(argv[4]);

			fscanf(fp, "%d %d %d", &available[0], &available[1], &available[2]);

			int initial_1, initial_2, initial_3;
			initial_1 = available[0];
			initial_2 = available[1];
			initial_3 = available[2];

			for (int i = 0; i < num_of_process; i++) {

				process[i] = (struct pcb*)malloc(sizeof(struct pcb));

				fscanf(fp, "%d %d %d %d %d %d %d", &process[i]->pid, &process[i]->allocation[0], &process[i]->allocation[1], &process[i]->allocation[2],
					&process[i]->max[0], &process[i]->max[1], &process[i]->max[2]);

				process[i]->finish = false;

				process[i]->next = NULL;

			}

			fscanf(fp, "%d	%d %d %d", &request_pid, &request_resource[0], &request_resource[1], &request_resource[2]);

			fclose(fp);

			
			print_proc();


			fprintf(output, "Current State : ");
			need_arr();
			bankers();


			// initialize
			available[0] = initial_1;
			available[1] = initial_2;
			available[2] = initial_3;

			for (int i = 0; i < num_of_process; i++) 
			{
				process[i]->finish = false;
			}

			start = NULL;


			fprintf(output, "After Arriving Request : ");
			request();
			need_arr();
			bankers();


		}
	
	}
}