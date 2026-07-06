#include <stdio.h>
#include "visuales.h"
#include <stdlib.h>
#include <windows.h>

// Color de la marca de colision: fondo rojo + texto blanco brillante, para que
// se note MUY distinto de todo lo demas (cuerpo verde, manzana roja sobre fondo
// negro, obstaculo amarillo). En SetConsoleTextAttribute, el nibble bajo (los
// primeros 4 bits) define el color de TEXTO y el nibble alto (los siguientes 4
// bits) define el color de FONDO. BACKGROUND_RED (0x40 = 64) + blanco brillante
// de texto (15) = 79.
#define COLOR_COLISION 79

// Cambia el color de texto/fondo que usa la consola para lo que se imprima
// de ahora en mas, hasta el proximo setColor().
void setColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // handle al output actual de la consola
    SetConsoleTextAttribute(hConsole, color);          // aplica el color pedido
}

// Dibuja el tablero completo: bordes, cuerpo, cabeza, manzana, obstaculos y
// la marca de colision si la hay. Se llama una vez por cada frame del juego.
void dibujoTablero(int tablero[FILAS][COLUMNAS], Serpiente *s, int puntaje)
{
    system("cls"); // limpia toda la pantalla antes de redibujar (mas simple, aunque
                   // puede generar algo de parpadeo comparado con reposicionar el cursor)
    int i, j;

    // --- BORDE SUPERIOR ---
    for (j = 0; j < COLUMNAS + 2; j++) // +2 para cubrir tambien las dos esquinas
        printf("#");
    printf("\n");

    // --- CUERPO DEL TABLERO, FILA POR FILA ---
    for (i = 0; i < FILAS; i++)
    {
        printf("#"); // borde izquierdo de esta fila
        for (j = 0; j < COLUMNAS; j++)
        {
            // IMPORTANTE: se chequea COLISION primero, ANTES que la cabeza.
            // Motivo: cuando el choque es contra una pared, la casilla marcada
            // como COLISION termina siendo matemáticamente la MISMA casilla
            // donde está parada la cabeza (para chocar contra el borde, la
            // cabeza ya tenía que estar pegada a él). Si el chequeo de la
            // cabeza fuera primero, la 'X' quedaría siempre tapada por el '@'
            // y nunca se vería en los choques contra pared.
            if (tablero[i][j] == COLISION)
            {
                setColor(COLOR_COLISION);
                printf("X");
                setColor(7);
            }
            // Caso especial: si esta casilla es exactamente donde esta la cabeza
            // AHORA MISMO (segun la lista enlazada real), se dibuja distinto,
            // sin necesidad de que la matriz tenga un valor especial para "cabeza"
            else if (i == s->cabeza->fila && j == s->cabeza->columna)
            {
                setColor(1); // azul, color de la cabeza
                printf("@");
                setColor(7); // vuelve al color por defecto (gris claro sobre negro)
            }
            else
            {
                // Para el resto de las casillas, se consulta el valor guardado
                // en la matriz "tablero" para decidir que dibujar
                switch (tablero[i][j])
                {
                case CUERPO:
                    setColor(10); // verde brillante para el cuerpo
                    printf("O");
                    setColor(7);
                    break;
                case MANZANA:
                    setColor(12); // rojo brillante para la manzana
                    printf("*");
                    setColor(7);
                    break;
                case OBSTACULO:
                    setColor(14); // amarillo para los obstaculos
                    printf("x");
                    setColor(7);
                    break;
                default: // VACIO (o cualquier otro valor no contemplado)
                    printf(" ");
                    break;
                }
            }
        }
        printf("#\n"); // borde derecho de esta fila + salto de linea
    } // <-- cierra el for de i (esto es lo que faltaba)

    // --- BORDE INFERIOR ---
    for (j = 0; j < COLUMNAS + 2; j++)
        printf("#");
    printf("\n");

    printf("Puntaje: %d\n", puntaje); // info debajo del tablero
} // <-- cierra la función dibujoTablero

// Oculta el cursor parpadeante de la consola de Windows (si no, se ve tildando
// sobre el dibujo del tablero en cada frame, bastante molesto visualmente)
void ocultarCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;     // tamano del cursor (irrelevante aca, ya que va a estar oculto)
    info.bVisible = FALSE; // lo oculta
    SetConsoleCursorInfo(hConsole, &info);
}