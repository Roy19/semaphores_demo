#include <stdio.h>
#include "common.h"
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
    int recv_data;

    int shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);

    if(shm_fd == -1)
    {
        fprintf(stderr, "%s\n", "Could not open shared memory for reading");
        return -1;
    }

    int *ptr = (int *)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    
    sem_t *sem_producer = sem_open(producer, O_CREAT);
    sem_t *sem_consumer = sem_open(consumer, O_CREAT);

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
        sem_wait(sem_consumer);
        recv_data = ptr[0];
        sem_post(sem_producer);
        fprintf(stdout, "%s:%d\n", "Received data ", recv_data);
    }

    // unlink all the closed semaphores
    sem_unlink(producer);
    sem_unlink(consumer);

    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME);

    return 0;
}