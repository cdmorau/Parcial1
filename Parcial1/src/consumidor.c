#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>



//Arreglo con los tamaños de los paquetes
int paquetes[6]={1000,10000,100000,1000000,10000000,100000000};
//Arreglo con el numero de pruebas según el tamaño de cada paquete
int pruebas[6]={1000,1000,100,100,10,5};
//Señar guardada en el archivo Flags
int flag=0;


//Guarda un paquete de determinado tamaño en el archivo paquete
void guardarPaquete(int dimension);
//Cambia el valor almacenado en el archivo Flags
void cambiarFlagA(int cambio);
//Lee el valor almacenado en el archivo Flags
int leerFlag();


int main()
{   
    
    cambiarFlagA(1);
    for(int i=0;i<6;i++){
        for (int j=0; j<pruebas[i];j++){
            while (leerFlag()!=0){}
            guardarPaquete(paquetes[i]);
            cambiarFlagA(1);
        }
    }
    return (0);
}

void guardarPaquete(int dimension){

    cambiarFlagA(-1);
    char *arreglochar = malloc(dimension * sizeof(char));

    int i = 0;
    FILE *paquete = fopen("../files/paquete", "r");
    if (!paquete)
    {
        perror("No fue posible abrir el archivo de comunicación");
    }

    //Se lee el archivo creado por productor
    for (int i = 0; i<dimension;i++)
    {
        fread(&arreglochar[i], sizeof(char), 1, paquete);
    }

    rewind(paquete);
    fclose(paquete);
    free(arreglochar);
}



void cambiarFlagA(int cambio){

    FILE *Flags = fopen("../files/Flags","wb");
    if (!Flags){
		perror("No fue posible abrir el archivo Flags");
	}
    
    fwrite(&cambio,sizeof(int),1,Flags);
    rewind(Flags);
    fclose(Flags);
    
}

int leerFlag(){
    FILE *Flags = fopen("../files/Flags","rb");
    if (!Flags){
		perror("No fue posible abrir el archivo Flags");
	}
    fread(&flag,sizeof(int),1,Flags);
    rewind(Flags);
    fclose(Flags);
    return flag;
}


