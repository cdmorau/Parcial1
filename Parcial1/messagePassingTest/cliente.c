//Creación de socket en cliente
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 3535

int main(){
	int clientfd;
	int r;
	struct sockaddr_in client;

	//Creación de socket.
	clientfd = socket(AF_INET, SOCK_STREAM, 0);

	client.sin_family = AF_INET;
	//Conexión con el servidor.
	client.sin_port = htons(PORT);
	inet_aton("192.168.0.4", &client.sin_addr);
	//Convierte la dirección a binario y la asigna al atributo del struct.
	
	r = connect(clientfd, (struct sockaddr*)&client, (socklen_t) sizeof(struct sockaddr));
	if (r < 0){
		perror("Error en connect");
		exit(-1);
	}

	r = recv(clientfd, buffer, 10, 0);
	if (r < 0){
		perror("Error en recv");
		exit(-1);
	}

	//Imprime el mensaje recibido en buffer.
	buffer[r] = 0;
	printf("\n Mensaje: %s", buffer);
	
	//Cierra conexión.
	close(clientfd);
	return 0;
}