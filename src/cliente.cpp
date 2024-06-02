#include "Conexion.h"
#include <iostream>
#include <string>
#include <sstream>
#include "Tablero.h"
#include "Jugador.h"

void procesarMensajeServidor(Tablero& tablero, Jugador& servidor, const std::string& mensaje) {
    if (mensaje.find("Servidor: ") == 0) {
        int columnaServidor = std::stoi(mensaje.substr(10));
        std::cout << "El servidor colocó su ficha en la columna " << columnaServidor << std::endl;
        if (!tablero.colocarFicha(columnaServidor - 1, servidor.getFicha())) {
            std::cout << "Error al colocar la ficha del servidor" << std::endl;
        }
        tablero.imprimir();  // Mostrar tablero después de la jugada del servidor
    } else if (mensaje == "Gana el servidor\n" || mensaje == "Gana el cliente\n" || mensaje == "Empate\n") {
        std::cout << mensaje << std::endl;
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <direccionIP> <puerto>" << std::endl;
        return 1;
    }

    std::string direccionIP = argv[1];
    int puerto = std::stoi(argv[2]);
    ClienteConexion cliente;

    if (!cliente.establecerConexion(direccionIP, puerto)) {
        std::cerr << "Error al establecer la conexión con el servidor" << std::endl;
        return 1;
    }

    Tablero tablero;
    Jugador jugador("Jugador", 'C');
    Jugador servidor("Servidor", 'S');
    bool turnoCliente = false;

    while (true) {
        tablero.imprimir();
        std::string datos;

        while (true) {
            datos = cliente.recibirDatos();
            if (datos.empty()) {
                std::cerr << "Error al recibir datos del servidor" << std::endl;
                exit(0);
            }

            std::istringstream stream(datos);
            std::string linea;
            while (std::getline(stream, linea)) {
                std::cout << "Datos recibidos del servidor: " << linea << std::endl;
                if (linea == "Tu turno") {
                    turnoCliente = true;
                } else {
                    procesarMensajeServidor(tablero, servidor, linea);
                }
            }

            if (turnoCliente) {
                break;  // Salir del bucle interno para proceder con el turno del cliente
            }
        }

        if (!turnoCliente) {
            continue;
        }

        // Ahora es el turno del cliente
        std::cout << "Ingrese la columna (1-" << Tablero::COLUMNAS << "): ";
        std::string columnaStr;
        std::getline(std::cin, columnaStr);
        int columna;

        try {
            columna = std::stoi(columnaStr);
        } catch (...) {
            std::cout << "Entrada inválida. Ingrese un número válido." << std::endl;
            continue;
        }

        if (columna < 1 || columna > Tablero::COLUMNAS) {
            std::cout << "Columna inválida" << std::endl;
            continue;
        }

        cliente.enviarDatos(columnaStr + "\n");

        if (!tablero.colocarFicha(columna - 1, jugador.getFicha())) {
            std::cout << "Error al colocar la ficha del cliente" << std::endl;
            continue;
        }

        tablero.imprimir();  // Mostrar tablero después de la jugada del cliente
        turnoCliente = false; // Indica que ya no es el turno del cliente
    }

    cliente.cerrarConexion();
    return 0;
}
