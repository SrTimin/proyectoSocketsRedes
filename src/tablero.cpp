#include "Tablero.h"
#include <iostream>
#include <iomanip>

Tablero::Tablero() {
    tablero = std::vector<std::vector<char>>(FILAS, std::vector<char>(COLUMNAS, ' '));
}

bool Tablero::colocarFicha(int columna, char jugador) {
    if (columna < 0 || columna >= COLUMNAS) {
        return false; // Columna inválida
    }

    for (int fila = FILAS - 1; fila >= 0; fila--) {
        if (tablero[fila][columna] == ' ') {
            tablero[fila][columna] = jugador;
            return true;
        }
    }

    return false; // Columna completa
}

bool Tablero::hayGanador(char jugador) {
    return comprobarFilas(jugador) || comprobarColumnas(jugador) || comprobarDiagonales(jugador);
}

bool Tablero::hayEmpate() {
    for (int col = 0; col < COLUMNAS; col++) {
        if (tablero[0][col] == ' ') {
            return false; // Aún hay columnas disponibles
        }
    }
    return true; // Todas las columnas están completas
}

void Tablero::reiniciar() {
    tablero = std::vector<std::vector<char>>(FILAS, std::vector<char>(COLUMNAS, ' '));
}

void Tablero::imprimir() {
    std::cout << "TABLERO" << std::endl;
    for (int fila = 0; fila < FILAS; fila++) {
        std::cout << std::setw(2) << FILAS - fila;
        for (int col = 0; col < COLUMNAS; col++) {
            std::cout << " " << tablero[fila][col];
        }
        std::cout << std::endl;
    }
    std::cout << " ";
    for (int col = 0; col < COLUMNAS; col++) {
        std::cout << " " << col + 1;
    }
    std::cout << std::endl;
}

bool Tablero::comprobarFilas(char jugador) {
    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col <= COLUMNAS - 4; col++) {
            if (tablero[fila][col] == jugador &&
                tablero[fila][col + 1] == jugador &&
                tablero[fila][col + 2] == jugador &&
                tablero[fila][col + 3] == jugador) {
                return true;
            }
        }
    }
    return false;
}

bool Tablero::comprobarColumnas(char jugador) {
    for (int col = 0; col < COLUMNAS; col++) {
        for (int fila = 0; fila <= FILAS - 4; fila++) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col] == jugador &&
                tablero[fila + 2][col] == jugador &&
                tablero[fila + 3][col] == jugador) {
                return true;
            }
        }
    }
    return false;
}

bool Tablero::comprobarDiagonales(char jugador) {
    // Diagonales ascendentes
    for (int fila = 0; fila <= FILAS - 4; fila++) {
        for (int col = 0; col <= COLUMNAS - 4; col++) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col + 1] == jugador &&
                tablero[fila + 2][col + 2] == jugador &&
                tablero[fila + 3][col + 3] == jugador) {
                return true;
            }
        }
    }

    // Diagonales descendentes
    for (int fila = 0; fila <= FILAS - 4; fila++) {
        for (int col = 3; col < COLUMNAS; col++) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col - 1] == jugador &&
                tablero[fila + 2][col - 2] == jugador &&
                tablero[fila + 3][col - 3] == jugador) {
                return true;
            }
        }
    }

    return false;
}
