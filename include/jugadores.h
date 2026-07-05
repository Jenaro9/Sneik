#ifndef JUGADORES_H
#define JUGADORES_H

#define ARCHIVO_RANKING "ranking.txt" // nombre del archivo donde se guarda el ranking en disco
#define MAX_NOMBRE 10                 // longitud maxima del nombre de un jugador (+1 para el '\0')

// Un jugador es un nodo de OTRA lista enlazada (independiente de la de la
// serpiente): guarda su nombre, su mejor puntaje historico, y el puntero
// al siguiente jugador de la lista.
typedef struct Jugador
{
    char nombre[MAX_NOMBRE];   // array de caracteres (string) de tamano fijo
    int puntajeMaximo;         // el mejor puntaje que hizo ese jugador hasta ahora
    struct Jugador *siguiente; // encadena con el proximo jugador (NULL si es el ultimo)
} Jugador;

// Carga todos los jugadores guardados en el archivo a una lista en memoria
Jugador *cargarRanking(void);

// Busca un jugador por nombre; si existe y el nuevo puntaje es mayor, lo actualiza.
// Si no existe, lo agrega al final de la lista.
Jugador *registrarPuntaje(Jugador *lista, char *nombre, int puntajeNuevo);

// Vuelca toda la lista actual al archivo (sobreescribiendo el contenido viejo)
void guardarRanking(Jugador *lista);

// Imprime el ranking ordenado (o en el orden que esté) por pantalla
void mostrarRanking(Jugador *lista);

// Libera toda la memoria de la lista al finalizar
void liberarJugadores(Jugador *lista);

// Pasa el nombre a mayusculas "en el lugar" (modifica el mismo array recibido)
void normalizarNombre(char *nombre);

#endif