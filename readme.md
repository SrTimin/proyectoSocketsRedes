Cuatro en Línea

Descripción:

Este proyecto implementa el clásico juego Cuatro en Línea utilizando un modelo Cliente/Servidor en C++. El objetivo es alinear cuatro fichas consecutivas en horizontal, vertical o diagonal antes que el oponente.

Funcionalidades:

    Implementación del juego Cuatro en Línea.
    Modelo Cliente/Servidor para la comunicación entre jugadores.
    Interfaz de línea de comandos para el servidor y el cliente.
    Compilación sencilla mediante Makefile.

Requisitos:

    Compilador C++ (g++)
    Herramienta make

Compilación:

    Asegúrese de tener g++ y make instalados.
    Diríjase al directorio del proyecto.
    Ejecute el siguiente comando para compilar el servidor y el cliente:

Bash

make



Ejecución:

Servidor:

    Ejecute el servidor indicando el puerto en el que se escuchará las conexiones entrantes:

Bash

./servidor <puerto>


Ejemplo:
Bash

./servidor 7777



Cliente:

    Ejecute el cliente indicando la dirección IP y el puerto del servidor al que se conectará:

Bash

./cliente <ip_servidor> <puerto>


Ejemplo:
Bash

./cliente 192.168.1.100 7777


