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

float avgTurnTime(struct Process processes[])
{
   float sumTurnaroundTime = 0;
   int i;

   for (i = 0; i < NUM_PROCESSES; i++)
   {
      sumTurnaroundTime += processes[i].turnaroundTime;
   }

   float avgTurnaroundTime = sumTurnaroundTime / NUM_PROCESSES;

   return avgTurnaroundTime;
}

float avgWaitTime(struct Process processes[])
{
   float sumWaitingTime = 0;
   int i;

   for (i = 0; i < NUM_PROCESSES; i++)
   {
      sumWaitingTime += processes[i].waitingTime;
   }

   float avgWaitingTime = sumWaitingTime / NUM_PROCESSES;

   return avgWaitingTime;
}

float avgResTime(struct Process processes[])
{
   float sumResponseTime = 0;
   int i;

   for (i = 0; i < NUM_PROCESSES; i++)
   {
      sumResponseTime += processes[i].responseTime;
   }

   float avgResponseTime = sumResponseTime / NUM_PROCESSES;

   return avgResponseTime;
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

void sjfSort(struct Process processes[], int currentTime, int temp2)
{
   currentTime = processes[0].arrivalTime + processes[0].burstTime;
   for (int i = 1; i < NUM_PROCESSES; i++)
   {
      if (processes[i].arrivalTime > currentTime)
      {
         // At the current index, there does not exist a process
         // that has arrived and is waiting to be processed
         // Therefore, we move the currentTime to the arrival time of
         // the current index since it is sorted by fastest arrival
         // Then we proceed as per usual

         currentTime = processes[i].arrivalTime;
      }

      int fastestTime = processes[i].burstTime;
      int fastestIndex = i;

      // Loop through the remaining items to find the fastest burst time
      for (int j = i + 1; j < NUM_PROCESSES; j++)
      {
         // If there exists a process that has arrived and is waiting
         if (processes[j].arrivalTime <= currentTime)
         {
            // If that process has a faster burst time
            if (processes[j].burstTime < fastestTime)
            {
               // Take note of the timing and index
               fastestTime = processes[j].burstTime;
               fastestIndex = j;
            }
         }
      }

      // Swap the contents of the current index with the contents of the fastest index
      currentTime += processes[fastestIndex].burstTime;
      temp2 = processes[fastestIndex].arrivalTime;
      processes[fastestIndex].arrivalTime = processes[i].arrivalTime;
      processes[i].arrivalTime = temp2;

      temp2 = processes[fastestIndex].burstTime;
      processes[fastestIndex].burstTime = processes[i].burstTime;
      processes[i].burstTime = temp2;
   }
}

void srtfSort(struct Process processes[])
{
   int n = NUM_PROCESSES;
   int current_time = 0;
   int completed = 0;
   int shortest_process = 0;
   int remaining_time[n];
   int start_time[n];

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
      if (start_time[shortest_process] == NULL)
         start_time[shortest_process] = current_time;

      // Decrement the remaining time of the shortest process
      remaining_time[shortest_process]--;

      // Check if the process has completed
      if (remaining_time[shortest_process] == 0)
      {
         completed++;
         processes[shortest_process].completionTime = current_time + 1;
         processes[shortest_process].turnaroundTime = processes[shortest_process].completionTime - processes[shortest_process].arrivalTime;
         processes[shortest_process].waitingTime = processes[shortest_process].turnaroundTime - processes[shortest_process].burstTime;
         processes[shortest_process].responseTime = processes[shortest_process].arrivalTime;
         processes[shortest_process].responseTime = start_time[shortest_process] - processes[shortest_process].arrivalTime;
         // printf("Process %d: Completion Time = %d, Turnaround Time = %d, Waiting Time = %d\n",processes[shortest_process].processID, processes[shortest_process].completionTime, processes[shortest_process].turnaroundTime, processes[shortest_process].waitingTime);
      }
      current_time++;
   }
}

void prioSort(struct Process processes[], int n)
{
   int current_time = 0;
   int completed = 0;
   int prioritized_process = 0;
   int remaining_time[n];
   int start_time[n];

   for (int i = 0; i < n; i++)
   {
      remaining_time[i] = processes[i].burstTime;
   }

   while (completed != n)
   {
      prioritized_process = -1;
      int priority = INT_MAX;

      // Find the process with the shortest remaining time
      for (int i = 0; i < n; i++)
      {
         if (processes[i].arrivalTime <= current_time && processes[i].priority < priority && remaining_time[i] > 0)
         {
            prioritized_process = i;
            priority = processes[i].priority;
         }
      }

      if (prioritized_process == -1)
      {
         current_time++;
         continue;
      }

      if (start_time[prioritized_process] == NULL)
         start_time[prioritized_process] = current_time;

      // Decrement the remaining time of the shortest process
      remaining_time[prioritized_process]--;

      // Check if the process has completed
      if (remaining_time[prioritized_process] == 0)
      {
         completed++;
         processes[prioritized_process].completionTime = current_time + 1;
         processes[prioritized_process].turnaroundTime = processes[prioritized_process].completionTime - processes[prioritized_process].arrivalTime;
         processes[prioritized_process].waitingTime = processes[prioritized_process].turnaroundTime - processes[prioritized_process].burstTime;
         processes[prioritized_process].responseTime = start_time[prioritized_process] - processes[prioritized_process].arrivalTime;

      }
      current_time++;
   }
}





float findSmallest(float array[], int size)
{
   float smallest = array[0]; // Assume the first element is the smallest

   for (int i = 1; i < size; i++)
   {
      if (array[i] < smallest)
      {
         smallest = array[i]; // Update the smallest value
      }
   }

   return smallest;
}

void writeFile(struct Process processes[], FILE *file, char name[])
{

   float floatTurnTime = avgTurnTime(processes);
   float floatWaitTime = avgWaitTime(processes);
   float floatResTime = avgResTime(processes);

   float waitArray[5];
   float turnArray[5];
   float resArray[5];

   turnArray[0] = floatTurnTime;
   waitArray[0] = floatWaitTime;
   resArray[0] = floatResTime;

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

   printf("Shortest Average Turnaround Time(%s): %.2f\n", name, floatTurnTime);
   printf("Shortest Average Waiting Time(%s): %.2f\n", name, floatWaitTime);
   printf("Shortest Average Response Time(%s): %.2f\n", name, floatResTime);
   // Write output to the file
   fprintf(file, "Shortest Average Turnaround Time(%s): %.2f\n", name, floatTurnTime);
   fprintf(file, "Shortest Average Waiting Time(%s): %.2f\n", name, floatWaitTime);
   fprintf(file, "Shortest Average Response Time(%s): %.2f\n", name, floatResTime);
   fprintf(file, "\n");

   // Close the file
   fclose(file);
   printf("Output written to the file successfully.\n");
}

void randomProcess(struct Process processes[])
{
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      processes[i].processID = i + 1;
      processes[i].arrivalTime = rand() % 10;
      processes[i].burstTime = rand() % 10 + 1;
      processes[i].priority = rand() % 5 + 1;
   }
}
