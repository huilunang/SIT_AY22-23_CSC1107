#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PROCESSES 6
#define TIME_QUANTUM 2

typedef struct {
    int process_id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int start_time;
    int end_time;
} Process;

void generateProcesses(Process processes[]) {
    srand(time(NULL));  // Seed the random number generator

    for (int i = 0; i < NUM_PROCESSES; i++) {
        processes[i].process_id = i + 1;

        // Random arrival time between 0 and 8 
        // TO DO: only 1-3 can have same
        processes[i].arrival_time = rand() % 9;   
        
        // Random burst time between 3 and 10
        // TO DO: only 1-3 can have same
        processes[i].burst_time = rand() % 8 + 3;  
        
        // Random priority between 1 and 4
        // TO DO: only 1-2 can have same
        processes[i].priority = rand() % 4 + 1;        

        // Initialize remaining time
        processes[i].remaining_time = processes[i].burst_time;  
        processes[i].start_time = -1;
        processes[i].end_time = 0;
    }
}

void displayProcesses(Process processes[]) {
    printf("Process\tArrival Time\tBurst Time\tPriority\n");
    printf("-------\t------------\t----------\t--------\n");

    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("%d\t%d\t\t%d\t\t%d\n", 
                processes[i].process_id, 
                processes[i].arrival_time,
                processes[i].burst_time, 
                processes[i].priority);
    }
}

void simulateRRScheduling(Process processes[]) {
    int total_time = 0;
    int completed_processes = 0;
    int current_time = 0;
    int next_process_id = 1;

    while (completed_processes < NUM_PROCESSES) {
        int selected_process = -1;

        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (processes[i].process_id == next_process_id && processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                selected_process = i;
                break;
            }
        }

        if (selected_process == -1) {
            for (int i = 0; i < NUM_PROCESSES; i++) {
                if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                    if (selected_process == -1) {
                        selected_process = i;
                        break;
                    }
                }
            }
        }

        if (selected_process == -1) {
            current_time++;
            continue;
        }

        if (processes[selected_process].start_time == -1) processes[selected_process].start_time = current_time;

        int execution_time = (processes[selected_process].remaining_time <= TIME_QUANTUM)
                                 ? processes[selected_process].remaining_time
                                 : TIME_QUANTUM;

        total_time += execution_time;
        processes[selected_process].remaining_time -= execution_time;
        current_time += execution_time;

        printf("Process %d executed for %d units of time.\n", processes[selected_process].process_id,
               execution_time);

        if (processes[selected_process].remaining_time == 0) {
            processes[selected_process].end_time = current_time;
            completed_processes++;
        }

        next_process_id = processes[selected_process].process_id + 1;
        if (next_process_id == 7) next_process_id = 1;

        // Perform Round Robin
        if (processes[selected_process].remaining_time > 0) {
            // Move the process to the end of the queue
            Process temp = processes[selected_process];
            for (int i = selected_process; i < NUM_PROCESSES - 1; i++) {
                processes[i] = processes[i + 1];
            }
            processes[NUM_PROCESSES - 1] = temp;
        }
    }

    printf("Total execution time: %d\n", total_time);
}

void generateFixedProcesses(Process processes[]) {
    processes[0].arrival_time = 0;
    processes[1].arrival_time = 2;
    processes[2].arrival_time = 8;
    processes[3].arrival_time = 0;
    processes[4].arrival_time = 2;
    processes[5].arrival_time = 0;

    processes[0].burst_time = 5;
    processes[1].burst_time = 3;
    processes[2].burst_time = 10;
    processes[3].burst_time = 4;
    processes[4].burst_time = 7;
    processes[5].burst_time = 4;

    processes[0].priority = 3;
    processes[1].priority = 4;
    processes[2].priority = 1;
    processes[3].priority = 3;
    processes[4].priority = 4;
    processes[5].priority = 1;

    for (int i = 0; i < NUM_PROCESSES; i++) {
        processes[i].process_id = i + 1;             

        // Initialize remaining time
        processes[i].remaining_time = processes[i].burst_time;  
        processes[i].start_time = -1;
        processes[i].end_time = 0;
    }
}

int main() {
    Process processes[NUM_PROCESSES];

    //generateProcesses(processes);
    generateFixedProcesses(processes);
    displayProcesses(processes);

    printf("\nSimulating Round-Robin Scheduling...\n\n");
    simulateRRScheduling(processes);

        printf("\nTurnaround Time\tWaiting Time\tResponse Time\tProcess ID\n");
    printf("---------------\t-------------\t--------------\t-----------\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\n", 
                processes[i].end_time - processes[i].arrival_time,  
                processes[i].end_time - processes[i].arrival_time - processes[i].burst_time, 
                processes[i].start_time - processes[i].arrival_time,
                processes[i].process_id);
    }

    // Turnaround Time = Exit Time - Arrival Time 
    // Waiting Time = Turnaround Time - Burst Time
    // Response Time = Start Time - Arrival Time

    return 0;
}


