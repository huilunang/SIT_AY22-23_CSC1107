#include <stdio.h>
    struct Jobs
    {
        int id;
        int burst;
        int arrival;
    };
    
    struct srtf
    {
        int id;
    };

int isJobsAvailable (struct Jobs jobs[]) {
    for (int i = 0; i < sizeof(jobs); i++) {
        printf("Job %d has burst %d \n", jobs[i].id, jobs[i].burst);
        if (jobs[i].burst > 0) {
            return 1;
        }
    }
    return 0;
}

int findJobsIndexByID (struct Jobs jobs[], int id) {
    for (int i = 0; i < sizeof(jobs); i++) {
        if (jobs[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findLowestBurst (struct Jobs jobs[]) {
    int lowestBurst = jobs[0].burst;
    int lowestBurstID = jobs[0].id;
    for (int i = 0; i < sizeof(jobs); i++) {
        if (jobs[i].burst > 0) {
            lowestBurst = jobs[i].burst;
            lowestBurstID = jobs[i].id;
        }
    }
    for (int i = 0; i < sizeof(jobs); i++) {
        if (jobs[i].burst < lowestBurst) {
            if (jobs[i].burst > 0) {
            lowestBurst = jobs[i].burst;
            lowestBurstID = jobs[i].id;
            }
        }
    }
    return lowestBurstID;
}

int main() {
	// Matrix for storing Process Id, Burst
	// Time, Average Waiting Time & Average
	// Turn Around Time.
	int A[100][4];
	int i, j, n, total = 0, index, temp;
	float avg_wt, avg_tat;
	// printf("Enter number of process: ");
	// scanf("%d", &n);
    n = 6;
    struct Jobs jobs[n];

	// User Input Burst Time and alloting Process Id.
	for (i = 0; i < n; i++) {
        jobs[i].id = i + 1;
        jobs[i].burst = 5;
        jobs[i].arrival = 0;
		// printf("Enter the Burst Time for P%d: ", jobs[i].id);
		// scanf("%d", &jobs[i].burst);
        // printf("Enter the Arrival Time for P%d: ", jobs[i].id);
        // scanf("%d", &jobs[i].arrival);
	}

    // Sorting process according to their Burst Time.
    // For each step in the arrival time
    struct srtf sortedJobs[100];
    int time = 0;
	while (isJobsAvailable(jobs)) {
        printf("Loop start \n");
        // Find the job with the lowest burst time that is not 0
        int lowestBurstID = findLowestBurst(jobs);

        // Add the job and to the burst time
        sortedJobs[time].id = lowestBurstID;
        jobs[findJobsIndexByID(jobs, lowestBurstID)].burst--;
        // Return to the top of the loop
        time++;
        if (time > 30) {
            break;
        }
	}

    for ( i = 0; i < sizeof(sortedJobs); i++)
    {
        printf("%d", sortedJobs[i].id);
    }

    /*
	// Calculation of Waiting Times
	for (i = 1; i < n; i++) {
		A[i][2] = 0;
		for (j = 0; j < i; j++)
			A[i][2] += A[j][1];
		total += A[i][2];
	}
	avg_wt = (float)total / n;
	total = 0;
	printf("P	 BT	 WT	 TAT\n");

	// Calculation of Turn Around Time and printing the
	// data.
	for (i = 0; i < n; i++) {
		A[i][3] = A[i][1] + A[i][2];
		total += A[i][3];
		printf("P%d	 %d	 %d	 %d\n", A[i][0],
			A[i][1], A[i][2], A[i][3]);
	}
	avg_tat = (float)total / n;
	printf("Average Waiting Time= %f", avg_wt);
	printf("\nAverage Turnaround Time= %f", avg_tat);*/
}
