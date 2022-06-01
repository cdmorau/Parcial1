all: main borrar eje
SRC_DIR    = ./src/
BIN_DIR    = ./bin/
OBJ_DIR    = ./obj/
INC_DIR    = ./includes/
F_DIR      = ./files/

#Generar el ejecutable de main
main: main.o shared_memory.o
	gcc -o $(BIN_DIR)main main.o shared_memory.o -pthread

main.o: $(SRC_DIR)main.c
	gcc -c $(SRC_DIR)main.c

shared_memory.o: $(INC_DIR)shared_memory.c
	gcc -c $(INC_DIR)shared_memory.c

borrar:
	rm -r *o

eje: 
	chmod 777 bashScript.sh
	clear
	./bashScript.sh
	
#Borra todos los *.o los ejecutables en bin
clean:
	clear