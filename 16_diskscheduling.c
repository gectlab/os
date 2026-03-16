

    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <math.h>
     
    #define CYLINDERS 5000
    #define REQUESTS 10
     
    // Function to sort array
    void sort(int arr[], int n) {
        int i, j, temp;
        for(i = 0; i < n-1; i++) {
            for(j = i+1; j < n; j++) {
                if(arr[i] > arr[j]) {
                    temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }
        }
    }
     
    // SSTF Algorithm
    int SSTF(int req[], int head) {
        int visited[REQUESTS] = {0};
        int count = 0, total = 0, i;
     
        while(count < REQUESTS) {
            int min = 10000, index = -1;
     
            for(i = 0; i < REQUESTS; i++) {
                if(!visited[i]) {
                    int dist = abs(head - req[i]);
                    if(dist < min) {
                        min = dist;
                        index = i;
                    }
                }
            }
     
            visited[index] = 1;
            total += abs(head - req[index]);
            head = req[index];
            count++;
        }
     
        return total;
    }
     
    // LOOK Algorithm
    int LOOK(int req[], int head) {
        int total = 0;
        int temp[REQUESTS];
        int i;
     
        for(i = 0; i < REQUESTS; i++)
            temp[i] = req[i];
     
        sort(temp, REQUESTS);
     
        int pos;
        for(pos = 0; pos < REQUESTS; pos++)
            if(temp[pos] > head)
                break;
     
        for(i = pos; i < REQUESTS; i++) {
            total += abs(head - temp[i]);
            head = temp[i];
        }
     
        for(i = pos-1; i >= 0; i--) {
            total += abs(head - temp[i]);
            head = temp[i];
        }
     
        return total;
    }
     
    // CSCAN Algorithm
    int CSCAN(int req[], int head) {
        int total = 0;
        int temp[REQUESTS];
        int i;
     
        for(i = 0; i < REQUESTS; i++)
            temp[i] = req[i];
     
        sort(temp, REQUESTS);
     
        int pos;
        for(pos = 0; pos < REQUESTS; pos++)
            if(temp[pos] > head)
                break;
     
        for(i = pos; i < REQUESTS; i++) {
            total += abs(head - temp[i]);
            head = temp[i];
        }
     
        total += abs(head - (CYLINDERS-1));
        head = 0;
        total += CYLINDERS-1;
     
        for(i = 0; i < pos; i++) {
            total += abs(head - temp[i]);
            head = temp[i];
        }
     
        return total;
    }
     
    int main(int argc, char *argv[]) {
     
        if(argc != 2) {
            printf("Usage: %s <initial_head_position>\n", argv[0]);
            return 1;
        }
     
        int head = atoi(argv[1]);
     
        int req[REQUESTS];
     
        srand(time(NULL));
     
        printf("Random Requests:\n");
     
        for(int i = 0; i < REQUESTS; i++) {
            req[i] = rand() % CYLINDERS;
            printf("%d ", req[i]);
        }
     
        printf("\nInitial Head Position: %d\n\n", head);
     
        printf("SSTF Total Head Movement : %d\n", SSTF(req, head));
        printf("LOOK Total Head Movement : %d\n", LOOK(req, head));
        printf("CSCAN Total Head Movement: %d\n", CSCAN(req, head));
     
        return 0;
    }

