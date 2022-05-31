#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

#include "../includes/shared_memory.h"

#define SIZES 6
#define KB 1024

int PACK_SIZES[SIZES] = {1*KB, 10*KB, 100*KB, 1*1000*KB, 10*1000*KB, 100*1000*KB};
int PACK_TESTS[SIZES] = {1000,1000,100,100,10,5};

int fill(char *bufptr, int size);

int main(){
    int size;
    int tests_amount;
    pid_t pid;

    sem_t *sem1;
    sem_t *sem2;

    sem1 = sem_open("/semaphore1", O_CREAT,  0644, 0);
    sem2 = sem_open("/semaphore2", O_CREAT,  0644, 1);

    pid = fork();
    for (int i = 0; i < SIZES; i++){
        tests_amount = PACK_TESTS[i];
        size = PACK_SIZES[i];

        int status;
        int cnt, time0, time1;
        int total_time = 0;

        char *block = attach_shm(FILENAME, size);
        
        if (block == NULL) {
            printf("No se pudo abrir la memoria compartida\n");
            return -1;
        }

        for (int j = 1; j <= tests_amount; j++){
            time0 = clock();
            if (pid == 0){
                //Productor
                sem_wait(sem2);
                printf("Escritura ShM. Test #%d/%d. Tamaño: %d Bytes.\n", j,tests_amount,size);
                cnt = fill(block, size);
                sem_post(sem1);
            } else {
                //Consumidor
                sem_wait(sem1);
                printf("Lectura ShM. Test #%d/%d. Tamaño: %d Bytes.\n", j,tests_amount,size);
                //printf("Contenido: \"%s\"\n\n", block);
                sem_post(sem2);
            }
            time1 = clock();
            double tiempo = (double)(time1) / CLOCKS_PER_SEC-(double)(time0) / CLOCKS_PER_SEC;
            total_time = total_time + tiempo;
        }

        detach_shm(block);
        
        if (destroy_shm(FILENAME)){
            printf("Bloque destruido.\n\n");
        } else {
            printf("No se pudo destruir el bloque.\n");
        }

        printf("El promedio con %d Bytes y %d pruebas es: %f \n",size,tests_amount,total_time/(tests_amount-1));
        total_time = 0;
    }
    sem_close(sem1);
    sem_unlink("/semaphore1");
    sem_close(sem2);
    sem_unlink("/semaphore2");

    return 0;
}

int fill(char * bufptr, int size){
    static char ch = 'A';
    int filled_count;

    //printf("size is %d\n", size);
    memset(bufptr, ch, size);
    
    if (ch > 90)
        ch = 65;
    
    filled_count = strlen(bufptr);

    printf("Bytes escritos: %d\n\n", filled_count);
    //printf("buffer filled is:%s\n", bufptr);
    ch++;
    return filled_count;
}