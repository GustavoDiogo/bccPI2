#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#define animarvore1frame 3

#define PER1_ANIM_ANDAR 2
#define PER1_ANIM_ATAQUE 3
#define PER1_ANIM_BLOQUEIO 1
#define PER1_ANIM_PARADO 1

struct ObjAnim 
{
	enum 
	{
		ANDAR,
		ATAQUE,
		BLOCK,
		PARADO
	} ECurrentPerAnim;

	enum
	{
		ATTACKING,
		IDLE

	} ECurrentPerState;

	int x;
	int y;

	enum ECurrentAnim currentAnimState;
	enum ECurrentPerState currentState;

	int currentFrame;
	int currentMaxFrame;

	int frameRate;
	int tick;

	bool invert;

	int walkSpeed;
	bool bAttackAnimController;

	ALLEGRO_BITMAP* currentAnim;

	ALLEGRO_BITMAP *animAndar[PER1_ANIM_ANDAR];
	ALLEGRO_BITMAP *animAtaque[PER1_ANIM_ATAQUE];
	ALLEGRO_BITMAP *animBloqueio[PER1_ANIM_BLOQUEIO];
	ALLEGRO_BITMAP *animParado[PER1_ANIM_PARADO];
};

int curFramearvore1 = 0;
int frameCountarvore1 = 0;
int frameDelayarvore1 = 13;

int curFramep1 = 0;
int frameCountp1 = 0;
int frameDelayp1 = 13;


ALLEGRO_DISPLAY *window = NULL;
int window_width = 1280;
int window_height = 720;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *praca1 = NULL;
ALLEGRO_BITMAP *p1parado = NULL;
ALLEGRO_BITMAP *animarvore1[animarvore1frame];
ALLEGRO_BITMAP *animp1[PER1_ANIM_ANDAR];

struct ObjAnim PerAnim1;

bool iniciar();
void fbackground();
void fanimp1(int x, int y);
void fanimarvore1(int x, int y);
void fanimataquep1(int x, int y);
void updateAnim(struct ObjAnim *objAnim, bool invert);
void updateObjAnim(struct ObjAnim *objAnim, ALLEGRO_BITMAP *anim[], bool invert);

void triggerAnimationUpdate();
void atackMobAnim(struct ObjAnim *objAnim);

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
				PerAnim1.currentState = ATTACKING;
				triggerAnimationUpdate();
				break;
			case ALLEGRO_KEY_RIGHT:
				
				break;
			case ALLEGRO_KEY_UP:
				break;
			case ALLEGRO_KEY_DOWN:
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			fbackground();
			
			// fanimataquep1(200, 430);
			fanimarvore1(950, 350);

			updateAnim(&PerAnim1, PerAnim1.invert);

			if (PerAnim1.currentAnim == ATTACKING)
			{
				atackMobAnim(&PerAnim1);
			}

			al_flip_display();

		}

	}


	// end
	al_destroy_display(window);
	al_destroy_font(font);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);


	return 0;
}

void fbackground() {
	al_draw_bitmap(praca1, 0, 0, 0);

	double rect_width = window_width / 2.0;
	double rect_height = window_height / 4.5;

	double center_x = rect_width;
	double rect_x1 = center_x - (rect_width / 2.0);
	double rect_x2 = center_x + (rect_width / 2.0);
	double rect_y1 = window_height - rect_height - 20.0;
	double rect_y2 = window_height - 20.0;

	double text_x = rect_x1 + (rect_width / 15.0);
	double text_y = rect_y1 + (rect_height / 15.0);

	al_draw_filled_rectangle(rect_x1, rect_y1, rect_x2, rect_y2, al_map_rgb(0, 0, 255));

	al_draw_text(font, al_map_rgb(255, 255, 255), text_x, text_y, ALLEGRO_ALIGN_LEFT, "Ataque normal");

	al_draw_text(font, al_map_rgb(255, 255, 255), rect_x2 - (text_x - rect_x1), text_y, ALLEGRO_ALIGN_RIGHT, "Items");


}

void updateAnim(struct ObjAnim *objAnim, bool invert)
{
	switch (objAnim->currentAnimState)
	{
	case ANDAR:
		objAnim->currentMaxFrame = PER1_ANIM_ANDAR;

		updateObjAnim(objAnim, objAnim->animAndar, invert);
		break;
	case ATAQUE:
		objAnim->currentMaxFrame = PER1_ANIM_ATAQUE;
		
		updateObjAnim(objAnim, objAnim->animAtaque, invert);
		break;
	case BLOCK:
		objAnim->currentMaxFrame = PER1_ANIM_BLOQUEIO;
		
		updateObjAnim(objAnim, objAnim->animBloqueio, invert);
		break;
	case PARADO:
		objAnim->currentMaxFrame = PER1_ANIM_PARADO;

		updateObjAnim(objAnim, objAnim->animParado, invert);
		break;
	default:
		break;
	}
}

void updateObjAnim(struct ObjAnim *objAnim, ALLEGRO_BITMAP *anim[], bool invert)
{
	objAnim->tick++;

	if (objAnim->tick >= objAnim->frameRate)
	{
		objAnim->tick = 0;
		
		objAnim->currentFrame++;

		if (objAnim->currentFrame >= objAnim->currentMaxFrame)
		{
			objAnim->currentFrame = 0;
		}
	}

	al_draw_bitmap(anim[objAnim->currentFrame], objAnim->x, objAnim->y, (invert == true) ? 1 : 0);
}

