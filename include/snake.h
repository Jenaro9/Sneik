#ifndef SNAKE_H // Si no se definió este archivo...
#define SNAKE_H // ...definilo ahora (evita errores si se incluye dos veces)

// Nodo de la lista enlazada simple: representa un segmento del cuerpo
typedef struct Nodo
{
    int fila;               // Posición vertical en el tablero
    int columna;            // Posición horizontal en el tablero
    struct Nodo *siguiente; // Puntero al siguiente eslabón del cuerpo (NULL si es el último)
    // Nota: adentro de la propia definicion de "struct Nodo" hace falta escribir
    // "struct Nodo *siguiente" (con la palabra struct) y no "Nodo *siguiente",
    // porque el typedef "Nodo" recien queda disponible DESPUES del ; final de este bloque.
} Nodo;

// Estructura contenedora de la serpiente: facilita el acceso rápido a la cabeza
typedef struct
{
    Nodo *cabeza; // Puntero al primer nodo (donde está la "boca")
    int longitud; // Contador útil para saber el puntaje actual
} Serpiente;

// --- PROTOTIPOS DE FUNCIONES (implementadas en snake.c) ---

// Recorre la LISTA ENLAZADA (no la matriz) de la serpiente buscando si (fila, col) ya
// esta ocupada por algun segmento del cuerpo. "ignorarCola" permite saltear el ultimo
// nodo de la lista (util porque, si la serpiente no va a crecer este turno, la cola
// se va a mover/liberar de todas formas, asi que pisarla no deberia contar como choque)
int colisionaConCuerpoLista(Serpiente *s, int fila, int col, int ignorarCola);

// Prepara la serpiente al iniciar el juego: crea el primer nodo en una posición
void inicializarSerpiente(Serpiente *s, int filaInicial, int colInicial);

// Lógica de movimiento principal: agrega cabeza, y gestiona si borra o no la cola
// 'crecio' es un booleano (1 si comió manzana, 0 si no)
void moverSerpiente(Serpiente *s, int nuevaFila, int nuevaCol, int crecio);


// Al terminar el juego, libera toda la memoria reservada con malloc (evita fugas)
void liberarSerpiente(Serpiente *s);

#endif // Fin del #ifndef