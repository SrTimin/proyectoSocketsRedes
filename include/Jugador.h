#ifndef JUGADOR_H
#define JUGADOR_H

#include <string>

class Jugador {
public:
    Jugador(const std::string& nombre, char ficha);

    std::string getNombre() const;
    char getFicha() const;
    int getPuntaje() const;
    void incrementarPuntaje();

private:
    std::string nombre;
    char ficha;
    int puntaje;
};

#endif