void fanimataquep1(int x, int y) {


	if (++frameCountp1 >= frameDelayp1)
	{

		frameCountp1 = 0;

		if (++curFramep1 >= 2)
		{

			curFramep1 = 0;

		}
	}
	
	al_draw_bitmap(animp1[curFramep1], x, y, 1);
}



void fanimarvore1(int x, int y) {

	if (++frameCountarvore1 >= frameDelayarvore1)
	{

		frameCountarvore1 = 0;

		if (++curFramearvore1 >= animarvore1frame)
		{
			curFramearvore1 = 0;
		}
	}

	al_draw_bitmap(animarvore1[curFramearvore1], x, y, 0);
}

void triggerAnimationUpdate()
{
	PerAnim1.currentAnimState = ANDAR;
	PerAnim1.bAttackAnimController = false;
	PerAnim1.walkSpeed = 5;
}

void atackMobAnim(struct ObjAnim *objAnim)
{
	objAnim->x += objAnim->walkSpeed;

	if (!objAnim->bAttackAnimController)
	{
		if (objAnim->x >= 900)
		{
			objAnim->walkSpeed = 0;

			objAnim->currentAnimState = ATAQUE;
			
			if (objAnim->currentAnimState == ATAQUE)
			{
				if (objAnim->currentFrame >= objAnim->currentMaxFrame - 1)
				{
					objAnim->currentAnimState = ANDAR;

					objAnim->bAttackAnimController = true;
				}
			}
		}
	}
	else
	{
		PerAnim1.invert = false;

		objAnim->walkSpeed = -5;

		if (objAnim->x <= 200)
		{
			objAnim->currentAnimState = PARADO;
			objAnim->currentFrame = 0;

			objAnim->currentState = IDLE;

			PerAnim1.invert = true;
			objAnim->walkSpeed = 0;
		}
	}
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
	font = al_load_font("C:/Windows/Fonts/impact.ttf", 32, 0);
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
	praca1 = al_load_bitmap("cenario/Praca1.jpg");
	if (!praca1)
	{
		printf("Falha ao carregar o fundo da tela");
		al_destroy_display(window);
		return false;
	}

	animarvore1[0] = al_load_bitmap("monstros/arvore1.png");
	if (!animarvore1[0]) {
		printf("Falha ao carregar o monstro");
		al_destroy_display(window);
		return false;

	}
	animarvore1[1] = al_load_bitmap("monstros/arvore2.png");
	if (!animarvore1[1]) {
		printf("Falha ao carregar o monstro");
		al_destroy_display(window);
		return false;

	}
	animarvore1[2] = al_load_bitmap("monstros/arvore3.png");
	if (!animarvore1[2])
	{
		printf("Falha ao carregar o monstro");
		al_destroy_display(window);
		return false;
	}

	// Inicializa struc do personagem

	PerAnim1.x = 200;
	PerAnim1.y = 430;

	PerAnim1.tick = 0;

	PerAnim1.currentFrame = 0; 
	PerAnim1.frameRate = 10;

	PerAnim1.animAndar[0] = al_load_bitmap("personagens/p1andando1.png");
	PerAnim1.animAndar[1] = al_load_bitmap("personagens/p1andando2.png");

	PerAnim1.animAtaque[0] = al_load_bitmap("personagens/p1ataque1.png");
	PerAnim1.animAtaque[1] = al_load_bitmap("personagens/p1ataque2.png");
	PerAnim1.animAtaque[2] = al_load_bitmap("personagens/p1ataque3.png");

	PerAnim1.animBloqueio[0] = al_load_bitmap("personagens/p1bloqueio.png");

	PerAnim1.animParado[0] = al_load_bitmap("personagens/p1parado.png");
		
	PerAnim1.currentAnimState = PARADO;
	PerAnim1.currentState = IDLE;
	
	PerAnim1.invert = true;
	
	PerAnim1.bAttackAnimController = false;
	PerAnim1.walkSpeed = 0;

	if (!PerAnim1.animAndar[0])
	{
		printf("Erro ao carregar Struct do Personagem! animAndar0\n");
		al_destroy_display(window);
		return false;
	}

	if (!PerAnim1.animAndar[1])
	{
		printf("Erro ao carregar Struct do Personagem! animAndar1\n");
		al_destroy_display(window);
		return false;
	}

	if (!PerAnim1.animAtaque[0])
	{
		printf("Erro ao carregar Struct do Personagem! animAtaque0\n");
		al_destroy_display(window);
		return false;
	}

	if (!PerAnim1.animAtaque[1])
	{
		printf("Erro ao carregar Struct do Personagem! animAtaque1\n");
		al_destroy_display(window);
		return false;
	}

	if (!PerAnim1.animAtaque[2])
	{
		printf("Erro ao carregar Struct do Personagem! animAtaque2\n");
		al_destroy_display(window);
		return false;
	}

	if (!PerAnim1.animBloqueio[0])
	{
		printf("Erro ao carregar Struct do Personagem! animBloqueio0\n");
		al_destroy_display(window);
		return false;
	}

	if (!PerAnim1.animParado[0])
	{
		printf("Erro ao carregar Struct do Personagem! animParado0\n");
		al_destroy_display(window);
		return false;
	}

	return true;
}
