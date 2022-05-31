#include <stdio.h>          
#include <stdlib.h>         //error
#include <sys/socket.h>     //Configura Socket
#include <sys/types.h>      //Se puede quitar en linux
#include <netinet/in.h>     //Configurar el socket
#include <netdb.h>          //Configurar el socket
#include <arpa/inet.h>      //Para htons
#include <unistd.h>         //

#include <strings.h>        
#include <sys/time.h>

#define PORT 5000

#define SERVER_ADDRESS  "127.0.0.1"     //Direccion del servidor
#define FILENAME        "./100MB.bin"   //se crea un archivo para guardar la info del archivo que viene del servidor
#define BUF_SIZE 10000        //Tamaño de los paquetes

#define N_TESTING 100       //Numero de pruebas

int main()
{
    int clientfd, r;
    struct sockaddr_in client;
    struct hostent *he;
    char buffer[BUF_SIZE];

    ssize_t len;
    int file_size;
    FILE *received_file;
    int remain_data = 0;

    //Para medir tiempo
	struct timeval ti, tf;
	double tiempo; 

    /* Create client socket */
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0){
        perror("\n-->Error en socket(): ");
        exit(-1);
    }

    /* Construct client struct */
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    inet_aton(SERVER_ADDRESS, &client.sin_addr);    //Cambio instruccion del profe (flojera)
    
    
    /* Connect to the server */
    r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
    if(r < 0){
        perror("\n-->Error en conect()");
        exit(-1);
    }

    /* Receiving file size */
    recv(clientfd, buffer, BUF_SIZE, 0);
    file_size = atoi(buffer);

    /* Create the new file */
    received_file = fopen(FILENAME, "w+b");
    if(received_file == NULL){
        perror("\n-->Error en fopen()");
        exit(-1);
    }

    remain_data = file_size;

    /* Receiving file */
    len = recv(clientfd, buffer, BUF_SIZE, 0);


    /* Initial instant for time measurement */
    gettimeofday(&ti, NULL); 

    /* Loop for average time */
    for (int i = 0; i < N_TESTING; i++)
    {
        while ((remain_data > 0) && (len > 0))
        {
                fwrite(buffer, sizeof(char), len, received_file);
                remain_data -= len;
                /* fprintf(stdout, "Receive %ld bytes and we hope :- %d bytes\n", len, remain_data); */
        }
    }
    /* Final instant for time measurement */
    gettimeofday(&tf, NULL);

    /* Average time calculation */
    tiempo = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0;
    tiempo = tiempo / N_TESTING;
    printf("La tiempo promedio tarda: %g milisegundos\n", tiempo);

    /* Close sockets and file */
    fclose(received_file);
    close(clientfd);

    return 0;

}


