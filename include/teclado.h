#ifndef TECLADO_H
#define TECLADO_H

#include "snake.h" // necesita conocer el tipo Direccion
// Enum de direcciones posibles. Internamente cada nombre es un numero entero
// correlativo empezando en 0 (podés pensarlo como "#define ARRIBA 0", etc.,
// pero con chequeo de tipos: el compilador sabe que una variable "Direccion"
// solo deberia guardar uno de estos 4 valores).

typedef enum
{
    ARRIBA,    // Internamente es 0
    ABAJO,     // Internamente es 1
    IZQUIERDA, // Internamente es 2
    DERECHA    // Internamente es 3
} Direccion;


// Lee TODAS las teclas pendientes en el buffer y actualiza *dirActual segun
// corresponda (o pone *gameOver en 1 si se aprieta 'x'). Recibe punteros
// porque necesita MODIFICAR las variables originales de quien la llama (main),
// no trabajar sobre una copia.
void leerTecla(Direccion *dirActual, int *gameOver);

void limpiarBufferTeclado(); //limpia el buffer de teclado para que no queden teclas pendientes

#endif