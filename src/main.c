#include <stdio.h>     // printf, scanf
#include <stdlib.h>    // srand
#include <time.h>      // time (semilla para los numeros aleatorios)
#include <conio.h>     // _kbhit, _getch (limpiar el buffer de teclado)
#include <windows.h>   // Sleep
#include "tablero.h"   // FILAS, COLUMNAS, Manzana, Obstaculo, y funciones del tablero
#include "visuales.h"  // dibujoTablero, ocultarCursor
#include "teclado.h"   // leerTecla
#include "jugadores.h" // Jugador, cargarRanking, registrarPuntaje, etc.
#include "snake.h"     // Colisiones, nodos Serpiente, movimiento serpiente, etc

// Definimos una cantidad constante de obstaculos por dificultad
#define OBSTACULOS_FACIL 0
#define OBSTACULOS_MEDIA 5
#define OBSTACULOS_DIFICIL 15

// Definimos una cantidad constante de velocidad por dificultad
#define VELOCIDAD_FACIL 175
#define VELOCIDAD_MEDIA 150
#define VELOCIDAD_DIFICIL 100

int main()
{
    int tablero[FILAS][COLUMNAS];         // la matriz del mundo del juego (20x40), sin inicializar todavia
    Manzana m;                            // posicion actual de la manzana
    int puntaje = 0;                      // puntos acumulados
    int velocidad;                        // ms de pausa entre frame y frame
    int opcion;                           // variable generica para leer las opciones de los menus
    int cantidad = 0;                     // cantidad de obstaculos de la partida (depende de la dificultad)
    Obstaculo obstaculos[MAX_OBSTACULOS]; // array (reservado en la pila) para guardar hasta 20 obstaculos

    // --- MENU PRINCIPAL: Jugar / Salir ---
    // do-while porque hace falta mostrar el menu al menos una vez antes de
    // poder preguntar "opcion != 1 && opcion != 2" (todavia no existe "opcion")
    do
    {
        system("cls"); // limpia la pantalla antes de mostrar el menu
        printf("=== BIVORITA ===\n\n");
        printf("1. Jugar\n");
        printf("2. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion); // lee un numero entero desde teclado (BLOQUEA hasta que el usuario responda)

        switch (opcion)
        {
        case 1:
            break; // sale del switch y sigue de largo (la condicion del do-while corta el bucle)
        case 2:
            printf("Gracias por jugar!\n");
            return 0; // termina el programa directamente, ni siquiera arranca el juego

        default:
            break; // opcion invalida: no hace nada, y el do-while vuelve a mostrar el menu
        }
    } while (opcion != 1 && opcion != 2); // repite mientras la opcion no sea ninguna de las dos validas

    // --- MENU DE DIFICULTAD ---
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
        // En cada case guardamos en las variables cantidad y velocidad, las constantes declaradas
        case 1:
            velocidad = VELOCIDAD_FACIL;
            cantidad = OBSTACULOS_FACIL;
            break;
        case 2:
            velocidad = VELOCIDAD_MEDIA;
            cantidad = OBSTACULOS_MEDIA;
            break;
        case 3:
            velocidad = VELOCIDAD_DIFICIL;
            cantidad = OBSTACULOS_DIFICIL;
            break;
        default:
            printf("Seleccione una dificultad valida por favor \n");
            break;
        }
    } while (opcion < 1 || opcion > 3); // repite hasta elegir 1, 2 o 3

    // scanf deja un salto de linea ('\n') pendiente en el buffer de teclado
    // despues de leer un numero. Si no lo "limpiamos", ese '\n' podria ser
    // leido por error mas adelante por _getch() dentro del juego. Este while
    // vacia todo lo que haya quedado pendiente en el buffer.
    limpiarBufferTeclado(); // limpia el '\n' que dejó el scanf

    // --- PREPARACION DE LA PARTIDA ---
    inicializarTablero(tablero); // pone las 800 casillas en VACIO
    srand(time(NULL));           // semilla distinta en cada ejecucion real

    int gameOver = 0;                 // bandera de fin de juego
    Direccion dirActual = DERECHA;    // direccion inicial
    Serpiente s;                      // la serpiente (lista enlazada)
    inicializarSerpiente(&s, 10, 10); // crea el primer nodo (cabeza) en (10, 10)
    tablero[10][10] = CUERPO;         // marcar la posición inicial también (sincroniza matriz y lista)

    generarManzana(&m, tablero);                      // sortea la primera manzana en una casilla libre
    tablero[m.fila][m.columna] = MANZANA;             // la marca en la matriz para que se dibuje
    generarObstaculos(tablero, obstaculos, cantidad); // sortea "cantidad" obstaculos (0 si es facil)

    ocultarCursor(); // esconde el cursor parpadeante de la consola

    printf("Usa W A S D para moverte. Evita usar el mouse dentro de la consola.\n");
    printf("Presiona cualquier tecla para comenzar...\n");
    _getch(); // esta si BLOQUEA: espera una tecla cualquiera antes de arrancar el loop

    // ============================= GAME LOOP =============================
    while (!gameOver) // se repite mientras gameOver siga en 0; cada vuelta es un frame
    {
        // A) CAPTURA DE TECLAS: delega todo el trabajo a teclado.c. Puede
        // cambiar "dirActual" y/o poner "gameOver" en 1 (tecla 'x'), por eso
        // se le pasan ambas variables POR PUNTERO (necesita modificar las
        // originales de aca, no una copia)
        leerTecla(&dirActual, &gameOver);

        // B) LÓGICA DE MOVIMIENTO: calculamos la posicion candidata para la
        // proxima cabeza, sin moverla todavia de verdad
        int nuevaFila = s.cabeza->fila;
        int nuevaCol = s.cabeza->columna;

        switch (dirActual)
        {
        case ARRIBA:
            nuevaFila--; // subir = restar a la fila
            break;
        case ABAJO:
            nuevaFila++; // bajar = sumar a la fila
            break;
        case IZQUIERDA:
            nuevaCol--; // izquierda = restar a la columna
            break;
        case DERECHA:
            nuevaCol++; // derecha = sumar a la columna
            break;
        }

        // C) VERIFICACIÓN DE COLISIONES Y MOVIMIENTO

        // Se calcula ANTES de saber si el movimiento es valido, porque hace
        // falta este valor tanto para decidir "ignorarCola" en la colision
        // contra el cuerpo, como (si no hay colision) para el bloque "else"
        int crecio = (nuevaFila == m.fila && nuevaCol == m.columna);

        if (fueraDeLimites(nuevaFila, nuevaCol)) // choque contra un borde del tablero
        {
            // NUEVO: (nuevaFila, nuevaCol) esta FUERA de la matriz real, asi que
            // no se puede escribir tablero[nuevaFila][nuevaCol] directamente
            // (seria un acceso invalido, fuera de los limites del array).
            // "Recortamos" (clamp) la posicion a la casilla valida mas cercana
            // -el borde exacto contra el que chocamos- para dibujar la X ahi.
            int filaMarca = nuevaFila;
            int colMarca = nuevaCol;
            if (filaMarca < 0)
                filaMarca = 0; // se fue arriba del todo -> clavar en fila 0
            if (filaMarca >= FILAS)
                filaMarca = FILAS - 1; // se fue abajo del todo -> ultima fila
            if (colMarca < 0)
                colMarca = 0; // se fue a la izquierda del todo -> columna 0
            if (colMarca >= COLUMNAS)
                colMarca = COLUMNAS - 1; // se fue a la derecha del todo -> ultima columna

            tablero[filaMarca][colMarca] = COLISION; // marca esa casilla para que se dibuje como X
            gameOver = 1;
        }
        else if (colisionaConCuerpoLista(&s, nuevaFila, nuevaCol, !crecio) || colisionaConObstaculo(obstaculos, cantidad, nuevaFila, nuevaCol))
        {
            // Choque contra el propio cuerpo O contra un obstaculo. En ambos
            // casos (nuevaFila, nuevaCol) SI esta dentro del tablero (si no,
            // hubieramos entrado al "if" de arriba), asi que se puede marcar
            // directamente sin necesidad de recortar nada.
            tablero[nuevaFila][nuevaCol] = COLISION; // pisa el CUERPO/OBSTACULO de esa celda con la X
            gameOver = 1;
        }
        else // movimiento valido: no hay ningun tipo de choque
        {
            moverSerpiente(&s, nuevaFila, nuevaCol, crecio); // agrega cabeza, y saca cola si no crecio
            actualizarMatrizSerpiente(tablero, &s);          // sincroniza la matriz con la lista actualizada

            if (crecio) // si comio manzana en este frame
            {
                if (velocidad > 50) // un piso para que no quede imposible de jugar
                {
                    velocidad -= 5; // acelera el juego un poco cada vez que come
                }
                puntaje += 1;                // suma un punto
                generarManzana(&m, tablero); // sortea la proxima manzana
            }

            // marcar la manzana actual en la matriz para que se vea/valide
            // (se hace SIEMPRE, no solo si crecio, porque actualizarMatrizSerpiente
            // no toca ni conoce la posicion de la manzana)
            tablero[m.fila][m.columna] = MANZANA;
        }

        // D) REDIBUJAR: se llama siempre, incluso en el frame donde hubo colision
        // (asi se alcanza a ver la X marcada arriba antes de que termine el loop)
        dibujoTablero(tablero, &s, puntaje);

        // E) VELOCIDAD: pausa la ejecucion antes del proximo frame
        Sleep(velocidad);
    }
    // ========================= FIN DEL GAME LOOP =========================

    printf("Juego terminado. Tu puntaje fue: %d\n", puntaje); // se ve debajo del tablero congelado con la X
    liberarSerpiente(&s);                                     // libera con free() cada nodo reservado con malloc

    // Limpiamos cualquier tecla que haya quedado pendiente (por ejemplo, si el
    // jugador siguio apretando teclas justo cuando termino la partida), para
    // que no interfiera con los scanf/getch que vienen ahora en la parte de ranking
    limpiarBufferTeclado();

    // --- GESTIÓN DE JUGADORES Y RANKING ---
    Jugador *listaJugadores = cargarRanking(); // trae del archivo todos los jugadores guardados

    char nombreJugador[MAX_NOMBRE];
    printf("Ingresa tu nombre: ");
    scanf("%9s", nombreJugador);     // %9s limita a 9 caracteres (deja lugar para el '\0')
    normalizarNombre(nombreJugador); // pasa el nombre a mayusculas, para no duplicar por may/min

    listaJugadores = registrarPuntaje(listaJugadores, nombreJugador, puntaje); // actualiza o agrega
    guardarRanking(listaJugadores);
    ordenarRankingDescendente(listaJugadores); // ordena antes de mostrar                                               // vuelca la lista completa de nuevo al archivo
    mostrarRanking(listaJugadores);            // imprime el ranking por pantalla
    liberarJugadores(listaJugadores);          // libera toda la memoria de esta lista (evita fugas)
    printf("\nPresiona una tecla para salir...\n");
    _getch();
    return 0; // avisa al sistema operativo que el programa termino sin errores
}