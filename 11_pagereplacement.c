/*
Simulate the FIFO, LRU, and optimal page-replacement algorithms as follows: First,
generate a random page-reference string where page numbers range from 0 to 9. Apply the random page-reference string to each algorithm, and record the number of page faults
incurred by each algorithm. Assume that demand paging is used. The length of the
reference string and the number of page frames (varying from 1 to 7) are to be received as command-line arguments.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_REF 1000
#define PAGE_RANGE 10   // pages 0–9

/* ---------------- FIFO ---------------- */
int fifo(int ref[], int n, int frames) {
    int frame[frames];
    int faults = 0, index = 0;

    for (int i = 0; i < frames; i++)
        frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < frames; j++) {
            if (frame[j] == ref[i]) {
                found = 1;
                break;
            }
        }

        if (!found) {
            frame[index] = ref[i];
            index = (index + 1) % frames;
            faults++;
        }
    }
    return faults;
}

/* ---------------- LRU ---------------- */
int lru(int ref[], int n, int frames) {
    int frame[frames];
    int time[frames];
    int faults = 0, counter = 0;

    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        time[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < frames; j++) {
            if (frame[j] == ref[i]) {
                counter++;
                time[j] = counter;
                found = 1;
                break;
            }
        }

        if (!found) {
            int min_time = time[0], pos = 0;

            for (int j = 1; j < frames; j++) {
                if (time[j] < min_time) {
                    min_time = time[j];
                    pos = j;
                }
            }

            counter++;
            frame[pos] = ref[i];
            time[pos] = counter;
            faults++;
        }
    }
    return faults;
}

/* ---------------- OPTIMAL ---------------- */
int optimal(int ref[], int n, int frames) {
    int frame[frames];
    int faults = 0;

    for (int i = 0; i < frames; i++)
        frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;

        for (int j = 0; j < frames; j++) {
            if (frame[j] == ref[i]) {
                found = 1;
                break;
            }
        }

        if (!found) {
            int pos = -1, farthest = i;

            for (int j = 0; j < frames; j++) {
                int k;
                for (k = i + 1; k < n; k++) {
                    if (frame[j] == ref[k]) {
                        if (k > farthest) {
                            farthest = k;
                            pos = j;
                        }
                        break;
                    }
                }

                if (k == n) {  // Not used again
                    pos = j;
                    break;
                }
            }

            if (pos == -1) pos = 0;
            frame[pos] = ref[i];
            faults++;
        }
    }
    return faults;
}

/* ---------------- MAIN ---------------- */
int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <reference_length> <num_frames (1-7)>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int frames = atoi(argv[2]);

    if (frames < 1 || frames > 7) {
        printf("Frames must be between 1 and 7\n");
        return 1;
    }

    if (n > MAX_REF) {
        printf("Reference string too long (max %d)\n", MAX_REF);
        return 1;
    }

    int ref[n];

    srand(time(NULL));

    for (int i = 0; i < n; i++)
        ref[i] = rand() % PAGE_RANGE;

    printf("\nReference String:\n");
    for (int i = 0; i < n; i++)
        printf("%d ", ref[i]);
    printf("\n");

    int fifo_faults = fifo(ref, n, frames);
    int lru_faults = lru(ref, n, frames);
    int opt_faults = optimal(ref, n, frames);

    printf("\nFrames: %d\n", frames);
    printf("FIFO Page Faults    : %d\n", fifo_faults);
    printf("LRU Page Faults     : %d\n", lru_faults);
    printf("Optimal Page Faults : %d\n", opt_faults);

    return 0;
}
