#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

ALLEGRO_DISPLAY *window = NULL;
int window_width = 1280;
int window_height = 720;
ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *praca1 = NULL;
ALLEGRO_BITMAP *arvore1 = NULL;


bool iniciar();

int main(void)
{
		if (!iniciar()) {
		return -1;
	}	
	
	// fill with white
	al_draw_bitmap(praca1, 0, 0, 0);

	al_flip_display();
	
	// Retângulo: x1, y1, x2, y2, cor, espessura
	double rect_width = window_width / 2.0;
	double rect_height = window_height / 4.5;

	double center_x = rect_width;
	double rect_x1 = center_x - (rect_width / 2.0);
	double rect_x2 = center_x + (rect_width / 2.0);
	double rect_y1 = window_height - rect_height - 20.0;
	double rect_y2 = window_height - 20.0;

	double text_x = rect_x1 + (rect_width / 15.0);
	double text_y = rect_y1 + (rect_height / 15.0);

	//Text box	
	al_draw_filled_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, al_map_rgb(0, 0, 255));

	//Ataque normal text
	al_draw_text(font, al_map_rgb(255, 255, 255), text_x, text_y, ALLEGRO_ALIGN_LEFT, "Ataque normal");

	//Items text
	al_draw_text(font, al_map_rgb(255, 255, 255), rect_x2 - (text_x - rect_x1), text_y, ALLEGRO_ALIGN_RIGHT, "Items");

	al_draw_bitmap(arvore1, 1000, 350, 0);
		
	al_flip_display();

	// hold for 10 seconds
	al_rest(10.0);

	// end
	al_destroy_display(window);
	al_destroy_font(font);
	al_destroy_bitmap(arvore1);

	return 0;
}

bool iniciar() {

	if (!al_init()) {
		printf("Failed to init allegro.");
		return false;
	}

	if (!al_init_primitives_addon()) {
		printf("Failed to init allegro primitives.");
		return false;
	}

	if (!al_init_font_addon()) {
		printf("Failed to init allegro font.");
		return false;
	}

	if (!al_init_ttf_addon()) {
		printf("Failed to init allegro TFF.");
		return false;
	}
	if (!al_init_image_addon())
	{
		fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
		return false;
	}
	window = al_create_display(window_width, window_height);
	if (!window) {
		printf("Falha ao carregar a tela");
		return false;
	}
	font = al_load_font("C:/Windows/Fonts/impact.ttf", 48, 0);
	if (!font) {
		printf("Falha ao carregar a fonte");
		al_destroy_display(window);
		return false;
	}
	arvore1 = al_load_bitmap("monstros/arvore1.png");
	if (!arvore1)
	{
		printf("Falha ao carregar personagem");
		al_destroy_display(window);
		return false;
	}
	praca1 = al_load_bitmap("cenario/Praca1.jpg");
	if (!praca1)
	{
		printf("Falha ao carregar o fundo da tela");
		al_destroy_display(window);
		return false;
	}
	return true;
}


