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
    int x = 0;
    int currentTime = 0;

    char name[5];
    int smallTime[NUM_PROCESSES];
    int i, j, temp2;

    srand(time(NULL));

    struct Process temp;
    struct Process processes[NUM_PROCESSES];
    struct Process sortedProcesses[NUM_PROCESSES];

    struct Process processes2[NUM_PROCESSES];

    while (option != 1)
    {

        printf("1)FCFS \n2)SJF \n3)SRTF \n4)RR \n5)Priority Scheduling: \n6)Exit Program \n");
        scanf("%d", &choice);
        FILE *file = fopen("C:\\Users\\Keag\\Desktop\\SITstoofs\\Y1S3\\OS\\Q2_Group_Number.txt", "w");

        if (file == NULL)
        {
            printf("Failed to open the file.\n");
            return 1;
        }
        switch (choice)
        {
        case 1: // FCFS being the shortest
            //randomise the values
            randomProcess(processes);
            for(int i =0;i<6;i++){
                processes2[i]=processes[i];
            }
            
            process(processes);
            process(processes2);

            fcfsSort(processes);
            calculateTimes(processes);
            process(processes);
            writeFile(processes, file, "FCFS");
            
            //reset back to original
            reset(processes,processes2);

            //SJF start here
            fcfsSort(processes);
            burstTimeSort(processes, smallTime, temp2);
            sjfSort(processes, currentTime, temp2);
            calculateTimes(processes);
            process(processes);
            writeFile(processes, file, "SJF");

            //reset back to original
            reset(processes,processes2);

            //SRTF start here
            srtfSort(processes);
            calculateTimes(processes);
            process(processes);
            writeFile(processes, file, "SRTF");
            
            //reset back to original
            reset(processes,processes2);
            //RR start here

            //PS start here
            prioSort(processes);
            calculateTimes(processes);
            process(processes);
            writeFile(processes, file, "PS");

            fclose(file);

            break;
        case 2: // SJF being the shortest

            randomProcess(processes);
            // Sort the AT
            fcfsSort(processes);
            burstTimeSort(processes, smallTime, temp2);
            // Sorting by SJF
            sjfSort(processes, currentTime, temp2);
            calculateTimes(processes);
            // After sorting
            process(processes);
            writeFile(processes, file, "SJF");
            break;

        case 3: // srtf
            randomProcess(processes);
            srtfSort(processes);
            calculateTimes(processes);
            process(processes);
            writeFile(processes, file, "SRTF");
            break;
        case 4: // RR
            printf("1");
            randomProcess(processes);
            printf("2");
            rrSort(processes);
            printf("3");

            calculateTimes(processes);
            printf("4");
            process(processes);
            writeFile(processes, file, "RR");

            break;
        case 5:
            randomProcess(processes);
            prioSort(processes);
            calculateTimes(processes);
            process(processes);
            writeFile(processes, file, "PS");
            break;
        case 6:
            return 0;
        default:
            printf("Incorrect input\n");
        }

        // return 0;
    }
}
