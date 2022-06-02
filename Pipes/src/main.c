#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#define SIZES 6
#define KB 1024

//1KB, 10KB, 100KB, 1MB, 10MB, 100MB
int PACK_SIZES[SIZES] = {1*KB, 10*KB, 100*KB, 1*KB*KB, 10*KB*KB, 100*KB*KB};
int PACK_TESTS[SIZES] = {1000,1000,100,100,10,5};
//int PACK_TESTS[SIZES] = {6,5,4,3,2,1};

static char get_next_byte() {
    static char byte = 'A';

    if (byte > 'Z')
        byte = 'A';

    return byte++;
}

static int read_exactly(int fd, size_t size) {
    unsigned char b;

    while (size > 0 && 1 == read(fd, &b, 1))
        size--;

    return 0 == size;
}

static int write_exact_string(int fd, size_t size, char b) {
    unsigned char zero = 0;

    while (size > 1 && 1 == write(fd, &b, 1))
        size--;

    return (1 == size) && (1 == write(fd, &zero, 1));
}

int main() {
    int tests_amount;
    size_t size;
    float time0, time1, total_time;
    double tiempo;
    int i,j;
    
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

    printf("\nMedidas de envío de paquetes con comunicación mediante Tuberías.\n\n");

    pid = fork();
    if (pid == (pid_t) -1){
        perror("Error en fork");
        exit(-1);
    }

    if (pid == 0){
        close(pipefd[1]);
    }else {
        close(pipefd[0]);
    }

    for (i = 0; i < SIZES; i++){
        tests_amount = PACK_TESTS[i];
        size = PACK_SIZES[i];
        
        total_time = 0;

        for (j = 1; j <= tests_amount; j++){
            time0 = clock();

            if (pid == 0){
                //Consumidor
                if (!read_exactly(pipefd[0], size)) {
                    fprintf(stderr, "Fallo en leer %zu bytes.\n", size);
                    exit(-1);
                }

            }else {
                //Productor
                char byte = get_next_byte();

                if (!write_exact_string(pipefd[1], size, byte)) {
                    fprintf(stderr, "Fallo en escribir [%zu*'%c'+'\\0'].\n", size - 1, byte);
                    exit(EXIT_FAILURE);
                }
            }
            time1 = clock();
            tiempo = ((double)(time1 - time0)) / CLOCKS_PER_SEC;
            //printf("Test<%d-%d>: %s %zu bytes en %f.\n", i, j, (pid == 0) ? "Leyó" : "Escribió", size, tiempo);
            total_time = total_time + tiempo;
        }

        printf("El promedio con %zu Bytes y %d pruebas en %s es: %f seg. \n\n",size,tests_amount,(pid == 0) ? "HIJO" : "PADRE", total_time/tests_amount);
        total_time = 0;

    }
    if (0 == pid) {
        close(pipefd[0]);
    } else {
        close(pipefd[1]);
        if (-1 == waitpid(pid, NULL, WUNTRACED)){
            perror("Error en waitpid()");
            exit(-1);
        }
    }
    return 0;
}