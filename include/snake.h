// . Definición de Estructuras y Constantes (include/snake.h)
// En este archivo deben definir los registros (structs) necesarios para representar la serpiente y los jugadores
// .
// Nodo de la Serpiente: Utilizarán una lista simplemente enlazada donde cada nodo guarda su posición (fila, columna)
// .
// Registro de Jugador: Para guardar el nombre y el puntaje máximo
// .
// Constantes: Definir dimensiones del tablero y caracteres usando #define
// .
// snake.h
#ifndef SNAKE_H
#define SNAKE_H

typedef struct Nodo {
    int fila;
    int columna;
    struct Nodo *siguiente;
} Nodo;

typedef struct {
    Nodo *cabeza;
    int longitud;
} Serpiente;

void inicializarSerpiente(Serpiente *s, int filaInicial, int colInicial);
void moverSerpiente(Serpiente *s, int nuevaFila, int nuevaCol, int crecio);
void liberarSerpiente(Serpiente *s);

#endif