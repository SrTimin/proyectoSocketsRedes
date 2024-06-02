#include "Conexion.h"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Implementación de los métodos de ClienteConexion

ClienteConexion::ClienteConexion() : socketCliente(-1) {}

ClienteConexion::~ClienteConexion() {
    cerrarConexion();
}

bool ClienteConexion::establecerConexion(const std::string& direccionIP, int puerto) {
    socketCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketCliente < 0) {
        std::cerr << "Error al crear el socket del cliente" << std::endl;
        return false;
    }

    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(puerto);
    if (inet_pton(AF_INET, direccionIP.c_str(), &direccionServidor.sin_addr) <= 0) {
        std::cerr << "Dirección IP inválida" << std::endl;
        close(socketCliente);
        return false;
    }

    if (connect(socketCliente, (struct sockaddr*)&direccionServidor, sizeof(direccionServidor)) < 0) {
        std::cerr << "Error al conectar con el servidor" << std::endl;
        close(socketCliente);
        return false;
    }

    return true;
}

bool ClienteConexion::enviarDatos(const std::string& datos) {
    if (send(socketCliente, datos.c_str(), datos.length(), 0) < 0) {
        std::cerr << "Error al enviar datos al servidor" << std::endl;
        return false;
    }
    return true;
}

std::string ClienteConexion::recibirDatos() {
    char buffer[1024] = {0};
    int bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer), 0);
    if (bytesRecibidos <= 0) {
        std::cerr << "Error al recibir datos del servidor" << std::endl;
        return "";
    }
    return std::string(buffer, bytesRecibidos);
}

void ClienteConexion::cerrarConexion() {
    if (socketCliente >= 0) {
        close(socketCliente);
        socketCliente = -1;
    }
}

// Implementación de los métodos de ServidorConexion

ServidorConexion::ServidorConexion() : socketServidor(-1), socketCliente(-1), puertoCliente(0) {}

ServidorConexion::~ServidorConexion() {
    cerrarConexion();
}

bool ServidorConexion::establecerConexion(const std::string& direccionIP, int puerto) {
    socketServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServidor < 0) {
        std::cerr << "Error al crear el socket del servidor" << std::endl;
        return false;
    }

    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = INADDR_ANY; // Escuchar en todas las interfaces
    direccionServidor.sin_port = htons(puerto);

    if (bind(socketServidor, (struct sockaddr*)&direccionServidor, sizeof(direccionServidor)) < 0) {
        std::cerr << "Error en bind" << std::endl;
        close(socketServidor);
        return false;
    }

    if (listen(socketServidor, 5) < 0) {
        std::cerr << "Error en listen" << std::endl;
        close(socketServidor);
        return false;
    }

    return true;
}

int ServidorConexion::aceptarConexion() {
    sockaddr_in direccionCliente;
    socklen_t len = sizeof(direccionCliente);
    int clienteSocket = accept(socketServidor, (struct sockaddr*)&direccionCliente, &len);
    if (clienteSocket < 0) {
        std::cerr << "Error al aceptar la conexión del cliente" << std::endl;
        return -1;
    }
    ipCliente = inet_ntoa(direccionCliente.sin_addr);
    puertoCliente = ntohs(direccionCliente.sin_port);
    return clienteSocket;
}

bool ServidorConexion::enviarDatos(const std::string& datos) {
    if (send(socketCliente, datos.c_str(), datos.size(), 0) < 0) {
        std::cerr << "Error al enviar datos al cliente" << std::endl;
        return false;
    }
    return true;
}

std::string ServidorConexion::recibirDatos() {
    char buffer[1024] = {0};
    int bytesRecibidos = recv(socketCliente, buffer, 1024, 0);
    if (bytesRecibidos <= 0) {
        std::cerr << "Error al recibir datos del cliente" << std::endl;
        return "";
    }
    return std::string(buffer, bytesRecibidos);
}

void ServidorConexion::cerrarConexion() {
    if (socketCliente >= 0) {
        close(socketCliente);
        socketCliente = -1;
    }
    if (socketServidor >= 0) {
        close(socketServidor);
        socketServidor = -1;
    }
}

void ServidorConexion::setSocketCliente(int socket) {
    if (socketCliente >= 0) {
        close(socketCliente);
    }
    socketCliente = socket;
}

std::string ServidorConexion::getDireccionCliente() const {
    return ipCliente;
}

int ServidorConexion::getPuertoCliente() const {
    return puertoCliente;
}
