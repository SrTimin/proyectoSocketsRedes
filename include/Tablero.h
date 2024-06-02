#ifndef TABLERO_H
#define TABLERO_H

#include <vector>

class Tablero {
public:
    static const int FILAS = 6;
    static const int COLUMNAS = 7;

    Tablero();

    bool colocarFicha(int columna, char jugador);
    bool hayGanador(char jugador);
    bool hayEmpate();
    void reiniciar();
    void imprimir();

private:
    std::vector<std::vector<char>> tablero;

    bool comprobarFilas(char jugador);
    bool comprobarColumnas(char jugador);
    bool comprobarDiagonales(char jugador);
};

#endif
