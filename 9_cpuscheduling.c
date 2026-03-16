/*
EXP-9: 
Input a list of processes, their CPU burst times (integral values), arrival times, and
priorities. Then simulate FCFS, SRTF, non-preemptive priority (a larger priority number
implies a higher priority), and RR (quantum = 3 units) scheduling algorithms on theprocess mix, determining which algorithm results in the minimum average waiting time(over all processes).
*/
#include <stdio.h>

#define MAX 20

typedef struct {
    int id;
    int at;
    int bt;
    int rt;
    int pr;
    int ct;
    int wt;
} Process;

void copy(Process a[], Process b[], int n)
{
    for(int i=0;i<n;i++)
        b[i] = a[i];
}

double fcfs(Process p[], int n)
{
    int time = 0;
    double total = 0;

    for(int i=0;i<n;i++)
    {
        if(time < p[i].at)
            time = p[i].at;

        time += p[i].bt;
        p[i].ct = time;
        p[i].wt = p[i].ct - p[i].at - p[i].bt;

        total += p[i].wt;
    }

    return total/n;
}

double priority_np(Process p[], int n)
{
    int completed = 0, time = 0;
    int done[MAX] = {0};
    double total = 0;

    while(completed < n)
    {
        int idx = -1;
        int best = -1;

        for(int i=0;i<n;i++)
        {
            if(!done[i] && p[i].at <= time)
            {
                if(p[i].pr > best)
                {
                    best = p[i].pr;
                    idx = i;
                }
            }
        }

        if(idx == -1)
        {
            time++;
            continue;
        }

        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].wt = p[idx].ct - p[idx].at - p[idx].bt;

        total += p[idx].wt;
        done[idx] = 1;
        completed++;
    }

    return total/n;
}

double srtf(Process p[], int n)
{
    int completed = 0, time = 0;
    double total = 0;

    for(int i=0;i<n;i++)
        p[i].rt = p[i].bt;

    while(completed < n)
    {
        int idx = -1;
        int min = 100000;

        for(int i=0;i<n;i++)
        {
            if(p[i].at <= time && p[i].rt > 0 && p[i].rt < min)
            {
                min = p[i].rt;
                idx = i;
            }
        }

        if(idx == -1)
        {
            time++;
            continue;
        }

        p[idx].rt--;
        time++;

        if(p[idx].rt == 0)
        {
            completed++;
            p[idx].ct = time;
            p[idx].wt = p[idx].ct - p[idx].at - p[idx].bt;
            total += p[idx].wt;
        }
    }

    return total/n;
}

double rr(Process p[], int n, int q)
{
    int time = 0, completed = 0;
    double total = 0;

    for(int i=0;i<n;i++)
        p[i].rt = p[i].bt;

    while(completed < n)
    {
        int done = 1;

        for(int i=0;i<n;i++)
        {
            if(p[i].rt > 0 && p[i].at <= time)
            {
                done = 0;

                if(p[i].rt > q)
                {
                    time += q;
                    p[i].rt -= q;
                }
                else
                {
                    time += p[i].rt;
                    p[i].rt = 0;
                    completed++;

                    p[i].ct = time;
                    p[i].wt = p[i].ct - p[i].at - p[i].bt;

                    total += p[i].wt;
                }
            }
        }

        if(done)
            time++;
    }

    return total/n;
}

int main()
{
    Process p[MAX], temp[MAX];
    int n;

    printf("Enter number of processes: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++)
    {
        p[i].id = i+1;
        printf("P%d Arrival Burst Priority: ",i+1);
        scanf("%d %d %d",&p[i].at,&p[i].bt,&p[i].pr);
    }

    double fcfs_wt, srtf_wt, pr_wt, rr_wt;

    copy(p,temp,n);
    fcfs_wt = fcfs(temp,n);

    copy(p,temp,n);
    srtf_wt = srtf(temp,n);

    copy(p,temp,n);
    pr_wt = priority_np(temp,n);

    copy(p,temp,n);
    rr_wt = rr(temp,n,3);

    printf("\nAverage Waiting Times\n");
    printf("FCFS: %.2f\n",fcfs_wt);
    printf("SRTF: %.2f\n",srtf_wt);
    printf("Priority: %.2f\n",pr_wt);
    printf("Round Robin: %.2f\n",rr_wt);

    double min = fcfs_wt;
    char algo[20] = "FCFS";

    if(srtf_wt < min){ min = srtf_wt; sprintf(algo,"SRTF"); }
    if(pr_wt < min){ min = pr_wt; sprintf(algo,"Priority"); }
    if(rr_wt < min){ min = rr_wt; sprintf(algo,"Round Robin"); }

    printf("\nMinimum average waiting time: %.2f (%s)\n",min,algo);

    return 0;
}
