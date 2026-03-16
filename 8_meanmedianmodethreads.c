/*
Write a multithreaded program that calculates the mean, median, and standard deviation
for a list of integers. This program should receive a series of integers on the command line
and will then create three separate worker threads. The first thread will determine the
mean value, the second will determine the median and the third will calculate the standard
deviation of the integers. The variables representing the mean, median, and standard
deviation values will be stored globally. The worker threads will set these values, and the
parent thread will output the values once the workers have exited.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int *numbers;
int n;

double mean;
double median;
double stddev;

void* calc_mean(void *arg)
{
    int sum = 0;

    for(int i = 0; i < n; i++)
        sum += numbers[i];

    mean = (double)sum / n;

    pthread_exit(0);
}

void* calc_median(void *arg)
{
    int temp[n];

    for(int i = 0; i < n; i++)
        temp[i] = numbers[i];

    for(int i = 0; i < n-1; i++)
        for(int j = i+1; j < n; j++)
            if(temp[i] > temp[j])
            {
                int t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }

    if(n % 2 == 0)
        median = (temp[n/2 - 1] + temp[n/2]) / 2.0;
    else
        median = temp[n/2];

    pthread_exit(0);
}

void* calc_stddev(void *arg)
{
    double sum = 0;

    for(int i = 0; i < n; i++)
        sum += (numbers[i] - mean) * (numbers[i] - mean);

    stddev = sqrt(sum / n);

    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Provide integers\n");
        return 1;
    }

    n = argc - 1;

    numbers = malloc(n * sizeof(int));

    for(int i = 0; i < n; i++)
        numbers[i] = atoi(argv[i+1]);

    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, calc_mean, NULL);
    pthread_join(t1, NULL);

    pthread_create(&t2, NULL, calc_median, NULL);
    pthread_create(&t3, NULL, calc_stddev, NULL);

    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Mean = %.2f\n", mean);
    printf("Median = %.2f\n", median);
    printf("Standard Deviation = %.2f\n", stddev);

    free(numbers);

    return 0;
}
