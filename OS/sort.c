#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_PROCESSES 6
#define TIME_QUANTUM 2

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
   int remaining_time;
   int start_time;
   int end_time;
};

// edit
void calculateTimes(struct Process processes[])
{
   // Waiting Time = Turnaround Time - Burst Time
   // Turnaround Time = Exit Time - Arrival Time
   // Response Time = Start Time - Arrival Time
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      processes[i].turnaroundTime = processes[i].end_time - processes[i].arrivalTime;
      processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
      processes[i].responseTime = processes[i].start_time - processes[i].arrivalTime;
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

// edit
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

// edit
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

// edit
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
   struct Process temp;

   for (int i = 0; i < NUM_PROCESSES - 1; i++)
   {
      for (int j = i + 1; j < NUM_PROCESSES; j++)
      {
         if (processes[i].arrivalTime > processes[j].arrivalTime)
         {
            temp = processes[i];
            processes[i] = processes[j];
            processes[j] = temp;
         }
      }
   }
   int currentTime = 0;
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      processes[i].start_time = currentTime;
      currentTime += processes[i].burstTime;
      processes[i].end_time = currentTime;
   }
}

void sjfSort(struct Process processes[], int currentTime, int temp2)
{
   int total_time = 0;
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
      processes[i].start_time = currentTime - processes[i].burstTime;  // Set start time for the current process
      processes[i].end_time = processes[i].start_time + processes[i].burstTime; 
      total_time += processes[i].burstTime;
   }
}

void srtfSort(struct Process processes[])
{
   int n = NUM_PROCESSES;
   int current_time = 0;
   int completed = 0;
   int shortest_process = 0;
   int remaining_time[n];

   for (int i = 0; i < n; i++)
   {
      remaining_time[i] = processes[i].burstTime;
      processes[shortest_process].start_time = -1;
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

      if (processes[shortest_process].start_time == -1) {
         processes[shortest_process].start_time = current_time;
      }
         
      // Decrement the remaining time of the shortest process
      remaining_time[shortest_process]--;

      // Check if the process has completed
      if (remaining_time[shortest_process] == 0)
      {
         completed++;
         processes[shortest_process].end_time = current_time + 1;
      }
      current_time++;
   }
}

void prioSort(struct Process processes[])
{
   int n = NUM_PROCESSES;
   int current_time = 0;
   int completed = 0;
   int prioritized_process = 0;
   int remaining_time[n];

   for (int i = 0; i < n; i++)
   {
      remaining_time[i] = processes[i].burstTime;
      processes[prioritized_process].start_time = -1;
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

      if (processes[prioritized_process].start_time == -1)
         processes[prioritized_process].start_time = current_time;

      // Decrement the remaining time of the shortest process
      remaining_time[prioritized_process]--;

      // Check if the process has completed
      if (remaining_time[prioritized_process] == 0)
      {
         completed++;
         processes[prioritized_process].end_time = current_time + 1;
      }
      current_time++;
   }
}

void rrSort(struct Process processes[])
{
   int total_time = 0;
   int completed_processes = 0;
   int current_time = 0;
   int next_process_id = 1;

   struct Process temp;

   while (completed_processes < NUM_PROCESSES)
   {
      int selected_process = -1;

      for (int i = 0; i < NUM_PROCESSES; i++)
      {
         if (processes[i].processID == next_process_id && processes[i].arrivalTime <= current_time && processes[i].remaining_time > 0)
         {
            selected_process = i;
            break;
         }
      }

      if (selected_process == -1)
      {
         for (int i = 0; i < NUM_PROCESSES; i++)
         {
            if (processes[i].arrivalTime <= current_time && processes[i].remaining_time > 0)
            {
               if (selected_process == -1)
               {
                  selected_process = i;
                  break;
               }
            }
         }
      }

      if (selected_process == -1)
      {
         current_time++;
         continue;
      }

      if (processes[selected_process].start_time == -1)
         processes[selected_process].start_time = current_time;

      int execution_time = (processes[selected_process].remaining_time <= TIME_QUANTUM)
                               ? processes[selected_process].remaining_time
                               : TIME_QUANTUM;

      total_time += execution_time;
      processes[selected_process].remaining_time -= execution_time;
      current_time += execution_time;

      printf("Process %d executed for %d units of time.\n", processes[selected_process].processID,
             execution_time);

      if (processes[selected_process].remaining_time == 0)
      {
         processes[selected_process].end_time = current_time;
         completed_processes++;
      }

      next_process_id = processes[selected_process].processID + 1;
      if (next_process_id == 7)
         next_process_id = 1;

      // Perform Round Robin
      if (processes[selected_process].remaining_time > 0)
      {
         // Move the process to the end of the queue
         temp = processes[selected_process];
         for (int i = selected_process; i < NUM_PROCESSES - 1; i++)
         {
            processes[i] = processes[i + 1];
         }
         processes[NUM_PROCESSES - 1] = temp;
      }
   }

   printf("Total execution time: %d\n", total_time);
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

      processes[i].remaining_time = processes[i].burstTime;
      processes[i].start_time = -1;
      processes[i].end_time = 0;
   }
}

void burstTimeSort(struct Process processes[], int smallTime[NUM_PROCESSES], int temp2)
{
   // Copy the BT
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      smallTime[i] = processes[i].burstTime;
   }

   // Sort the BT Copy
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      for (int j = 0; j < NUM_PROCESSES - i - 1; j++)
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
}

void reset(struct Process processes[], struct Process processes2[])
{
   // reset back to original
   for (int i = 0; i < 6; i++)
   {
      processes[i] = processes2[i];
   }
}
