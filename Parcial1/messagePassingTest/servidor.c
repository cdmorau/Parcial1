//Creación de socket en servidor.
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 3535
#define BACKLOG 10

int main(){
	int serverfd, clientfd; //Files descriptors
	int r;
	struct sockaddr_in server;
	socklen_t socklen;

	//Creación de socket
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd < 0){
		perror("Error en socket");
		exit(-1);
	}

	//Configuración de dirección del socket servidor.
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = INADDR_ANY; //Cualquier dirección.
	bzero(server.sin_zero,8); //Llenar los ceros.
	//htons: Permite gestionar el direccionamiento de red, 
	//esto debido a que ciertas computadoras usan el estándar little endian y otras usan el estándar big endian.

	//Aplicación de la configuración en el socket.
	r = bind(serverfd, (struct sockaddr*) &server, sizeof(struct sockaddr));
	if (r < 0){
		perror("Error en bind");
		exit(-1);
	}

	//Se establece como socket pasivo.
	r = listen(serverfd, BACKLOG);
	if (r < 0){
		perror("Error en listen");
		exit(-1);
	}

	//Trae una conexión de la pila.
	clientfd = accept(serverfd, (struct sockaddr*)&client, &socklen);
	//Socket de cliente recibido cuando este realiza una conexión.
	if (clientfd < 0){
		perror("Error en accept");
		exit(-1);
	}
	return 0;

	//Envía un mensaje al socket conectado.
	r = send(clientfd, "hola mundo", 10, 0);
	if (r < 0){
		perror("Error en send");
		exit(-1);
	}

	//Cierra conexiones.
	close(clientfd);
	close(serverfd);
	return 0;
}