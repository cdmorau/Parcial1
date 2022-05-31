#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../includes/shared_memory.h"

#define SIZES 6
#define KB 1000
#define SHM_KEY 0x1234

int PACK_SIZES[SIZES] = {1*KB, 10*KB, 100*KB, 1*1000*KB, 10*1000*KB, 100*1000*KB};
int PACK_TESTS[SIZES] = {1e06, 1e05, 1e04, 1e03, 1e02, 1e01};

int main(){
    int size;
    int tests_amount;

    for (int i = 0; i < SIZES; i++){
        tests_amount = PACK_TESTS[i];
        size = PACK_SIZES[i];

        char *block = attach_shm(FILENAME, size);
        if (block == NULL) {
            printf("No se pudo abrir la memoria compartida\n");
            return -1;
        }

        for (int j = 1; j <= tests_amount; j++){
            printf("Lectura ShM. Test #%d/%d. Tamaño: %d Bytes.\n", j,tests_amount,size);
            printf("Contenido: \"%s\"\n\n", block);
        }

        detach_shm(block);

        if (destroy_shm(FILENAME)){
            printf("Bloque destruido.");
        } else {
            printf("No se pudo destruir el bloque.");
        }
    }
    printf("Lectura Completada.\n");
    return 0;
}