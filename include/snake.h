
#ifndef SNAKE_H // Si no se definió este archivo...
#define SNAKE_H // ...definilo ahora (evita errores si se incluye dos veces)

typedef enum
{
    ARRIBA,    // Internamente es 0
    ABAJO,     // Internamente es 1
    IZQUIERDA, // Internamente es 2
    DERECHA    // Internamente es 3
} Direccion;

// Nodo de la lista enlazada simple: representa un segmento del cuerpo
typedef struct Nodo
{
    int fila;               // Posición vertical en el tablero
    int columna;            // Posición horizontal en el tablero
    struct Nodo *siguiente; // Puntero al siguiente eslabón del cuerpo
} Nodo;

// Estructura contenedora de la serpiente: facilita el acceso rápido a la cabeza
typedef struct
{
    Nodo *cabeza; // Puntero al primer nodo (donde está la "boca")
    int longitud; // Contador útil para saber el puntaje actual
} Serpiente;

// --- PROTOTIPOS DE FUNCIONES ---

// Prepara la serpiente al iniciar el juego: crea el primer nodo en una posición
void inicializarSerpiente(Serpiente *s, int filaInicial, int colInicial);

// Lógica de movimiento principal: agrega cabeza, y gestiona si borra o no la cola
// 'crecio' es un booleano (1 si comió manzana, 0 si no)
void moverSerpiente(Serpiente *s, int nuevaFila, int nuevaCol, int crecio);

// Al terminar el juego, libera toda la memoria reservada con malloc (evita fugas)
void liberarSerpiente(Serpiente *s);

#endif // Fin del #ifndef