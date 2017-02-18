## Projeto Integrador II: Jogo Educacional 2D
#### Bacharelado em Ciências da Computação: 2º Semestre(2016)

O objetivo deste projeto é a confecção de um Jogo Educacional 2D utilizando a linguagem C e a biblioteca Allegro.

O resultado foi um jogo baseado por turnos com sistema de RPG, porém para poder progredir e fazer ações dentro do jogo é necessário responder várias perguntas customizáveis que estão dentro do arquivo de texto <b>questions.txt</b>, possibilitando assim que professores possam utilizar este jogo como metódo de ensino interativo e divertido. O ambiente do jogo é o próprio campus da faculdade.

Para alterar/customizar as questões bastar editar o arquivo <b>questions</b> e seguir os seguintes passos:
1ª linha: Número total de questões
2ª linha: Número total de alternativas da questão que você irá escrever
3ª linha: Texto da pergunta
4ª linha: Respota certa
Demais linhas: Respostas erradas
Outro número: Número total de alternativas da próxima questão

OBS: Dentro do jogo as questões sempre se misturarão, portanto a primeira alternativa não necessariamente será a certa.



##Linux
###Compilação

>sudo apt-get install liballegro5-dev

>gcc main.c -o main -lm -pedantic -std=c11 -W -Wall $(pkg-config –cflags –libs allegro-5) -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_primitives -lallegro_ttf -lallegro_audio -lallegro_acodec

>./main

##Windows

Utilizou-se o Visual Studio para compilação no Windows, o código-fonte permanece o mesmo do <b>main.c</b>, sua localização está no diretório <b>Windows/game.exe</b>. É necessário baixar o diretório Windows/ inteiro.
