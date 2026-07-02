#include "tablero.h"
#include <stdio.h>
#include <stdlib.h>

void inicializarTablero(int tablero[FILAS][COLUMNAS]) // recorre la matriz iniciando todo en 0
{
    int i, j;
    for (i = 0; i < FILAS; i++)
    {
        for (j = 0; j < COLUMNAS; j++)
        {
            tablero[i][j] = 0;
        }
    }
}

int fueraDeLimites(int nuevaFila, int nuevaColumna)
{ // verdadero cuando esta fuera de tablero
    if (nuevaFila >= 0 && nuevaColumna >= 0 && nuevaFila < FILAS && nuevaColumna < COLUMNAS)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void generarManzana(Manzana *m, int tablero[FILAS][COLUMNAS])
{

    int filaRandom;
    int columnaRandom;

    do
    {
        filaRandom = rand() % FILAS;
        columnaRandom = rand() % COLUMNAS;
    } while (tablero[filaRandom][columnaRandom] != 0);

    m->fila = filaRandom;
    m->columna = columnaRandom;
}

int colisionaConCuerpo(int tablero[FILAS][COLUMNAS], int fila, int columna)
{
    return tablero[fila][columna] == CUERPO;
}