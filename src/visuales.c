#include <stdio.h>
#include "visuales.h"
#include <stdlib.h>
#include <windows.h>

void setColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void dibujoTablero(int tablero[FILAS][COLUMNAS], Serpiente *s, int puntaje)
{
    system("cls");
    int i, j;
    for (j = 0; j < COLUMNAS + 2; j++)
        printf("#");
    printf("\n");

    for (i = 0; i < FILAS; i++)
    {
        printf("#");
        for (j = 0; j < COLUMNAS; j++)
        {
            if (i == s->cabeza->fila && j == s->cabeza->columna)
            {
                setColor(1);
                printf("@");
                setColor(7);
            }
            else
            {
                switch (tablero[i][j])
                {
                case CUERPO:
                    setColor(10); // verde para el cuerpo
                    printf("O");
                    setColor(7);
                    break;
                case MANZANA:
                    setColor(12); // rojo para la manzana
                    printf("*");
                    setColor(7);
                    break;
                case OBSTACULO:
                    setColor(14);
                    printf("x");
                    setColor(7);
                    break;
                default:
                    printf(" ");
                    break;
                }
            }
        }
        printf("#\n");
    } // <-- cierra el for de i (esto es lo que faltaba)

    for (j = 0; j < COLUMNAS + 2; j++)
        printf("#");
    printf("\n");

    printf("Puntaje: %d\n", puntaje);
} // <-- cierra la función dibujoTablero

void ocultarCursor(void)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &info);
}