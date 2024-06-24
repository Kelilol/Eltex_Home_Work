#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

#define SHM_NAME "/my_shared_memory"
#define SEM_NAME "/my_semaphore"
#define MAX_NUMBERS 100

typedef struct {
    int numbers[MAX_NUMBERS];
    int count;
    int min;
    int max;
    int sum;
    float average;
    int ready;
} SharedData;

SharedData *shared_data;
sem_t *sem;
int sets_processed = 0;
volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    stop = 1;
}

void find_min_max() {
    int min = shared_data->numbers[0];
    int max = shared_data->numbers[0];
    for (int i = 1; i < shared_data->count; i++) {
        if (shared_data->numbers[i] < min) min = shared_data->numbers[i];
        if (shared_data->numbers[i] > max) max = shared_data->numbers[i];
    }
    shared_data->min = min;
    shared_data->max = max;
}

void calculate_sum() {
    int sum = 0;
    for (int i = 0; i < shared_data->count; i++) {
        sum += shared_data->numbers[i];
    }
    shared_data->sum = sum;
}

void calculate_average() {
    if (shared_data->count > 0) {
        shared_data->average = (float)shared_data->sum / shared_data->count;
    } else {
        shared_data->average = 0.0;
    }
}

void child_process() {
    while (!stop) {
        sem_wait(sem);

        if (shared_data->ready) {
            find_min_max();
            calculate_sum();
            calculate_average();
            shared_data->ready = 0;
        }

        sem_post(sem);
        usleep(100000); // Sleep for a short period
    }
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);

    // Create shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Configure the size of the shared memory
    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    // Map the shared memory
    shared_data = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Initialize shared data
    shared_data->count = 0;
    shared_data->ready = 0;

    // Create semaphore
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        child_process();
    } else {
        // Parent process
        while (!stop) {
            sem_wait(sem);

            // Generate a random number of random numbers
            shared_data->count = rand() % MAX_NUMBERS;
            for (int i = 0; i < shared_data->count; i++) {
                shared_data->numbers[i] = rand() % 1000;
            }
            shared_data->ready = 1; // Mark as ready

            sem_post(sem);

            // Wait for the child to process
            while (shared_data->ready) {
                usleep(100000); // Sleep for a short period
            }

            // Output the results
            printf("Set %d: Min = %d, Max = %d, Sum = %d, Average = %.2f\n", sets_processed + 1, shared_data->min, shared_data->max, shared_data->sum, shared_data->average);
            sets_processed++;

            usleep(1000000); // Sleep for demonstration purposes
        }

        // Wait for child process to exit
        wait(NULL);

        // Unlink and close shared memory
        munmap(shared_data, sizeof(SharedData));
        shm_unlink(SHM_NAME);

        // Unlink semaphore
        sem_close(sem);
        sem_unlink(SEM_NAME);

        printf("Processed %d sets of data.\n", sets_processed);
    }

    return 0;
}
