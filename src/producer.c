#include <stdio.h>
#include "common.h"
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if(shm_fd == -1)
    {
        fprintf(stderr, "%s\n", "Failed to create a shared memory block for writing");
        return -1;
    }

    if(ftruncate(shm_fd, SIZE) == -1)
    {
        fprintf(stderr, "%s\n", "Failed truncate the shared memory block");
        return -1;
    }

    int *ptr = (int *)mmap(NULL, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

#ifdef DEBUG
    fprintf(stdout, "%s:%p\n", "Pointer to shared memory", ptr);
#endif

    sem_t *sem_producer = sem_open(producer, O_CREAT, 0666, 1);
    sem_t *sem_consumer = sem_open(consumer, O_CREAT, 0666, 0);

    if(sem_producer == SEM_FAILED)
    {
        fprintf(stderr, "%s\n", "Failed to create a semaphore for producer process");
        return -1;
    }
    if(sem_consumer == SEM_FAILED)
    {
        fprintf(stderr, "%s\n", "Failed to create a semaphore for consumer process");
        return -1;
    }

    for(int i = 0;i < NUMS;i++)
    {
        sem_wait(sem_producer);
        ptr[0] = i;
        sem_post(sem_consumer);
        fprintf(stdout, "%s:%d\n", "Produced data ", i);
    }

    // close all the semaphores
    sem_close(sem_producer);
    sem_close(sem_consumer);

    close(shm_fd);
    munmap(ptr, SIZE);

    return 0;
}