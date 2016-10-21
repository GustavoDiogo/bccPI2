// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdbool.h>
//#include "stdafx.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
//#include <allegro5\allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/events.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#define animarvore1frames 3
#define animp1frame 1

const int window_height = 720;
const int window_width = 1280;

int t_curFrame = 0;
int t_frameCount = 0;
int t_frameDelay = 13;

//LIFES
int char_hp_total = 100;
int char_hp_current = 100;
int mon1_hp_total = 300;
int mon1_hp_current = 300;
int mon2_hp_total = 0;
int mon2_hp_current = 0;
int mon3_hp_total = 0;
int mon3_hp_current = 0;


ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_FONT *font = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *arrow = NULL;
ALLEGRO_BITMAP *monster_arrow = NULL;
ALLEGRO_BITMAP *praca1 = NULL;
ALLEGRO_BITMAP *arvore1 = NULL;
ALLEGRO_BITMAP *arvore2 = NULL;
ALLEGRO_BITMAP *arvore3 = NULL;
ALLEGRO_BITMAP *animp1[animp1frame];
ALLEGRO_BITMAP *animarvore1[animarvore1frames];

typedef struct question{
    
    int ans_num;
    char question[100];
    char answer[100];
    char other1[100];
    char other2[100];
    char other3[100];
    char other4[100];
    struct question *next;
    
}Question;

Question *head_question;
Question *current_queston;

void load_questions() {
    
    FILE *file = fopen("questions.txt", "r");
    
    if(!file) {
        printf("Erro ao abrir o arquivo de questoes.");
        return;
    }
    
    char str[100];
    fgets(str, 100, file);
    
    int quantity = str[0] - 48;
    int x;
    
    head_question = (Question *) malloc(sizeof(Question));
    current_queston = head_question;
    
    if(quantity <= 0) {
        printf("Erro ao carregar as questoes.");
        return;
    }
    
    for(x = 0; x < quantity; x++) {
        Question *new_question = (Question *) malloc(sizeof(Question));
        
        char nstr[100];
        fgets(nstr, 100, file);
        
        int num = nstr[0] - 48;
        
        new_question->ans_num = num;
        
        fgets(new_question->question, 100, file);
        fgets(new_question->answer, 100, file);
        fgets(new_question->other1, 100, file);
        
        if(num > 2) {
            
            fgets(new_question->other2, 100, file);
            
            if(num > 3) {
                
                fgets(new_question->other3, 100, file);
                
                if(num > 4){
                    
                    fgets(new_question->other4, 100, file);
                }
            }
        }
        
        current_queston->next = new_question;
        current_queston = new_question;
    }
    
    if(current_queston->next) {
        current_queston->next = NULL;
    }
    current_queston = head_question->next;
    
    fclose(file);
}

void destroy_questions() {
    
    Question *current = head_question->next;
    Question *prev = head_question;
    
    while(current) {
        free(prev);
        prev = current;
        current = current->next;
    }
    
}


ALLEGRO_BITMAP *load_bitmap_at_size(const char *filename, int w, int h) {
    
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
    
    //Trade font for Windows/Linux
    //font = al_load_font("C:/Windows/Fonts/arial.ttf", 36, 0);
    font = al_load_font("/Library/Fonts/arial.ttf", 36, 0);
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
    
    //Trade font for Windows/Linux
//    arrow = load_bitmap_at_size("images/arrow.png", 40, 40);
//    praca1 = al_load_bitmap("cenario/Praca1.jpg");
//    animarvore1[0] = al_load_bitmap("monstros/arvore1.png");
//    animarvore1[1] = al_load_bitmap("monstros/arvore2.png");
//    animarvore1[2] = al_load_bitmap("monstros/arvore3.png");
//    animp1[0] = al_load_bitmap("personagens/p1parado.png");
    
    arrow = load_bitmap_at_size("arrow.png", 35, 35);
    praca1 = al_load_bitmap("Praca1.jpg");
    animarvore1[0] = al_load_bitmap("arvore1.png");
    animarvore1[1] = al_load_bitmap("arvore2.png");
    animarvore1[2] = al_load_bitmap("arvore3.png");
    animp1[0] = al_load_bitmap("p1parado.png");
    monster_arrow = load_bitmap_at_size("arrow.png", 80, 80);
    
    if (!arrow) {
        return false;
    }
    if (!praca1) {
        return false;
    }
    if (!animarvore1[0]) {
        return false;
    }
    if (!animarvore1[1]) {
        return false;
    }
    if (!animarvore1[2]) {
        return false;
    }
    if (!animp1[0]) {
        return false;
    }
    if(!monster_arrow) {
        return false;
    }
    
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_flip_display();
    
    load_questions();
    
    return true;
}

