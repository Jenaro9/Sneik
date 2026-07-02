#include <stdio.h>
#include <stdlib.h>
#include "../include/snake.h" 

// --- INICIALIZACIÓN ---
void inicializarSerpiente(Serpiente *s, int filaInicial, int colInicial) {
    // Reservamos memoria para el primer eslabón (la cabeza)
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    
    // Cargamos los datos iniciales
    nuevo->fila = filaInicial;
    nuevo->columna = colInicial;
    // Como es el único, no apunta a nadie todavía
    nuevo->siguiente = NULL;

    // Conectamos la estructura contenedora con nuestra nueva cabeza
    s->cabeza = nuevo;
    s->longitud = 1; // La serpiente empieza midiendo 1 unidad
}

// --- MOVIMIENTO (CORAZÓN DEL JUEGO) ---
void moverSerpiente(Serpiente *s, int nuevaFila, int nuevaCol, int crecio) {
    // 1. CREACIÓN DE NUEVA CABEZA
    // Siempre que la serpiente avanza, creamos un nuevo nodo en el lugar de destino
    Nodo *nuevaCabeza = (Nodo*)malloc(sizeof(Nodo));
    nuevaCabeza->fila = nuevaFila;
    nuevaCabeza->columna = nuevaCol;
    
    // El nuevo nodo debe apuntar a la vieja cabeza para que la lista siga unida
    nuevaCabeza->siguiente = s->cabeza; 
    
    // Ahora, el "contenedor" de la serpiente apunta a este nuevo nodo como el inicio
    s->cabeza = nuevaCabeza; 
    
    // 2. GESTIÓN DEL CUERPO (CRECIMIENTO vs AVANCE)
    if (!crecio) {
        // Si NO creció, debemos eliminar el último nodo para mantener el tamaño
        // Usamos un puntero temporal para no perder la referencia de la lista
        Nodo *temp = s->cabeza;
        
        // Recorremos hasta encontrar el PENÚLTIMO nodo
        // (es decir, el que apunta al que queremos borrar)
        while (temp->siguiente->siguiente != NULL) {
            temp = temp->siguiente;
        }
        
        // En este punto, 'temp' es el penúltimo. 
        // temp->siguiente es el último nodo. Liberamos su memoria.
        free(temp->siguiente);
        
        // Ahora el penúltimo pasa a ser el nuevo final de la lista
        temp->siguiente = NULL;
    } else {
        // Si comió manzana, no borramos el último. 
        // Como agregamos una cabeza pero no quitamos cola, la serpiente aumenta su largo.
        s->longitud++; 
    }
}

// --- COLISIONES ---
int colisionaConCuerpo(Serpiente *s, int fila, int col) {
    // Empezamos a recorrer desde el segundo nodo, 
    // porque es imposible chocar con la cabeza (la boca) al moverse
    Nodo *temp = s->cabeza->siguiente; 

    // Recorremos toda la lista hasta el final
    while (temp != NULL) {
        // Si las coordenadas coinciden con cualquier eslabón, chocamos
        if (temp->fila == fila && temp->columna == col) {
            return 1; // True: Hay colisión
        }
        temp = temp->siguiente; // Saltamos al próximo
    }
    return 0; // False: Camino libre
}

// --- LIMPIEZA DE MEMORIA (EVITAR FUGAS) ---
void liberarSerpiente(Serpiente *s) {
    Nodo *actual = s->cabeza;
    Nodo *siguiente;

    // Recorremos nodo por nodo eliminando la memoria de a uno
    while (actual != NULL) {
        // Primero guardamos la dirección del siguiente para no perderla
        siguiente = actual->siguiente;
        
        // Liberamos la memoria del nodo actual
        free(actual); 
        
        // Avanzamos al nodo que guardamos previamente
        actual = siguiente; 
    }
    // Reseteamos el contenedor
    s->cabeza = NULL;
    s->longitud = 0;
}