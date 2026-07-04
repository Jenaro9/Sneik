#include <conio.h>
#include "teclado.h"

void leerTecla(Direccion *dirActual, int *gameOver)
{
    Direccion direccionOriginal = *dirActual; // fijamos la direccion real de este frame

    while (_kbhit())
    {
        switch (_getch())
        {
        case 'w':
            if (direccionOriginal != ABAJO)
                *dirActual = ARRIBA;
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
            *gameOver = 1;
            break;
        }
    }
}