#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jugadores.h"

Jugador *cargarRanking(void)
{
    Jugador *lista = NULL;
    Jugador *ultimo = NULL;

    FILE *archivo = fopen(ARCHIVO_RANKING, "r");
    if (archivo == NULL)
    {
        // No existe el archivo todavía (primera vez que se juega) -> lista vacía, no es error
        return NULL;
    }

    char nombreLeido[MAX_NOMBRE];
    int puntajeLeido;

    // Cada línea del archivo tiene el formato: nombre puntaje
    while (fscanf(archivo, "%9s %d", nombreLeido, &puntajeLeido) == 2)
    {
        Jugador *nuevo = (Jugador *)malloc(sizeof(Jugador));
        if (nuevo == NULL)
        {
            printf("Error: no se pudo reservar memoria.\n");
            break;
        }
        strcpy(nuevo->nombre, nombreLeido);
        nuevo->puntajeMaximo = puntajeLeido;
        nuevo->siguiente = NULL;

        if (lista == NULL)
        {
            lista = nuevo;
            ultimo = nuevo;
        }
        else
        {
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
        }
    }

    fclose(archivo);
    return lista;
}

Jugador *registrarPuntaje(Jugador *lista, char *nombre, int puntajeNuevo)
{
    Jugador *temp = lista;

    // Buscamos si el jugador ya existe
    while (temp != NULL)
    {
        if (strcmp(temp->nombre, nombre) == 0)
        {
            // Ya existe: actualizamos solo si el nuevo puntaje es mayor
            if (puntajeNuevo > temp->puntajeMaximo)
            {
                temp->puntajeMaximo = puntajeNuevo;
            }
            return lista; // no hace falta tocar la lista, ya está actualizado
        }
        temp = temp->siguiente;
    }

    // No existía: lo agregamos al final
    Jugador *nuevo = (Jugador *)malloc(sizeof(Jugador));
    strcpy(nuevo->nombre, nombre);
    nuevo->puntajeMaximo = puntajeNuevo;
    nuevo->siguiente = NULL;

    if (lista == NULL)
    {
        return nuevo; // la lista estaba vacía, este pasa a ser el primero
    }

    // Buscamos el último nodo para engancharlo ahí
    temp = lista;
    while (temp->siguiente != NULL)
    {
        temp = temp->siguiente;
    }
    temp->siguiente = nuevo;

    return lista;
}

void guardarRanking(Jugador *lista)
{
    FILE *archivo = fopen(ARCHIVO_RANKING, "w"); // "w" sobreescribe todo el archivo
    if (archivo == NULL)
    {
        printf("Error: no se pudo abrir el archivo de ranking para guardar.\n");
        return;
    }

    Jugador *temp = lista;
    while (temp != NULL)
    {
        fprintf(archivo, "%s %d\n", temp->nombre, temp->puntajeMaximo);
        temp = temp->siguiente;
    }

    fclose(archivo);
}

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

void liberarJugadores(Jugador *lista)
{
    Jugador *actual = lista;
    Jugador *siguiente;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

void normalizarNombre(char *nombre)
{
    int i;
    for (i = 0; nombre[i] != '\0'; i++)
    {
        if (nombre[i] >= 'a' && nombre[i] <= 'z')
        {
            nombre[i] = nombre[i] - 32;
        }
    }
}