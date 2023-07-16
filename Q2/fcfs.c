#include <stdio.h>
// Function to find the waiting time for all processes
int waitingtime(int proc[], int n,
int burstTime[], int waitTime[]) {
   // waiting time for first process is 0
   waitTime[0] = 0;
   // calculating waiting time
   for (int i = 1; i < n ; i++ )
   waitTime[i] = burstTime[i-1] + waitTime[i-1] ;
   return 0;
}
// Function to calculate turn around time
int turnaroundtime( int proc[], int n,
int burstTime[], int waitTime[], int tat[]) {
   // calculating turnaround time by adding
   // burstTime[i] + waitTime[i]
   int i;
   for ( i = 0; i < n ; i++)
   tat[i] = burstTime[i] + waitTime[i];
   return 0;
}
//Function to calculate average time
int avgtime( int proc[], int n, int burstTime[]) {
   int waitTime[n], tat[n], totalWT = 0, totalTat = 0;
   int i;
   //Function to find waiting time of all processes
   waitingtime(proc, n, burstTime, waitTime);
   //Function to find turn around time for all processes
   turnaroundtime(proc, n, burstTime, waitTime, tat);
   //Display processes along with all details
   printf("Processes  Burst   Waiting Turn around ");
   // Calculate total waiting time and total turn
   // around time
   for ( i=0; i<n; i++) {
      totalWT = totalWT + waitTime[i];
      totalTat = totalTat + tat[i];
      printf(" %d\t  %d\t\t %d \t%d", i+1, burstTime[i], waitTime[i], tat[i]);
   }
   printf("Average waiting time = %f", (float)totalWT / (float)n);
   printf("Average turn around time = %f", (float)totalTat / (float)n);
   return 0;
}