#ifndef TABLERO_H
#define TABLERO_H

#define FILAS 20
#define COLUMNAS 40

#define VACIO 0
#define CUERPO 1
#define MANZANA 2
#define OBSTACULO 3
#define MAX_OBSTACULOS 20

#include "snake.h"

typedef struct
{
    int fila;
    int columna;
} Manzana;

typedef struct
{
    int fila;
    int columna;
} Obstaculo;

void actualizarMatrizSerpiente(int tablero[FILAS][COLUMNAS], Serpiente *s);
void inicializarTablero(int tablero[FILAS][COLUMNAS]);
int fueraDeLimites(int fila, int col);
void generarManzana(Manzana *m, int tablero[FILAS][COLUMNAS]);
void generarObstaculos(int tablero[FILAS][COLUMNAS], Obstaculo obstaculos[], int cantidad);
int colisionaConObstaculo(Obstaculo obstaculos[], int cantidad, int fila, int columna);
int colisionaConCuerpoLista(Serpiente *s, int fila, int col, int ignorarCola);

#endif