#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>


#include <fcntl.h>          //  Maneja las constantes en los open
#include <sys/stat.h>       //  Da info del archivo
#include <sys/sendfile.h>   //  reemplazar el read y el write

#include <sys/time.h>

#define PORT 5000
#define BACKLOG 2
#define BUF_SIZE 10000
#define FILE_TO_SEND "../files/Test_1KB.bin"
#define N_TESTING 100

//char* files[5] = {"../files/Test_1KB.bin", "../files/Test_10KB.bin", "../files/Test_1MB.bin", "../files/Test_10MB.bin", "../files/Test_100MB.bin"};

int main(int argc, char **argv)
{
    printf("Ejecutando servidor\n");
    int serverfd, clientfd, r, opt = 1;
    struct sockaddr_in server, client; 
    socklen_t   tamano;
    ssize_t len, len2;
    int fd;
    struct stat file_stat;
    char file_size[BUF_SIZE];
    off_t offset = 0;
    int remain_data;
    int sent_bytes = 0;

    /* Create server socket */
    serverfd = socket(AF_INET, SOCK_STREAM, 0); //AF_INET IPv4,SOCK_STREM(TCP- ANALOGIA NITRO), 
    if(serverfd < 0){
        perror("\n-->Error en socket() servidor: ");
        exit(-1);
    }
    
    /* Construct server struct */
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8);
    
   /*  setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int)); */

    /* Bind */
    r = bind(serverfd, (struct sockaddr *)&server, sizeof(struct sockaddr));
    if(r < 0){
        perror("\n-->Error en bind() servidor: ");
        exit(-1);
    }

    /* Listening to incoming connections */
    r = listen(serverfd, BACKLOG);
    if(r < 0){
        perror("\n-->Error en listen() servidor: ");
        exit(-1);
    }
    
    /* Open file to send */
    fd = open(FILE_TO_SEND, O_RDONLY);
    if(fd < 0){
        perror("\n-->Error en open() servidor: ");
        exit(-1);
    }

    /* Get file stats */
    r = fstat(fd, &file_stat);
    if (r < 0){
        perror("\n-->Error en fstat() servidor: ");
        exit(-1);
    }
    
    /* Print size of file */
    fprintf(stdout, "File Size: \n%ld bytes\n", file_stat.st_size);//se puede quitar
    
    tamano = sizeof(struct sockaddr_in);
    
    /* Accepting incoming connection */
    clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
    if(clientfd < 0){
        perror("\n-->Error en accept() servidor: ");
        exit(-1);
    }
    
   /* Store the size of the file in file_size */
    sprintf(file_size, "%ld", file_stat.st_size);
    
    /* Sending file size */
    len = send(clientfd, file_size, sizeof(file_size), 0);
    if(len < 0){
        perror("\n-->Error en recv() servidor: ");
        exit(-1);
    }
    
    remain_data = file_stat.st_size;

    
    /* Loop for average time */
    for (int i = 0; i < N_TESTING; i++)
    {
        /* Sending file data */
        while (((sent_bytes = sendfile(clientfd, fd, &offset, BUF_SIZE)) > 0) && (remain_data > 0))
        {
            /* fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %ld and remaining data = %d\n", sent_bytes, offset, remain_data); */
            remain_data -= sent_bytes;
            /* fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %ld and remaining data = %d\n", sent_bytes, offset, remain_data); */
        }
    }

    /* Close sockets */
    close(clientfd);
    close(serverfd);

    return 0;
}
