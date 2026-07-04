#include "tablero.h"
#include <stdio.h>
#include <stdlib.h> // rand()

// Recorre las FILAS x COLUMNAS casillas y pone cada una en 0 (VACIO).
// Hace falta llamarla al principio porque un array local en C NO arranca
// en cero solo: contiene basura de memoria hasta que se inicializa a mano.
void inicializarTablero(int tablero[FILAS][COLUMNAS]) // recorre la matriz iniciando todo en 0
{
    int i, j;                  // i = fila actual, j = columna actual (convencion universal)
    for (i = 0; i < FILAS; i++)     // recorre cada fila...
    {
        for (j = 0; j < COLUMNAS; j++) // ...y dentro de cada fila, cada columna
        {
            tablero[i][j] = 0; // 0 es exactamente el valor de VACIO
        }
    }
}

// Devuelve 1 (verdadero) si la posicion (nuevaFila, nuevaColumna) esta afuera
// del rango valido del tablero; 0 (falso) si es una posicion dentro de rango.
int fueraDeLimites(int nuevaFila, int nuevaColumna)
{ // verdadero cuando esta fuera de tablero
    // Las 4 condiciones deben cumplirse TODAS (&&) para que la posicion sea valida:
    // no antes de la fila/columna 0, y no en o despues de FILAS/COLUMNAS
    // (el ultimo indice valido es FILAS-1 y COLUMNAS-1, por eso se usa "<" y no "<=")
    if (nuevaFila >= 0 && nuevaColumna >= 0 && nuevaFila < FILAS && nuevaColumna < COLUMNAS)
    {
        return 0; // esta DENTRO del tablero -> no esta fuera de limites
    }
    else
    {
        return 1; // esta FUERA del tablero
    }
}

// Sincroniza la matriz "tablero" con el estado actual de la lista enlazada "s".
// Hace falta llamarla despues de cada moverSerpiente(), porque son dos representaciones
// distintas del mismo estado (la lista es la "fuente de verdad", la matriz es una
// "foto" para consultas rapidas tipo colision).
void actualizarMatrizSerpiente(int tablero[FILAS][COLUMNAS], Serpiente *s)
{
    // 1. Primero "limpiamos" toda la matriz de cuerpo viejo,
    //    así no queda basura de la vuelta anterior (ej: la cola que se movió)
    int i, j;
    for (i = 0; i < FILAS; i++)
    {
        for (j = 0; j < COLUMNAS; j++)
        {
            if (tablero[i][j] == CUERPO) // solo borramos marcas de CUERPO
            {
                tablero[i][j] = VACIO; // (no toca MANZANA ni OBSTACULO, quedan igual)
            }
        }
    }

    // 2. Recorremos la lista actual y marcamos CUERPO en cada nodo vigente
    // (nota: en esta version, la cabeza tambien queda marcada como CUERPO en la
    // matriz; visuales.c es quien decide dibujarla distinto, comparando directo
    // contra s->cabeza->fila/columna en vez de leer un valor especial del tablero)
    Nodo *temp = s->cabeza;    // puntero auxiliar para recorrer sin perder la cabeza real
    while (temp != NULL)       // hasta llegar al final de la lista (siguiente == NULL)
    {
        tablero[temp->fila][temp->columna] = CUERPO;
        temp = temp->siguiente; // avanza al proximo nodo
    }
}

// Sortea una posicion libre al azar para la proxima manzana, y la guarda en "m".
void generarManzana(Manzana *m, int tablero[FILAS][COLUMNAS])
{

    int filaRandom;
    int columnaRandom;

    // do-while: primero sorteamos una posicion candidata, y RECIEN DESPUES
    // preguntamos si sirve. Hace falta un do-while (no un while comun) porque
    // no hay nada que evaluar antes de tener el primer candidato sorteado.
    do
    {
        filaRandom = rand() % FILAS;       // resto de la division: da un numero entre 0 y FILAS-1
        columnaRandom = rand() % COLUMNAS; // idem para columnas
    } while (tablero[filaRandom][columnaRandom] != 0); // repetir mientras la casilla NO este vacia

    m->fila = filaRandom;       // guardamos el resultado en la Manzana real (se paso por puntero)
    m->columna = columnaRandom;
}

// Busca si (fila, col) coincide con algun segmento de la SERPIENTE, recorriendo
// directamente la lista enlazada (no la matriz). "ignorarCola" permite saltear
// el ultimo nodo: si la serpiente no va a crecer este turno, ese ultimo nodo
// se va a mover (o liberar) de todas formas, asi que pisarlo no es un choque real.
int colisionaConCuerpoLista(Serpiente *s, int fila, int col, int ignorarCola)
{
    Nodo *actual = s->cabeza; // arrancamos desde la cabeza
    while (actual != NULL)    // mientras queden nodos por revisar
    {
        // si hay que ignorar la cola y este es el ultimo nodo, no lo comparamos
        if (ignorarCola && actual->siguiente == NULL)
        {
            break; // cortamos el while: no seguimos comparando (ya no queda nada mas igual)
        }
        if (actual->fila == fila && actual->columna == col) // coincide con este segmento?
        {
            return 1; // si, hay colision
        }
        actual = actual->siguiente; // pasamos al proximo segmento
    }
    return 0; // recorrimos todo (lo que correspondia) sin encontrar coincidencia
}

// Sortea "cantidad" posiciones libres y las marca como OBSTACULO, tanto en la
// matriz (para que se dibujen) como en el array "obstaculos" (para poder
// consultarlas rapido despues, sin tener que recorrer toda la matriz).
void generarObstaculos(int tablero[FILAS][COLUMNAS], Obstaculo obstaculos[], int cantidad)
{
    int i;
    int filaRandom, columnaRandom;

    for (i = 0; i < cantidad; i++) // uno por cada obstaculo pedido
    {
        do // mismo patron que generarManzana: sortear y repetir si esta ocupado
        {
            filaRandom = rand() % FILAS;
            columnaRandom = rand() % COLUMNAS;
        } while (tablero[filaRandom][columnaRandom] != 0);

        tablero[filaRandom][columnaRandom] = OBSTACULO; // para que se dibuje

        // guardamos la posicion tambien en el array de structs, para consultas rapidas
        obstaculos[i].fila = filaRandom;
        obstaculos[i].columna = columnaRandom;
    }
}

// Recorre el array de obstaculos (no la matriz) y devuelve 1 si (fila, columna)
// coincide con la posicion de alguno.
int colisionaConObstaculo(Obstaculo obstaculos[], int cantidad, int fila, int columna)
{
    int i;
    for (i = 0; i < cantidad; i++)
    {
        if (obstaculos[i].fila == fila && obstaculos[i].columna == columna)
        {
            return 1; // encontramos coincidencia
        }
    }
    return 0; // ninguno coincidio
}