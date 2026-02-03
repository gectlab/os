#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int priority;
} Process;

/* ---------- Utility ---------- */
void copy_array(int *dest, int *src, int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

/* ---------- FCFS ---------- */
float fcfs(Process p[], int n) {
    Process temp[n];

    for (int i = 0; i < n; i++)
        temp[i] = p[i];

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (temp[i].arrival > temp[j].arrival) {
                Process t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }

    int time = 0;
    float total_wait = 0;

    for (int i = 0; i < n; i++) {
        if (time < temp[i].arrival)
            time = temp[i].arrival;

        int wait = time - temp[i].arrival;
        total_wait += wait;
        time += temp[i].burst;
    }

    return total_wait / n;
}

/* ---------- SRTF (Preemptive SJF) ---------- */
float srtf(Process p[], int n) {
    int remaining[n];
    int completion[n];
    int done = 0, time = 0;

    for (int i = 0; i < n; i++)
        remaining[i] = p[i].burst;

    while (done < n) {
        int idx = -1;
        int min_rem = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && remaining[i] > 0 && remaining[i] < min_rem) {
                min_rem = remaining[i];
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        remaining[idx]--;
        time++;

        if (remaining[idx] == 0) {
            completion[idx] = time;
            done++;
        }
    }

    float total_wait = 0;
    for (int i = 0; i < n; i++) {
        int turnaround = completion[i] - p[i].arrival;
        int wait = turnaround - p[i].burst;
        total_wait += wait;
    }

    return total_wait / n;
}

/* ---------- Non-Preemptive Priority ---------- */
float priority_np(Process p[], int n) {
    int completed[n];
    int completion[n];

    for (int i = 0; i < n; i++)
        completed[i] = 0;

    int done = 0, time = 0;

    while (done < n) {
        int idx = -1;
        int best_pr = -INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!completed[i] && p[i].arrival <= time && p[i].priority > best_pr) {
                best_pr = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        time += p[idx].burst;
        completion[idx] = time;
        completed[idx] = 1;
        done++;
    }

    float total_wait = 0;
    for (int i = 0; i < n; i++) {
        int turnaround = completion[i] - p[i].arrival;
        int wait = turnaround - p[i].burst;
        total_wait += wait;
    }

    return total_wait / n;
}

/* ---------- Round Robin (Quantum = 3) ---------- */
float round_robin(Process p[], int n, int quantum) {
    int remaining[n], completion[n];
    int in_queue[n];
    int queue[1000];
    int front = 0, rear = 0;

    for (int i = 0; i < n; i++) {
        remaining[i] = p[i].burst;
        in_queue[i] = 0;
    }

    int time = 0, done = 0;

    while (done < n) {

        // Add newly arrived processes
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && remaining[i] > 0 && p[i].arrival <= time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (front == rear) {
            time++;
            continue;
        }

        int idx = queue[front++];

        int exec = remaining[idx] < quantum ? remaining[idx] : quantum;
        remaining[idx] -= exec;
        time += exec;

        // Add processes that arrived during execution
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && remaining[i] > 0 && p[i].arrival <= time) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }

        if (remaining[idx] > 0) {
            queue[rear++] = idx;
        } else {
            completion[idx] = time;
            done++;
        }
    }

    float total_wait = 0;
    for (int i = 0; i < n; i++) {
        int turnaround = completion[i] - p[i].arrival;
        int wait = turnaround - p[i].burst;
        total_wait += wait;
    }

    return total_wait / n;
}

/* ---------- MAIN ---------- */
int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("\nProcess %d\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);
        printf("Burst Time: ");
        scanf("%d", &p[i].burst);
        printf("Priority (higher number = higher priority): ");
        scanf("%d", &p[i].priority);
    }

    float fcfs_avg = fcfs(p, n);
    float srtf_avg = srtf(p, n);
    float pr_avg = priority_np(p, n);
    float rr_avg = round_robin(p, n, 3);

    printf("\nAverage Waiting Times:\n");
    printf("FCFS: %.2f\n", fcfs_avg);
    printf("SRTF: %.2f\n", srtf_avg);
    printf("Priority (Non-Preemptive): %.2f\n", pr_avg);
    printf("Round Robin (Q=3): %.2f\n", rr_avg);

    // Find minimum
    float min = fcfs_avg;
    char best[50] = "FCFS";

    if (srtf_avg < min) { min = srtf_avg; sprintf(best, "SRTF"); }
    if (pr_avg < min) { min = pr_avg; sprintf(best, "Priority (Non-Preemptive)"); }
    if (rr_avg < min) { min = rr_avg; sprintf(best, "Round Robin"); }

    printf("\nBest Algorithm (Minimum Avg Waiting Time): %s\n", best);

    return 0;
}
