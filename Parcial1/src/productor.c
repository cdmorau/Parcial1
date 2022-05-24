#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//constantes

//Arreglo con los tamaños de los paquetes
int paquetes[6]={1000,10000,100000,1000000,10000000,100000000};
//Arreglo con el numero de pruebas según el tamaño de cada paquete
int pruebas[6]={1000,1000,100,100,10,5};
//Tiempo inicial del proceso
int inicio=0;
//Tiempo finalizado
int fin=0;
//Suma de todos los tiempos obtenidos de un número determinado de pruebas
double sumT=0;
//Señar guardada en el archivo Flags
int flag=0;

//Funciones

//Calcula el tiempo que toma enviar y recibir un paquete con determinada dimensión
double tiempoParticular(int dimension);
//Guarda un paquete de determinado tamaño en el archivo paquete
void guardarPaquete(int dimension,char* arreglochar);
//Cambia el valor almacenado en el archivo Flags
void cambiarFlagA(int cambio);
//Lee el valor almacenado en el archivo Flags
int leerFlag();
//Vacia el archivo paquete
void vaciarPaquete();


int main() {

    system("clear");
    printf("Medidas de envío de paquetes con comunicación mediante Archivos \n");

    //Este for cambia el tamaño del paquete una vez terminadas las pruebas
    for(int i=0;i<6;i++){
        // //Este for reproduce las pruebas para cada paquete
        for (int j=0; j<pruebas[i];j++){
            //Este if ignora el primer valor obtenido ya que suele ser más lento 
            if (j==0){
                tiempoParticular(paquetes[i]);
            }
            else{
                //Aquí se acumulan los tiempos obtenidos en cada prueba
                sumT=sumT+tiempoParticular(paquetes[i]);
            }
        }
        //Imprime el promedio de los tiempos obtenidos y reinicia el valor acumulado de sumaT a 0
        printf("El promedio con %d Bytes y %d pruebas es: %f \n",paquetes[i],pruebas[i],sumT/(pruebas[i]-1));
        sumT=0;
    }
    vaciarPaquete();
    return(0);

}


void guardarPaquete(int dimension, char* arreglochar){
    FILE *paquete = fopen("../files/paquete","w");
    if (!paquete){
		perror("No fue posible abrir el archivo paqueteario");
	}
	rewind(paquete);
    //Recorre el array recibido y almacena su contenido en el archivo paquete
	for(int i = 0; i<dimension; i++){
		fwrite(&arreglochar[i],sizeof(char),1,paquete);
	}
    rewind(paquete);
	fclose(paquete);
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

double tiempoParticular(int dimension){
    
    //Creo y relleno un arreglo a modo de paquete
    char* arreglochar= malloc(dimension*sizeof(char));
    //Llena el malloc de letras 
    for(int i = 0; i<dimension; i++){
        arreglochar[i]= 'd'+(i%3);
	}
    
    arreglochar[dimension-1]='0';
    
    //Guardo el tiempo inicial
    inicio = clock();    
    //Meto los datos de dicho arreglo en un archivo
    guardarPaquete(dimension,arreglochar);
    free(arreglochar);
    //Mando la señal para leerlo
    cambiarFlagA(0);
    //Espero la señal para guardar el tiempo final
    while (leerFlag()!=1){}
    //Guardo el tiempo final
    fin= clock();
    
    //Calculo el tiempo obtenido
    double tiempo = (double)(fin) / CLOCKS_PER_SEC-(double)(inicio) / CLOCKS_PER_SEC;


    return tiempo;
}
void vaciarPaquete(){
    FILE *paquete = fopen("../files/paquete","w");
    if (!paquete){
		perror("No fue posible abrir el archivo paqueteario");
	}

    //Recorre el array recibido y almacena su contenido en el archivo paquete
	fwrite("No me quiero ir Señor Stark",28*sizeof(char),1,paquete);
    fclose(paquete);
	
}