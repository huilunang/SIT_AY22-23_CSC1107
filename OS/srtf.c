#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define NUM_PROCESSES 4

struct Process
{
    int processID;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    int completionTime;
};
struct Node {
    int processID;
    int arrivalTime;
    int burstTime;
    struct Node *next;
};

void append(struct Node **head_ref, int processID, int arrivalTime, int burstTime)
{
    // 1. Allocate node
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    // Used in step 5
    struct Node *last = *head_ref;

    // 2. Put in the data
    new_node->processID = processID;
    new_node->arrivalTime = arrivalTime;
    new_node->burstTime = burstTime;

    // 3. This new node is going to be the
    //    last node, so make next of it as NULL
    new_node->next = NULL;

    // 4. If the Linked List is empty, then make
    //    the new node as head
    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;
    }

    // 5. Else traverse till the last node
    while (last->next != NULL)
        last = last->next;

    // 6. Change the next of last node
    last->next = new_node;
    return;
}
void printList(struct Node *node)
{
    printf("Printing list:");
    while (node != NULL)
    {
        printf(" %d ", node->processID);
        node = node->next;
    }
}


void srtfSort(struct Process processes[], struct Node *head)
{
    int n = NUM_PROCESSES;
    int current_time = 0;
    int completed = 0;
    int shortest_process = 0;
    int remaining_time[n];
    int start_time[n];

    for (int i = 0; i < n; i++)
    {
        remaining_time[i] = processes[i].burstTime;
        start_time[i] = -1;
    }

    while (completed != n)
    {
        shortest_process = -1;
        int shortest_time = INT_MAX;

        // Find the process with the shortest remaining time
        for (int i = 0; i < n; i++)
        {
            if (processes[i].arrivalTime <= current_time && remaining_time[i] < shortest_time && remaining_time[i] > 0)
            {
                shortest_process = i;
                shortest_time = remaining_time[i];
            }
        }
        
        if (shortest_process == -1)
        {
            current_time++;
            continue;
        }
        printf("running process: %d\n current time: %d\n", shortest_process + 1, current_time);
        if (start_time[shortest_process] == -1)
        {
            start_time[shortest_process] = current_time;
        }
        // append(&head, processes[shortest_process].processID, processes[shortest_process].arrivalTime, processes[shortest_process].burstTime);

        // Decrement the remaining time of the shortest process
        remaining_time[shortest_process]--;

        // Check if the process has completed
        if (remaining_time[shortest_process] == 0)
        {
            completed++;
            processes[shortest_process].completionTime = current_time + 1;
            processes[shortest_process].turnaroundTime = processes[shortest_process].completionTime - processes[shortest_process].arrivalTime;
            processes[shortest_process].waitingTime = processes[shortest_process].turnaroundTime - processes[shortest_process].burstTime;
            processes[shortest_process].responseTime = start_time[shortest_process] - processes[shortest_process].arrivalTime;
            // printf("Process %d: Completion Time = %d, Turnaround Time = %d, Waiting Time = %d\n",processes[shortest_process].processID, processes[shortest_process].completionTime, processes[shortest_process].turnaroundTime, processes[shortest_process].waitingTime);
        }
        
        current_time++;
    }
}

void calculateTimes(struct Process processes[])
{
    int currentTime = 0;
    int i;

    for (i = 0; i < NUM_PROCESSES; i++)
    {
        if (currentTime < processes[i].arrivalTime)
        {
            currentTime = processes[i].arrivalTime;
        }

        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        processes[i].responseTime = processes[i].waitingTime;

        currentTime += processes[i].burstTime;
    }
}

void printGanttChart(struct Process processes[], int n)
{
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf(" |");
        for (int j = 0; j < processes[i].burstTime; j++)
        {
            printf(" P%1d", processes[i].processID);
        }
    }
    printf(" |\n");
    int currentTime = 0;
    for (int i = 0; i < n; i++)
    {
        printf("%2d", currentTime);
        for (int j = 0; j < processes[i].burstTime; j++)
        {
            printf("   ", processes[i].processID);
        }
        currentTime += processes[i].burstTime;
    }
    printf("%2d", currentTime);
}
void printGanttChartPreemptive(int process, int currentTime, int end)
{

}

int main()
{
    // Example processes
    struct Process processes[] = {
        {1, 3, 6, 6, 0, 0, 0, 0},
        {2, 0, 8, 8, 0, 0, 0, 0},
        {3, 2, 7, 7, 0, 0, 0, 0},
        {4, 1, 3, 3, 0, 0, 0, 0},
    };
    struct Node* head = NULL;
    int num_processes = sizeof(processes) / sizeof(processes[0]);
    srtfSort(processes, num_processes, head);
    // Show the processes
    printf("Processes:\n");
    for (int i = 0; i < num_processes; i++)
    {
        printf("\nProcess %d: Arrival Time = %d, Burst Time = %d, Priority = %d\n", processes[i].processID, processes[i].arrivalTime, processes[i].burstTime, processes[i].priority);
        printf("Process %d: Completion Time = %d, Turnaround Time = %d, Waiting Time = %d\n, Response Time = %d\n", processes[i].processID, processes[i].completionTime, processes[i].turnaroundTime, processes[i].waitingTime, processes[i].responseTime);
    }
    printList(head);
    //printGanttChart(processes, num_processes);

    return 0;
}
