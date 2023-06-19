// FCFS
// SJF
// SRTF
// RR
// Priority Scheduling
// ctrl a+k+f

#include <stdio.h>
#include "fcfs.c"

int printRandom(int lower, int upper, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        int num = (rand() %
                   (upper - lower + 1)) +
                  lower;
        return num;
    }
}

main()

{
    int option = 0;
    int choice;

    srand(time(0));
    int arrTime[] = {};
    int burstTime[] = {};
    int priority[] = {};
    int proc[] = {1, 2, 3, 4, 5, 6};

    /*
                int i;
                for (i = 0; i < 6; i++)
                {
                    arrTime[i] = printRandom(1, 10, 1);
                    burstTime[i] = printRandom(1, 10, 1);
                    priority[i] = printRandom(1, 10, 1);
                }     */

    while (option != 1)
    {
        printf("1)FCFS \n2)SJF \n3)SRTF \n4)RR \n5)Priority Scheduling: \n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // FCFS
            int n = sizeof proc / sizeof proc[0];
            int i;
            for (i = 0; i < 6; i++)
            {
                priority[i] = printRandom(1, 10, 1);
                burstTime[i] = printRandom(1, 10, 1);
            }
            avgtime(proc, n, burstTime);
            return 0;
            break;

        case 2: // SJF


        }
    }
    return 0;
}
