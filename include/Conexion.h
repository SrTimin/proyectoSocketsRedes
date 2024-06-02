#ifndef CONEXION_H
#define CONEXION_H

#include <string>
#include <netinet/in.h> // Incluir para sockaddr_in

class Conexion {
public:
    virtual ~Conexion() {}
    virtual bool establecerConexion(const std::string& direccionIP, int puerto) = 0;
    virtual bool enviarDatos(const std::string& datos) = 0;
    virtual std::string recibirDatos() = 0;
    virtual void cerrarConexion() = 0;
};

class ClienteConexion : public Conexion {
private:
    int socketCliente;
    sockaddr_in direccionServidor;
public:
    ClienteConexion(); // Constructor por defecto
    ~ClienteConexion(); // Destructor
    bool establecerConexion(const std::string& direccionIP, int puerto) override;
    bool enviarDatos(const std::string& datos) override;
    std::string recibirDatos() override;
    void cerrarConexion() override;
};

class ServidorConexion : public Conexion {
private:
    int socketServidor;
    sockaddr_in direccionServidor;
    int socketCliente;
    std::string ipCliente;  // Almacena la IP del cliente
    int puertoCliente;      // Almacena el puerto del cliente
public:
    ServidorConexion(); // Constructor por defecto
    ~ServidorConexion(); // Destructor
    bool establecerConexion(const std::string& direccionIP, int puerto) override;
    bool enviarDatos(const std::string& datos) override;
    std::string recibirDatos() override;
    void cerrarConexion() override;
    int aceptarConexion();
    void setSocketCliente(int socket);
    std::string getDireccionCliente() const;  // Nueva función para obtener la IP del cliente
    int getPuertoCliente() const;  // Nueva función para obtener el puerto del cliente
};

#endif
