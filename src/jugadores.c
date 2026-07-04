#include <stdio.h>  // FILE, fopen, fscanf, fprintf, fclose, printf
#include <stdlib.h> // malloc, free
#include <string.h> // strcpy, strcmp
#include "jugadores.h"

// Lee el archivo de ranking (si existe) y arma una lista enlazada en memoria
// con todos los jugadores guardados. Se llama una vez, al terminar la partida,
// antes de registrar el puntaje nuevo.
Jugador *cargarRanking(void)
{
    Jugador *lista = NULL;  // arranca vacia (NULL = "todavia no hay ningun nodo")
    Jugador *ultimo = NULL; // puntero auxiliar al ULTIMO nodo agregado, para enganchar rapido

    // "r" = abrir en modo lectura. Si el archivo no existe, fopen devuelve NULL
    // en vez de crear el archivo (eso solo pasa con el modo "w").
    FILE *archivo = fopen(ARCHIVO_RANKING, "r");
    if (archivo == NULL)
    {
        // No existe el archivo todavía (primera vez que se juega) -> lista vacía, no es error
        return NULL;
    }

    char nombreLeido[MAX_NOMBRE];
    int puntajeLeido;

    // Cada línea del archivo tiene el formato: nombre puntaje
    // fscanf devuelve la CANTIDAD de valores que logro leer con exito; si logra
    // leer los 2 (nombre Y puntaje), sigue el while. "%9s" limita a 9 caracteres
    // leidos (MAX_NOMBRE - 1, dejando lugar para el '\0' final del string).
    while (fscanf(archivo, "%9s %d", nombreLeido, &puntajeLeido) == 2)
    {
        // Por cada linea leida, creamos un nuevo nodo Jugador con malloc
        Jugador *nuevo = (Jugador *)malloc(sizeof(Jugador));
        if (nuevo == NULL)
        {
            // malloc puede fallar si no queda memoria disponible; hay que revisarlo
            printf("Error: no se pudo reservar memoria.\n");
            break; // cortamos la carga acá, con lo que se pudo leer hasta ahora
        }
        strcpy(nuevo->nombre, nombreLeido);   // copia el contenido del string leido al nodo
        nuevo->puntajeMaximo = puntajeLeido;
        nuevo->siguiente = NULL;              // por ahora es el ultimo (se puede reasignar despues)

        if (lista == NULL) // es el primer nodo que se agrega?
        {
            lista = nuevo;  // entonces la lista arranca en el
            ultimo = nuevo;
        }
        else
        {
            // enganchamos el nuevo nodo al final de la lista (donde apunta "ultimo"),
            // y actualizamos "ultimo" para que apunte a este nodo nuevo
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
        }
    }

    fclose(archivo); // siempre hay que cerrar lo que se abrio con fopen
    return lista;    // devuelve la lista completa armada en memoria
}

// Busca si "nombre" ya existe en la lista. Si existe, actualiza su puntaje
// SOLO si el nuevo es mayor al guardado. Si no existe, crea un jugador nuevo
// y lo agrega al final. Devuelve la lista (puede ser la misma, o con un nodo mas).
Jugador *registrarPuntaje(Jugador *lista, char *nombre, int puntajeNuevo)
{
    Jugador *temp = lista; // puntero auxiliar para recorrer sin perder "lista"

    // Buscamos si el jugador ya existe
    while (temp != NULL)
    {
        if (strcmp(temp->nombre, nombre) == 0) // strcmp devuelve 0 cuando los strings son IGUALES
        {
            // Ya existe: actualizamos solo si el nuevo puntaje es mayor
            if (puntajeNuevo > temp->puntajeMaximo)
            {
                temp->puntajeMaximo = puntajeNuevo;
            }
            return lista; // no hace falta tocar la lista, ya está actualizado (o no hacia falta)
        }
        temp = temp->siguiente; // sigue buscando en el resto de la lista
    }

    // Si llegamos aca, recorrimos toda la lista y no existia: lo agregamos al final
    Jugador *nuevo = (Jugador *)malloc(sizeof(Jugador));
    strcpy(nuevo->nombre, nombre);
    nuevo->puntajeMaximo = puntajeNuevo;
    nuevo->siguiente = NULL;

    if (lista == NULL)
    {
        return nuevo; // la lista estaba vacía, este pasa a ser el primero (y unico) nodo
    }

    // Buscamos el último nodo para engancharlo ahí (esta funcion no recibe un
    // puntero "ultimo" como cargarRanking, asi que hay que recorrer de nuevo)
    temp = lista;
    while (temp->siguiente != NULL)
    {
        temp = temp->siguiente;
    }
    temp->siguiente = nuevo; // enganchamos el nuevo jugador al final

    return lista; // la lista sigue empezando en el mismo lugar, solo se agrego un nodo al final
}

// Escribe TODA la lista al archivo, un jugador por linea, en el mismo formato
// que lee cargarRanking ("nombre puntaje"). Sobreescribe el archivo entero.
void guardarRanking(Jugador *lista)
{
    FILE *archivo = fopen(ARCHIVO_RANKING, "w"); // "w" sobreescribe todo el archivo
    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir el archivo de ranking para guardar.\n");
        return; // no hay archivo para escribir, no tiene sentido seguir
    }

    Jugador *temp = lista; // recorremos la lista de punta a punta
    while (temp != NULL)
    {
        fprintf(archivo, "%s %d\n", temp->nombre, temp->puntajeMaximo); // una linea por jugador
        temp = temp->siguiente;
    }

    fclose(archivo); // cierra y "vuelca" los cambios al disco de forma definitiva
}

// Imprime por pantalla todos los jugadores de la lista, en el orden en que
// estan enlazados (no estan ordenados por puntaje, solo por orden de carga/alta).
void mostrarRanking(Jugador *lista)
{
    printf("\n--- RANKING ---\n");
    if (lista == NULL)
    {
        printf("Todavia no hay puntajes registrados.\n");
        return;
    }

    Jugador *temp = lista;
    while (temp != NULL)
    {
        printf("%s: %d puntos\n", temp->nombre, temp->puntajeMaximo);
        temp = temp->siguiente;
    }
}

// Libera toda la memoria de la lista de jugadores (mismo patron que
// liberarSerpiente en snake.c: guardar "siguiente" antes de liberar "actual").
void liberarJugadores(Jugador *lista)
{
    Jugador *actual = lista;
    Jugador *siguiente;

    while (actual != NULL)
    {
        siguiente = actual->siguiente; // guardamos la referencia antes de destruir "actual"
        free(actual);
        actual = siguiente;
    }
}

// Convierte el nombre a MAYUSCULAS, modificando el mismo array recibido
// (no crea un string nuevo). Se usa para que el ranking no distinga
// mayusculas de minusculas al comparar nombres (ej: "ana" y "ANA" cuentan
// como el mismo jugador).
void normalizarNombre(char *nombre)
{
    int i;
    // Recorremos caracter por caracter hasta el terminador nulo '\0'
    // (asi es como C sabe donde termina un string: no guarda la longitud,
    // busca el primer byte en 0)
    for (i = 0; nombre[i] != '\0'; i++)
    {
        if (nombre[i] >= 'a' && nombre[i] <= 'z') // es una letra minuscula?
        {
            // En la tabla ASCII, cada letra minuscula esta exactamente 32
            // posiciones despues de su version mayuscula ('a'=97, 'A'=65).
            // Restando 32 se obtiene la mayuscula correspondiente.
            nombre[i] = nombre[i] - 32;
        }
    }
}