#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

int curframe = 0;
int framecount = 0;
int framedelay = 5;


ALLEGRO_DISPLAY *window = NULL;
int window_width = 1280;
int window_height = 720;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *praca1 = NULL;
ALLEGRO_BITMAP *arvore1 = NULL;
ALLEGRO_BITMAP *arvore2 = NULL;
ALLEGRO_BITMAP *arvore3 = NULL;
ALLEGRO_BITMAP *p1parado = NULL;
ALLEGRO_BITMAP *image[8];

bool iniciar();

int main(void)
{
		if (!iniciar()) {
		return -1;
	}	
		bool sair = false;
		
		al_register_event_source(event_queue, al_get_timer_event_source(timer));
		al_register_event_source(event_queue, al_get_keyboard_event_source());
		al_start_timer(timer);
	
		while (!sair)
		{
			ALLEGRO_EVENT ev;
			al_wait_for_event(event_queue, &ev);
						
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					sair = true;
					break;
				case ALLEGRO_KEY_LEFT:
					break;
				case ALLEGRO_KEY_RIGHT:
					break;
				case ALLEGRO_KEY_UP:
					break;
				case ALLEGRO_KEY_DOWN:
					break;
				}
			}
			else if (ev.type == ALLEGRO_EVENT_TIMER) {
				// background
				al_draw_bitmap(praca1, 0, 0, 0);

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

				// Atualizando display
				al_flip_display();

				// hold for 10 seconds
				al_rest(10.0);
			}

		}
			
											
				/*image[0] = al_load_bitmap("monstros/arvore1.png");
				image[1] = al_load_bitmap("monstros/arvore2.png");
				image[2] = al_load_bitmap("monstros/arvore3.png");*/

			
		/*for (int i = 0; i < maxframes; i++) {
		
	}
	
	for(int i = 0; i< maxframes;i++){
		al_destroy_bitmap(image[i]);
	

	}*/

	// end
		al_destroy_display(window);
		al_destroy_font(font);
		al_destroy_event_queue(event_queue);
		al_destroy_bitmap(praca1);

	return 0;
}

		
bool iniciar()
{

	if (!al_init()) 
	{
		printf("Falha ao iniciar o Allegro");
		return false;
	}

	if (!al_init_primitives_addon())
	{
		printf("Falha ao iniciar Allegro primitives");
		return false;
	}

	if (!al_init_font_addon())
	{
		printf("Falha ao iniciar Allegro font");
		return false;
	}

	if (!al_init_ttf_addon())
	{
		printf("Falha ao iniciar Allegro TFF");
		return false;
	}
	if (!al_init_image_addon())	
	{
		printf("Falha ao iniciar Allegro_image.");
		return false;
	}
	window = al_create_display(window_width, window_height);
	if (!window) 
	{
		printf("Falha ao iniciar a tela");
		return false;
	}
	font = al_load_font("C:/Windows/Fonts/impact.ttf", 48, 0);
	if (!font) 
	{
		printf("Falha ao iniciar a fonte");
		al_destroy_display(window);
		return false;
	}
	if (!al_install_keyboard())
	{
		printf("Falha ao iniciar o teclado");
		return false;
	}
	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		printf("Falha ao iniciar fila de eventos");
		al_destroy_display(window);
		return false;
	}
	timer = al_create_timer(1.0 / 60);
	if (!timer)
	{
		printf("Falha ao iniciar o timer");
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
	arvore2 = al_load_bitmap("monstros/arvore2.png");
	if (!arvore1)
	{
		printf("Falha ao carregar personagem");
		al_destroy_display(window);
		return false;
	}
	arvore3 = al_load_bitmap("monstros/arvore3.png");
	if (!arvore1)
	{
		printf("Falha ao carregar personagem");
		al_destroy_display(window);
		return false;
	}
	p1parado = al_load_bitmap("personagens/p1parado.png");
	if (!p1parado)
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
