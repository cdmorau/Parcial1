all: consumidor productor borrarO eje
SRC_DIR    = ./src/
BIN_DIR    = ./bin/
OBJ_DIR    = ./obj/
INC_DIR    = ./includes/
F_DIR      = ./files/

#Generar el ejecutable de consumidor
consumidor: consumidor.o
	gcc -o  $(BIN_DIR)consumidor consumidor.o 

consumidor.o: $(SRC_DIR)consumidor.c 
	gcc -c  $(SRC_DIR)consumidor.c -o consumidor.o

#Generar el ejecutable de productor
productor: productor.o 
	gcc -o  $(BIN_DIR)productor productor.o 

productor.o:  $(SRC_DIR)productor.c 
	gcc -c  $(SRC_DIR)productor.c -o productor.o

borrarO:
	rm -r *o 

eje: 
	chmod 777 bashScript.sh
	clear
	./bashScript.sh
	

#Borra todos los *.o los ejecutables en bin
clean:
	clear