void draw_background() {
    al_draw_bitmap(praca1, 0, 0, 0);
}

void animate_character1(int x, int y) {
    
    al_draw_bitmap(animp1[0], x, y, ALLEGRO_FLIP_HORIZONTAL);
}

void animate_arvore1(int x, int y) {
    
    if (++t_frameCount >= t_frameDelay)
    {
        t_frameCount = 0;
        if (++t_curFrame >= animarvore1frames)
        {
            t_curFrame = 0;
        }
    }
    
    al_draw_bitmap(animarvore1[t_curFrame], x, y, 0);
}


void draw_battle_menu() {
    
    // Ret‚ngulo: x1, y1, x2, y2, cor, espessura
    double rect_width = window_width / 2.0;
    double rect_height = window_height / 4.5;
    
    double center_x = rect_width;
    double rect_x1 = center_x - (rect_width / 2.0);
    double rect_x2 = center_x + (rect_width / 2.0);
    double rect_y1 = window_height - rect_height - 20.0;
    double rect_y2 = window_height - 20.0;
    
    double text_x = rect_x1 + (rect_width / 15.0);
    double text_y = rect_y1 + (rect_height / 15.0);
    
    //printf("%f %f %f %f\n", rect_x1, rect_y1, rect_x2, rect_y2);
    //printf("%f %f\n", text_x, text_y);
    //printf("%f %f\n", rect_x2 - (text_x - rect_x1), rect_y2 - (rect_height / 4.0));
    //
    //320 540 960 700
    //362.66 550.66
    //917.33 660
    
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
    
    //al_flip_display();
    
}

void draw_arrow_with(bool sel_x, bool sel_y) {
    
    
    double x, y;
    
    
    if (sel_x)
        x = 788.0;
    else
        x = 322.0;
    if (sel_y)
        y = 660.0;
    else
        y = 550.0;
    
    
    /*if (sel_x)
        x = rect_x2 - (text_x - rect_x1) - 50;
    else
        x = text_x - 50;
    if (sel_y)
        y = rect_y2 - 50;
    else
        y = text_y; */
    
    draw_battle_menu();
    al_draw_bitmap(arrow, x, y, 0);
    //al_flip_display();
}

void destroy_allegro() {
    
    destroy_questions();
    
    int a;
    
    for(a = 0; a < animarvore1frames; a++) {
        al_destroy_bitmap(animarvore1[a]);
    }
    
    for(a = 0; a < animp1frame; a++) {
        al_destroy_bitmap(animp1[a]);
    }
    
    al_destroy_bitmap(praca1);
    al_destroy_bitmap(arrow);
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
    
    al_draw_text(font, al_map_rgb(255, 0, 0), window_width / 2, y1 + (h / 2.8), ALLEGRO_ALIGN_CENTER, "ENTER - Sim");
    al_draw_text(font, al_map_rgb(255, 0, 0), window_width / 2, y1 + (h / 1.5), ALLEGRO_ALIGN_CENTER, "ESC - Nao");
    
    al_flip_display();
    
    bool quit_while = false;
    
    al_rest(0.3);
    
    while (!quit_while) {
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
                    
                case ALLEGRO_KEY_ENTER:
                    
                    //NEED TO SAVE GAME
                    destroy_allegro();
                    break;
                    
                case ALLEGRO_KEY_ESCAPE:
                    
                    quit_while = true;
                    break;
            }
        }
    }
    
}


void draw_question_arrow(int num) {
    
    double x = 140, y;
    
    switch (num) {
        case 0:
            y = 430;
            break;
        case 1:
            y = 480;
            break;
        case 2:
            y = 530;
            break;
        case 3:
            y = 580;
            break;
        case 4:
            y = 630;
            break;
        default:
            return;
            break;
    }
    
    al_draw_bitmap(arrow, x, y, 0);
    
}


