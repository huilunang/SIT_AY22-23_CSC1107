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
struct Node
{
   int processID;
   int arrivalTime;
   int burstTime;
   struct Node *next;
};
struct Node *head = NULL;

void append(struct Node **head_ref, int processID, int arrivalTime, int burstTime)
{
   // 1. Allocate node
   struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

   // Used in step 5
   struct Node *last = *head_ref;

   // 2. Put in the data
   new_node->processID = processID;
   new_node->arrivalTime = arrivalTime;
   new_node->burstTime = burstTime;

   // 3. This new node is going to be the
   //    last node, so make next of it as NULL
   new_node->next = NULL;

   // 4. If the Linked List is empty, then make
   //    the new node as head
   if (*head_ref == NULL)
   {
      *head_ref = new_node;
      return;
   }

   // 5. Else traverse till the last node
   while (last->next != NULL)
      last = last->next;

   // 6. Change the next of last node
   last->next = new_node;
   return;
}
void clearList(struct Node **head)
{
   struct Node *current = *head;
   struct Node *next;
   while (current != NULL)
   {
      next = current->next;
      free(current);
      current = next;
   }
   *head = NULL;
}
void printGanttChart2(struct Process processes[])
{
   int currentTime = 0;
   printf("\n  ");
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      for (int j = 0; j < processes[i].burstTime; j++)
      {
         if (j == 0)
            printf("|");
         else
            printf(" ");
         printf("P%1d", processes[i].processID);
      }
   }
   printf("|\n");
   currentTime = 0;
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      for (int j = 0; j < processes[i].burstTime; j++)
      {
         if (j == 0)
            printf(" %2d", currentTime);
         else
            printf("   ");
      }
      currentTime += processes[i].burstTime;
   }
   printf(" %2d\n", currentTime);
}

void printGanttChart(struct Process processes[])
{
   int currentTime = 0;
   printf("\n  ");
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      for (int j = currentTime; processes[i].arrivalTime > j; j++)
      {
         if (j == currentTime)
            printf("|");
         else
            printf(" ");
         printf("P0");
      }
      currentTime += processes[i].arrivalTime;
      for (int j = 0; j < processes[i].burstTime; j++)
      {
         if (j == 0)
            printf("|");
         else
            printf(" ");
         printf("P%1d", processes[i].processID);
      }
      currentTime += processes[i].burstTime;
   }
   printf("|\n");
   currentTime = 0;
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      int start = currentTime;
      for (int j = currentTime; processes[i].arrivalTime > j; j++)
      {
         if (j == start)
            printf(" %2d", j);
         else
            printf("   ");
         currentTime++;
      }
      for (int j = 0; j < processes[i].burstTime; j++)
      {
         if (j == 0)
            printf(" %2d", currentTime);
         else
            printf("   ");
      }
      currentTime += processes[i].burstTime;
   }
   printf(" %2d\n", currentTime);
}


void printGanttChartPreemptive(struct Node *node)
{
   struct Node *nodeStart = node;
   printf("\n  |");
   while (node != NULL)
   {
      if (node->next == NULL)
      {
         printf("P%1d", node->processID);
         printf("|");
      }
      else
      {
         printf("P%1d", node->processID);
         if (node->processID != node->next->processID)
            printf("|");
         else
            printf(" ");
      }
      node = node->next;
   }
   printf("\n");
   node = nodeStart;
   int currentTime = 0;
   printf(" %2d", currentTime++);
   while (node != NULL)
   {
      if (node->next == NULL)
         printf(" %2d\n", currentTime);
      else
      {
         if (node->processID != node->next->processID)
            printf(" %2d", currentTime);
         else
            printf("   ");
      }
      currentTime++;
      node = node->next;
   }
   clearList(&head);
}

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

