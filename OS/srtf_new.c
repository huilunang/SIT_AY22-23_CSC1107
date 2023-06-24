#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define NUM_PROCESSES 6


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

void srtfSort(struct Process processes[], int n)
{
    int current_time = 0;
    int completed = 0;
    int shortest_process = 0;
    int remaining_time[n];

    for (int i = 0; i < n; i++)
    {
        remaining_time[i] = processes[i].burstTime;
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

        // Decrement the remaining time of the shortest process
        remaining_time[shortest_process]--;

        // Check if the process has completed
        if (remaining_time[shortest_process] == 0)
        {
            completed++;
            processes[shortest_process].completionTime = current_time + 1;
            processes[shortest_process].turnaroundTime = processes[shortest_process].completionTime - processes[shortest_process].arrivalTime;
            processes[shortest_process].waitingTime = processes[shortest_process].turnaroundTime - processes[shortest_process].burstTime;
            //printf("Process %d: Completion Time = %d, Turnaround Time = %d, Waiting Time = %d\n",processes[shortest_process].processID, processes[shortest_process].completionTime, processes[shortest_process].turnaroundTime, processes[shortest_process].waitingTime);
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

void print_gantt_chart(struct Process processes[], int n) {
    printf("Gantt Chart:\n");

    // Find the maximum completion time
    int max_completion_time = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].completionTime > max_completion_time) {
            max_completion_time = processes[i].completionTime;
        }
    }

    // Print the top line of the Gantt chart
    printf(" ");
    for (int i = 0; i < max_completion_time; i++) {
        printf("--");
    }
    printf("\n");

    // Print the process IDs and bars
    printf("|");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processes[i].burstTime; j++) {
            printf("P%d", processes[i].processID);
        }
        printf("|");
    }
    printf("\n");

    // Print the bottom line of the Gantt chart
    printf(" ");
    for (int i = 0; i < max_completion_time; i++) {
        printf("--");
    }
    printf("\n");
}

int main()
{
    // Example processes
    struct Process processes[] = {
        {1, 0, 6, 6, 0, 0, 0, 0},
        {2, 1, 8, 8, 0, 0, 0, 0},
        {3, 2, 7, 7, 0, 0, 0, 0},
        {4, 3, 3, 3, 0, 0, 0, 0},
    };
    int num_processes = sizeof(processes) / sizeof(processes[0]);
    srtfSort(processes, num_processes);
    // Show the processes
    printf("Processes:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("\nProcess %d: Arrival Time = %d, Burst Time = %d, Priority = %d\n", processes[i].processID, processes[i].arrivalTime, processes[i].burstTime, processes[i].priority);
        printf("Process %d: Completion Time = %d, Turnaround Time = %d, Waiting Time = %d\n",processes[i].processID, processes[i].completionTime, processes[i].turnaroundTime, processes[i].waitingTime);
    }
    print_gantt_chart(processes, num_processes);

    return 0;
}
