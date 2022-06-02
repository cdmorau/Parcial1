Para ejecutar el archivo simplemente ejecute make en su consola en la carpeta donde esté ubicado el documento
Una breve descripción de los archivos que encontrará

productor.c
    Encargado de construir y enviar paquetes a consumidor, junto con una flag
    que le indique en que momento debe de leer.
    Dicho paquete es un array de caracteres hecho con malloc que será almacenado
    en el archivo paquete para su posterior lectura.

consumidor.c
    Encargado de recibir el paquete y enviar una flag al proceso
    productor que indique a productor que el paquete ha sido leido por completo.

Flags
    Archivo con contenido binario que permite el envio de señales entre el proceso
    productor y consumidor por medio de un entero almacenado en dicho archivo.

    A continuación un resumen de las señales:

    0:  Indica que el paquete ha sido almacenado con exito en el archivo paquete,
        lo cual indica a consumidor que es momento de que los lea.
    1:  Indica que el paquete ha sido leido en su totalidad y por tanto es momento
        de guardar el tiempo en el cual dicho proceso finalizó, y calcular el tiempo de ejecución.
        Tambíen indica que productor que puede iniciar con el almacenamiento de un nuevo paquete.
paquete
    Almacena los paquetes generados por productor
    

