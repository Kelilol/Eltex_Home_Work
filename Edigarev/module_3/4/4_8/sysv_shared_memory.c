#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

#define SHM_KEY 1234
#define SEM_KEY 5678
#define MAX_NUMBERS 100

struct shared_data {
    int numbers[MAX_NUMBERS];
    int count;
    int min;
    int sum;
    float average;
};

int semid;
int shmid;
struct shared_data *data;
volatile sig_atomic_t stop = 0;
int sets_processed = 0;

void handle_sigint(int sig) {
    stop = 1;
}

void lock_semaphore() {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void unlock_semaphore() {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

void find_min() {
    while (!stop) {
        lock_semaphore();
        if (data->count > 0) {
            int min = data->numbers[0];
            for (int i = 1; i < data->count; ++i) {
                if (data->numbers[i] < min) {
                    min = data->numbers[i];
                }
            }
            data->min = min;
            data->count = 0;
            unlock_semaphore();
        } else {
            unlock_semaphore();
            usleep(100000);  // Sleep for 100ms
        }
    }
    exit(0);
}

void find_sum() {
    while (!stop) {
        lock_semaphore();
        if (data->count > 0) {
            int sum = 0;
            for (int i = 0; i < data->count; ++i) {
                sum += data->numbers[i];
            }
            data->sum = sum;
            unlock_semaphore();
        } else {
            unlock_semaphore();
            usleep(100000);  // Sleep for 100ms
        }
    }
    exit(0);
}

void find_average() {
    while (!stop) {
        lock_semaphore();
        if (data->count > 0) {
            data->average = (float)data->sum / data->count;
            unlock_semaphore();
        } else {
            unlock_semaphore();
            usleep(100000);  // Sleep for 100ms
        }
    }
    exit(0);
}

int main() {
    pid_t pid_min, pid_sum, pid_avg;

    signal(SIGINT, handle_sigint);

    shmid = shmget(SHM_KEY, sizeof(struct shared_data), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    data = (struct shared_data *)shmat(shmid, NULL, 0);
    if (data == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    semctl(semid, 0, SETVAL, 1);

    if ((pid_min = fork()) == 0) {
        find_min();
    }

    if ((pid_sum = fork()) == 0) {
        find_sum();
    }

    if ((pid_avg = fork()) == 0) {
        find_average();
    }

    srand(time(NULL));
    while (!stop) {
        lock_semaphore();
        data->count = rand() % MAX_NUMBERS + 1;
        for (int i = 0; i < data->count; ++i) {
            data->numbers[i] = rand() % 1000;
        }
        unlock_semaphore();

        sleep(1);  // Give time for child processes to process the data

        lock_semaphore();
        printf("Set %d: Min = %d, Sum = %d, Average = %.2f\n",
               ++sets_processed, data->min, data->sum, data->average);
        unlock_semaphore();

        sleep(1);
    }

    printf("Processed %d sets of data.\n", sets_processed);

    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
