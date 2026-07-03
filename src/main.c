#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "tablero.h"
#include "visuales.h"
#include "snake.h"
#include <conio.h>

int main()
{
    int tablero[FILAS][COLUMNAS];
    Manzana m;
    int puntaje = 0;
    int velocidad = 150;

    inicializarTablero(tablero);
    srand(time(NULL));
    generarManzana(&m, tablero);

    int gameOver = 0;
    Direccion dirActual = DERECHA;
    Serpiente s;
    inicializarSerpiente(&s, 10, 10);
    tablero[10][10] = CUERPO; // marcar la posición inicial también

    while (!gameOver)
    {
        // A) CAPTURA DE TECLAS
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'w':
                if (dirActual != ABAJO)
                    dirActual = ARRIBA;
                break;
            case 's':
                if (dirActual != ARRIBA)
                    dirActual = ABAJO;
                break;
            case 'a':
                if (dirActual != DERECHA)
                    dirActual = IZQUIERDA;
                break;
            case 'd':
                if (dirActual != IZQUIERDA)
                    dirActual = DERECHA;
                break;
            case 'x':
                gameOver = 1;
                break;
            }
        }

        // B) LÓGICA DE MOVIMIENTO
        int nuevaFila = s.cabeza->fila;
        int nuevaCol = s.cabeza->columna;

        switch (dirActual)
        {
        case ARRIBA:
            nuevaFila--;
            break;
        case ABAJO:
            nuevaFila++;
            break;
        case IZQUIERDA:
            nuevaCol--;
            break;
        case DERECHA:
            nuevaCol++;
            break;
        }

        // C) VERIFICACIÓN DE COLISIONES Y MOVIMIENTO
        if (fueraDeLimites(nuevaFila, nuevaCol))
        {
            gameOver = 1;
        }
        else if (colisionaConCuerpo(tablero, nuevaFila, nuevaCol))
        {
            gameOver = 1;
        }
        else
        {
            int crecio = (nuevaFila == m.fila && nuevaCol == m.columna);

            moverSerpiente(&s, nuevaFila, nuevaCol, crecio);
            actualizarMatrizSerpiente(tablero, &s);

            if (crecio)
            {
                if (velocidad > 50) // un piso para que no quede imposible de jugar
                {
                    velocidad -= 5;
                }
                puntaje += 10;
                generarManzana(&m, tablero);
            }

            // marcar la manzana actual en la matriz para que se vea/valide
            tablero[m.fila][m.columna] = MANZANA;
        }

        // D) REDIBUJAR
        dibujoTablero(tablero, &s, puntaje);

        // E) VELOCIDAD
        Sleep(velocidad);
    }

    printf("Juego terminado. Tu puntaje fue: %d\n", puntaje);
    liberarSerpiente(&s);

    // TODO Juli: acá va la lógica de pedir nombre, buscar/agregar
    // en la lista de jugadores, y guardar el ranking en archivo

    return 0;
}