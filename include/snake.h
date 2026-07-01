// . Definición de Estructuras y Constantes (include/snake.h)
// En este archivo deben definir los registros (structs) necesarios para representar la serpiente y los jugadores
// .
// Nodo de la Serpiente: Utilizarán una lista simplemente enlazada donde cada nodo guarda su posición (fila, columna)
// .
// Registro de Jugador: Para guardar el nombre y el puntaje máximo
// .
// Constantes: Definir dimensiones del tablero y caracteres usando #define
// .
#include <stdbool.h> // Para usar tipos booleanos [11]

#define FILAS 20
#define COLS 40

typedef struct Nodo {
    int fila;
    int col;
    struct Nodo *sig;
} Nodo;

typedef struct {
    char nombre[12];
    int puntajeMax;
} Jugador;
