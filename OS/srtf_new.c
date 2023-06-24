#include <stdio.h>

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
};

void srtf_schedule(struct Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    int shortest_process = 0;

    while (completed != n) {
        shortest_process = -1;
        int shortest_time = INT_MAX;

        // Find the process with the shortest remaining time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time < shortest_time && processes[i].remaining_time > 0) {
                shortest_process = i;
                shortest_time = processes[i].remaining_time;
            }
        }

        if (shortest_process == -1) {
            current_time++;
            continue;
        }

        // Decrement the remaining time of the shortest process
        processes[shortest_process].remaining_time--;

        // Check if the process has completed
        if (processes[shortest_process].remaining_time == 0) {
            completed++;
            int completion_time = current_time + 1;
            int turnaround_time = completion_time - processes[shortest_process].arrival_time;
            int waiting_time = turnaround_time - processes[shortest_process].burst_time;

            printf("Process %d: Completion Time = %d, Turnaround Time = %d, Waiting Time = %d\n",
                   processes[shortest_process].id, completion_time, turnaround_time, waiting_time);
        }

        current_time++;
    }
}

int main() {
    // Example processes
    struct Process processes[] = {
        {1, 0, 6, 6},
        {2, 1, 8, 8},
        {3, 2, 7, 7},
        {4, 3, 3, 3},
    };
    int num_processes = sizeof(processes) / sizeof(processes[0]);

    srtf_schedule(processes, num_processes);

    return 0;
}
