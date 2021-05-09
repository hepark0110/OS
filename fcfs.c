#include <stdio.h>
#include <stdlib.h>

typedef enum process_state {init=0, ready, running, terminate} ProcState;

typedef struct process {
	int pid;

	int priority;

	int arrival_time;

	int burst_time;

	ProcState state;

	PCB *next;

} PCB;

PCB * pcb[10];

void bubbleSort(PCB *start)
{
    int swapped, i;
    PCB *ptr1;
    PCB *lptr = NULL;
  
    if (start == NULL)
        return;
  
    do
    {
        swapped = 0;
        ptr1 = start;
  
        while (ptr1->next != lptr)
        {
            if (ptr1->arrival_time > ptr1->next->arrival_time)
            { 
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}

void swap(PCB *a, PCB *b)
{
    int temp = a->arrival_time;
    a->arrival_time = b->arrival_time;
    b->arrival_time = temp;
}

void print_process(PCB *pcb)
{
    printf("Scheduling : FCFS \n ==============================================\n");

	PCB *temp = pcb;

	while (temp != NULL) {
        printf("<time > pid: %d process is running. \n", temp->pid);
        temp = temp->next;
    }
}


int main(int argc, char *argv[])
{
	// printf("Scheduling : FCFS \n ==============================================\n");

	if (argc != 2)
		printf("usage: %s filename", argv[0]);

	else {

		FILE *fp = fopen(argv[1], "r");

		if (fp == 0) {
			printf("Could not open file.\n");
			exit(1);
		}

		else {
            
			int id, pri, arr_t, bur_t;

			PCB *start = NULL;
			
			for (int i=0; i<10; i++) {

		        fscanf(fp, "%d %d %d %d", &pcb[i]->pid, &pcb[i]->priority, &pcb[i]->arrival_time, &pcb[i]->burst_time);

				if(i==9) pcb[i]->next = NULL;

                else pcb[i]->next = pcb[i+1];

	        }

			bubblesort(pcb[0]);

			print_process(pcb[0]);

			fclose(fp);

		}
	}

}

/*
#include<stdio.h>
// Struct Storing all the Processes
struct process
{
	int process;
	int arrival;
	int burst;
	int wait;
	int response;
	int tat;
}p[10];

// Main Function
int main()
{
    int n,avwt=0,avtat=0,i,j,pos,max,temp,sum=0;
    printf("\nEnter total number of processes:");
    scanf("%d",&n);

// Taking the Input from the User
    for(i=0;i<n;i++)
    {
	p[i].process = i+1;
    	printf("\n Process [%d] :\n",i+1);
        printf("\n Arrival Time:");
        scanf("%d",&p[i].arrival);
        printf("\n Burst Time:");
        scanf("%d",&p[i].burst);
    }
    
// Sorting all the Processes according to their Arrival Time
    
    for(i=0;i<n;i++)
    {
        pos=i;
        for(j=i+1;j<n;j++)
        {
            if(p[j].arrival<p[pos].arrival)
            {
                pos=j;
            }
        }
 
        temp=p[i].arrival;
        p[i].arrival=p[pos].arrival;
        p[pos].arrival=temp;

        temp=p[i].burst;
        p[i].burst=p[pos].burst;
        p[pos].burst=temp;
 
        temp=p[i].process;
        p[i].process=p[pos].process;
        p[pos].process=temp;
    
    }
    p[0].wait = 0;
// Calculating Waiting and Response Time
    for(i=1;i<n;i++)
    {
	sum = sum + p[i-1].burst;
        p[i].wait = sum - p[i].arrival ; 
	if(p[i].wait <0)
		p[i].wait= 0;
        p[i].response = p[i].wait;
    }
 
    printf("\nProcess\t\tArrival Time\tBurst Time\tResponse Time\tWaiting Time\tTurnaround Time");
 
// Calculating Turn Around Time and Printing the Table
    for(i=0;i<n;i++)
    {
        p[i].tat=p[i].burst+p[i].wait;
        avwt+=p[i].wait;
        avtat+=p[i].tat;
        printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d",p[i].process,p[i].arrival,p[i].burst,p[i].response,p[i].wait,p[i].tat);
    }
 
    avwt/=i;
    avtat/=i;
    printf("\n\nAverage Waiting Time:%d",avwt);
    printf("\nAverage Turnaround Time:%d\n\n",avtat);
 
    return 0;
}
*/