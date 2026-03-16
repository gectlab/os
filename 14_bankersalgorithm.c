

    #include <stdio.h>
    #include <stdbool.h>
     
    #define P 5   // Number of processes
    #define R 3   // Number of resources
     
    int main() {
     
        int allocation[P][R] = {
            {0, 1, 0},
            {2, 0, 0},
            {3, 0, 2},
            {2, 1, 1},
            {0, 0, 2}
        };
     
        int max[P][R] = {
            {7, 5, 3},
            {3, 2, 2},
            {9, 0, 2},
            {2, 2, 2},
            {4, 3, 3}
        };
     
        int available[R] = {3, 3, 2};
     
        int need[P][R];
        int safeSequence[P];
        bool finished[P] = {false};
     
        // Step 1: Calculate Need matrix
        for (int i = 0; i < P; i++) {
            for (int j = 0; j < R; j++) {
                need[i][j] = max[i][j] - allocation[i][j];
            }
        }
     
        int count = 0;
     
        while (count < P) {
            bool found = false;
     
            for (int i = 0; i < P; i++) {
                if (!finished[i]) {
     
                    int j;
                    for (j = 0; j < R; j++) {
                        if (need[i][j] > available[j])
                            break;
                    }
     
                    // If all resources can be satisfied
                    if (j == R) {
                        for (int k = 0; k < R; k++)
                            available[k] += allocation[i][k];
     
                        safeSequence[count++] = i;
                        finished[i] = true;
                        found = true;
                    }
                }
            }
     
            // If no process was found in this loop
            if (!found) {
                printf("System is NOT in a safe state.\n");
                return 0;
            }
        }
     
        printf("System is in a SAFE state.\nSafe sequence: ");
     
        for (int i = 0; i < P; i++)
            printf("P%d ", safeSequence[i]);
     
        printf("\n");
     
        return 0;
    }

