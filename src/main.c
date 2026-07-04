#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include "tablero.h"
#include "visuales.h"
#include "teclado.h"
#include "jugadores.h"

int main()
{
    int tablero[FILAS][COLUMNAS];
    Manzana m;
    int puntaje = 0;
    int velocidad = 50;
    int opcion;
    int cantidad = 0;
    Obstaculo obstaculos[MAX_OBSTACULOS];

    do
    {
        system("cls");
        printf("=== BIVORITA ===\n\n");
        printf("1. Jugar\n");
        printf("2. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
            break;
        case 2:
            printf("Gracias por jugar!\n");
            return 0;

        default:
            break;
        }
    } while (opcion != 1 && opcion != 2);

    printf("Elegi la dificultad:\n");
    printf("1) Facil\n");
    printf("2) Media\n");
    printf("3) Dificil\n");

    do
    {
        printf("Opcion: ");
        scanf("%d", &opcion);
        switch (opcion)
        {
        case 1:
            velocidad = 200;
            break;
        case 2:
            cantidad = 5;
            velocidad = 150;
            break;
        case 3:
            cantidad = 15;
            velocidad = 100;
            break;
        default:
            printf("Seleccione una dificultad valida por favor \n");
            break;
        }
    } while (opcion < 1 || opcion > 3);

    while (_kbhit()) // limpia el '\n' que dejó el scanf
    {
        _getch();
    }

    inicializarTablero(tablero);
    srand(time(NULL));

    int gameOver = 0;
    Direccion dirActual = DERECHA;
    Serpiente s;
    inicializarSerpiente(&s, 10, 10);
    tablero[10][10] = CUERPO; // marcar la posición inicial también

    generarManzana(&m, tablero);
    tablero[m.fila][m.columna] = MANZANA;
    generarObstaculos(tablero, obstaculos, cantidad);

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
        int crecio = (nuevaFila == m.fila && nuevaCol == m.columna);

        if (fueraDeLimites(nuevaFila, nuevaCol))
        {
            gameOver = 1;
        }
        else if (colisionaConCuerpoLista(&s, nuevaFila, nuevaCol, !crecio) || colisionaConObstaculo(obstaculos, cantidad, nuevaFila, nuevaCol))
        {
            gameOver = 1;
        }
        else
        {
            moverSerpiente(&s, nuevaFila, nuevaCol, crecio);
            actualizarMatrizSerpiente(tablero, &s);

            if (crecio)
            {
                if (velocidad > 50) // un piso para que no quede imposible de jugar
                {
                    velocidad -= 5;
                }
                puntaje += 1;
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
    scanf("%9s", nombreJugador);
    normalizarNombre(nombreJugador);

    listaJugadores = registrarPuntaje(listaJugadores, nombreJugador, puntaje);
    guardarRanking(listaJugadores);
    mostrarRanking(listaJugadores);
    liberarJugadores(listaJugadores);

    return 0;
}