// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>



bool begin_allegro_init() {

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

	return true;
}


int main(void)
{
	int window_height = 1080;
	int window_width = 1920;

	// Window
	ALLEGRO_DISPLAY *window = NULL;

	//Font
	ALLEGRO_FONT *font = NULL;

	// Init
	if (!begin_allegro_init()) {
		return -1;
	}
	
	// creating a 1080p window
	window = al_create_display(window_width, window_height);
	font = al_load_font("C:/Windows/Fonts/arial.ttf", 48, 0);
	//C:/usr/share/fonts/type1/gsfonts/a010013l.pfb

	if (!font) {
		printf("Failed to create font.");
		return -1;
	}

	// fill with white
	al_clear_to_color(al_map_rgb(255, 255, 255));
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
	al_draw_text(font, al_map_rgb(255, 0, 0), text_x, text_y, ALLEGRO_ALIGN_LEFT, "Ataque normal");

	//Items text
	al_draw_text(font, al_map_rgb(255, 0, 0), rect_x2 - (text_x - rect_x1), text_y, ALLEGRO_ALIGN_RIGHT, "Items");

	
	al_flip_display();

	// hold for 10 seconds
	al_rest(10.0);

	// end
	al_destroy_display(window);
	al_destroy_font(font);

	return 0;
}