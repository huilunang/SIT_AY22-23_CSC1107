// FCFS
// SJF
// SRTF
// RR
// Priority Scheduling
// ctrl a+k+f

#include <stdio.h>
#include "sort.c"

main()

{
    int option;
    int choice;

    int i;
    srand(time(NULL));

    struct Process processes[NUM_PROCESSES];
    // Generate random arrival time, burst time, and priority for each process
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        processes[i].processID = i + 1;
        processes[i].arrivalTime = rand() % 10;
        processes[i].burstTime = rand() % 10 + 1;
        processes[i].priority = rand() % 5 + 1;
    }

    while (option != 1)
    {
        printf("1)FCFS \n2)SJF \n3)SRTF \n4)RR \n5)Priority Scheduling: \n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // FCFS

            fcfsSort(processes);
            calculateTimes(processes);
            process(processes);
            avgTurnTime(processes);
            avgWaitTime(processes);
            avgResTime(processes);
            option = 1;
            break;

        case 2: // SJF

            sjfSort(processes);
            calculateTimes(processes);
            process(processes);
            avgTurnTime(processes);
            avgWaitTime(processes);
            avgResTime(processes);

            break;

        }
    }
    return 0;
}
