#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>

char * attach_shm(char *filename, int size);
bool detach_shm(char *block);
bool destroy_shm(char *filename);

#define FILENAME "../src/main.c"

#endif