void draw_question_box(int answer_pos) {
    
    
    double y1, y2, y3, y4, y5;
    
    switch (answer_pos) {
        case 0:
            y1 = 550;
            y2 = 0;
            y3 = 0;
            y4 = 0;
            y5 = 0;
            break;
        case 1:
            y1 = 0;
            y2 = 0;
            y3 = 0;
            y4 = 0;
            y5 = 0;
            break;
        case 2:
            y1 = 0;
            y2 = 0;
            y3 = 0;
            y4 = 0;
            y5 = 0;
            break;
        case 3:
            y1 = 0;
            y2 = 0;
            y3 = 0;
            y4 = 0;
            y5 = 0;
            break;
        case 4:
            y1 = 0;
            y2 = 0;
            y3 = 0;
            y4 = 0;
            y5 = 0;
            break;
        default:
            break;
    }
    
    al_draw_filled_rectangle(20, 270, 1260, 700, al_map_rgb(0, 0, 255));
    
    al_draw_text(font, al_map_rgb(255, 0, 0), 100, 290, ALLEGRO_ALIGN_LEFT, current_queston->question);
    al_draw_text(font, al_map_rgb(255, 0, 0), 200, 430, ALLEGRO_ALIGN_LEFT, current_queston->answer);
    al_draw_text(font, al_map_rgb(255, 0, 0), 200, 480, ALLEGRO_ALIGN_LEFT, current_queston->other1);
    
    int num = current_queston->ans_num;
    
    if(num > 2) {
        
        al_draw_text(font, al_map_rgb(255, 0, 0), 200, 530, ALLEGRO_ALIGN_LEFT, current_queston->other2);
        
        if(num > 3) {
            
            al_draw_text(font, al_map_rgb(255, 0, 0), 200, 580, ALLEGRO_ALIGN_LEFT, current_queston->other3);
            
            if(num > 4) {
                
                al_draw_text(font, al_map_rgb(255, 0, 0), 200, 630, ALLEGRO_ALIGN_LEFT, current_queston->other4);
                
            }
            
        }
        
    }
}


// 0 -> Failed to load question
// 1 -> Question answer was correct
// 2 -> Question answer was incorrect
// 3 -> Cancel question

int show_question() {
    
    Question *the_question = current_queston;
    
    
    int arrow_pos = 0;
    
    int answer_pos = 2;
    
    if(the_question) {
        
        //current_queston = current_queston->next;
        
        printf("drawing_question after if\n");
        printf("%s\n", head_question->question);
        printf("%s\n", current_queston->question);
        draw_question_box(answer_pos);
        draw_question_arrow(arrow_pos);
        al_flip_display();
        
        while(1) {
            
            ALLEGRO_EVENT ev;
            al_wait_for_event(event_queue, &ev);
            
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (ev.keyboard.keycode) {
                        
                    case ALLEGRO_KEY_UP:
                        arrow_pos--;
                        if(arrow_pos < 0) {
                            arrow_pos = the_question->ans_num - 1;
                        }
                        break;
                    case ALLEGRO_KEY_DOWN:
                        arrow_pos++;
                        if(arrow_pos >= the_question->ans_num) {
                            arrow_pos = 0;
                        }
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        return 3;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        if(arrow_pos == answer_pos) {
                            current_queston = current_queston->next;
                            return 1;
                        }
                        
                        else {
                            current_queston = current_queston->next;
                            return 2;
                        }
                        break;
                }
                
                printf("drawing_question after key down\n");
                printf("%s\n", current_queston->question);
                draw_question_box(answer_pos);
                draw_question_arrow(arrow_pos);
                al_flip_display();
            }
            
        }
        
    }
    
    else {
        printf("Failed to load question\n");
        
        if(head_question->next != NULL) {
            current_queston = head_question->next;
            
            return show_question();
        }
        
        else return 0;
    }
    
}

