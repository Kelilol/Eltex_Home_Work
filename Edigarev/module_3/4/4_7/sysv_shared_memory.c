#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678
#define MAX_NUMBERS 100

typedef struct {
    int numbers[MAX_NUMBERS];
    int count;
    int min;
    int max;
    int ready;
} SharedData;

int shm_id;
int sem_id;
SharedData *shared_data;
int sets_processed = 0;
volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
}

void semaphore_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop wait");
        exit(EXIT_FAILURE);
    }
}

void semaphore_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop signal");
        exit(EXIT_FAILURE);
    }
}

int main() {
    signal(SIGINT, handle_sigint);

    // Create shared memory
    shm_id = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach shared memory
    shared_data = (SharedData *)shmat(shm_id, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Initialize shared data
    shared_data->count = 0;
    shared_data->ready = 0;

    // Create semaphore
    sem_id = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore to 1
    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        while (!stop) {
            semaphore_wait(sem_id);

            if (shared_data->ready) {
                // Find min and max
                int min = shared_data->numbers[0];
                int max = shared_data->numbers[0];
                for (int i = 1; i < shared_data->count; i++) {
                    if (shared_data->numbers[i] < min) min = shared_data->numbers[i];
                    if (shared_data->numbers[i] > max) max = shared_data->numbers[i];
                }
                shared_data->min = min;
                shared_data->max = max;
                shared_data->ready = 0; // Mark as processed
            }

            semaphore_signal(sem_id);
            usleep(100000); // Sleep for a short period
        }
        exit(0);
    } else {
        // Parent process
        while (!stop) {
            semaphore_wait(sem_id);

            // Generate a random number of random numbers
            shared_data->count = rand() % MAX_NUMBERS;
            for (int i = 0; i < shared_data->count; i++) {
                shared_data->numbers[i] = rand() % 1000;
            }
            shared_data->ready = 1; // Mark as ready

            semaphore_signal(sem_id);

            // Wait for the child to process
            while (shared_data->ready) {
                usleep(100000); // Sleep for a short period
            }

            // Output the results
            printf("Set %d: Min = %d, Max = %d\n", sets_processed + 1, shared_data->min, shared_data->max);
            sets_processed++;

            usleep(1000000); // Sleep for demonstration purposes
        }

        // Wait for child process to exit
        wait(NULL);

        // Detach and remove shared memory
        shmdt(shared_data);
        shmctl(shm_id, IPC_RMID, NULL);

        // Remove semaphore
        semctl(sem_id, 0, IPC_RMID);

        printf("Processed %d sets of data.\n", sets_processed);
    }

    return 0;
}
