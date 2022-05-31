#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "shared_memory.h"

static int get_shm(char *filename, int size) {
    key_t key;

    key = ftok(filename, 0);

    if (key < 0) {
        perror("Error en Key");
        return -1;
    }

    return shmget(key, size, 0644 | IPC_CREAT);
}

char * attach_shm(char *filename, int size) {
    int shmId = get_shm(filename, size);
    char *ptr;

    if (shmId < 0){
        perror("Error en shmget");
        return NULL;
    }

    ptr = (char *) shmat(shmId, NULL, 0);

    if (ptr == (void *)-1){
        perror("Error en shmat");
        return NULL;
    }

    return ptr;
}

bool detach_shm(char *block){
    return (shmdt(block) != -1);
}

bool destroy_shm(char *filename) {
    int shmId = get_shm(filename, 0);

    if (shmId < 0){
        perror("Error en shmget");
        return NULL;
    }
    return (shmctl(shmId, IPC_RMID, NULL) != -1);
}