void draw_life_bars() {
    
    //   -| 130 |- //
    // 15 |
    
    double y_fator = (double)char_hp_current / char_hp_total;
    
    al_draw_filled_rectangle(170, 410, 300, 425, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(172, 412, 168 + (y_fator * 130), 423, al_map_rgb(0, 255, 0));
    
    y_fator = mon1_hp_current / mon1_hp_total;
    
    al_draw_filled_rectangle(980, 335, 1110, 350, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(982, 337, 978 + (y_fator * 130), 348, al_map_rgb(0, 255, 0));
    
    if(mon2_hp_total > 0) {
        
        y_fator = mon2_hp_current / mon2_hp_total;
        
        al_draw_filled_rectangle(0, 0, 0, 0, al_map_rgb(255, 0, 0));
        al_draw_filled_rectangle(0, 0, 0, 0, al_map_rgb(0, 255, 0));
        
        if(mon3_hp_total > 0){
            
            y_fator = mon3_hp_current / mon3_hp_total;
            
            al_draw_filled_rectangle(0, 0, 0, 0, al_map_rgb(255, 0, 0));
            al_draw_filled_rectangle(0, 0, 0, 0, al_map_rgb(0, 255, 0));
        }
    }
}

void draw_arrow_monster(int selected) {
    
    switch (selected) {
        case 0:
            al_draw_bitmap(monster_arrow, 1020, 285, 0);
            break;
        case 1:
            al_draw_bitmap(monster_arrow, 0, 0, 0);
            break;
        case 2:
            al_draw_bitmap(monster_arrow, 0, 0, 0);
            break;
        default:
            break;
    }
    
}


//Comentado para trabalhar fora do Mac OS
int main(int argc, char **argv) {

//Comentado para trabalhar no Mac OS
//int main(void) {
    
    
    // Init
    if (!begin_allegro_init()) {
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
    
    bool battle_on = true;
    
    //Wait for command
    while(battle_on) {
        
        mon1_hp_total = 300;
        mon1_hp_current = 300;
        
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
                
                al_flip_display();
            }
            
            else if (ev.type == ALLEGRO_EVENT_TIMER) {
                
                al_clear_to_color(al_map_rgb(255, 255, 255));
                draw_background();
                draw_arrow_with(selected_x, selected_y);
                animate_character1(200, 430);
                animate_arvore1(950, 350);
                draw_life_bars();
                al_flip_display();
                
            }
        }
        
        //BEGIN QUESTION
        int result = show_question();
        
        
        if(result == 0) {
            //sem perguntas! END ALLEGRO??
        }
        
        
        else if(result == 1) {
            //EXECUTAR AÇÃO
            
            if(!selected_x && !selected_y) {
                
                //ATACAR MONSTRO
                bool monster_selected = false;
                
                // maior valor à esquerda
                int selected = 0, num_monster = 1;
                
                if(mon2_hp_total > 0) {
                    if(mon3_hp_total > 0) {
                        num_monster = 3;
                    }
                    
                    else {
                        num_monster = 2;
                    }
                }
                
                while(!monster_selected) {
                    
                    ALLEGRO_EVENT ev;
                    al_wait_for_event(event_queue, &ev);
                    
                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                        
                        switch (ev.keyboard.keycode) {
                                
                            case ALLEGRO_KEY_LEFT:
                                selected++;
                                
                                if(selected >= num_monster) {
                                    selected = 0;
                                }
                                break;
                            case ALLEGRO_KEY_RIGHT:
                                selected--;
                                
                                if(selected < 0) {
                                    selected = num_monster - 1;
                                }
                                break;
                            case ALLEGRO_KEY_ENTER:
                                monster_selected = true;
                                break;
                            case ALLEGRO_KEY_ESCAPE:
                                //voltar? ele terá que responder denovo se voltar?
                                break;
                        }
                        
                        //UPDATE VISUAL OF SELECT MONSTER ARROW
                    }
                }
                
                if(selected == 0) {
                  
                    mon1_hp_current -= 70;
                    
                } else if(selected == 1) {
                    
                    mon2_hp_current -= 70;
                    
                } else {
                   
                    mon3_hp_current -= 70;
                }
            }
            
            else if(!selected_x && selected_y) {
                //USAR ITEM
            }
            
            else if(selected_x && !selected_y) {
                //USAR HABILIDADE
            }
            
            else if(selected_x && selected_y) {
                //FUGIR
                battle_on = false;
            }
        }
        
        else if(result == 2) {
            //SOFRER ATAQUE
            char_hp_current -= 30;
        }
        
        else if(result == 3) {
            //IGNORAR
        }
        
        enter_pressed = false;
        
        
        //CHECK BATTLE STATUS
        
        if(char_hp_current <= 0) {
            //GAME OVER!!
            battle_on = false;
        }
        
        else if(mon1_hp_current <=0 && mon2_hp_current <=0 && mon3_hp_current <=0) {
            //BATTLE WON!
            battle_on = false;
        }
    }
    
    //al_rest(10);
    
    destroy_allegro();
    
    return 0;
    
}
