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
void actualizarMatrizSerpiente(int tablero[FILAS][COLUMNAS], Serpiente *s)
{
    // 1. Primero "limpiamos" toda la matriz de cuerpo viejo,
    //    así no queda basura de la vuelta anterior (ej: la cola que se movió)
    int i, j;
    for (i = 0; i < FILAS; i++)
    {
        for (j = 0; j < COLUMNAS; j++)
        {
            if (tablero[i][j] == CUERPO)
            {
                tablero[i][j] = VACIO;
            }
        }
    }

    // 2. Recorremos la lista actual y marcamos CUERPO en cada nodo vigente
    Nodo *temp = s->cabeza;
    while (temp != NULL)
    {
        tablero[temp->fila][temp->columna] = CUERPO;
        temp = temp->siguiente;
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

int colisionaConCuerpoLista(Serpiente *s, int fila, int col, int ignorarCola)
{
    Nodo *actual = s->cabeza;
    while (actual != NULL)
    {
        // si hay que ignorar la cola y este es el ultimo nodo, no lo comparamos
        if (ignorarCola && actual->siguiente == NULL)
        {
            break;
        }
        if (actual->fila == fila && actual->columna == col)
        {
            return 1;
        }
        actual = actual->siguiente;
    }
    return 0;
}
void generarObstaculos(int tablero[FILAS][COLUMNAS], Obstaculo obstaculos[], int cantidad)
{
    int i;
    int filaRandom, columnaRandom;

    for (i = 0; i < cantidad; i++)
    {
        do
        {
            filaRandom = rand() % FILAS;
            columnaRandom = rand() % COLUMNAS;
        } while (tablero[filaRandom][columnaRandom] != 0);

        tablero[filaRandom][columnaRandom] = OBSTACULO; // para que se dibuje

        // guardamos la posicion tambien en el array de structs
        obstaculos[i].fila = filaRandom;
        obstaculos[i].columna = columnaRandom;
    }
}

int colisionaConObstaculo(Obstaculo obstaculos[], int cantidad, int fila, int columna)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (obstaculos[i].fila == fila && obstaculos[i].columna == columna)
        {
            return 1;
        }
    }
    return 0;
}
