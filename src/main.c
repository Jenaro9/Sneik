// Control Principal(src / main.c)
//     El núcleo del programa será un bucle iterativo(while o do... while) que controle el flujo del juego
//         .Bucle de Juego : Capturar tecla(investigar _kbhit() y _getch() para Windows)
//         .Actualizar lógica de posición.Verificar colisiones.Redibujar tablero.Controlar la velocidad mediante Sleep()
//         .Persistencia : Al finalizar,
//     se debe pedir el nombre del jugador, buscarlo en la lista dinámica de jugadores(o agregarlo) y guardar el ranking actualizado en un archivo

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>    // Necesario para _kbhit() y _getch()
#include <windows.h>  // Necesario para Sleep()
#include "snake.h"    // Tu archivo de cabecera
#include "tablero.h"  // Suponiendo que acá manejás el dibujo y las manzanas

int main() {
    // 1. CONFIGURACIÓN INICIAL
    int gameOver = 0;
    Direccion dirActual = DERECHA; 
    Serpiente s;
    inicializarSerpiente(&s, 10, 10); // Empieza en el medio
    
    // 2. BUCLE PRINCIPAL DEL JUEGO
    while (!gameOver) {
        
        // A) CAPTURA DE TECLAS
        if (_kbhit()) {
            switch (_getch()) {
                case 'w': if (dirActual != ABAJO) dirActual = ARRIBA; break;
                case 's': if (dirActual != ARRIBA) dirActual = ABAJO; break;
                case 'a': if (dirActual != DERECHA) dirActual = IZQUIERDA; break;
                case 'd': if (dirActual != IZQUIERDA) dirActual = DERECHA; break;
                case 'x': gameOver = 1; break; // Salir con X
            }
        }

        // B) LÓGICA DE MOVIMIENTO
        // Acá calculás la nueva posición según dirActual
        int nuevaFila = s.cabeza->fila;
        int nuevaCol = s.cabeza->columna;

        switch(dirActual) {
            case ARRIBA:    nuevaFila--; break;
            case ABAJO:     nuevaFila++; break;
            case IZQUIERDA: nuevaCol--; break;
            case DERECHA:   nuevaCol++; break;
        }

        // C) VERIFICACIÓN DE COLISIONES
        if (colisionaConCuerpo(&s, nuevaFila, nuevaCol)) {
            gameOver = 1;
        } else {
            // Mover (el 0 es "no comió manzana", después deberías chequear eso con tu tablero)
            moverSerpiente(&s, nuevaFila, nuevaCol, 0);
        }

        // D) REDIBUJAR
        // dibujoTablero(tablero, &s); // Tu función de dibujo

        // E) VELOCIDAD
        Sleep(100); // 100ms de pausa
    }

    // 3. CIERRE Y PERSISTENCIA
    printf("Juego terminado. Tu puntaje fue: %d\n", s.longitud);
    liberarSerpiente(&s); // ¡IMPORTANTE: Limpiamos la lista al final!
    
    // Aquí iría tu lógica de guardar el ranking en archivo
    
    return 0;
}