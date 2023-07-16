// FCFS
// SJF
// SRTF
// RR
// Priority Scheduling
// ctrl a+k+f

#include <stdio.h>
#include "sort.c"
#include <time.h>

int doAlgo(int choice);
main()

{
    int option = 0;
    int choice;
    int loop = 0;
    int x = 0;

    char name[5];

    int i, j;

    srand(time(NULL));

    struct Process temp;

    struct Process sortedProcesses[NUM_PROCESSES];

    while (option != 1)
    {

        printf("1)FCFS \n2)SJF \n3)SRTF \n4)RR \n5)Priority Scheduling: \n6)Exit Program \n");
        scanf("%d", &choice);

        int loop = 0;

        while (loop == 0)
        {
            loop = doAlgo(choice);
        }
        break;
    }
}

int doAlgo(int choice)
{
    int currentTime = 0;
    int smallTime[NUM_PROCESSES];
    struct Process processes[NUM_PROCESSES];
    struct Process processes2[NUM_PROCESSES];
    int temp2;
    float waitArray[5];
    float turnArray[5];
    float resArray[5];
    FILE *file = fopen("Q2_Group22.txt", "w");

    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 1;
    }
    randomProcess(processes);
   fprintf(file,"Process\tArrival Time\tBurst Time\tPriority\n");

    for (int i = 0; i < NUM_PROCESSES; i++)
    {
        processes2[i] = processes[i];
         fprintf(file, "P%d\t    %d\t           \t%d\t    \t%d\n", processes[i].processID,
             processes[i].arrivalTime, processes[i].burstTime, processes[i].priority);
    }
    fprintf(file,"\n");


    fcfsSort(processes);
    calculateTimes(processes);
    process(processes,file);
    turnArray[0] = writeFileTurn(processes, file, "FCFS");
    waitArray[0] = writeFileWait(processes, file, "FCFS");
    resArray[0] = writeFileRes(processes, file, "FCFS");

    // reset back to original
    reset(processes, processes2);

    // SJF start here
    fcfsSort(processes);
    burstTimeSort(processes, smallTime, temp2);
    sjfSort(processes, currentTime, temp2);
    calculateTimes(processes);
    turnArray[1] = writeFileTurn(processes, file, "SJF");
    waitArray[1] = writeFileWait(processes, file, "SJF");
    resArray[1] = writeFileRes(processes, file, "SJF");

    // reset back to original
    reset(processes, processes2);

    // SRTF start here
    srtfSort(processes);
    calculateTimes(processes);
    turnArray[2] = writeFileTurn(processes, file, "SRTF");
    waitArray[2] = writeFileWait(processes, file, "SRTF");
    resArray[2] = writeFileRes(processes, file, "SRTF");

    // reset back to original
    reset(processes, processes2);

    // RR start here
    rrSort(processes);
    calculateTimes(processes);
    turnArray[3] = writeFileTurn(processes, file, "RR");
    waitArray[3] = writeFileWait(processes, file, "RR");
    resArray[3] = writeFileRes(processes, file, "RR");

    // reset back to original
    reset(processes, processes2);

    // PS start here
    prioSort(processes);
    calculateTimes(processes);
    turnArray[4] = writeFileTurn(processes, file, "PS");
    waitArray[4] = writeFileWait(processes, file, "PS");
    resArray[4] = writeFileRes(processes, file, "PS");

    //find the fastest turnaround time OR waiting time OR response time out for the given choice
    float fastestTurn = findFast(turnArray, 5);
    float fastestWait = findFast(waitArray, 5);
    float fastestRes = findFast(resArray, 5);

    fclose(file);
    if (fastestTurn == turnArray[choice - 1] || fastestWait == waitArray[choice - 1] || fastestRes == resArray[choice - 1])
    {
        printf("\nOutput written to the file successfully.\n");
        return 1;
    }
}
