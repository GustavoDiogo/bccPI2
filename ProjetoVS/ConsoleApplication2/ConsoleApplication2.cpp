// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdbool.h>
#include "stdafx.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\events.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_image.h>

const int window_height = 720;
const int window_width = 1280;

ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *arrow = NULL;


ALLEGRO_BITMAP *load_bitmap_at_size(const char *filename, int w, int h)  {

	ALLEGRO_BITMAP *resized_bmp, *loaded_bmp, *prev_target;

	// 1. create a temporary bitmap of size we want
	resized_bmp = al_create_bitmap(w, h);
	if (!resized_bmp) return NULL;

	// 2. load the bitmap at the original size
	loaded_bmp = al_load_bitmap(filename);
	if (!loaded_bmp)
	{
		al_destroy_bitmap(resized_bmp);
		return NULL;
	}

	// 3. set the target bitmap to the resized bmp
	prev_target = al_get_target_bitmap();
	al_set_target_bitmap(resized_bmp);

	// 4. copy the loaded bitmap to the resized bmp
	al_draw_scaled_bitmap(loaded_bmp,
		0, 0,                                // source origin
		al_get_bitmap_width(loaded_bmp),     // source width
		al_get_bitmap_height(loaded_bmp),    // source height
		0, 0,                                // target origin
		w, h,                                // target dimensions
		0                                    // flags
	);

	// 5. restore the previous target and clean up
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	return resized_bmp;
}


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

	if (!al_install_keyboard()) {
		printf("Failed to install keyboard.");
		return false;
	}

	if (!al_init_image_addon()) {
		printf("Failed to init image addon.");
		return false;
	}

	window = al_create_display(window_width, window_height);
	font = al_load_font("C:/Windows/Fonts/arial.ttf", 36, 0);
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	if (!window) {
		printf("Insert error here");
		return false;
	}
	if (!font) {
		printf("Insert error here");
		return false;
	}
	if (!event_queue) {
		printf("Insert error here");
		return false;
	}
	if (!timer) {
		printf("Insert error here");
		return false;
	}

	arrow = load_bitmap_at_size("images/arrow.png", 50, 50);
	if (!arrow) {
		return false;
	}

	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_flip_display();

	return true;
}



void draw_battle_menu() {

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

	//Habilidades
	al_draw_text(font, al_map_rgb(255, 0, 0), text_x, rect_y2 - (rect_height / 4.0), ALLEGRO_ALIGN_LEFT, "Habilidades");

	//Fugir
	al_draw_text(font, al_map_rgb(255, 0, 0), rect_x2 - (text_x - rect_x1), rect_y2 - (rect_height / 4.0), ALLEGRO_ALIGN_RIGHT, "Fugir");

	al_flip_display();

	return;
}

void draw_arrow_with(bool sel_x, bool sel_y) {

	double rect_width = window_width / 2.0;
	double rect_height = window_height / 4.5;

	double center_x = rect_width;
	double rect_x1 = center_x - (rect_width / 2.0);
	double rect_x2 = center_x + (rect_width / 2.0);
	double rect_y1 = window_height - rect_height - 20.0;
	double rect_y2 = window_height - 20.0;

	double text_x = rect_x1 + (rect_width / 15.0);
	double text_y = rect_y1 + (rect_height / 15.0);

	double x, y;

	if (sel_x)
		x = rect_x2 - (text_x - rect_x1) - 50;
	else
		x = text_x - 50;
	if (sel_y)
		y = rect_y2 - 50;
	else
		y = text_y;

	al_clear_to_color(al_map_rgb(255, 255, 255));
	draw_battle_menu();
	al_draw_bitmap(arrow, x, y, 0);
	al_flip_display();
}

void destroy_allegro() {

	al_destroy_display(window);
	al_destroy_font(font);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
}

void show_exit_screen() {

	double w = window_width / 3.0;
	double h = window_height / 3.0;
	double x1 = (window_width / 2.0) - (w / 2.0);
	double x2 = (window_width / 2.0) + (w / 2.0);
	double y1 = (window_height / 2.0) - (h / 2.0);
	double y2 = (window_height / 2.0) + (h / 2.0);

	//
	al_draw_filled_rectangle(0, 0, window_width, window_height, al_map_rgba(0, 0, 0, 150));

	//Text box
	al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0, 0, 255));


	al_draw_text(font, al_map_rgb(255, 0, 0), window_width / 2, y1 + 15.0, ALLEGRO_ALIGN_CENTER, "Deseja sair?");

	al_draw_text(font, al_map_rgb(255, 0, 0), window_width / 2, y1 + (h / 2.8), ALLEGRO_ALIGN_CENTER, "Sim");
	al_draw_text(font, al_map_rgb(255, 0, 0), window_width / 2, y1 + (h / 1.5), ALLEGRO_ALIGN_CENTER, "Nao");

	al_flip_display();
}


int main(void)
{
	// Init
	if (!begin_allegro_init()) {
        destroy_allegro();
		return -1;
	}

	draw_battle_menu();

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_start_timer(timer);


	bool selected_x = false;
	bool selected_y = false;
	bool enter_pressed = false;

	draw_arrow_with(selected_x, selected_y);

	//Wait for command
	while (!enter_pressed) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_UP:
				selected_y = !selected_y;
				draw_arrow_with(selected_x, selected_y);
				break;
			case ALLEGRO_KEY_DOWN:
				selected_y = !selected_y;
				draw_arrow_with(selected_x, selected_y);
				break;
			case ALLEGRO_KEY_LEFT:
				selected_x = !selected_x;
				draw_arrow_with(selected_x, selected_y);
				break;
			case ALLEGRO_KEY_RIGHT:
				selected_x = !selected_x;
				draw_arrow_with(selected_x, selected_y);
				break;
			case ALLEGRO_KEY_ENTER:
				enter_pressed = true;
				break;
			case ALLEGRO_KEY_ESCAPE:
				show_exit_screen();
				break;
			}
		}
	}


	destroy_allegro();

	return 0;
}
