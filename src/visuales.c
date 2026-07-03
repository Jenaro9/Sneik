#include <stdio.h>
#include "visuales.h"
#include <stdlib.h>

void dibujoTablero(int tablero[FILAS][COLUMNAS], Serpiente *s, int puntaje)
{
    system("cls");
    int i, j;

    for (i = 0; i < FILAS; i++)
    {
        for (j = 0; j < COLUMNAS; j++)
        {
            switch (tablero[i][j])
            {
            case CUERPO:
                printf("O");
                break;
            case MANZANA:
                printf("@");
                break;
                break;
            default:
                printf(" ");
                break;
            }
        }
        printf("\n");
    } // <-- cierra el for de i (esto es lo que faltaba)

    printf("Puntaje: %d\n", puntaje);
} // <-- cierra la función dibujoTablero