void process(struct Process processes[], FILE *file)
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
   printf("\nFirst Come First Serve Scheduling\n");
   printGanttChart(processes);
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
      processes[i].start_time = currentTime - processes[i].burstTime; // Set start time for the current process
      processes[i].end_time = processes[i].start_time + processes[i].burstTime;
      total_time += processes[i].burstTime;
   }
   printf("\nShortest Job First Scheduling\n");
   printGanttChart(processes);
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
         append(&head, 0, 0, 0);
         current_time++;
         continue;
      }

      if (processes[shortest_process].start_time == -1)
      {
         processes[shortest_process].start_time = current_time;
      }
      append(&head, processes[shortest_process].processID, processes[shortest_process].arrivalTime, processes[shortest_process].burstTime);
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
   printf("\nShortest Remaining Time First Scheduling\n");
   printGanttChartPreemptive(head);
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
         append(&head, 0, 0, 0);
         current_time++;
         continue;
      }

      if (processes[prioritized_process].start_time == -1)
         processes[prioritized_process].start_time = current_time;

      append(&head, processes[prioritized_process].processID, processes[prioritized_process].arrivalTime, processes[prioritized_process].burstTime);
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
   printf("\nPriority Scheduling\n");
   printGanttChartPreemptive(head);
}
void rrSort(struct Process processes[])
{
   int total_time = 0;
   int completed_processes = 0;
   int current_time = 0;
   int next_process_id = 0;

   struct Process temp;

   while (completed_processes < NUM_PROCESSES)
   {
      int selected_process = -1;

      for (int i = 0; i < NUM_PROCESSES; i++)
      {
         int n = (i + next_process_id)%6;
         if (processes[n].arrivalTime <= current_time && processes[n].remaining_time > 0)
         {
            selected_process = n;
            break;
         }
      }

      if (selected_process == -1)
      {
         append(&head, 0, 0, 0);
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

      // printf("Process %d executed for %d units of time.\n", processes[selected_process].processID, execution_time);
      for (int i = 0; i < execution_time; i++)
         append(&head, processes[selected_process].processID, processes[selected_process].arrivalTime, processes[selected_process].burstTime);

      if (processes[selected_process].remaining_time == 0)
      {
         processes[selected_process].end_time = current_time;
         completed_processes++;
      }

      next_process_id = (selected_process + 1)%6;
   }

   // printf("Total execution time: %d\n", total_time);
   printf("\nRound Robin Scheduling\n");
   printGanttChartPreemptive(head);
}

float findFast(float array[], int size)
{
   float fast = array[0]; // Assume the first element is the smallest

   for (int i = 1; i < size; i++)
   {
      if (array[i] < fast)
      {
         fast = array[i]; // Update the smallest value
      }
   }

   return fast;
}

float writeFileTurn(struct Process processes[], FILE *file, char name[])
{

   float floatTurnTime = avgTurnTime(processes);

   printf("Shortest Average Turnaround Time(%s): %.2f\n", name, floatTurnTime);
   // Write output to the file
   fprintf(file, "Shortest Average Turnaround Time(%s): %.2f", name, floatTurnTime);
   fprintf(file, "\n");

   // Close the file
   printf("Output written to the file successfully.\n");
   return floatTurnTime;
}

float writeFileWait(struct Process processes[], FILE *file, char name[])
{

   float floatWaitTime = avgWaitTime(processes);
   printf("Shortest Average Waiting Time(%s): %.2f\n", name, floatWaitTime);
   // Write output to the file
   fprintf(file, "Shortest Average Waiting Time(%s): %.2f", name, floatWaitTime);
   fprintf(file, "\n");

   // Close the file
   printf("Output written to the file successfully.\n");
   return floatWaitTime;
}

float writeFileRes(struct Process processes[], FILE *file, char name[])
{
   float floatResTime = avgResTime(processes);

   printf("Shortest Average Response Time(%s): %.2f\n", name, floatResTime);
   // Write output to the file
   fprintf(file, "Shortest Average Response Time(%s): %.2f\n", name, floatResTime);
   fprintf(file, "\n");

   // Close the file
   printf("Output written to the file successfully.\n");
   return floatResTime;
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
