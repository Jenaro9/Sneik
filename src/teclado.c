#include <conio.h> // _kbhit, _getch
#include "teclado.h"

void leerTecla(Direccion *dirActual, int *gameOver)
{
    // Guardamos la direccion con la que arranco este frame ANTES de leer nada.
    // Por que hace falta esto y no comparar directo contra "*dirActual"? Porque
    // dentro del while de mas abajo se pueden llegar a procesar VARIAS teclas en
    // un mismo frame (si el jugador aprieta rapido). Si comparasemos siempre contra
    // "*dirActual" ya actualizado, podria pasar esto: el jugador va para ABAJO,
    // aprieta 'a' (pasa a IZQUIERDA) y en el mismo frame aprieta 's'; como
    // *dirActual ya es IZQUIERDA en ese instante, "s" pasaria el chequeo
    // (IZQUIERDA != ARRIBA... pero el chequeo real es contra ABAJO/ARRIBA segun
    // el caso) y se podria terminar generando una reversa invalida en dos pasos.
    // Usando "direccionOriginal" (fija durante todo este llamado), todas las
    // comparaciones de este frame se hacen contra la MISMA direccion de referencia.
    Direccion direccionOriginal = *dirActual; // fijamos la direccion real de este frame

    // A diferencia de un simple "if (_kbhit())", este "while" vacia TODO el
    // buffer de teclado pendiente en un solo llamado. Si el jugador aprieto
    // varias teclas mientras el juego estaba "ocupado" (dibujando o durmiendo
    // con Sleep), esto evita que se vayan acumulando y se procesen tarde,
    // desincronizadas del momento real en que se apretaron.
    while (_kbhit())
    {
        switch (_getch()) // lee y consume la proxima tecla pendiente
        {
        case 'w':
            if (direccionOriginal != ABAJO) // no permitir el giro de 180 grados
                *dirActual = ARRIBA;        // modificamos la variable ORIGINAL de main (via puntero)
            break;
        case 's':
            if (direccionOriginal != ARRIBA)
                *dirActual = ABAJO;
            break;
        case 'a':
            if (direccionOriginal != DERECHA)
                *dirActual = IZQUIERDA;
            break;
        case 'd':
            if (direccionOriginal != IZQUIERDA)
                *dirActual = DERECHA;
            break;
        case 'x':
            *gameOver = 1; // avisa a main que el jugador quiere salir
            break;
        }
    }
}
void limpiarBufferTeclado() // limpia el '\n' que dejó el scanf
{
    while (_kbhit()) // Devuelve 0 si no hay nada pendiente
    {
        _getch(); // limpia el buffer
    }
}
