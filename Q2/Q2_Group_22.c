#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

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
// Node for linkedlist used in displaying gantt chart
struct Node
{
   int processID;
   int arrivalTime;
   int burstTime;
   struct Node *next;
};
// head of linkedlist
struct Node *head = NULL;

// Function to create newNode or add node in a linkedlist
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
// Function to clear linkedlist
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
// Function to print linkedlist for FCFS and SJF
void printGanttChart(struct Process processes[])
{
   // Printing the first row, processID
   int currentTime = 0;
   printf("\n  ");
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      // If the current time is less than the arrival time of the current process
      // This means that there is no process that has arrived and is waiting to be processed
      for (int j = currentTime; processes[i].arrivalTime > j; j++)
      {
         if (j == currentTime)
            printf("|");
         else
            printf(" ");
         printf("P0");
      }
      currentTime += processes[i].arrivalTime;
      // Print the current process ID
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

   // Printing the second row, time
   printf("|\n");
   currentTime = 0;
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      int start = currentTime;
      // If the current time is less than the arrival time of the current process
      // This means that there is no process that has arrived and is waiting to be processed
      for (int j = currentTime; processes[i].arrivalTime > j; j++)
      {
         if (j == start)
            printf(" %2d", j);
         else
            printf("   ");
         currentTime++;
      }
      // Print the current time
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
// Function to print linkedlist for SRTF, RR and PS
void printGanttChartPreemptive(struct Node *node)
{
   // Printing the first row, processID
   struct Node *nodeStart = node;
   printf("\n  |");
   while (node != NULL)
   {
      // If the next node is NULL, then we are at the last node
      if (node->next == NULL)
      {
         printf("P%1d", node->processID);
         printf("|");
      }
      else
      {
         // Print the current process ID
         printf("P%1d", node->processID);
         // If the current process is not the same as the next process
         // Then we print a "|" to separate the processes
         if (node->processID != node->next->processID)
            printf("|");
         else
            printf(" ");
      }
      node = node->next;
   }
   // Printing the second row, time
   printf("\n");
   node = nodeStart;
   int currentTime = 0;
   // Print the arrival time of the first process
   printf(" %2d", currentTime++);
   while (node != NULL)
   {
      // If the next node is NULL, then we are at the last node
      if (node->next == NULL)
         printf(" %2d\n", currentTime);
      else
      {
         // If the current process is not the same as the next process
         // Then we print the current time
         if (node->processID != node->next->processID)
            printf(" %2d", currentTime);
         else
            printf("   ");
      }
      currentTime++;
      node = node->next;
   }
   // Clear the linkedlist for the next sorting algorithm
   clearList(&head);
}

void calculateTimes(struct Process processes[])
{
   // Waiting Time = Turnaround Time - Burst Time
   // Turnaround Time = Exit Time - Arrival Time
   // Response Time = Start Time - Arrival Time
   for (int i = 0; i < NUM_PROCESSES; i++)
   {
      processes[i].turnaroundTime = processes[i].end_time - processes[i].arrivalTime; //calculate turnaround time
      processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime; //calculate waiting time
      processes[i].responseTime = processes[i].start_time - processes[i].arrivalTime; //calculate response time
   }
}

void process(struct Process processes[], FILE *file) //print out processes neatly with heading
{
   int i;

   printf("Process\tArrival Time\tBurst Time\tPriority\n");
   for (i = 0; i < NUM_PROCESSES; i++)
   {
      printf("P%d\t%d\t\t%d\t\t%d\n", processes[i].processID,
             processes[i].arrivalTime, processes[i].burstTime, processes[i].priority);
   }
}

float avgTurnTime(struct Process processes[]) //calculate avg turnaround time
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

float avgWaitTime(struct Process processes[]) //calculate avg waiting time
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

float avgResTime(struct Process processes[]) //calculate avg response time
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
         if (processes[i].arrivalTime > processes[j].arrivalTime) //sort by first arrival time
         {
            temp = processes[i];
            processes[i] = processes[j];
            processes[j] = temp;
         }
      }
   }
   int currentTime = 0;
   for (int i = 0; i < NUM_PROCESSES; i++) //calculate endtime of processes
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

   // Initialize remaining time to burst time and start time to -1
   // -1 indicates that the process has not started
   for (int i = 0; i < n; i++)
   {
      remaining_time[i] = processes[i].burstTime;
      processes[shortest_process].start_time = -1;
   }

   // Loop until all processes are completed
   while (completed != n)
   {
      shortest_process = -1; // Index of the process with the shortest remaining time
      int shortest_time = INT_MAX; // Initialize shortest time to the largest possible integer

      // Find the process with the shortest remaining time
      for (int i = 0; i < n; i++)
      {
         if (processes[i].arrivalTime <= current_time && remaining_time[i] < shortest_time && remaining_time[i] > 0)
         {
            shortest_process = i;
            shortest_time = remaining_time[i];
         }
      }

      // If no process is prioritized, then there are no processes that have arrived
      // Therefore, we increment the current time and continue
      if (shortest_process == -1)
      {
         append(&head, 0, 0, 0);
         current_time++;
         continue;
      }

      // If the process has not started, then we set the start time to the current time
      if (processes[shortest_process].start_time == -1)
      {
         processes[shortest_process].start_time = current_time;
      }
      // Add the process to the Gantt Chart
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
   // Print the Gantt Chart after all processes have been completed
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

   // Initialize remaining time to burst time and start time to -1
   // -1 indicates that the process has not started
   for (int i = 0; i < n; i++)
   {
      remaining_time[i] = processes[i].burstTime;
      processes[prioritized_process].start_time = -1;
   }

   // Loop until all processes are completed
   while (completed != n)
   {
      prioritized_process = -1; // Index of the process with the shortest remaining time
      int priority = INT_MAX; // Lower number indicates higher priority

      // Find the process with the lowest priotity integer
      for (int i = 0; i < n; i++)
      {
         if (processes[i].arrivalTime <= current_time && processes[i].priority < priority && remaining_time[i] > 0)
         {
            prioritized_process = i;
            priority = processes[i].priority;
         }
      }

      // If no process is prioritized, then there are no processes that have arrived
      // Therefore, we increment the current time and continue
      if (prioritized_process == -1)
      {
         append(&head, 0, 0, 0);
         current_time++;
         continue;
      }

      // If the process has not started, then we set the start time to the current time
      if (processes[prioritized_process].start_time == -1)
         processes[prioritized_process].start_time = current_time;

      // Add the process to the Gantt Chart
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
   // Print the Gantt Chart after all processes have been completed
   printf("\nPriority Scheduling\n");
   printGanttChartPreemptive(head);
}
void rrSort(struct Process processes[])
{
   // Initialize variables for sorting
   int completed_processes = 0;
   int current_time = 0;
   int next_process_id = 0;

   struct Process temp;

   // Will loop until all processes have completed
   while (completed_processes < NUM_PROCESSES)
   {
      int selected_process = -1;

      // Rotate through processes with an offset based on previous process
      for (int i = 0; i < NUM_PROCESSES; i++)
      {
         int next = (i + next_process_id)%6;
         if (processes[next].arrivalTime <= current_time && processes[next].remaining_time > 0)
         {
            selected_process = next;
            break;
         }
      }

      // If there is no selected process then increment the time and return to the start
      if (selected_process == -1)
      {
         append(&head, 0, 0, 0);
         current_time++;
         continue;
      }

      // If the selected process just started, set the starting time to the current time
      if (processes[selected_process].start_time == -1)
         processes[selected_process].start_time = current_time;

      // Sets the execution time to be 2 or 1 depending on the remaining time
      int execution_time = (processes[selected_process].remaining_time <= TIME_QUANTUM)
                               ? processes[selected_process].remaining_time
                               : TIME_QUANTUM;

      // Update remaining time and current time depending on execution time
      processes[selected_process].remaining_time -= execution_time;
      current_time += execution_time;

      // Append information for the Gantt Chart to keep track of
      for (int i = 0; i < execution_time; i++)
         append(&head, processes[selected_process].processID, processes[selected_process].arrivalTime, processes[selected_process].burstTime);

      // Sets end time and increase completed processess if remaining time is 0
      if (processes[selected_process].remaining_time == 0)
      {
         processes[selected_process].end_time = current_time;
         completed_processes++;
      }

      // Sets the next offset for the rotation
      next_process_id = (selected_process + 1)%6;
   }

   // Print the Gantt Chart after all processes have been completed
   printf("\nRound Robin Scheduling\n");
   printGanttChartPreemptive(head);
}

float findFast(float array[], int size) // Find the fastest time in the array
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

float writeFileTurn(struct Process processes[], FILE *file, char name[]) //print turnaround time in .txt file
{

   float floatTurnTime = avgTurnTime(processes);

   printf("Shortest Average Turnaround Time(%s): %.2f\n", name, floatTurnTime);
   // Write output to the file
   fprintf(file, "Shortest Average Turnaround Time(%s): %.2f", name, floatTurnTime);
   fprintf(file, "\n");
   return floatTurnTime;
}

float writeFileWait(struct Process processes[], FILE *file, char name[]) //print waiting time in .txt file
{

   float floatWaitTime = avgWaitTime(processes);
   printf("Shortest Average Waiting Time(%s): %.2f\n", name, floatWaitTime);
   // Write output to the file
   fprintf(file, "Shortest Average Waiting Time(%s): %.2f", name, floatWaitTime);
   fprintf(file, "\n");
   return floatWaitTime;
}

float writeFileRes(struct Process processes[], FILE *file, char name[]) //print response time in .txt file
{
   float floatResTime = avgResTime(processes);

   printf("Shortest Average Response Time(%s): %.2f\n", name, floatResTime);
   // Write output to the file
   fprintf(file, "Shortest Average Response Time(%s): %.2f\n", name, floatResTime);
   fprintf(file, "\n");
   return floatResTime;
}

void randomProcess(struct Process processes[]) //randomly generate processes with random arrivaltime, bursttime and priority
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

void burstTimeSort(struct Process processes[], int smallTime[NUM_PROCESSES], int temp2) //sort processes by burst time
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

void reset(struct Process processes[], struct Process processes2[]) //ensures that each algo uses the original processes generated
{
   // reset back to original
   for (int i = 0; i < 6; i++)
   {
      processes[i] = processes2[i];
   }
}


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
    FILE *file = fopen("Q2_Group_22.txt", "w");

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
