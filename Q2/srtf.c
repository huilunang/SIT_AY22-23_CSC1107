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
    int start_time;
    int end_time;
};
struct Node {
    int processID;
    int arrivalTime;
    int burstTime;
    struct Node *next;
};
struct Node* head = NULL;

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

void srtfSort(struct Process processes[])
{
    int n = NUM_PROCESSES;
    int current_time = 0;
    int completed = 0;
    int shortest_process = 0;
    int remaining_time[n];

    for (int i = 0; i < n; i++)
    {
        remaining_time[i] = processes[i].burstTime;
        processes[shortest_process].start_time = -1;
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

        if (processes[shortest_process].start_time == -1)
        {
            processes[shortest_process].start_time = current_time;
        }
        append(&head, processes[shortest_process].processID, processes[shortest_process].arrivalTime, processes[shortest_process].burstTime);
        // Decrement the remaining time of the shortest process
        remaining_time[shortest_process]--;

        // Check if the process has completed
        if (remaining_time[shortest_process] == 0)
        {
            completed++;
            processes[shortest_process].end_time = current_time + 1;
        }
        current_time++;
    }
}

void calculateTimes(struct Process processes[])
{
   // Waiting Time = Turnaround Time - Burst Time
   // Turnaround Time = Exit Time - Arrival Time
   // Response Time = Start Time - Arrival Time
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        processes[i].turnaroundTime = processes[i].end_time - processes[i].arrivalTime;
        processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
        processes[i].responseTime = processes[i].start_time - processes[i].arrivalTime;
    }
}


void printGanttChart(struct Process processes[])
{
    printf("\n  ");
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        for (int j = 0; j < processes[i].burstTime; j++)
        {
            if (j == 0)
            {
                printf("|");
            } else {
                printf(" ");
            }
            printf("P%1d", processes[i].processID);
        }
    }
    printf("|\n");
    int currentTime = 0;
    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        for (int j = 0; j < processes[i].burstTime; j++)
        {
            if (j == 0)
            {
                printf(" %2d", currentTime);
            } else {
                printf("   ");
            }
        }
        currentTime += processes[i].burstTime;
    }
    printf(" %2d", currentTime);
}

void printGanttChartPreemptive(struct Node *node)
{
    struct Node *nodeStart = node;
    printf("\n  |");
    while (node != NULL)
    {
        if (node->next == NULL)
        {
            printf("P%1d", node->processID);
            printf("|");
        }
        else
        {
            printf("P%1d", node->processID);
            if (node->processID != node->next->processID)
            {
                printf("|");
            }
            else
            {
                printf(" ");
            }
        }
        node = node->next;
    }
    printf("\n");
    node = nodeStart;
    int currentTime = 0;
    printf(" %2d", currentTime++);
    while (node != NULL)
    {
        if (node->next == NULL)
        {
            printf(" %2d", currentTime);
        }
        else
        {
            if (node->processID != node->next->processID)
            {
                printf(" %2d", currentTime);
            }
            else
            {
                printf("   ");
            }
        }
        currentTime++;
        node = node->next;
    }
}
void printList (struct Node *node)
{
    printf("\n");
    while (node != NULL)
    {
        printf(" %d ", node->processID);
        node = node->next;
    }
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
    srtfSort(processes);
    calculateTimes(processes);
    // Show the processes
    // printf("Processes:\n");
    /*
    for (int i = 0; i < num_processes; i++)
    {
        printf("\nProcess %d: Arrival Time = %d, Burst Time = %d, Priority = %d\n", processes[i].processID, processes[i].arrivalTime, processes[i].burstTime, processes[i].priority);
        printf("Process %d: Completion Time = %d, Turnaround Time = %d, Waiting Time = %d\n, Response Time = %d\n", processes[i].processID, processes[i].completionTime, processes[i].turnaroundTime, processes[i].waitingTime, processes[i].responseTime);
    }*/
    printGanttChart(processes);
    printGanttChartPreemptive(head);
    //printList(head);

    return 0;
}
