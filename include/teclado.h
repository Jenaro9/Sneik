#ifndef TECLADO_H
#define TECLADO_H

#include "snake.h" // necesita conocer el tipo Direccion

// Lee TODAS las teclas pendientes en el buffer y actualiza *dirActual segun
// corresponda (o pone *gameOver en 1 si se aprieta 'x'). Recibe punteros
// porque necesita MODIFICAR las variables originales de quien la llama (main),
// no trabajar sobre una copia.
void leerTecla(Direccion *dirActual, int *gameOver);

#endif