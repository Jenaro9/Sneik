#include <stdio.h>  // (no se usa realmente en este archivo, pero no genera error dejarlo)
#include <stdlib.h> // malloc, free
#include "snake.h"  // Nodo, Serpiente, Direccion y los prototipos de este archivo

// --- INICIALIZACIÓN ---
void inicializarSerpiente(Serpiente *s, int filaInicial, int colInicial)
{
    // Reservamos memoria DINAMICA (en el heap) para el primer eslabón (la cabeza).
    // malloc devuelve un puntero "void*" generico; el (Nodo *) de adelante es un
    // "cast" que le dice al compilador que lo trate como puntero a Nodo.
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));

    // Cargamos los datos iniciales del nodo recien creado
    nuevo->fila = filaInicial;
    nuevo->columna = colInicial;
    // Como es el único, no apunta a nadie todavía
    nuevo->siguiente = NULL;

    // Conectamos la estructura contenedora (la que vive en main) con nuestra nueva cabeza.
    // "s" es un puntero a la Serpiente real de quien llamo a esta funcion, por eso usamos "->"
    s->cabeza = nuevo;
    s->longitud = 1; // La serpiente empieza midiendo 1 unidad
}

// --- MOVIMIENTO (CORAZÓN DEL JUEGO) ---
void moverSerpiente(Serpiente *s, int nuevaFila, int nuevaCol, int crecio)
{
    // 1. CREACIÓN DE NUEVA CABEZA
    // Siempre que la serpiente avanza, creamos un nuevo nodo en el lugar de destino,
    // sin importar si crecio o no (la diferencia esta en si despues borramos la cola)
    Nodo *nuevaCabeza = (Nodo *)malloc(sizeof(Nodo));
    nuevaCabeza->fila = nuevaFila;
    nuevaCabeza->columna = nuevaCol;

    // El nuevo nodo debe apuntar a la vieja cabeza para que la lista siga unida
    // (si no hicieramos esto, perderiamos la referencia a todo el resto del cuerpo)
    nuevaCabeza->siguiente = s->cabeza;

    // Ahora, el "contenedor" de la serpiente apunta a este nuevo nodo como el inicio
    s->cabeza = nuevaCabeza;

    // 2. GESTIÓN DEL CUERPO (CRECIMIENTO vs AVANCE)
    if (!crecio) // si NO crecio (crecio vale 0, !0 es verdadero)
    {
        // Si NO creció, debemos eliminar el último nodo para mantener el tamaño
        // (si agregamos cabeza y no sacamos cola, la serpiente se alargaria sin comer)
        // Usamos un puntero temporal para no perder la referencia de la lista real
        Nodo *temp = s->cabeza;

        // Recorremos hasta encontrar el PENÚLTIMO nodo
        // (es decir, el nodo cuyo "siguiente" es el ULTIMO nodo de todos)
        // Se compara "temp->siguiente->siguiente != NULL": mientras el nodo que sigue
        // despues del siguiente exista, temp todavia no es el penultimo, hay que avanzar
        while (temp->siguiente->siguiente != NULL)
        {
            temp = temp->siguiente;
        }

        // En este punto, 'temp' es el penúltimo.
        // temp->siguiente es el último nodo. Liberamos su memoria con free()
        // (fue reservada con malloc en algun momento anterior, hay que devolverla)
        free(temp->siguiente);

        // Ahora el penúltimo pasa a ser el nuevo final de la lista: su "siguiente"
        // debe apuntar a NULL, si no quedaria "colgando" hacia memoria ya liberada
        temp->siguiente = NULL;
    }
    else
    {
        // Si comió manzana, no borramos el último nodo.
        // Como agregamos una cabeza pero no quitamos cola, la serpiente aumenta su largo,
        // asi que reflejamos ese crecimiento en el contador
        s->longitud++;
    }
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

// --- LIMPIEZA DE MEMORIA (EVITAR FUGAS) ---
void liberarSerpiente(Serpiente *s)
{
    Nodo *actual = s->cabeza;   // puntero auxiliar que va a recorrer toda la lista
    Nodo *siguiente;            // variable auxiliar para no perder la referencia al avanzar

    // Recorremos nodo por nodo eliminando la memoria de a uno
    while (actual != NULL)
    {
        // Primero guardamos la dirección del siguiente ANTES de liberar "actual",
        // porque una vez liberado no es seguro leer actual->siguiente
        siguiente = actual->siguiente;

        // Liberamos la memoria del nodo actual (devuelve esa RAM al sistema operativo)
        free(actual);

        // Avanzamos al nodo que guardamos previamente
        actual = siguiente;
    }
    // Reseteamos el contenedor: como ya no queda ningun nodo vivo, cabeza no debe
    // seguir apuntando a memoria ya liberada, y longitud vuelve a reflejar la realidad
    s->cabeza = NULL;
    s->longitud = 0;
}