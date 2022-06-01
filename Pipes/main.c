#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    int r;
    int pipefd[2];
    char buf[11];

    r = pipe(pipefd);
    if (r < 0){
        perror("Error pipe()");
        exit(-1);
    }

    pid = fork();

    if (pid < 0){
        perror("Error en fork");
        exit(-1);
    }

    if (pid == 0) {
        close(pipefd[0]);
        write(pipefd[1], "hola mundo", 10);
        close(pipefd[1]);
        exit(0);
    }else {
        close(pipefd[1]);
        r = read(pipefd[0], buf, 10);
        buf[r] = 0;
        close(pipefd[0]);
        printf("\n Mensaje del hijo: %s \n", buf);
    }

    return 0;
}