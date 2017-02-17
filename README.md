## Projeto Integrador II: Jogo Educacional 2D
#### Bacharelado em Ciências da Computação: 2º Semestre(2016)

##Compilação no Linux

>sudo apt-get install liballegro5-dev

>gcc main2.c -o main2 -lm -pedantic -std=c11 -W -Wall $(pkg-config –cflags –libs allegro-5) -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_primitives -lallegro_ttf -lallegro_audio -lallegro_acodec
