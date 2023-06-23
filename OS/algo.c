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
    int option = 0;
    int choice;
    int loop = 0;

    int currentTime = 0;

    int i, j, temp2;

    float floatWaitTime;
    float floatTurnTime;
    float floatResTime;

    float waitArray[5];
    float turnArray[5];
    float resArray[5];

    // delete once rest of code is implemented
    turnArray[2] = 100;
    waitArray[2] = 100;
    resArray[2] = 100;
    turnArray[3] = 100;
    waitArray[3] = 100;
    resArray[3] = 100;
    turnArray[4] = 100;
    waitArray[4] = 100;
    resArray[4] = 100;

    FILE *file = fopen("C:\\Users\\Keag\\Desktop\\SITstoofs\\Y1S3\\OS\\Q2_Group_Number.txt", "w");

    srand(time(NULL));

    struct Process temp;
    struct Process processes[NUM_PROCESSES];
    struct Process sortedProcesses[NUM_PROCESSES];
    int smallTime[NUM_PROCESSES];
    // Generate random arrival time, burst time, and priority for each process

    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
    }

    while (option != 1)
    {

        printf("1)FCFS \n2)SJF \n3)SRTF \n4)RR \n5)Priority Scheduling: \n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // FCFS being the shortest
            loop = 0;
            while (loop != 1)
            {
                int x = 0;
                for (i = 0; i < NUM_PROCESSES; i++)
                {
                    processes[i].processID = i + 1;
                    processes[i].arrivalTime = rand() % 10 + x;
                    processes[i].burstTime = rand() % 10 + 1 + x;
                    processes[i].priority = rand() % 5 + 1 + x;
                }
                x += 1;
                fcfsSort(processes);
                calculateTimes(processes);
                process(processes);

                floatTurnTime = avgTurnTime(processes);
                floatWaitTime = avgWaitTime(processes);
                floatResTime = avgResTime(processes);

                turnArray[0] = floatTurnTime;
                waitArray[0] = floatWaitTime;
                resArray[0] = floatResTime;

                printf("Shortest Average Turnaround Time(FCFS): %.2f\n", floatTurnTime);
                printf("Shortest Average Waiting Time(FCFS): %.2f\n", floatWaitTime);
                printf("Shortest Average Response Time(FCFS): %.2f\n", floatResTime);
                // Write output to the file
                fprintf(file, "Shortest Average Turnaround Time(FCFS): %.2f\n", floatTurnTime);
                fprintf(file, "Shortest Average Waiting Time(FCFS): %.2f\n", floatWaitTime);
                fprintf(file, "Shortest Average Response Time(FCFS): %.2f\n", floatResTime);
                fprintf(file, "\n");

                // Close the file
                fclose(file);
                printf("Output written to the file successfully.\n");

                loop = 1;
            }
            break;
        case 2: // SJF being the shortest

            for (i = 0; i < NUM_PROCESSES; i++)
            {
                processes[i].processID = i + 1;
                processes[i].arrivalTime = rand() % 10;
                processes[i].burstTime = rand() % 10 + 1;
                processes[i].priority = rand() % 5 + 1;
            }
            // Sort the AT
            fcfsSort(processes);

            // Copy the BT
            for (i = 0; i < NUM_PROCESSES; i++)
            {
                smallTime[i] = processes[i].burstTime;
            }

            // Sort the BT Copy
            for (i = 0; i < NUM_PROCESSES; i++)
            {
                for (j = 0; j < NUM_PROCESSES - i - 1; j++)
                {
                    if (smallTime[j] > smallTime[j + 1])
                    {
                        // Swap the elements
                        temp2 = smallTime[j];
                        smallTime[j] = smallTime[j + 1];
                        smallTime[j + 1] = temp2;
                    }
                }
            }

            // Sorting by SJF
            sjfSort(processes, currentTime, temp2);

            // After sorting
            process(processes);

            printf("Output written to the file successfully.\n");

            option = 1;

            break;
        }
    }
    return 0;
}
