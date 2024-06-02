#include "Jugador.h"

Jugador::Jugador(const std::string& nombre, char ficha)
    : nombre(nombre), ficha(ficha), puntaje(0) {}

std::string Jugador::getNombre() const {
    return nombre;
}

char Jugador::getFicha() const {
    return ficha;
}

int Jugador::getPuntaje() const {
    return puntaje;
}

void Jugador::incrementarPuntaje() {
    puntaje++;
}