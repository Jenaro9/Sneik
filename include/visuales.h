#ifndef VISUALES_H
#define VISUALES_H

#include "tablero.h" // FILAS, COLUMNAS
#include "snake.h"   // Serpiente

// Dibuja el tablero completo (bordes, cuerpo, cabeza, manzana, obstaculos,
// y ahora tambien la marca de colision) mas el puntaje debajo.
void dibujoTablero(int tablero[FILAS][COLUMNAS], Serpiente *s, int puntaje);

// Oculta el cursor parpadeante de la consola, para que no moleste sobre el dibujo
void ocultarCursor();

#endif