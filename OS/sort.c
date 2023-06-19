#include <stdio.h>
#include <stdlib.h>

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
};

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

void process(struct Process processes[])
{
   int i;

   printf("Process\tArrival Time\tBurst Time\tPriority\n");
   for (i = 0; i < NUM_PROCESSES; i++)
   {
      printf("P%d\t%d\t\t%d\t\t%d\n", processes[i].processID,
             processes[i].arrivalTime, processes[i].burstTime, processes[i].priority);
   }
}

void avgTurnTime(struct Process processes[])
{
   float sumTurnaroundTime = 0;
   int i;

   for (i = 0; i < NUM_PROCESSES; i++)
   {
      sumTurnaroundTime += processes[i].turnaroundTime;
   }

   float avgTurnaroundTime = sumTurnaroundTime / NUM_PROCESSES;
   printf("\nShortest Average Turnaround Time: %.2f\n", avgTurnaroundTime);
}

void avgWaitTime(struct Process processes[])
{
   float sumWaitingTime = 0;
   int i;

   for (i = 0; i < NUM_PROCESSES; i++)
   {
      sumWaitingTime += processes[i].waitingTime;
   }

   float avgWaitingTime = sumWaitingTime / NUM_PROCESSES;
   printf("Shortest Average Waiting Time: %.2f\n", avgWaitingTime);
}

void avgResTime(struct Process processes[])
{
   float sumResponseTime = 0;
   int i;

   for (i = 0; i < NUM_PROCESSES; i++)
   {
      sumResponseTime += processes[i].responseTime;
   }

   float avgResponseTime = sumResponseTime / NUM_PROCESSES;
   printf("Shortest Average Response Time: %.2f\n", avgResponseTime);
}

void fcfsSort(struct Process processes[])
{
   int i, j;
   struct Process temp;

   for (i = 0; i < NUM_PROCESSES - 1; i++)
   {
      for (j = i + 1; j < NUM_PROCESSES; j++)
      {
         if (processes[i].arrivalTime > processes[j].arrivalTime)
         {
            temp = processes[i];
            processes[i] = processes[j];
            processes[j] = temp;
         }
      }
   }
}


void sjfSort(struct Process processes[])
{
   int i, j;
   struct Process temp;

   for (i = 0; i < NUM_PROCESSES - 1; i++)
   {
      for (j = i + 1; j < NUM_PROCESSES; j++)
      {
         if (processes[i].burstTime > processes[j].burstTime)
         {
            temp = processes[i];
            processes[i] = processes[j];
            processes[j] = temp;
         }
      }
   }
}
