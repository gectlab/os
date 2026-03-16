/* 
write a simple C program to demonstrate  semaphores to solve the readers-writers problem with writers being given priority 
over readers
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t resource;     // Controls access to shared resource
sem_t readTry;      // Writers block readers here
sem_t rmutex;       // Protects readCount
sem_t wmutex;       // Protects writeCount

int readCount = 0;
int writeCount = 0;

void* reader(void* arg) {
    int id = *(int*)arg;

    sem_wait(&readTry);          // Check if writer is waiting
    sem_wait(&rmutex);
    readCount++;
    if (readCount == 1)
        sem_wait(&resource);     // First reader locks resource
    sem_post(&rmutex);
    sem_post(&readTry);

    // --- Reading Section ---
    printf("Reader %d is reading\n", id);
    sleep(1);
    printf("Reader %d finished reading\n", id);

    sem_wait(&rmutex);
    readCount--;
    if (readCount == 0)
        sem_post(&resource);     // Last reader releases resource
    sem_post(&rmutex);

    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;

    sem_wait(&wmutex);
    writeCount++;
    if (writeCount == 1)
        sem_wait(&readTry);      // First writer blocks readers
    sem_post(&wmutex);

    sem_wait(&resource);

    // --- Writing Section ---
    printf("Writer %d is writing\n", id);
    sleep(2);
    printf("Writer %d finished writing\n", id);

    sem_post(&resource);

    sem_wait(&wmutex);
    writeCount--;
    if (writeCount == 0)
        sem_post(&readTry);      // Allow readers again
    sem_post(&wmutex);

    return NULL;
}

int main() {
    pthread_t r[3], w[2];
    int r_id[3] = {1, 2, 3};
    int w_id[2] = {1, 2};

    // Initialize semaphores
    sem_init(&resource, 0, 1);
    sem_init(&readTry, 0, 1);
    sem_init(&rmutex, 0, 1);
    sem_init(&wmutex, 0, 1);

    // Create writer threads
    for (int i = 0; i < 2; i++)
        pthread_create(&w[i], NULL, writer, &w_id[i]);

    // Create reader threads
    for (int i = 0; i < 3; i++)
        pthread_create(&r[i], NULL, reader, &r_id[i]);

    // Join threads
    for (int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);

    for (int i = 0; i < 3; i++)
        pthread_join(r[i], NULL);

    // Destroy semaphores
    sem_destroy(&resource);
    sem_destroy(&readTry);
    sem_destroy(&rmutex);
    sem_destroy(&wmutex);

    return 0;
}
