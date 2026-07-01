#ifndef TABLERO_H
#define TABLERO_H

#define FILAS 20
#define COLUMNAS 40

typedef struct
{
    int fila;
    int columna;
} Manzana;

void inicializarTablero(int tablero[FILAS][COLUMNAS]);
void generarManzana(Manzana *m, int tablero[FILAS][COLUMNAS]);
int fueraDeLimites(int fila, int col);

#endif