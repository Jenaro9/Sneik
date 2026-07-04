#ifndef JUGADORES_H
#define JUGADORES_H

#define ARCHIVO_RANKING "ranking.txt"
#define MAX_NOMBRE 10
typedef struct Jugador
{
    char nombre[MAX_NOMBRE];
    int puntajeMaximo;
    struct Jugador *siguiente;
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

void normalizarNombre(char *nombre);

#endif