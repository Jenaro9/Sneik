// Lógica del Juego (src/snake.c)
// Este archivo debe contener las funciones de control de la serpiente utilizando memoria dinámica con malloc
// .
// Movimiento: Para mover la serpiente, deben implementar una función que realice un "insertar al inicio" de la lista para la nueva cabeza
//  y, si no comió una manzana, elimine el último nodo (la cola)
// .
// Detección de Colisiones: Recorrer la lista dinámica para verificar si la nueva posición de la cabeza coincide con alguna posición del cuerpo o con los límites del arreglo bidimensional
// .
// Generación de Manzanas: Usar la función rand() de stdlib.h para obtener coordenadas aleatorias dentro de los límites del tablero que no estén ocupadas por la serpiente
