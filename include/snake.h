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
typedef enum {
    ARRIBA,    // Internamente es 0
    ABAJO,     // Internamente es 1
    IZQUIERDA, // Internamente es 2
    DERECHA    // Internamente es 3
} Direccion;
#ifndef SNAKE_H // Si no se definió este archivo...
#define SNAKE_H // ...definilo ahora (evita errores si se incluye dos veces)

// --- DEFINICIÓN DE ESTRUCTURAS ---

// Nodo de la lista enlazada simple: representa un segmento del cuerpo
typedef struct Nodo {
    int fila;                   // Posición vertical en el tablero
    int columna;                // Posición horizontal en el tablero
    struct Nodo *siguiente;     // Puntero al siguiente eslabón del cuerpo
} Nodo;

// Estructura contenedora de la serpiente: facilita el acceso rápido a la cabeza
typedef struct {
    Nodo *cabeza;               // Puntero al primer nodo (donde está la "boca")
    int longitud;               // Contador útil para saber el puntaje actual
} Serpiente;

// --- PROTOTIPOS DE FUNCIONES ---

// Prepara la serpiente al iniciar el juego: crea el primer nodo en una posición
void inicializarSerpiente(Serpiente *s, int filaInicial, int colInicial);

// Lógica de movimiento principal: agrega cabeza, y gestiona si borra o no la cola
// 'crecio' es un booleano (1 si comió manzana, 0 si no)
void moverSerpiente(Serpiente *s, int nuevaFila, int nuevaCol, int crecio);

// Al terminar el juego, libera toda la memoria reservada con malloc (evita fugas)
void liberarSerpiente(Serpiente *s);

// Recorre la lista buscando si las coordenadas (fila, col) ya están ocupadas
// Devuelve 1 si hay colisión, 0 si está libre
int colisionaConCuerpo(Serpiente *s, int fila, int col);

#endif // Fin del #ifndef