#ifndef TABLERO_H // si el simbolo TABLERO_H todavia no existe...
#define TABLERO_H // ...lo definimos ya, para que si este archivo se vuelve a incluir en otro lado
                  // dentro de la misma compilacion, el preprocesador salte directo al #endif
                  // y no vuelva a declarar todo esto por segunda vez (error de "redefinicion")

#define FILAS 20    // cantidad de filas del tablero (alto del mundo del juego)
#define COLUMNAS 40 // cantidad de columnas del tablero (ancho del mundo del juego)

#define VACIO 0     // valor de una casilla sin nada
#define CUERPO 1    // valor de una casilla ocupada por el cuerpo de la serpiente
#define MANZANA 2   // valor de una casilla ocupada por la manzana
#define OBSTACULO 3 // valor de una casilla ocupada por un obstaculo fijo
#define COLISION 4  // valor especial para marcar la casilla exacta donde chocamos

#include "snake.h" // necesitamos el tipo "Serpiente" para el prototipo de actualizarMatrizSerpiente

// Struct para la posicion de la manzana: solo necesita saber donde esta (fila, columna)
typedef struct
{
    int fila;
    int columna;
} Manzana;

// Struct para la posicion de CADA obstaculo: misma forma que Manzana, pero es un tipo
// aparte porque conceptualmente representa una cosa distinta (aunque los campos sean iguales)
typedef struct
{
    int fila;
    int columna;
} Obstaculo;

// --- PROTOTIPOS: el "contrato publico" de este modulo, implementado en tablero.c ---

// Sincroniza la matriz "tablero" con la lista enlazada real de la serpiente "s"
void actualizarMatrizSerpiente(int tablero[FILAS][COLUMNAS], Serpiente *s);

// Pone todas las casillas de la matriz en VACIO (0); hay que llamarla antes de jugar
void inicializarTablero(int tablero[FILAS][COLUMNAS]);

// Devuelve 1 si (fila, col) cae afuera del tablero, 0 si es una posicion valida
int fueraDeLimites(int fila, int col);

// Sortea una posicion libre para la proxima manzana y la guarda en "m"
void generarManzana(Manzana *m, int tablero[FILAS][COLUMNAS]);

// Sortea "cantidad" posiciones libres para obstaculos, las marca en el tablero
// y las guarda en el array "obstaculos" (que viene reservado desde afuera, en main)
void generarObstaculos(int tablero[FILAS][COLUMNAS], Obstaculo obstaculos[], int cantidad);

// Recorre el array de obstaculos y devuelve 1 si (fila, columna) coincide con alguno
int colisionaConObstaculo(Obstaculo obstaculos[], int cantidad, int fila, int columna);

#endif // fin del include guard