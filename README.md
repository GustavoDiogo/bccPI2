# Projeto Toronto

Compilação no Linux

>sudo apt-get install liballegro5-dev

>gcc main.c -o main $(pkg-config –cflags –libs allegro-5) -lallegro -lallegro_main -lallegro_image

Exemplo para add dependencias: -lallegro_fonts e etc no final do comando de compilação e dar include no começo do código também.
