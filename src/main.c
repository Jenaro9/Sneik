Control Principal(src / main.c)
    El núcleo del programa será un bucle iterativo(while o do... while) que controle el flujo del juego
        .Bucle de Juego : Capturar tecla(investigar _kbhit() y _getch() para Windows)
        .Actualizar lógica de posición.Verificar colisiones.Redibujar tablero.Controlar la velocidad mediante Sleep()
        .Persistencia : Al finalizar,
    se debe pedir el nombre del jugador, buscarlo en la lista dinámica de jugadores(o agregarlo) y guardar el ranking actualizado en un archivo