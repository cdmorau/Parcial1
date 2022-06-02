all: servidor cliente borrar0 eje
SRC_DIR = ./src/
BIN_DIR = ./bin/
OBJ_DIR = ./obj/
INC_DIR = ./includes/
F_DIR = ./files/

# Generar el ejecutable de servidor
servidor: servidor.o
	gcc -o $(BIN_DIR)servidor servidor.o

servidor.o: $(SRC_DIR)servidor.c
	gcc -c $(SRC_DIR)servidor.c -o servidor.o

# Generar el ejecutable de cliente
cliente: cliente.o
	gcc -o $(BIN_DIR)cliente cliente.o

cliente.o: $(SRC_DIR)cliente.c
	gcc -c $(SRC_DIR)cliente.c -o cliente.o

borrar0:
	rm -r *o

eje:
	chmod 777 bashScript.sh
	clear
	./bashScript.sh