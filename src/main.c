#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "tablero.h"
#include "visuales.h"
#include "snake.h"
#include "teclado.h"
#include "jugadores.h"

int main()
{
    int tablero[FILAS][COLUMNAS];
    Manzana m;
    int puntaje = 0;
    int velocidad = 50;

    inicializarTablero(tablero);
    srand(time(NULL));
    generarManzana(&m, tablero);

    int gameOver = 0;
    Direccion dirActual = DERECHA;
    Serpiente s;
    inicializarSerpiente(&s, 10, 10);
    tablero[10][10] = CUERPO; // marcar la posición inicial también

    ocultarCursor();

    printf("Usa W A S D para moverte. Evita usar el mouse dentro de la consola.\n");
    printf("Presiona cualquier tecla para comenzar...\n");
    _getch();
    while (!gameOver)
    {
        // A) CAPTURA DE TECLAS
        leerTecla(&dirActual, &gameOver);

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

    while (_kbhit())
    {
        _getch();
    }

    // Gestión de jugadores y ranking
    Jugador *listaJugadores = cargarRanking();

    char nombreJugador[MAX_NOMBRE];
    printf("Ingresa tu nombre: ");
    scanf("%s", nombreJugador);
    normalizarNombre(nombreJugador);

    listaJugadores = registrarPuntaje(listaJugadores, nombreJugador, puntaje);
    guardarRanking(listaJugadores);
    mostrarRanking(listaJugadores);
    liberarJugadores(listaJugadores);

    return 0;
}