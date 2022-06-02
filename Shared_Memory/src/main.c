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

//1KB, 10KB, 100KB, 1MB, 10MB, 100MB
int PACK_SIZES[SIZES] = {1*KB, 10*KB, 100*KB, 1*KB*KB, 10*KB*KB, 100*KB*KB};
int PACK_TESTS[SIZES] = {1000,1000,100,100,10,5};
//int PACK_TESTS[SIZES] = {6,5,4,3,2,1};

void fill(char *bufptr, int size);

int main(){
    int size;
    int tests_amount;
    pid_t pid;

    sem_t *sem_prod;
    sem_t *sem_cons;

    sem_close(sem_prod);
    sem_unlink("/semaphore1");
    sem_close(sem_cons);
    sem_unlink("/semaphore2");

    sem_prod = sem_open("/semaphore1", O_CREAT,  0644, 0);
    if (sem_prod == SEM_FAILED){
        perror("Error en sem_open/sem_prod");
        exit(-1);
    }

    sem_cons = sem_open("/semaphore2", O_CREAT,  0644, 1);
    if (sem_prod == SEM_FAILED){
        perror("Error en sem_open/sem_coms");
        exit(-1);
    }

    printf("\nMedidas de envío de paquetes con comunicación mediante Memoria Compartida.\n\n");
    
    pid = fork();

    if (pid == (pid_t) -1){
        perror("Error en fork");
        exit(-1);
    }

    for (int i = 0; i < SIZES; i++){
        tests_amount = PACK_TESTS[i];
        size = PACK_SIZES[i];
        
        float time0, time1;
        float total_time = 0;

        if (pid == 0){sem_wait(sem_cons);}
        else {sem_wait(sem_prod);}

        char *block = attach_shm(FILENAME, size);
        if (block == NULL) {
            printf("No se pudo abrir la memoria compartida\n");
            return -1;
        }

        if (pid == 0){sem_post(sem_prod);}
        else {sem_post(sem_cons);}

        for (int j = 1; j <= tests_amount; j++){
            time0 = clock();
            if (pid == 0){
                //Productor
                sem_wait(sem_cons);
                //printf("Escritura ShM. Test #%d/%d. Tamaño: %d Bytes.\n", j,tests_amount,size);
                fill(block, size);
                sem_post(sem_prod);

            } else {
                //Consumidor
                sem_wait(sem_prod);
                //printf("Lectura ShM. Test #%d/%d. Tamaño: %d Bytes.\n", j,tests_amount,size);
                //printf("Contenido: \"%s\"\n\n", block);
                sem_post(sem_cons);
            }
            time1 = clock();
            double tiempo = ((double)(time1 - time0)) / CLOCKS_PER_SEC;
            total_time = total_time + tiempo;
        }

        if (pid == 0){
            sem_wait(sem_cons);

            detach_shm(block);

            if (destroy_shm(FILENAME)){
                //printf("Bloque destruido.\n\n");
            } else {
                printf("No se pudo destruir el bloque.\n");
            }

            sem_post(sem_prod);

        } else {
            sem_wait(sem_prod);

            detach_shm(block);

            sem_post(sem_cons);
        }

        printf("El promedio con %d Bytes y %d pruebas es: %f seg. \n\n",size,tests_amount,total_time/tests_amount);
        total_time = 0;
    }

    sem_close(sem_prod);
    sem_unlink("/semaphore1");
    sem_close(sem_cons);
    sem_unlink("/semaphore2");

    return 0;
}

void fill(char * bufptr, int size){
    static char ch = 'A';
    int filled_count;

    //printf("size is %d\n", size);
    if (size != 0){
        memset(bufptr, ch, size);
    } else {
        memset(bufptr, 'q', size);
    }
    
    bufptr[size-1] = '\0';
    
    if (ch > 90)
        ch = 65;
    
    filled_count = strlen(bufptr);

    //printf("Bytes escritos: %d\n\n", filled_count);
    //printf("buffer filled is:%s\n", bufptr);
    ch++;
}