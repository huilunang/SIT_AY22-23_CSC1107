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

    int i;

    float floatWaitTime;
    float floatTurnTime;
    float floatResTime;

    float waitArray[5];
    float turnArray[5];
    float resArray[5];

    FILE *file = fopen("C:\\Users\\Keag\\Desktop\\SITstoofs\\Y1S3\\OS\\Q2_Group_Number.txt", "w");

    srand(time(NULL));

    struct Process processes[NUM_PROCESSES];
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

                turnArray[2] = 100;
                waitArray[2] = 100;
                resArray[2] = 100;
                turnArray[3] = 100;
                waitArray[3] = 100;
                resArray[3] = 100;
                turnArray[4] = 100;
                waitArray[4] = 100;
                resArray[4] = 100;

                printf("Shortest Average Turnaround Time(FCFS): %.2f\n", floatTurnTime);
                printf("Shortest Average Waiting Time(FCFS): %.2f\n", floatWaitTime);
                printf("Shortest Average Response Time(FCFS): %.2f\n", floatResTime);
                // Write output to the file
                fprintf(file, "Shortest Average Turnaround Time(FCFS): %.2f\n", floatTurnTime);
                fprintf(file, "Shortest Average Waiting Time(FCFS): %.2f\n", floatWaitTime);
                fprintf(file, "Shortest Average Response Time(FCFS): %.2f\n", floatResTime);
                fprintf(file, "\n");

                // sjf algo starts here

                sjfSort(processes);
                calculateTimes(processes);
                process(processes);

                floatTurnTime = avgTurnTime(processes);
                floatWaitTime = avgWaitTime(processes);
                floatResTime = avgResTime(processes);

                turnArray[1] = floatTurnTime;
                waitArray[1] = floatWaitTime;
                resArray[1] = floatResTime;

                printf("Shortest Average Turnaround Time(SJF): %.2f\n", floatTurnTime);
                printf("Shortest Average Waiting Time(SJF): %.2f\n", floatWaitTime);
                printf("Shortest Average Response Time(SJF): %.2f\n", floatResTime);
                // Write output to the file
                fprintf(file, "Shortest Average Turnaround Time(SJF): %.2f\n", floatTurnTime);
                fprintf(file, "Shortest Average Waiting Time(SJF): %.2f\n", floatWaitTime);
                fprintf(file, "Shortest Average Response Time(SJF): %.2f\n", floatResTime);
                fprintf(file, "\n");

                // Close the file
                fclose(file);

                for (int i = 0; i < 5; i++)
                {
                    printf("Element %d: %.3f\n", i, turnArray[i]);
                }
                float smallestTurn = findSmallest(turnArray, 5);
                float smallestWait = findSmallest(waitArray, 5);
                float smallestRes = findSmallest(resArray, 5);

                if (turnArray[0] == smallestTurn || waitArray[0] == smallestWait || resArray[0] == smallestRes)
                {
                    loop = 1;
                }
            }

            printf("Output written to the file successfully.\n");

            option = 1;

            break;

        case 2: // SJF being the shortest

            sjfSort(processes);
            calculateTimes(processes);
            process(processes);

            floatTurnTime = avgTurnTime(processes);
            floatWaitTime = avgWaitTime(processes);
            floatResTime = avgResTime(processes);

            printf("Shortest Average Turnaround Time(SJF): %.2f\n", floatTurnTime);
            printf("Shortest Average Waiting Time(SJF): %.2f\n", floatWaitTime);
            printf("Shortest Average Response Time(SJF): %.2f\n", floatResTime);
            // Write output to the file
            fprintf(file, "Shortest Average Turnaround Time(SJF): %.2f\n", floatTurnTime);
            fprintf(file, "Shortest Average Waiting Time(SJF): %.2f\n", floatWaitTime);
            fprintf(file, "Shortest Average Response Time(SJF): %.2f\n", floatResTime);
            fprintf(file, "\n");
            // Close the file
            fclose(file);

            printf("Output written to the file successfully.\n");

            option = 1;

            break;
        }
    }
    return 0;
}
