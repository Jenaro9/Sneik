#ifndef TECLADO_H
#define TECLADO_H

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

void limpiarBufferTeclado(); // limpia el buffer de teclado para que no queden teclas pendientes

#endif