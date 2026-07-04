#include <conio.h>
#include "teclado.h"

void leerTecla(Direccion *dirActual, int *gameOver)
{
    while (_kbhit())
    {
        switch (_getch())
        {
        case 'w':
            if (*dirActual != ABAJO)
                *dirActual = ARRIBA;
            break;
        case 's':
            if (*dirActual != ARRIBA)
                *dirActual = ABAJO;
            break;
        case 'a':
            if (*dirActual != DERECHA)
                *dirActual = IZQUIERDA;
            break;
        case 'd':
            if (*dirActual != IZQUIERDA)
                *dirActual = DERECHA;
            break;
        case 'x':
            *gameOver = 1;
            break;
        }
    }
}