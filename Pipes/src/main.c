#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#define SIZES 6
#define KB 1024

//1KB, 10KB, 100KB, 1MB, 10MB, 100MB
int PACK_SIZES[SIZES] = {1*KB, 10*KB, 100*KB, 1*KB*KB, 10*KB*KB, 100*KB*KB};
//int PACK_TESTS[SIZES] = {1000,1000,100,100,10,5};
int PACK_TESTS[SIZES] = {6,5,4,3,2,1};

void fill(char * bufptr, size_t size);

int main() {
    int tests_amount;
    size_t size;
    
    //pipefd[0] -> lectura
    //pipefd[1] -> escritura
    int pipefd[2];

    int r;  //Variable de retorno.
    pid_t pid;

    r = pipe(pipefd);
    if (r < 0){
        perror("Error al crear la tubería");
        exit(-1);
    }

    printf("Medidas de envío de paquetes con comunicación mediante Tuberías.\n\n");

    pid = fork();
    if (pid == (pid_t) -1){
        perror("Error en fork");
        exit(-1);
    }

    for (int i = 0; i < SIZES; i++){
        int packages_amount;
        tests_amount = PACK_TESTS[i];
        size = PACK_SIZES[i];

        float time0, time1;
        float total_time = 0;

        char *write_buffer[size];
        char *read_buffer[size];
        fill(write_buffer, size);

        for (int j = 1; j <= tests_amount; j++){
            time0 = clock();

            if (pid == 0){
                //Consumidor

                close(pipefd[1]);
                read(pipefd[0], read_buffer, size);
                close(pipefd[0]);
                //printf("\n Mensaje recibido de tamaño %d: %d \n", size, strlen(read_buffer));

            }else {
                //Productor
                close(pipefd[0]);
                write(pipefd[1], write_buffer, size);
                close(pipefd[1]);
            }
            time1 = clock();
            double tiempo = (double)(time1) / CLOCKS_PER_SEC-(double)(time0) / CLOCKS_PER_SEC;
            total_time = total_time + tiempo;
        }

        printf("El promedio con %d Bytes y %d pruebas es: %f seg. \n\n",size,tests_amount,total_time/tests_amount);
        total_time = 0;

    }
    return 0;
}

void fill(char * bufptr, size_t size){
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