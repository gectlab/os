// Implement the deadlock-free semaphore-based solution for the dining philosopher's problem



    #include <stdio.h>
    #include <pthread.h>
    #include <semaphore.h>
    #include <unistd.h>
     
    #define N 5
     
    sem_t fork[N];
    sem_t room;
     
    void think(int id) {
        printf("Philosopher %d is thinking\n", id);
        sleep(1);
    }
     
    void eat(int id) {
        printf("Philosopher %d is eating\n", id);
        sleep(2);
    }
     
    void* philosopher(void* num) {
     
        int id = *(int*)num;
     
        while(1) {
     
            think(id);
     
            // allow only N-1 philosophers
            sem_wait(&room);
     
            // pick forks
            sem_wait(&fork[id]);             // left fork
            sem_wait(&fork[(id+1)%N]);       // right fork
     
            eat(id);
     
            // put forks down
            sem_post(&fork[id]);
            sem_post(&fork[(id+1)%N]);
     
            sem_post(&room);
        }
    }
     
    int main() {
     
        pthread_t ph[N];
        int id[N];
     
        // initialize semaphores
        sem_init(&room, 0, N-1);
     
        for(int i=0;i<N;i++)
            sem_init(&fork[i],0,1);
     
        // create philosophers
        for(int i=0;i<N;i++){
            id[i]=i;
            pthread_create(&ph[i],NULL,philosopher,&id[i]);
        }
     
        // join threads
        for(int i=0;i<N;i++)
            pthread_join(ph[i],NULL);
     
        return 0;
    }

