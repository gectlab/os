

    #include <stdio.h>
    #include <pthread.h>
    #include <semaphore.h>
    #include <unistd.h>
     
    sem_t S1, S2;
     
    void* process1(void* arg) {
        printf("Process 1 waiting for S1\n");
        sem_wait(&S1);
        printf("Process 1 acquired S1\n");
     
        sleep(1);
     
        printf("Process 1 waiting for S2\n");
        sem_wait(&S2);   // Will block here
        printf("Process 1 acquired S2\n");
     
        sem_post(&S2);
        sem_post(&S1);
        return NULL;
    }
     
    void* process2(void* arg) {
        printf("Process 2 waiting for S2\n");
        sem_wait(&S2);
        printf("Process 2 acquired S2\n");
     
        sleep(1);
     
        printf("Process 2 waiting for S1\n");
        sem_wait(&S1);   // Will block here
        printf("Process 2 acquired S1\n");
     
        sem_post(&S1);
        sem_post(&S2);
        return NULL;
    }
     
    int main() {
        pthread_t t1, t2;
     
        sem_init(&S1, 0, 1);
        sem_init(&S2, 0, 1);
     
        pthread_create(&t1, NULL, process1, NULL);
        pthread_create(&t2, NULL, process2, NULL);
     
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
     
        sem_destroy(&S1);
        sem_destroy(&S2);
     
        return 0;
    }

