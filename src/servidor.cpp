#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <memory>
#include <random>
#include "Servidor.h"
#include "Tablero.h"
#include "Jugador.h"
#include "Conexion.h"

enum class EstadoJuego { TurnoServidor, TurnoCliente, GanaServidor, GanaCliente, Empate };

void turnoServidor(Tablero& tablero, Jugador& servidor, std::shared_ptr<ServidorConexion> conexion, const std::string& clienteInfo) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, Tablero::COLUMNAS - 1);

    int columnaServidor;
    do {
        columnaServidor = dis(gen);
    } while (!tablero.colocarFicha(columnaServidor, servidor.getFicha()));

    std::cout << "Juego " << clienteInfo << ": servidor juega columna " << (columnaServidor + 1) << std::endl;
    conexion->enviarDatos("Servidor: " + std::to_string(columnaServidor + 1) + "\n");

    if (tablero.hayGanador(servidor.getFicha())) {
        conexion->enviarDatos("Gana el servidor\n");
        std::cout << "Gana el servidor\n" << std::endl;
    } else if (tablero.hayEmpate()) {
        conexion->enviarDatos("Empate\n");
        std::cout << "Empate\n" << std::endl;
    } else {
        conexion->enviarDatos("Tu turno\n");
    }
}

EstadoJuego turnoCliente(Tablero& tablero, Jugador& cliente, std::shared_ptr<ServidorConexion> conexion, const std::string& clienteInfo) {
    std::string datos = conexion->recibirDatos();
    if (datos.empty()) {
        std::cout << "Cliente desconectado" << std::endl;
        return EstadoJuego::Empate;
    }

    int columna;
    try {
        columna = std::stoi(datos);
    } catch (...) {
        conexion->enviarDatos("Columna inválida\n");
        return EstadoJuego::TurnoCliente;
    }

    if (!tablero.colocarFicha(columna - 1, cliente.getFicha())) {
        conexion->enviarDatos("Columna inválida\n");
        return EstadoJuego::TurnoCliente;
    }

    std::cout << "Juego " << clienteInfo << ": cliente juega columna " << columna << std::endl;

    if (tablero.hayGanador(cliente.getFicha())) {
        conexion->enviarDatos("Gana el cliente\n");
        std::cout << "Gana el cliente\n" << std::endl;
        return EstadoJuego::GanaCliente;
    } else if (tablero.hayEmpate()) {
        conexion->enviarDatos("Empate\n");
        std::cout << "Empate\n" << std::endl;
        return EstadoJuego::Empate;
    }

    return EstadoJuego::TurnoServidor;
}

void manejarJuego(std::shared_ptr<ServidorConexion> conexion, const std::string& clienteInfo) {
    Tablero tablero;
    Jugador servidor("Servidor", 'S');
    Jugador cliente("Cliente", 'C');

    EstadoJuego estado = EstadoJuego::TurnoServidor;

    std::cout << "Juego " << clienteInfo << ": inicia juego el " << (estado == EstadoJuego::TurnoServidor ? "servidor" : "cliente") << "." << std::endl;

    while (estado == EstadoJuego::TurnoServidor || estado == EstadoJuego::TurnoCliente) {
        if (estado == EstadoJuego::TurnoServidor) {
            turnoServidor(tablero, servidor, conexion, clienteInfo);
            estado = EstadoJuego::TurnoCliente;
            tablero.imprimir();
        } else {
            estado = turnoCliente(tablero, cliente, conexion, clienteInfo);
            tablero.imprimir();
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <puerto>" << std::endl;
        return 1;
    }

    int puerto = std::stoi(argv[1]);

    std::shared_ptr<ServidorConexion> servidor = std::make_shared<ServidorConexion>();
    if (!servidor->establecerConexion("", puerto)) {
        std::cerr << "Error al establecer la conexión del servidor" << std::endl;
        return 1;
    }

    std::cout << "Servidor escuchando en el puerto " << puerto << std::endl;
    std::cout << "Esperando conexiones de clientes..." << std::endl;

    std::vector<std::thread> hilosJuego;

    while (true) {
        int clienteSocket = servidor->aceptarConexion();
        if (clienteSocket >= 0) {
            auto conexionCliente = std::make_shared<ServidorConexion>();
            conexionCliente->setSocketCliente(clienteSocket);

            std::string clienteInfo = "[" + conexionCliente->getDireccionCliente() + ":" + std::to_string(conexionCliente->getPuertoCliente()) + "]";
            std::cout << "Juego nuevo " << clienteInfo << std::endl;

            hilosJuego.emplace_back([conexionCliente, clienteInfo]() {
                manejarJuego(conexionCliente, clienteInfo);
            });
        }
    }

    for (auto& hilo : hilosJuego) {
        hilo.join();
    }

    return 0;
}
