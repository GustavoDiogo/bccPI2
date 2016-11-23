// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/events.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#define animarvore1frames 3
#define animp1frame 1
#define MAX_ITEMS 20
#define MAX_SKILLS 4
#define MAX_ITEM_ID 3
#define MAX_CHARA 4
#define MAX_MONSTERS 3
#define PER1_ANIM_WALK 2
#define PER1_ANIM_ATACK 3
#define PER1_ANIM_BLOCK 1
#define PER1_ANIM_STOP 1
#define MONSTER_ANIM_MAX 12
#define MAX_WALK_Y 420
#define WALK_SPEED_X 110
#define WALK_SPEED_Y 85
#define WORLD_TIMER 0.01
#define BATTLE_TIMER 0.01
#define MONSTER_ANIM_DELAY 0.25
#define STEPS_WALK_ANIM 20
#define CHARACTER_HEIGHT 85
#define CHARACTER_WIDTH 65
#define CHAR_INTERACTION_SIZE 120
#define WALK_MONSTER_MIN 400
#define WALK_MONSTER_MAX 600
#define MAX_STAGES 18
#define START_X_RIGHT 1100
#define START_Y 550
#define START_X_LEFT 180
#define FADE_MAX 50
#define ICON_WIDTH 100
#define ICON_HEIGHT 100

const int window_height = 720;
const int window_width = 1280;

int t_curFrame = 0;
int t_frameCount = 0;
int t_frameDelay = 13;


double questions_pos_y[5] = {250, 350, 450, 550, 650};

char item_list[MAX_ITEM_ID][100];
int item_id_list[MAX_ITEM_ID];
char skill_list[MAX_SKILLS][100];
int skill_quantity = 0;
int item_quantity = 0;
int mon_quantity = 0;
int char_quantity = 1;
int current_stage = 0;
int current_song = 0;
int char_icon_num = -1;
int walk_interations = 0;
int interations_to_monster = 0;
int fx_count = 0;
int loaded_battle_tracks = 0;
int loaded_stage_tracks = 0;
bool monsters_on_walk = true;
bool is_on_battle = true;
bool boss_fight = false;
bool game_over = false;
bool boss_defeated[3] = {false, false, false};


ALLEGRO_COLOR interface_color, font_color;
ALLEGRO_THREAD *monster_anim_thread = NULL, *load_songs = NULL;
ALLEGRO_BITMAP *fxs[30];
ALLEGRO_DISPLAY *window = NULL;
ALLEGRO_FONT *font = NULL, *question_font = NULL, *answer_font = NULL;
ALLEGRO_EVENT_QUEUE *timer_queue = NULL;
ALLEGRO_EVENT_QUEUE *keyboard_queue = NULL;
ALLEGRO_TIMER *battle_timer, *world_timer;
ALLEGRO_BITMAP *arrow = NULL, *main_arrow = NULL;
ALLEGRO_BITMAP *monster_arrow = NULL;
ALLEGRO_BITMAP *background, *sup_screen, *char_icon;

#define btracks 8
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer;

void add_skill_to_character(int chara, int skill_id, int index);

//MARK: Structs

struct Playlist{
    ALLEGRO_SAMPLE *musica[btracks];
};

enum ObjectType {
    NPC,
    CHEST,
    OBSTACLE,
    MONSTER
};

enum ECurrentPerAnim {
    WALK,
    ATACK,
    BLOCK,
    STOP
};

enum ECurrentPerState {
    ATTACKING,
    IDLE,
    WALKING
};

struct MonsterAnim {
    
    int x;
    int y;
    
    int currentFrame;
    int currentMaxFrame;
    
    int frameRate;
    int tick;
    
    bool invert;
    
    int walkSpeed;
    bool bAttackAnimController;
    
    ALLEGRO_BITMAP* currentAnim;
    
    ALLEGRO_BITMAP *animations[MONSTER_ANIM_MAX];
    
};

struct PerAnim {
    
    double x;
    double y;
    
    enum ECurrentPerAnim currentAnimState;
    enum ECurrentPerState currentState;
    
    int currentFrame;
    int currentMaxFrame;
    
    int frameRate;
    int tick;
    int walkCount;
    int walkIndex;
    
    bool invert;
    
    int walkSpeed;
    bool bAttackAnimController;
    
    ALLEGRO_BITMAP* currentAnim;
    
    ALLEGRO_BITMAP *animAndar[PER1_ANIM_WALK];
    ALLEGRO_BITMAP *animAtaque[PER1_ANIM_ATACK];
    ALLEGRO_BITMAP *animBloqueio[PER1_ANIM_BLOCK];
    ALLEGRO_BITMAP *animParado[PER1_ANIM_STOP];
    
};

struct skill{
    
    int id;
    int fx_id;
    float atk;
    float matak;
    int mana_cost;
    bool target_monster;
    char name[20];
    
};

struct item{
    
    int id;
    int hp;
    int sp;
    char name[20];
    
};

typedef struct inventory{
    
    struct item items[MAX_ITEMS];
    
    
}Inventory;


typedef struct character{
    
    struct skill skills[MAX_SKILLS];
    struct PerAnim animation_object;
    int id;
    char name[20];
    int hp;
    int maxhp;
    int sp;
    int maxsp;
    int atk;
    int def;
    int matak;
    int mdef;
    int level;
    int exp;
    
}Character;

typedef struct monster{
    
    struct MonsterAnim animation_object;
    int id;
    char name[20];
    int hp;
    int maxhp;
    int atk;
    int def;
    int matak;
    int mdef;
    int exp;
    
}Monster;

typedef struct question{
    
    int ans_num;
    char question[500];
    char answer[300];
    char other1[300];
    char other2[300];
    char other3[300];
    char other4[300];
    struct question *next;
    
}Question;

struct worldObject {
    
    ALLEGRO_BITMAP *bitmap;
    int x1;
    int y1;
    int x2;
    int y2;
    
    int action_id;
    int action_quantity;
    
    enum ObjectType object_type;
};

//MARK: Objects

Question *head_question;
Question *current_queston;

//Character *main_char;

Inventory *inventory;

struct Playlist Batalha;
struct Playlist Stage;
struct monster monsters[3];
struct character characters[4];
struct worldObject objects[20];
int objects_quantity = 0;


//MARK: Convenience Funx


void remove_final_character(char str[100]) {
    
    for(int x = 0; x < 100; x++) {
        if(str[x] == '\n') {
            str[x] = '\0';
            return;
        }
    }
    
}

int rand_lim(int limit) {
    /* return a random number between 0 and limit inclusive.
     */
    
    int divisor = RAND_MAX/(limit+1);
    int retval;
    
    do {
        retval = rand() / divisor;
    } while (retval > limit);
    
    return retval;
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
    
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
    //al_draw_filled_rectangle(0, 0, w, h, al_map_rgba_f(1, 1, 1, 0.5));
    
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
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);
    
    return resized_bmp;
}

//MARK: Database


void clear_all_objects() {
    
    objects_quantity = 0;
    
    for(int x = 0; x < 20; x++) {
        objects[0].action_id = -1;
    }
    
}

void load_objects_on_stage(int stage) {
  
// Format of object
//    ALLEGRO_BITMAP *bitmap;
//    int x1;
//    int y1;
//    int x2;
//    int y2;
//    
//    int action_id;
//    int action_quantity;
//    
//    enum ObjectType object_type;
    
    
    clear_all_objects();
    
    if(stage == 0) {
        objects_quantity = 1;
        objects[0].bitmap = al_load_bitmap("segurancaparado.png");
        
        if(!objects[0].bitmap) {
            printf("Error on object");
            return;
        }
        
        double h = al_get_bitmap_height(objects[0].bitmap);
        double w = al_get_bitmap_width(objects[0].bitmap);
        objects[0].x1 = 800;
        objects[0].x2 = 800 + w;
        objects[0].y1 = 500;
        objects[0].y2 = 500 + h;
        
        objects[0].object_type = NPC;
        objects[0].action_id = 4;
    }
    
    //BOSS 1
    if(stage == 3) {
        objects_quantity = 1;
        objects[0].bitmap = al_load_bitmap("arvoreboss1.png");
        
        if(!objects[0].bitmap) {
            printf("Error on object");
            return;
        }
        
        double h = al_get_bitmap_height(objects[0].bitmap);
        double w = al_get_bitmap_width(objects[0].bitmap);
        objects[0].x1 = 400;
        objects[0].x2 = 400 + w;
        objects[0].y1 = 300;
        objects[0].y2 = 300 + h;
        
        objects[0].object_type = MONSTER;
        objects[0].action_id = 100;
        objects[0].action_quantity = 1;
    }
    
    //BOSS 2
    if(stage == 7) {
        objects_quantity = 1;
        objects[0].bitmap = al_load_bitmap("boloboss1.png");
        
        if(!objects[0].bitmap) {
            printf("Error on object");
            return;
        }
        
        double h = al_get_bitmap_height(objects[0].bitmap);
        double w = al_get_bitmap_width(objects[0].bitmap);
        objects[0].x1 = 400;
        objects[0].x2 = 400 + w;
        objects[0].y1 = 300;
        objects[0].y2 = 300 + h;
        
        objects[0].object_type = MONSTER;
        objects[0].action_id = 200;
        objects[0].action_quantity = 1;
    }
    
    //BOSS 3
    if(stage == 17) {
        objects_quantity = 1;
        objects[0].bitmap = al_load_bitmap("roboboss1.png");
        
        if(!objects[0].bitmap) {
            printf("Error on object");
            return;
        }
        
        double h = al_get_bitmap_height(objects[0].bitmap);
        double w = al_get_bitmap_width(objects[0].bitmap);
        objects[0].x1 = 400;
        objects[0].x2 = 400 + w;
        objects[0].y1 = 400;
        objects[0].y2 = 400 + h;
        
        objects[0].object_type = MONSTER;
        objects[0].action_id = 300;
        objects[0].action_quantity = 1;
    }
    
}


ALLEGRO_BITMAP *get_char_icon(int num) {
    
    switch (num) {
        case 0:
            return load_bitmap_at_size("p1icone.png", ICON_WIDTH, ICON_HEIGHT);
            break;
        case 1:
            return load_bitmap_at_size("p2icone.png", ICON_WIDTH, ICON_HEIGHT);
            break;
            
        default:
            break;
    }
 
    return NULL;
}

bool load_stage_tracks() {
    
    Stage.musica[0] = al_load_sample( "stage1.ogg" );
    loaded_stage_tracks++;
    Stage.musica[1] = al_load_sample( "stage2.ogg" );
    loaded_stage_tracks++;
    Stage.musica[2] = al_load_sample( "stage3.ogg" );
    loaded_stage_tracks++;
    Stage.musica[3] = al_load_sample( "stage4.ogg" );
    loaded_stage_tracks++;
    Stage.musica[4] = al_load_sample( "stage5.ogg" );
    loaded_stage_tracks++;
    Stage.musica[5] = al_load_sample( "stage6.ogg" );
    loaded_stage_tracks++;
    Stage.musica[6] = al_load_sample( "stage7.ogg" );
    loaded_stage_tracks++;
    Stage.musica[7] = al_load_sample( "stage8.ogg" );
    loaded_stage_tracks++;
    
    for(int x = 0; x < btracks; x++) {
        if(!Stage.musica[x]) {
            return false;
        }
    }
    
    return true;
}

bool load_battle_tracks() {
    
    Batalha.musica[0] = al_load_sample( "track01.ogg" );
    loaded_battle_tracks++;
    Batalha.musica[1] = al_load_sample( "track02.ogg" );
    loaded_battle_tracks++;
    Batalha.musica[2] = al_load_sample( "track03.ogg" );
    loaded_battle_tracks++;
    Batalha.musica[3] = al_load_sample( "track04.ogg" );
    loaded_battle_tracks++;
    Batalha.musica[4] = al_load_sample( "track05.ogg" );
    loaded_battle_tracks++;
    Batalha.musica[5] = al_load_sample( "track06.ogg" );
    loaded_battle_tracks++;
    Batalha.musica[6] = al_load_sample( "track07.ogg" );
    loaded_battle_tracks++;
    Batalha.musica[7] = al_load_sample( "track08.ogg" );
    loaded_battle_tracks++;
    
    for(int x = 0; x < btracks; x++) {
        if(!Batalha.musica[x]) {
            return false;
        }
    }
    
    return true;
}

const char *text_for_id(int txt_id) {
    
    switch (txt_id) {
        case 1:
            return "Personagem não possui nenhuma habilidade!";
            break;
        case 2:
            return "Você não possui nenhum item!";
            break;
        case 3:
            return "Não há mana suficiente para usar a habilidade!";
            break;
        case 4:
            return "Ei, cuidado! O Senac está sendo invadido por monstros!";
            break;
        case 5:
            return "Olá, vou te acompanhar nessa aventura!";
            break;
        case 6:
            return "Você não pode fugir deste monstro!";
            break;
        default:
            return "";
            break;
    }
    
}

void destroy_fx() {
    
    for(int x = 0; x < 30; x++) {
        if(fxs[x]) {
            al_destroy_bitmap(fxs[x]);
        }
    }
}

void load_fx_animation(int ani_id) {
    
    destroy_fx();
    
    switch (ani_id) {
            
            //Fire
        case 0:
            fx_count = 8;
            fxs[0] = al_load_bitmap("fogo1.png");
            fxs[1] = al_load_bitmap("fogo2.png");
            fxs[2] = al_load_bitmap("fogo3.png");
            fxs[3] = al_load_bitmap("fogo4.png");
            fxs[4] = al_load_bitmap("fogo5.png");
            fxs[5] = al_load_bitmap("fogo6.png");
            fxs[6] = al_load_bitmap("fogo7.png");
            fxs[7] = al_load_bitmap("fogo8.png");
            break;
            //Potion
        case 1:
            fx_count = 21;
            fxs[0] = al_load_bitmap("mana01.png");
            fxs[1] = al_load_bitmap("mana02.png");
            fxs[2] = al_load_bitmap("mana03.png");
            fxs[3] = al_load_bitmap("mana04.png");
            fxs[4] = al_load_bitmap("mana05.png");
            fxs[5] = al_load_bitmap("mana06.png");
            fxs[6] = al_load_bitmap("mana07.png");
            fxs[7] = al_load_bitmap("mana08.png");
            fxs[8] = al_load_bitmap("mana09.png");
            fxs[9] = al_load_bitmap("mana10.png");
            fxs[10] = al_load_bitmap("mana11.png");
            fxs[11] = al_load_bitmap("mana12.png");
            fxs[12] = al_load_bitmap("mana13.png");
            fxs[13] = al_load_bitmap("mana14.png");
            fxs[14] = al_load_bitmap("mana15.png");
            fxs[15] = al_load_bitmap("mana16.png");
            fxs[16] = al_load_bitmap("mana17.png");
            fxs[17] = al_load_bitmap("mana18.png");
            fxs[18] = al_load_bitmap("mana19.png");
            fxs[19] = al_load_bitmap("mana20.png");
            fxs[20] = al_load_bitmap("mana21.png");
            
            break;
            //Healing
        case 2:
            fx_count = 21;
            fxs[0] = al_load_bitmap("cura01.png");
            fxs[1] = al_load_bitmap("cura02.png");
            fxs[2] = al_load_bitmap("cura03.png");
            fxs[3] = al_load_bitmap("cura04.png");
            fxs[4] = al_load_bitmap("cura05.png");
            fxs[5] = al_load_bitmap("cura06.png");
            fxs[6] = al_load_bitmap("cura07.png");
            fxs[7] = al_load_bitmap("cura08.png");
            fxs[8] = al_load_bitmap("cura09.png");
            fxs[9] = al_load_bitmap("cura10.png");
            fxs[10] = al_load_bitmap("cura11.png");
            fxs[11] = al_load_bitmap("cura12.png");
            fxs[12] = al_load_bitmap("cura13.png");
            fxs[13] = al_load_bitmap("cura14.png");
            fxs[14] = al_load_bitmap("cura15.png");
            fxs[15] = al_load_bitmap("cura16.png");
            fxs[16] = al_load_bitmap("cura17.png");
            fxs[17] = al_load_bitmap("cura18.png");
            fxs[18] = al_load_bitmap("cura19.png");
            fxs[19] = al_load_bitmap("cura20.png");
            fxs[20] = al_load_bitmap("cura21.png");
            break;
            //Simple Tree attack
        case 3:
            fx_count = 8;
            fxs[0] = load_bitmap_at_size("ataquearvoreboss1.png", CHARACTER_HEIGHT, CHARACTER_HEIGHT);
            fxs[1] = load_bitmap_at_size("ataquearvoreboss2.png", CHARACTER_HEIGHT, CHARACTER_HEIGHT);
            fxs[2] = load_bitmap_at_size("ataquearvoreboss3.png", CHARACTER_HEIGHT, CHARACTER_HEIGHT);
            fxs[3] = load_bitmap_at_size("ataquearvoreboss4.png", CHARACTER_HEIGHT, CHARACTER_HEIGHT);
            fxs[4] = load_bitmap_at_size("ataquearvoreboss5.png", CHARACTER_HEIGHT, CHARACTER_HEIGHT);
            fxs[5] = load_bitmap_at_size("ataquearvoreboss6.png", CHARACTER_HEIGHT, CHARACTER_HEIGHT);
            fxs[6] = load_bitmap_at_size("ataquearvoreboss7.png", CHARACTER_HEIGHT, CHARACTER_HEIGHT);
            fxs[7] = load_bitmap_at_size("ataquearvoreboss8.png", CHARACTER_HEIGHT, CHARACTER_HEIGHT);

            break;
            //Boss Tree attack
        case 4:
            fx_count = 8;
            fxs[0] = al_load_bitmap("ataquearvoreboss1.png");
            fxs[1] = al_load_bitmap("ataquearvoreboss2.png");
            fxs[2] = al_load_bitmap("ataquearvoreboss3.png");
            fxs[3] = al_load_bitmap("ataquearvoreboss4.png");
            fxs[4] = al_load_bitmap("ataquearvoreboss5.png");
            fxs[5] = al_load_bitmap("ataquearvoreboss6.png");
            fxs[6] = al_load_bitmap("ataquearvoreboss7.png");
            fxs[7] = al_load_bitmap("ataquearvoreboss8.png");
            
            break;
        default:
            fx_count = 0;
            break;
    }
    
    for(int x = 0; x < fx_count; x ++) {
        if(!fxs[x]) {
            printf("Some error occured when loading the fxs.\n");
            return;
        }
    }
}


int monster_id_on_stage(int stage) {
    
    return stage / 2;
}

int exp_needed_for_level(int level) {
    
    return 50 + (level * 50);
}


void load_stage(int stage) {
    
    load_objects_on_stage(current_stage);
    
    if(background) {
        al_destroy_bitmap(background);
    }
    
    char string[20];
    
    sprintf(string, "stage%d.jpg", stage+1);
    
    background = al_load_bitmap(string);
    
    if(!background) {
        printf("Failed to load background");
    }
}


void set_char_values_for_level(int num, int level) {
    
    switch (num) {
        case 0:
            characters[0].id = 0;
            characters[0].level = level;
            characters[0].name[0] = ' ';
            characters[0].hp = 65 + (level * 35);
            characters[0].maxhp = 65 + (level * 35);
            characters[0].sp = 20 + (level * 2);
            characters[0].maxsp = 20 + (level * 2);
            characters[0].atk = 90 + (level * 10);
            characters[0].def = 67 + (level * 3);
            characters[0].matak = 1 + (level * 1);
            characters[0].mdef = 23 + (level * 2);
            characters[0].exp = 0;
            
            //Ataque triplo at level 10
            if(level >= 10) {
                if(characters[0].skills[0].id <= 0) {
                    add_skill_to_character(0, 001, 0);
                }
            }
            
            
            break;
        case 1:
            characters[1].id = 1;
            characters[1].level = level;
            characters[1].name[0] = ' ';
            characters[1].hp = 55 + (level * 15);
            characters[1].maxhp = 55 + (level * 15);
            characters[1].sp = 50 + (level * 20);
            characters[1].maxsp = 50 + (level * 20);
            characters[1].atk = 5 + (level * 1);
            characters[1].def = 20 + (level * 2);
            characters[1].matak = 70 + (level * 10);
            characters[1].mdef = 70 + (level * 2);
            characters[1].exp = 0;
            
            //Bola de Fogo at level 2
            if(level >= 2) {
                if(characters[1].skills[0].id <= 0) {
                    add_skill_to_character(1, 002, 0);
                }
            }
            
            //Cura at level 10
            if(level >= 10) {
                if(characters[1].skills[1].id <= 0) {
                    add_skill_to_character(1, 003, 1);
                }
            }
            break;
            
        default:
            break;
    }
    
}


//MARK: Soundtracks

void stop_tracks() {
    //al_stop_sample(Batalha.musica[current_song]);
    al_stop_samples();
}

void play_battle_track(){
    
    current_song = rand_lim(loaded_battle_tracks - 1);
    printf("song: %d\n", current_song);
    
    if(Batalha.musica[current_song])
        al_play_sample(Batalha.musica[current_song], 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    
}

void play_stage_track(){
    
    current_song = rand_lim(loaded_stage_tracks - 1);
    printf("song: %d\n", current_song);
    
    if(Stage.musica[current_song])
        al_play_sample(Stage.musica[current_song], 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    
}

//MARK: Load Musics Thread

void *t_load_tracks(ALLEGRO_THREAD *thr, void *dados) {
    
    load_stage_tracks();
    load_battle_tracks();
    
    al_destroy_thread(load_songs);
    return NULL;
}

//MARK: Inits

void set_new_interations_to_monster() {
    interations_to_monster = WALK_MONSTER_MIN + rand_lim(WALK_MONSTER_MAX - WALK_MONSTER_MIN);
}


bool load_character_animations(int num) {
    
    if(num == 0) {
        
        characters[num].animation_object.x = START_X_RIGHT;
        characters[num].animation_object.y = START_Y;
        
        characters[num].animation_object.tick = 0;
        
        characters[num].animation_object.currentFrame = 0;
        characters[num].animation_object.frameRate = 10;
        
        characters[num].animation_object.animAndar[0] = al_load_bitmap("p1andando1.png");
        characters[num].animation_object.animAndar[1] = al_load_bitmap("p1andando2.png");
        
        characters[num].animation_object.animAtaque[0] = al_load_bitmap("p1ataque1.png");
        characters[num].animation_object.animAtaque[1] = al_load_bitmap("p1ataque2.png");
        characters[num].animation_object.animAtaque[2] = al_load_bitmap("p1ataque3.png");
        
        characters[num].animation_object.animBloqueio[0] = al_load_bitmap("p1bloqueio.png");
        characters[num].animation_object.animParado[0] = al_load_bitmap("p1parado.png");
        
        characters[num].animation_object.currentAnimState = STOP;
        characters[num].animation_object.currentState = IDLE;
        
        characters[num].animation_object.invert = false;
        
        characters[num].animation_object.bAttackAnimController = false;
        characters[num].animation_object.walkSpeed = 0;
        
    }
    
    else if(num == 1) {
        
        characters[num].animation_object.x = START_X_RIGHT;
        characters[num].animation_object.y = START_Y;
        
        characters[num].animation_object.tick = 0;
        
        characters[num].animation_object.currentFrame = 0;
        characters[num].animation_object.frameRate = 10;
        
        characters[num].animation_object.animAndar[0] = al_load_bitmap("p2andando1.png");
        characters[num].animation_object.animAndar[1] = al_load_bitmap("p2andando2.png");
        
        characters[num].animation_object.animAtaque[0] = al_load_bitmap("p2ataque1.png");
        characters[num].animation_object.animAtaque[1] = al_load_bitmap("p2ataque2.png");
        characters[num].animation_object.animAtaque[2] = al_load_bitmap("p2ataque3.png");
        
        characters[num].animation_object.animBloqueio[0] = al_load_bitmap("p2bloqueio.png");
        characters[num].animation_object.animParado[0] = al_load_bitmap("p2parado.png");
        
        characters[num].animation_object.currentAnimState = STOP;
        characters[num].animation_object.currentState = IDLE;
        
        characters[num].animation_object.invert = false;
        
        characters[num].animation_object.bAttackAnimController = false;
        characters[num].animation_object.walkSpeed = 0;
        
    }
    

    return true;
    
}


bool check_string_end(char str[], int n) {
    
    for(int x = n - 1; x > 0; x--) {
        if(str[x] == '\n' || str[x] == '\0')
            return true;
    }
    
    return false;
}

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
        
        fgets(new_question->question, 500, file);
        
        if(!check_string_end(new_question->question, 500)) {
            printf("Question exceeded the character limit.");
        }
        
        fgets(new_question->answer, 300, file);
        
        if(!check_string_end(new_question->answer, 300)) {
            printf("Answer exceeded the character limit.");
        }
        
        fgets(new_question->other1, 300, file);
        
        if(!check_string_end(new_question->other1, 300)) {
            printf("Answer exceeded the character limit.");
        }
        
        //Removes \n characters
        remove_final_character(new_question->question);
        remove_final_character(new_question->answer);
        remove_final_character(new_question->other1);
        
        if(num > 2) {
            
            fgets(new_question->other2, 300, file);
            
            if(!check_string_end(new_question->other2, 300)) {
                printf("Answer exceeded the character limit.");
            }
            remove_final_character(new_question->other2);
            
            if(num > 3) {
                
                fgets(new_question->other3, 300, file);
                
                if(!check_string_end(new_question->other3, 300)) {
                    printf("Answer exceeded the character limit.");
                }
                remove_final_character(new_question->other3);
                
                if(num > 4){
                    
                    fgets(new_question->other4, 300, file);
                    
                    if(!check_string_end(new_question->other4, 300)) {
                        printf("Answer exceeded the character limit.");
                    }
                    remove_final_character(new_question->other4);
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


bool begin_allegro_init() {
    
    //Inits of allegro
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
    
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        return -1;
    }
    
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        return -1;
    }
    
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        return -1;
    }
    
    //inits of objects of allegro
    window = al_create_display(window_width, window_height);
    
    //Trade font for Windows/Linux
    //font = al_load_font("C:/Windows/Fonts/arial.ttf", 36, 0);
    font = al_load_font("/Library/Fonts/arial.ttf", 36, 0);
    question_font = al_load_font("/Library/Fonts/arial.ttf", 31, 0);
    answer_font = al_load_font("/Library/Fonts/arial.ttf", 25, 0);
    keyboard_queue = al_create_event_queue();
    timer_queue = al_create_event_queue();
    battle_timer = al_create_timer(BATTLE_TIMER);
    world_timer = al_create_timer(WORLD_TIMER);
    interface_color = al_map_rgba(113, 24, 52, 200);
    font_color = al_map_rgba(255, 255, 255, 255);
    
    
    if (!window) {
        printf("Insert error here");
        return false;
    }
    if(!question_font) {
        printf("Insert error here");
        return false;
    }
    if(!answer_font) {
        printf("Insert error here");
        return false;
    }
    if (!font) {
        printf("Insert error here");
        return false;
    }
    if (!keyboard_queue) {
        printf("Insert error here");
        return false;
    }
    if (!timer_queue) {
        printf("Insert error here");
        return false;
    }
    if (!battle_timer) {
        printf("Insert error here");
        return false;
    }
    if (!world_timer) {
        printf("Insert error here");
        return false;
    }
    
    //init bitmaps
    
    //Trade font for Windows/Linux
    //    arrow = load_bitmap_at_size("images/arrow.png", 40, 40);
    //    praca1 = al_load_bitmap("cenario/Praca1.jpg");
    //    animarvore1[0] = al_load_bitmap("monstros/arvore1.png");
    //    animarvore1[1] = al_load_bitmap("monstros/arvore2.png");
    //    animarvore1[2] = al_load_bitmap("monstros/arvore3.png");
    //    animp1[0] = al_load_bitmap("personagens/p1parado.png");
    
    //Load loading screen
    sup_screen = al_load_bitmap("Loading.jpg");
    
    if(!sup_screen) {
        return false;
    }
    al_draw_bitmap(sup_screen, 0, 0, 0);
    al_flip_display();
    
    arrow = load_bitmap_at_size("rightarrow.png", 35, 35);
    main_arrow = load_bitmap_at_size("rightarrow.png", 55, 55);
    monster_arrow = load_bitmap_at_size("downarrow.png", 80, 80);
    load_stage(current_stage);
    
    if(!main_arrow) {
        return false;
    }
    if (!arrow) {
        return false;
    }
    if (!background) {
        return false;
    }
    if(!monster_arrow) {
        return false;
    }
    
    load_songs = al_create_thread(t_load_tracks, NULL);
    al_start_thread(load_songs);
    
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_flip_display();
    
    //load questions from txt
    load_questions();
    
    //set how many interations is needed to appear the monster
    set_new_interations_to_monster();
    
    //init RPG game objects
    //main_char = (Character *) malloc(sizeof(Character));
    inventory = (Inventory *) malloc(sizeof(Inventory));
    
    //Set all items id to zero, meaning a empty inventory
    for(int x = 0; x < MAX_ITEMS; x++) {
        inventory->items[x].id = 0;
    }
    
    //Set all skills id to zero, meaning no skills
    for(int x = 0; x < MAX_SKILLS; x++) {
        for(int y = 0; y < MAX_CHARA; y++) {
            characters[y].skills[x].id = 0;
            characters[y].id = -1;
        }
    }
    
    set_char_values_for_level(0, 1);
    
    //Check animations loading
    if(!load_character_animations(0)) {
        return false;
    }
    
    return true;
}


//MARK: Deallocs


void clear_monsters() {
    
    for(int x = 0; x < MAX_MONSTERS; x++) {
        monsters[x].id = -1;
    }
    
}

void destroy_characters_animations() {
    
    for(int num = 0; num < MAX_CHARA; num++) {
        
        if(characters[num].id >= 0) {
            
            if (characters[num].animation_object.animAndar[0]) {
                al_destroy_bitmap(characters[num].animation_object.animAndar[0]);
            }
            if (characters[num].animation_object.animAndar[1]) {
                al_destroy_bitmap(characters[num].animation_object.animAndar[1]);
            }
            if (characters[num].animation_object.animAtaque[0]) {
                al_destroy_bitmap(characters[num].animation_object.animAtaque[0]);
            }
            if (characters[num].animation_object.animAtaque[1]) {
                al_destroy_bitmap(characters[num].animation_object.animAtaque[1]);
            }
            if (characters[num].animation_object.animAtaque[2]) {
                al_destroy_bitmap(characters[num].animation_object.animAtaque[2]);
            }
            if (characters[num].animation_object.animBloqueio[0]) {
                al_destroy_bitmap(characters[num].animation_object.animBloqueio[0]);
            }
            if (characters[num].animation_object.animParado[0]) {
                al_destroy_bitmap(characters[num].animation_object.animParado[0]);
            }
        }
    }
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


void destroy_objects() {
    
    if(inventory) {
        free(inventory);
    }
    
}


void destroy_allegro() {
    
    destroy_questions();
    destroy_objects();
    destroy_characters_animations();
    destroy_fx();
    
    al_destroy_bitmap(monster_arrow);
    al_destroy_bitmap(background);
    al_destroy_bitmap(arrow);
    al_destroy_bitmap(main_arrow);
    al_destroy_display(window);
    al_destroy_font(font);
    al_destroy_font(question_font);
    al_destroy_event_queue(keyboard_queue);
    al_destroy_event_queue(timer_queue);
    al_destroy_timer(battle_timer);
    al_destroy_timer(world_timer);

    exit(0);
}

//MARK: Create Objects Identifier


void create_monster_animations(int index) {
    
    switch (monsters[index].id) {
            
            //Arvore
        case 000:
            
            monsters[index].animation_object.currentMaxFrame = 3;
            monsters[index].animation_object.currentFrame = 0;
            
            monsters[index].animation_object.animations[0] = al_load_bitmap("arvore1.png");
            monsters[index].animation_object.animations[1] = al_load_bitmap("arvore2.png");
            monsters[index].animation_object.animations[2] = al_load_bitmap("arvore3.png");
            break;
            
            //Cogumelo
        case 001:
            monsters[index].animation_object.currentMaxFrame = 3;
            monsters[index].animation_object.currentFrame = 0;
            
            monsters[index].animation_object.animations[0] = al_load_bitmap("cogumelo1.png");
            monsters[index].animation_object.animations[1] = al_load_bitmap("cogumelo2.png");
            monsters[index].animation_object.animations[2] = al_load_bitmap("cogumelo3.png");
            
            //Arvore BOSS
        case 100:
            monsters[index].animation_object.currentMaxFrame = 12;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 12; x++) {
                char str[30];
                sprintf(str, "arvoreboss%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Melancia
        case 002:
            monsters[index].animation_object.currentMaxFrame = 4;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 4; x++) {
                char str[30];
                sprintf(str, "melancia%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Biscoito
        case 003:
            monsters[index].animation_object.currentMaxFrame = 8;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 8; x++) {
                char str[30];
                sprintf(str, "biscoito%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Bolo BOSS
        case 200:
            monsters[index].animation_object.currentMaxFrame = 4;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 4; x++) {
                char str[30];
                sprintf(str, "boloboss%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Caveira
        case 004:
            monsters[index].animation_object.currentMaxFrame = 4;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 4; x++) {
                char str[30];
                sprintf(str, "caveira%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Fantasma
        case 005:
            monsters[index].animation_object.currentMaxFrame = 6;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 6; x++) {
                char str[30];
                sprintf(str, "fantasma%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Caveira
        case 006:
            monsters[index].animation_object.currentMaxFrame = 4;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 4; x++) {
                char str[30];
                sprintf(str, "caveira%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Robo
        case 007:
            monsters[index].animation_object.currentMaxFrame = 6;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 6; x++) {
                char str[30];
                sprintf(str, "robo%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Robo
        case 8:
            monsters[index].animation_object.currentMaxFrame = 6;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 6; x++) {
                char str[30];
                sprintf(str, "robo%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Robo
        case 9:
            monsters[index].animation_object.currentMaxFrame = 6;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 6; x++) {
                char str[30];
                sprintf(str, "robo%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
            //Robo BOSS
        case 300:
            monsters[index].animation_object.currentMaxFrame = 6;
            monsters[index].animation_object.currentFrame = 0;
            
            for(int x = 1; x <= 6; x++) {
                char str[30];
                sprintf(str, "roboboss%d.png", x);
                monsters[index].animation_object.animations[x-1] = al_load_bitmap(str);
            }
            break;
        default:
            printf("No monster ID found.");
            break;
    }
    
    for(int x = 0; x < monsters[index].animation_object.currentMaxFrame; x++) {
        if(!monsters[index].animation_object.animations[x]) {
            printf("Failed to create allegro bitmap for monster on index %d", index);
        }
    }
    
}


void create_monster(int mon_id, int index) {
    
    switch (mon_id) {
            
            //Arvore
        case 000:
            
            monsters[index].id = 000;
            monsters[index].name[0] = 'A';
            monsters[index].name[1] = 'r';
            monsters[index].name[2] = 'v';
            monsters[index].name[3] = 'o';
            monsters[index].name[4] = 'r';
            monsters[index].name[5] = 'e';
            monsters[index].hp = 90;
            monsters[index].maxhp = 90;
            monsters[index].atk = 100;
            monsters[index].def = 40;
            monsters[index].matak = 0;
            monsters[index].mdef = 10;
            monsters[index].exp = 50;
            
            break;
            
            //Cogumelo
        case 001:
            
            monsters[index].id = 001;
            monsters[index].name[0] = 'C';
            monsters[index].name[1] = 'o';
            monsters[index].name[2] = 'g';
            monsters[index].name[3] = 'u';
            monsters[index].name[4] = 'm';
            monsters[index].name[5] = 'e';
            monsters[index].name[5] = 'l';
            monsters[index].name[5] = 'o';
            monsters[index].hp = 40;
            monsters[index].maxhp = 40;
            monsters[index].atk = 120;
            monsters[index].def = 30;
            monsters[index].matak = 0;
            monsters[index].mdef = 5;
            monsters[index].exp = 20;
            
            break;
            //Arvore BOSS
        case 100:
            monsters[index].id = 100;
            monsters[index].hp = 1000;
            monsters[index].maxhp = 1000;
            monsters[index].atk = 180;
            monsters[index].def = 60;
            monsters[index].matak = 0;
            monsters[index].mdef = 20;
            monsters[index].exp = 500;
            break;
            //Melancia
        case 002:
            monsters[index].id = 2;
            monsters[index].hp = 200;
            monsters[index].maxhp = 200;
            monsters[index].atk = 130;
            monsters[index].def = 60;
            monsters[index].matak = 0;
            monsters[index].mdef = 40;
            monsters[index].exp = 90;
            break;
            //Biscoito
        case 003:
            monsters[index].id = 3;
            monsters[index].hp = 280;
            monsters[index].maxhp = 280;
            monsters[index].atk = 0;
            monsters[index].def = 40;
            monsters[index].matak = 150;
            monsters[index].mdef = 100;
            monsters[index].exp = 120;
            break;
            //Bolo BOSS
        case 200:
            monsters[index].id = 200;
            monsters[index].hp = 2500;
            monsters[index].maxhp = 2500;
            monsters[index].atk = 0;
            monsters[index].def = 200;
            monsters[index].matak = 250;
            monsters[index].mdef = 200;
            monsters[index].exp = 1000;
            break;
            //Caveira
        case 004:
            monsters[index].id = 4;
            monsters[index].hp = 530;
            monsters[index].maxhp = 530;
            monsters[index].atk = 200;
            monsters[index].def = 110;
            monsters[index].matak = 0;
            monsters[index].mdef = 110;
            monsters[index].exp = 256;
            break;
            //Fantasma
        case 005:
            monsters[index].id = 5;
            monsters[index].hp = 280;
            monsters[index].maxhp = 695;
            monsters[index].atk = 0;
            monsters[index].def = 999;
            monsters[index].matak = 280;
            monsters[index].mdef = 50;
            monsters[index].exp = 190;
            break;
            //Caveira
        case 006:
            monsters[index].id = 6;
            monsters[index].hp = 890;
            monsters[index].maxhp = 890;
            monsters[index].atk = 250;
            monsters[index].def = 130;
            monsters[index].matak = 0;
            monsters[index].mdef = 130;
            monsters[index].exp = 314;
            break;
            //Robo
        case 007:
            monsters[index].id = 7;
            monsters[index].hp = 1240;
            monsters[index].maxhp = 1240;
            monsters[index].atk = 350;
            monsters[index].def = 200;
            monsters[index].matak = 0;
            monsters[index].mdef = 200;
            monsters[index].exp = 555;
            break;
            //Robo
        case 8:
            monsters[index].id = 8;
            monsters[index].hp = 1599;
            monsters[index].maxhp = 1599;
            monsters[index].atk = 380;
            monsters[index].def = 240;
            monsters[index].matak = 0;
            monsters[index].mdef = 240;
            monsters[index].exp = 777;
            break;
            //Robo
        case 9:
            monsters[index].id = 9;
            monsters[index].hp = 2060;
            monsters[index].maxhp = 2060;
            monsters[index].atk = 410;
            monsters[index].def = 280;
            monsters[index].matak = 0;
            monsters[index].mdef = 280;
            monsters[index].exp = 999;
            break;
            //Robo BOSS
        case 300:
            monsters[index].id = 300;
            monsters[index].hp = 10985;
            monsters[index].maxhp = 10985;
            monsters[index].atk = 700;
            monsters[index].def = 390;
            monsters[index].matak = 0;
            monsters[index].mdef = 9999;
            monsters[index].exp = 9999;
            break;
        default:
            printf("No monster ID found.");
            break;
    }
    
    
    create_monster_animations(index);
    
    switch (index) {
        case 0:
            monsters[index].animation_object.x = 950;
            monsters[index].animation_object.y = 350;
            break;
        case 1:
            monsters[index].animation_object.x = 800;
            monsters[index].animation_object.y = 300;
            break;
        case 2:
            monsters[index].animation_object.x = 650;
            monsters[index].animation_object.y = 320;
            break;
            
        default:
            break;
    }
    
}


void set_item_name(int item_index) {

    
    switch (inventory->items[item_index].id) {
            
            //VAZIO
        case 0:
            break;
            
            //Poção de HP
        case 1:
            
            sprintf(inventory->items[item_index].name, "Poção de HP\n");
            inventory->items[item_index].hp = 70;
            inventory->items[item_index].sp = 0;
            break;
            //Poção de SP
        case 2:
            
            sprintf(inventory->items[item_index].name, "Poção de SP\n");
            inventory->items[item_index].hp = 0;
            inventory->items[item_index].sp = 100;
            break;
            //Poção de HP & SP
        case 3:
            
            sprintf(inventory->items[item_index].name, "Poção Mista\n");
            inventory->items[item_index].hp = 35;
            inventory->items[item_index].sp = 50;
            break;
        case 4:
            
            sprintf(inventory->items[item_index].name, "Super Poção de HP\n");
            inventory->items[item_index].hp = 500;
            inventory->items[item_index].sp = 0;
            break;
        case 5:
            
            sprintf(inventory->items[item_index].name, "Super Poção de SP\n");
            inventory->items[item_index].hp = 0;
            inventory->items[item_index].sp = 500;
            break;
    }
    
}


bool add_item_to_inventory(int item_id) {
    
    int x, y;
    
    //Add and sort in the array of items id
    for (x = 0; x < MAX_ITEMS; x++) {
        
        if(inventory->items[x].id == 0) {
            inventory->items[x].id = item_id;
            set_item_name(x);
            
            return true;
        }
        
        else if(inventory->items[x].id > item_id && inventory->items[MAX_ITEMS - 1].id == 0) {
            
            y = MAX_ITEMS - 1;
            
            while(y > x) {
                
                inventory->items[y] = inventory->items[y - 1];
                y--;
            }
            
            inventory->items[x].id = item_id;
            set_item_name(x);
            
            return true;
            
        }
    }
    
    
    printf("%d\n", inventory->items[0].id);
    
    
    //Inventory is full!
    return false;
}


void shift_item_array_left_from_index(int index) {
    
    int x = index;
    
    while(x < MAX_ITEMS) {
        
        inventory->items[x] = inventory->items[x+1];
        x++;
    }
}


int get_item_index_from_inventory(int item_place) {
    
    int pos = 0, index = 0, val = 0, prev_val = 0;
    
    prev_val = inventory->items[index].id;
    
    while(index < MAX_ITEMS) {
        
        val = inventory->items[index].id;
        
        if(val > prev_val) {
            pos++;
            prev_val = val;
        }
        
        if(pos == item_place) {
            
            return index;
        }
        
        index++;
    }
    
    return -1;
}

void remove_item_from_inventory(int item_place) {
    
    int index = get_item_index_from_inventory(item_place);
    inventory->items[index].id = 0;
    shift_item_array_left_from_index(index);
    
}


void load_battle_background_at_stage(int stage) {
    
    if(background) {
        al_destroy_bitmap(background);
    }
    
    char str[20];
    
    sprintf(str, "stage%d.jpg", stage + 1);
    background = al_load_bitmap(str);
    
    if(!background) {
        printf("Problem loading the battle background.\n");
        destroy_allegro();
    }
    
}


void load_item_list() {
    
    int x = 0;
    int pos = 1;
    
    while(x < MAX_ITEMS) {
        
        int id = inventory->items[x].id;
        int val = id;
        int quantity = 0;
        
        //No item in this index, finished loading
        if(id == 0) {
            break;
        }
        
        //Keep checking if there is more items equal to this one
        while(val == id) {
            
            quantity++;
            x++;
            
            val = inventory->items[x].id;
        }
        
        item_id_list[pos-1] = inventory->items[x-1].id;
        
        char strnum[5];
        
        int y = 0;
        
        for(y = 0; y < 20; y++) {
            item_list[pos-1][y] = inventory->items[x-1].name[y];
            
            if(inventory->items[x-1].name[y+1] == '\n') {
                break;
            }
        }
        
        item_list[pos-1][y+1] = ' ';
        item_list[pos-1][y+2] = '(';
        
        sprintf(strnum, "%d", quantity);
        
        item_list[pos-1][y+3] = strnum[0];
        
        if(quantity > 9) {
            item_list[pos-1][y+4] = strnum[1];
            item_list[pos-1][y+5] = ')';
        }
        
        else item_list[pos-1][y+4] = ')';
        
        pos++;
    }
    
    item_quantity = pos - 1;
}


void load_skill(int chara, int index) {
    
    switch (characters[chara].skills[index].id) {
            
            //VAZIO
        case 0:
            break;
            
            //Ataque Triplo
        case 1:
            sprintf(characters[chara].skills[index].name, "Ataque Triplo");
            
            characters[chara].skills[index].atk = 1.3;
            characters[chara].skills[index].matak = 0.0;
            characters[chara].skills[index].mana_cost = 10;
            characters[chara].skills[index].fx_id = -1;
            characters[chara].skills[index].target_monster = true;
            //Bola de Fogo
        case 2:
            
            sprintf(characters[chara].skills[index].name, "Bola de Fogo");
            
            characters[chara].skills[index].atk = 0.0;
            characters[chara].skills[index].matak = 1.5;
            characters[chara].skills[index].mana_cost = 50;
            characters[chara].skills[index].fx_id = 0;
            characters[chara].skills[index].target_monster = true;
            //Cura
        case 3:
            sprintf(characters[chara].skills[index].name, "Curar");
            
            characters[chara].skills[index].atk = 0.0;
            characters[chara].skills[index].matak = 1.0;
            characters[chara].skills[index].mana_cost = 70;
            characters[chara].skills[index].fx_id = 2;
            characters[chara].skills[index].target_monster = false;
            
            break;
    }
    
}


void add_skill_to_character(int chara, int skill_id, int index) {
    
    if(index > MAX_SKILLS) {
        printf("Erro: Index passado para adicionar habilidade não existe.\n");
        return;
    }
    
    characters[chara].skills[index].id = skill_id;
    load_skill(chara, index);
    
}


void load_skill_list(int index) {
    
    int x = 0;
    int pos = 1;
    
    while(x < MAX_SKILLS) {
        
        int id = characters[index].skills[x].id;
        
        if(id == 0) {
            break;
        }
        
        char strnum[5];
        
        int y = 0;
        
        for(y = 0; y < 20; y++) {
            skill_list[pos-1][y] = characters[index].skills[x].name[y];
            
            if(characters[index].skills[x].name[y+1] == '\n') {
                break;
            }
        }
        
        skill_list[pos-1][y+1] = ' ';
        
        sprintf(strnum, "%d", characters[index].skills[x].mana_cost);
        
        skill_list[pos-1][y+2] = strnum[0];
        
        if(characters[index].skills[x].mana_cost > 9) {
            skill_list[pos-1][y+3] = strnum[1];
            
            if(characters[index].skills[x].mana_cost > 99) {
                skill_list[pos-1][y+4] = strnum[2];
                skill_list[pos-1][y+5] = 'S';
                skill_list[pos-1][y+6] = 'P';
            }
            
            else {
                skill_list[pos-1][y+4] = 'S';
                skill_list[pos-1][y+5] = 'P';
            }
            
        }
        
        else {
            skill_list[pos-1][y+3] = 'S';
            skill_list[pos-1][y+4] = 'P';
        }
        
        pos++;
        x++;
    }
    
    skill_quantity = pos - 1;
}


//MARK: Exit Screen and Saves

bool char_is_not_number(char c) {
    
    if(c == '0' ||c == '1' ||c == '2' ||c == '3' ||c == '4' ||c == '5' ||c == '6' ||c == '7' ||c == '8' ||c == '9') {
        return false;
    }
    
    else return true;
    
}

void fill_array_with_letter(char array[70]) {
    
    for(int x = 0;x < 70; x++) {
        array[x] = 'e';
    }
}

void load_saved_game() {
    
    FILE *file = fopen("save1.txt", "r");
    
    if(!file) {
        printf("No save file found.");
        fclose(file);
        return;
    }
    
    int x = 0;
    
    char read[70];
    
    //Load Items
    fill_array_with_letter(read);
    fgets(read, 70, file);
    
    while(1) {
        
        if(char_is_not_number(read[x])) {
            break;
        }
        
        int mult = 1, id = 0;
        
        for(int y = 0; y < 3; y++) {
            id += (read[x + y] - '0') * mult;
            mult = mult * 10;
        }
        
        add_item_to_inventory(id);
        x += 3;
    }
    
    //Load Stage
    x = 0;
    fill_array_with_letter(read);
    fgets(read, 70, file);
    
    int mult = 1, stage = 0;
    
    for(int y = 0; y < 3; y++) {
        stage += (read[x + y] - '0') * mult;
        mult = mult * 10;
    }
    
    current_stage = stage;
    
    //Load Character count
    fgets(read, 70, file);
    char_quantity = read[0] - '0';
    
    //Load Level
    int chara = 0;
    x = 0;
    fill_array_with_letter(read);
    fgets(read, 70, file);
    
    while(1) {
        
        if(char_is_not_number(read[x])) {
            break;
        }
        
        int mult = 1, lvl = 0;
        
        for(int y = 0; y < 3; y++) {
            lvl += (read[x + y] - '0') * mult;
            mult = mult * 10;
        }
        
        set_char_values_for_level(chara, lvl);
        chara++;
        x += 3;
    }
    
    //Load Hp
    chara = 0;
    x = 0;
    fill_array_with_letter(read);
    fgets(read, 70, file);
    
    while(1) {
        
        if(char_is_not_number(read[x])) {
            break;
        }
        
        int mult = 1, value = 0;
        
        for(int y = 0; y < 5; y++) {
            value += (read[x + y] - '0') * mult;
            mult = mult * 10;
        }
        
        characters[chara].hp = value;
        chara++;
        x += 5;
    }
    
    //Load Sp
    chara = 0;
    x = 0;
    fill_array_with_letter(read);
    fgets(read, 70, file);
    
    while(1) {
        
        if(char_is_not_number(read[x])) {
            break;
        }
        
        int mult = 1, value = 0;
        
        for(int y = 0; y < 5; y++) {
            value += (read[x + y] - '0') * mult;
            mult = mult * 10;
        }
        
        characters[chara].sp = value;
        chara++;
        x += 5;
    }
    
    //Load Exp
    chara = 0;
    x = 0;
    fill_array_with_letter(read);
    fgets(read, 70, file);
    
    while(1) {
        
        if(char_is_not_number(read[x])) {
            break;
        }
        
        int mult = 1, value = 0;
        
        for(int y = 0; y < 5; y++) {
            value += (read[x + y] - '0') * mult;
            mult = mult * 10;
        }
        
        characters[chara].exp = value;
        chara++;
        x += 5;
    }
    
    //Load Skills
    x = 0;
    fill_array_with_letter(read);
    fgets(read, 70, file);
    
    while(1) {
        
        if(char_is_not_number(read[x])) {
            break;
        }
        
        int mult = 1, value = 0;
        
        for(int y = 0; y < 3; y++) {
            value += (read[x + y] - '0') * mult;
            mult = mult * 10;
        }
        
        add_skill_to_character(read[x+3] - '0', value, read[x+4] - '0');
        x += 5;
    }
    
    fclose(file);
//    
//    add_item_to_inventory(0);
//    add_item_to_inventory(0);
//    add_item_to_inventory(0);
//    
//    current_stage = 1;
//    char_quantity = 2;
//    
//    characters[0].level = 3;
//    characters[1].level = 5;
//    characters[0].hp = 30;
//    characters[1].hp = 50;
//    characters[0].sp = 30;
//    characters[1].sp = 50;
//    characters[0].exp = 300;
//    characters[1].exp = 500;
//    add_skill_to_character(0, 1, 0);
//    add_skill_to_character(1, 1, 0);
}

void save_game() {
    
    FILE *file = fopen("save1.txt", "w");
    int x = 0, num, n;
    char newline[] = "\r\n";
    size_t size = sizeof(newline) - sizeof(char);
    
    //Saving all items
    char item[3];
    while(x < MAX_ITEMS) {
        item[0] = '0';
        item[1] = '0';
        item[2] = '0';
        
        if(inventory->items[x].id <= 0)
            break;
        
        num = inventory->items[x].id;
        n = log10(num) + 1;
        
        for (int i = 0; i < n; ++i, num /= 10 ) {
            item[i] = num % 10 + '0';
        }
        
        x++;
        
        fwrite(item, sizeof(item), 1, file);
    }
    
    fwrite(newline, size, 1, file);
    
    //Saving current stage
    char stage[3] = {'0', '0', '0'};
    num = current_stage;
    n = log10(num) + 1;
    
    for (int i = 0; i < n; ++i, num /= 10 ) {
        stage[i] = num % 10 + '0';
    }
    
    fwrite(stage, sizeof(stage), 1, file);
    fwrite(newline, size, 1, file);
    
    //Saving character count
    char charas[1];
    charas[0] = char_quantity + '0';
    fwrite(charas, sizeof(charas), 1, file);
    fwrite(newline, size, 1, file);
    
    //Saving characters level
    char level[3];
    x = 0;
    
    while(x < char_quantity) {
        level[0] = '0';
        level[1] = '0';
        level[2] = '0';
        
        num = characters[x].level;
        n = log10(num) + 1;
        
        for (int i = 0; i < n; ++i, num /= 10 ) {
            level[i] = num % 10 + '0';
        }
        
        x++;
        
        fwrite(level, sizeof(level), 1, file);
    }
    
    fwrite(newline, size, 1, file);
    
    //Saving characters HP
    char value[5];
    x = 0;
    
    while(x < char_quantity) {
        value[0] = '0';
        value[1] = '0';
        value[2] = '0';
        value[3] = '0';
        value[4] = '0';
        
        num = characters[x].hp;
        n = log10(num) + 1;
        
        for (int i = 0; i < n; ++i, num /= 10 ) {
            value[i] = num % 10 + '0';
        }
        
        x++;
        
        fwrite(value, sizeof(value), 1, file);
    }
    fwrite(newline, size, 1, file);
    
    
    //Saving characters SP
    x = 0;
    
    while(x < char_quantity) {
        value[0] = '0';
        value[1] = '0';
        value[2] = '0';
        value[3] = '0';
        value[4] = '0';
        
        num = characters[x].sp;
        n = log10(num) + 1;
        
        for (int i = 0; i < n; ++i, num /= 10 ) {
            value[i] = num % 10 + '0';
        }
        
        x++;
        
        fwrite(value, sizeof(value), 1, file);
    }
    fwrite(newline, size, 1, file);
    
    //Saving characters EXP
    x = 0;
    
    while(x < char_quantity) {
        value[0] = '0';
        value[1] = '0';
        value[2] = '0';
        value[3] = '0';
        value[4] = '0';
        
        num = characters[x].exp;
        n = log10(num) + 1;
        
        for (int i = 0; i < n; ++i, num /= 10 ) {
            value[i] = num % 10 + '0';
        }
        
        x++;
        
        fwrite(value, sizeof(value), 1, file);
    }
    fwrite(newline, size, 1, file);
    
    //Saving characters skills
    x = 0;
    
    while(x < char_quantity) {
        
        int y = 0;
        while(characters[x].skills[y].id > 0) {
            
            value[0] = '0';
            value[1] = '0';
            value[2] = '0';
            value[3] = x + '0';
            value[4] = y + '0';
            
            num = characters[x].exp;
            n = log10(num) + 1;
            
            for (int i = 0; i < n; ++i, num /= 10 ) {
                value[i] = num % 10 + '0';
            }
            
            fwrite(value, sizeof(value), 1, file);
            
            y++;
        }
        
        x++;
    }
    
    fwrite(newline, size, 1, file);
    
    fclose(file);
    
}

void show_gameover_screen() {
    
    //PLAY GAMEOVER SONG**
    game_over = true;
    
    al_clear_to_color(al_map_rgb_f(0, 0, 0));
    al_flip_display();
    al_destroy_bitmap(sup_screen);
    sup_screen = al_load_bitmap("gameover.jpg");
    al_draw_bitmap(sup_screen, 0, 0, 0);
    
    for(double x = 0.0; x < 1.0; x = x + 0.05) {
        al_clear_to_color(al_map_rgb_f(0, 0, 0));
        al_draw_tinted_bitmap(sup_screen, al_map_rgba_f(x, x, x, x), 0, 0, 0);
        al_rest(0.05);
        al_flip_display();
    }
    
    al_rest(5);
    
    stop_tracks();
}

void show_exit_screen(bool save) {
    
    double w = window_width / 3.0;
    double h = window_height / 3.0;
    double x1 = (window_width / 2.0) - (w / 2.0);
    double x2 = (window_width / 2.0) + (w / 2.0);
    double y1 = (window_height / 2.0) - (h / 2.0);
    double y2 = (window_height / 2.0) + (h / 2.0);
    
    //
    al_draw_filled_rectangle(0, 0, window_width, window_height, al_map_rgba_f(0, 0, 0, 0.5));
    
    //Text box
    al_draw_filled_rectangle(x1, y1, x2, y2, interface_color);
    
    
    al_draw_text(font, font_color, window_width / 2, y1 + 15.0, ALLEGRO_ALIGN_CENTER, "Deseja sair?");
    
    al_draw_text(font, font_color, window_width / 2, y1 + (h / 2.8), ALLEGRO_ALIGN_CENTER, "ENTER - Sim");
    al_draw_text(font, font_color, window_width / 2, y1 + (h / 1.5), ALLEGRO_ALIGN_CENTER, "ESC - Nao");
    
    al_flip_display();
    
    bool quit_while = false;
    
    al_rest(0.3);
    
    while (!quit_while) {
        
        ALLEGRO_EVENT ev;
        
        if(!al_event_queue_is_empty(keyboard_queue)) {
            
            al_get_next_event(keyboard_queue, &ev);
            
            if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (ev.keyboard.keycode) {
                        
                    case ALLEGRO_KEY_ENTER:
                        
                        if(save) {
                            save_game();
                        }
                        
                        destroy_allegro();
                        return;
                        break;
                        
                    case ALLEGRO_KEY_ESCAPE:
                        
                        quit_while = true;
                        break;
                }
            }
        }
        
        al_rest(0.01);
    }
    
}


//MARK: Drawings

void draw_background() {
    al_draw_bitmap(background, 0, 0, 0);
}


void draw_battle_menu() {
    
    //Text box
    al_draw_filled_rectangle(320, 540, 960, 700, interface_color);
    
    //Ataque normal text
    al_draw_text(font, font_color, 362.66, 550.66, ALLEGRO_ALIGN_LEFT, "Ataque normal");
    
    //Items text
    al_draw_text(font, font_color, 917.33, 550.66, ALLEGRO_ALIGN_RIGHT, "Items");
    
    //Habilidades
    al_draw_text(font, font_color, 362.66, 660, ALLEGRO_ALIGN_LEFT, "Habilidades");
    
    //Fugir
    al_draw_text(font, font_color, 917.33, 660, ALLEGRO_ALIGN_RIGHT, "Fugir");
    
}


void draw_life_bars() {
    
    //   -| 130 |- //
    // 15 |
    
    //Main character
    double y_fator = (double)characters[0].hp / characters[0].maxhp;
    
    al_draw_filled_rectangle(170, 410, 300, 425, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(172, 412, 168 + (y_fator * 130), 423, al_map_rgb(0, 255, 0));
    
    
    //Monster 1
    if(monsters[0].hp > 0) {
        
        y_fator = (double)monsters[0].hp / monsters[0].maxhp;
        
        al_draw_filled_rectangle(980, 335, 1110, 350, al_map_rgb(255, 0, 0));
        al_draw_filled_rectangle(982, 337, 978 + (y_fator * 130), 348, al_map_rgb(0, 255, 0));
    }
    
    //Monster 2
    if(monsters[1].id >= 0 && monsters[1].hp > 0) {
        
        y_fator = (double)monsters[1].hp / monsters[1].maxhp;
        
        al_draw_filled_rectangle(830, 285, 960, 300, al_map_rgb(255, 0, 0));
        al_draw_filled_rectangle(832, 287, 828 + (y_fator * 130), 298, al_map_rgb(0, 255, 0));
        
    }
    //Monster 3
    if(monsters[2].id >= 0 && monsters[2].hp > 0){
        
        y_fator = (double)monsters[2].hp / monsters[2].maxhp;
        
        al_draw_filled_rectangle(680, 310, 810, 325, al_map_rgb(255, 0, 0));
        al_draw_filled_rectangle(682, 312, 678 + (y_fator * 130), 323, al_map_rgb(0, 255, 0));
        
    }
}


void clear_string_array(char str[7][100], int lines, int chars) {
    
    for(int x = 0; x < lines; x++) {
        for(int y = 0; y < chars; y++) {
            str[x][y] = ' ';
        }
        
        str[x][79] = '\0';
    }
    
}

//Type: 0 - question
// 1 - answer 1...
//...

int get_string_array(char str[7][100], int lines, int chars, int type) {
    
    clear_string_array(str, lines, chars);
    
    int max_line = chars, index = 0, end_line = 0;
    bool end = false;
    
    
    for(int x = 0; x < lines; x++) {
        
        str[x][max_line - 2] = 'a';
        str[x][max_line - 1] = '\0';
        
        for(int y = 0; y < max_line - 2; y++) {
            
            switch (type) {
                case 0:
                    str[x][y] = current_queston->question[index];
                    break;
                case 1:
                    str[x][y] = current_queston->answer[index];
                    break;
                case 2:
                    str[x][y] = current_queston->other1[index];
                    break;
                case 3:
                    str[x][y] = current_queston->other2[index];
                    break;
                case 4:
                    str[x][y] = current_queston->other3[index];
                    break;
                case 5:
                    str[x][y] = current_queston->other4[index];
                    break;
            }
            
            index++;
            
            if(str[x][y] == '\0' || str[x][y] == '\n') {
                str[x][y] = '\0';
                end_line = x;
                end = true;
                break;
            }
        }
        
        //Ignores auto adjust
        //str[x][78] = '-';
        //str[x][79] = '\0';
        
        //Auto adjust the entire word to the next array
        for(int z = max_line - 1; z >= 0; z--) {
            if(str[x][z] == ' ') {
                
                //Word too big to go next line
                if(z < (max_line / 2) - 3) {
                    index += ((max_line - 1) - z);
                    
                    break;
                }
                
                //Clear all characters to a blank char
                while(z < max_line - 1) {
                    z++;
                    str[x][z] = ' ';
                }
                
                //Add \0 to define the end of the string and breaks FOR
                str[x][z] = '\0';
                index++;
                break;
            }
            
            //else str[x][z] = ' ';
            
            index--;
            
            //single word with all chars
            if(z == 0) {
                str[x][max_line - 2] = ' ';
                index += max_line;
            }
        }
        
        if(end) {
            break;
        }
    }
    
    return end_line;
}

void copy_string_from_to(char from[100], char to[100]) {
    
    for(int x = 0; x < 100; x++) {
        to[x] = from[x];
        if(from[x] == '\0' || from[x] == '\n')
            to[x] = ' ';
    }
    
    to[99] = '\0';
}

void draw_question_box() {
    
    double a = interface_color.a;
    interface_color.a = 1;
    
    //Draw the big box
    al_draw_filled_rectangle(20, 20, 1260, 710, interface_color);
    
    interface_color.a = a;
    
    int end_line = 0;
    
    char str[7][100];
    
    //Drawing question text
    end_line = get_string_array(str, 7, 85, 0);
    
    al_draw_justified_text(question_font, font_color, 90, 1240, 40, 1000, ALLEGRO_ALIGN_LEFT, str[0]);
    
    for(int x = 1; x <= end_line; x++) {
        al_draw_justified_text(question_font, font_color, 40, 1240, 40+ (x * 35), 1000, ALLEGRO_ALIGN_LEFT, str[x]);
    }
    
    //Drawing answers text
    int num = current_queston->ans_num;
    
    char str2[100];
    
    for(int y = 0; y < num; y++) {
        end_line = get_string_array(str, 3, 100, y + 1);
        
        copy_string_from_to(str[0], str2);
        al_draw_justified_text(answer_font, font_color, 100, 1240, questions_pos_y[y], 900, ALLEGRO_ALIGN_LEFT, str2);
        
        for(int x = 1; x <= end_line; x++) {
            copy_string_from_to(str[x], str2);
            al_draw_justified_text(answer_font, font_color, 100, 1240, questions_pos_y[y] + (25 * x), 900, ALLEGRO_ALIGN_LEFT, str2);
        }
        
        al_draw_line(90, questions_pos_y[y] - 3, 1250, questions_pos_y[y] - 3, font_color, 1.0);
    }
}



void draw_items_box() {
    
    //Text box
    al_draw_filled_rectangle(320, 540, 960, 700, interface_color);
    
    al_draw_text(font, font_color, 363, 550, ALLEGRO_ALIGN_LEFT, item_list[0]);
    
    if(item_quantity > 1) {
        al_draw_text(font, font_color, 363, 600, ALLEGRO_ALIGN_LEFT, item_list[1]);
        
        if(item_quantity > 2) {
            al_draw_text(font, font_color, 363, 650, ALLEGRO_ALIGN_LEFT, item_list[2]);
            
            if(item_quantity > 3) {
                //al_draw_text(font, font_color, 363, 700, ALLEGRO_ALIGN_LEFT, item_list[3]);
                
            }
        }
    }
}

void draw_skills_box() {
    
    //Text box
    al_draw_filled_rectangle(320, 540, 960, 700, interface_color);
    
    al_draw_text(font, font_color, 363, 550, ALLEGRO_ALIGN_LEFT, skill_list[0]);
    
    if(skill_quantity > 1) {
        al_draw_text(font, font_color, 363, 600, ALLEGRO_ALIGN_LEFT, skill_list[1]);
        
        if(skill_quantity > 2) {
            al_draw_text(font, font_color, 363, 650, ALLEGRO_ALIGN_LEFT, skill_list[2]);
            
            if(skill_quantity > 3) {
                al_draw_text(font, font_color, 363, 700, ALLEGRO_ALIGN_LEFT, skill_list[3]);
                
            }
        }
    }
    
}

void draw_world_objects() {
    
    for(int x = 0; x < objects_quantity; x++) {
        if(objects[x].action_id >= 0)
            al_draw_bitmap(objects[x].bitmap, objects[x].x1, objects[x].y1, 0);
    }
}

void draw_character_info(int chara) {
    
    //Character Icon
    if(char_icon_num != chara) {
        char_icon_num = chara;
        
        if(char_icon) {
            al_destroy_bitmap(char_icon);
        }
        
        char_icon = get_char_icon(chara);
        
        if(!char_icon) {
            printf("Error loading char icon.\n");
            char_icon_num = -1;
        }
    }
    
    if(char_icon) {
        al_draw_bitmap(char_icon, 80 - (ICON_WIDTH / 2), 80 - (ICON_HEIGHT / 2), ALLEGRO_FLIP_HORIZONTAL);
    }
    
    //Circle for chara icon
    al_draw_circle(80, 80, 70, al_map_rgba(100, 100, 100, 0.5), 10.0);
    
    
    //Life bar
    double y_fator = (double)characters[chara].hp / characters[chara].maxhp;
    
    al_draw_filled_circle(190, 40, 20, al_map_rgb(255, 0, 0));
    al_draw_filled_circle(590, 40, 20, al_map_rgb(255, 0, 0));
    
    al_draw_filled_rectangle(190, 20, 590, 60, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(192, 22, 190 + (y_fator * 398), 58, al_map_rgb(0, 255, 0));
    
    al_draw_filled_circle(190, 40, 18, al_map_rgb(0, 255, 0));
    al_draw_filled_circle(190 + (y_fator * 398), 40, 18, al_map_rgb(0, 255, 0));
    
    al_draw_textf(question_font, al_map_rgb(0, 0, 0), 390, 23, ALLEGRO_ALIGN_CENTER, "%d / %d", characters[chara].hp,characters[chara].maxhp);
    
    //Mana bar
    y_fator = (double)characters[chara].sp / characters[chara].maxsp;
    
    al_draw_filled_circle(190, 100, 20, al_map_rgb(255, 0, 0));
    al_draw_filled_circle(590, 100, 20, al_map_rgb(255, 0, 0));
    
    al_draw_filled_rectangle(190, 80, 590, 120, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(192, 82, 190 + (y_fator * 398), 118, al_map_rgb(0, 0, 255));
    
    al_draw_filled_circle(190, 100, 18, al_map_rgb(0, 0, 255));
    al_draw_filled_circle(190 + (y_fator * 398), 100, 18, al_map_rgb(0, 0, 255));

    
    al_draw_textf(question_font, al_map_rgb(255, 255, 255), 390, 83, ALLEGRO_ALIGN_CENTER, "%d / %d", characters[chara].sp,characters[chara].maxsp);
    
    y_fator = (double)characters[chara].exp / exp_needed_for_level(characters[chara].level);
    
    al_draw_filled_circle(180, 150, 10, al_map_rgb(200, 200, 0));
    al_draw_filled_circle(480, 150, 10, al_map_rgb(200, 200, 0));
    
    al_draw_filled_rectangle(180, 140, 480, 160, al_map_rgb(200, 200, 0));
    al_draw_filled_rectangle(182, 142, 180 + (y_fator * 298), 158, al_map_rgb(255, 255, 0));
    
    al_draw_filled_circle(180, 150, 8, al_map_rgb(255, 255, 0));
    al_draw_filled_circle(180 + (y_fator * 298), 150, 8, al_map_rgb(255, 255, 0));
    
    al_draw_textf(font, al_map_rgb(255, 255, 0), 25, 170, ALLEGRO_ALIGN_LEFT, "Level %d", characters[chara].level);
    
}

void draw_main_character_on_position() {
    
    draw_background();
    draw_world_objects();
    draw_character_info(0);
    
    int walk_index = characters[0].animation_object.walkIndex;
    bool invert = characters[0].animation_object.invert;
    ALLEGRO_BITMAP *bitmap;
    
    if(characters[0].animation_object.currentState == IDLE) {
        bitmap = characters[0].animation_object.animParado[0];
    }
    
    else {
        bitmap = characters[0].animation_object.animAndar[walk_index];
    }
    
    if(invert)
        al_draw_bitmap(bitmap, characters[0].animation_object.x, characters[0].animation_object.y, ALLEGRO_FLIP_HORIZONTAL);
    else al_draw_bitmap(bitmap, characters[0].animation_object.x, characters[0].animation_object.y, 0);
    
    //printf("drawing on %d, %d \n", characters[0].animation_object.x, characters[0].animation_object.y);
    
    //al_flip_display();
}


//MARK: Drawing arrows


void draw_main_menu_arrow(int selected) {
    
    double x = 420, y = 400 + (73 * selected);
    
    if(selected == 0 || selected == 1) {
        x = 420;
    }
    
    if(selected == 2) {
        x = 480;
    }
    
    else if(selected == 3) {
        x = 520;
    }
    
    al_draw_bitmap(main_arrow, x, y, 0);
    
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
    
    draw_battle_menu();
    al_draw_bitmap(arrow, x, y, 0);
}


void draw_question_arrow(int num) {
    
    double x = 30, y = 300 + (85 * num);

    al_draw_bitmap(main_arrow, x, y, 0);
    
}


void draw_arrow_character(int selected) {
    
    switch (selected) {
        case 0:
            al_draw_bitmap(monster_arrow, 200, 330, 0);
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

void draw_arrow_monster(int selected) {
    
    switch (selected) {
        case 0:
            al_draw_bitmap(monster_arrow, 1005, 245, 0);
            break;
        case 1:
            al_draw_bitmap(monster_arrow, 855, 195, 0);
            break;
        case 2:
            al_draw_bitmap(monster_arrow, 705, 220, 0);
            break;
        default:
            break;
    }
}


void draw_arrow_items(int selected) {
    
    
    draw_items_box();
    
    switch (selected) {
        case 0:
            al_draw_bitmap(arrow, 322, 550, 0);
            break;
        case 1:
            al_draw_bitmap(arrow, 322, 600, 0);
            break;
        case 2:
            al_draw_bitmap(arrow, 322, 650, 0);
            break;
        case 3:
            al_draw_bitmap(arrow, 322, 700, 0);
        default:
            break;
    }
}


void draw_arrow_skills(int selected) {
    
    draw_skills_box();
    
    switch (selected) {
        case 0:
            al_draw_bitmap(arrow, 322, 550, 0);
            break;
        case 1:
            al_draw_bitmap(arrow, 322, 600, 0);
            break;
        case 2:
            al_draw_bitmap(arrow, 322, 650, 0);
            break;
        case 3:
            al_draw_bitmap(arrow, 322, 700, 0);
        default:
            break;
    }
}

//MARK: NPCs

void show_text(int text_id, bool on_battle) {
    
    for(int x = 0; x < 255; x+=10) {
        
        if(on_battle)
            draw_background();
        else draw_main_character_on_position();
        
        al_draw_filled_rectangle(20, 570, 1260, 700, al_map_rgba(0, 0, 255, x));
        al_draw_text(font, al_map_rgba(255, 255, 255, x), 50, 580, ALLEGRO_ALIGN_LEFT, text_for_id(text_id));
        al_flip_display();
        al_rest(0.01);
    }
    
    if(on_battle)
        draw_background();
    else draw_main_character_on_position();
    
    al_draw_filled_rectangle(20, 570, 1260, 700, al_map_rgba(0, 0, 255, 255));
    al_draw_text(font, al_map_rgba(255, 255, 255, 255), 50, 580, ALLEGRO_ALIGN_LEFT, text_for_id(text_id));
    al_flip_display();
    
    while (1) {
        ALLEGRO_EVENT ev;
        
        if(!al_event_queue_is_empty(keyboard_queue)) {
            
            al_get_next_event(keyboard_queue, &ev);
            
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                
                for(int x = 255; x > 0; x-=10) {
                    
                    if(on_battle)
                        draw_background();
                    else draw_main_character_on_position();
                    
                    al_draw_filled_rectangle(20, 570, 1260, 700, al_map_rgba(0, 0, 255, 255));
                    al_draw_text(font, al_map_rgba(255, 255, 255, x), 50, 580, ALLEGRO_ALIGN_LEFT, text_for_id(text_id));
                    al_flip_display();
                    al_rest(0.01);
                }
                break;
            }
        }
        
        al_rest(0.01);
    }
}

//MARK: Animations

void fade_in() {
    
    double radius = window_width * 2;
    
    for(int x = 0; x < FADE_MAX; x++) {
        
        al_draw_circle((double)window_width / 2, (double)window_height / 2, window_width / 2, al_map_rgb(0, 0, 0), (radius / FADE_MAX) * x);
        al_flip_display();
        al_rest(0.01);
    }
}

void fade_out_for(void (*draw)() ) {
    
    double radius = window_width * 2;
    
    for(int x = FADE_MAX - 1; x >= 0; x--) {
        
        draw();
        al_draw_circle((double)window_width / 2, (double)window_height / 2, window_width / 2, al_map_rgb(0, 0, 0), (radius / FADE_MAX) * x);
        al_flip_display();
        al_rest(0.01);
    }
    
}




void draw_character_stopped(int chara) {
    
    int frame = characters[chara].animation_object.currentFrame;
    int x = characters[chara].animation_object.x;
    int y = characters[chara].animation_object.y;
    
    if(characters[chara].animation_object.invert)
        al_draw_bitmap(characters[chara].animation_object.animParado[frame], x, y, ALLEGRO_FLIP_HORIZONTAL);
    else al_draw_bitmap(characters[chara].animation_object.animParado[frame], x, y, 0);
    
}

void updateObjAnim(struct PerAnim objAnim, ALLEGRO_BITMAP *anim[]) {

    if(objAnim.invert)
        al_draw_bitmap(anim[objAnim.currentFrame], objAnim.x, objAnim.y, ALLEGRO_FLIP_HORIZONTAL);
    else al_draw_bitmap(anim[objAnim.currentFrame], objAnim.x, objAnim.y, 0);
    
}

void draw_monster(int mon) {
    
    if(monsters[mon].hp <= 0) {
        return;
    }
    
    int frame = monsters[mon].animation_object.currentFrame;
    int x = monsters[mon].animation_object.x;
    int y = monsters[mon].animation_object.y;
    
    if(monsters[mon].animation_object.invert)
        al_draw_bitmap(monsters[mon].animation_object.animations[frame], x, y, ALLEGRO_FLIP_HORIZONTAL);
    else al_draw_bitmap(monsters[mon].animation_object.animations[frame], x, y, 0);
}

void draw_all_monsters() {
    
    for(int x = 0; x < mon_quantity; x++) {
        draw_monster(x);
    }
    
}

void draw_all_characters() {
    
    for(int x = 0; x < char_quantity; x++) {
        draw_character_stopped(x);
    }
    
}

void update_animation_character(int character) {
    
    switch (characters[character].animation_object.currentAnimState) {
        case WALK:
            characters[character].animation_object.currentMaxFrame = PER1_ANIM_WALK;
            
            updateObjAnim(characters[character].animation_object, characters[character].animation_object.animAndar);
            break;
        case ATACK:
            characters[character].animation_object.currentMaxFrame = PER1_ANIM_ATACK;
            
            updateObjAnim(characters[character].animation_object, characters[character].animation_object.animAtaque);
            break;
        case BLOCK:
            characters[character].animation_object.currentMaxFrame = PER1_ANIM_BLOCK;
            
            updateObjAnim(characters[character].animation_object, characters[character].animation_object.animBloqueio);
            break;
        case STOP:
            characters[character].animation_object.currentMaxFrame = PER1_ANIM_STOP;
            
            updateObjAnim(characters[character].animation_object, characters[character].animation_object.animParado);
            break;
        default:
            break;
    }
    
    characters[character].animation_object.currentFrame++;
    
    if (characters[character].animation_object.currentFrame >= characters[character].animation_object.currentMaxFrame)
        characters[character].animation_object.currentFrame = 0;
}

void update_animations_on_monster(int mon) {
    
    if(monsters[mon].id >= 0) {
        
        //draw_monster(mon);
        
        monsters[mon].animation_object.currentFrame++;
        
        if (monsters[mon].animation_object.currentFrame >= monsters[mon].animation_object.currentMaxFrame)
            monsters[mon].animation_object.currentFrame = 0;
        
    }
    
}

void update_animations_monsters() {
    
    for(int x = 0; x < MAX_MONSTERS; x++) {
        
        if(monsters[x].id >= 0) {
            //printf("Monster %d with frame %d", x, monsters[x].animation_object.currentFrame);
            
            //draw_monster(x);
            
            monsters[x].animation_object.currentFrame++;
            
            if (monsters[x].animation_object.currentFrame >= monsters[x].animation_object.currentMaxFrame)
                monsters[x].animation_object.currentFrame = 0;
            
        }
    }
    
}


void execute_fx_animation(double seconds, int fx_id, int idx, bool monster) {
    
    load_fx_animation(fx_id);
    
    int current_frame = 0;
    
    ALLEGRO_TIMER *ani_timer = al_create_timer(seconds / (double)fx_count);
    
    al_stop_timer(battle_timer);
    al_register_event_source(timer_queue, al_get_timer_event_source(ani_timer));
    al_flush_event_queue(timer_queue);
    al_start_timer(ani_timer);
    
    
    while(1) {
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(timer_queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_TIMER) {
            
            draw_background();
            
            for(int x = 0; x < mon_quantity; x++) {
                draw_monster(x);
            }
            
            for(int x = 0; x < char_quantity; x++) {
                draw_character_stopped(x);
            }
            
            current_frame++;
            
            if(current_frame >= fx_count) {
                break;
            }
            
            if(monster) {
                al_draw_bitmap(fxs[current_frame], monsters[idx].animation_object.x, monsters[idx].animation_object.y, 0);
            }
            
            else {
                al_draw_bitmap(fxs[current_frame], characters[idx].animation_object.x, characters[idx].animation_object.y, 0);
            }
            
            al_flip_display();
        }
        
    }
    
    printf("End of FX\n");
    
    al_stop_timer(ani_timer);
    al_destroy_timer(ani_timer);
    al_start_timer(battle_timer);
}


void animate_attack_on_monster(int chara, int mon, double seconds) {
    
    //Start and end positions
    double start_x = characters[chara].animation_object.x;
    double start_y = characters[chara].animation_object.y;
    
    double end_x = monsters[mon].animation_object.x;
    double end_y = monsters[mon].animation_object.y;
    
    int interactions = (seconds * 100);
    
    double change_x = (end_x - start_x) / (double)interactions;
    double change_y = (end_y - start_y) / (double)interactions;
    
    double x = start_x, y = start_y;
    bool backing = false;
    
    //New timer for the animation
    ALLEGRO_TIMER *ani_timer = al_create_timer(0.01);
    int frame = 19, change = 20, anim_index = 0;
    
    //Stops the timer from battle so it doesnt interfere with the animation
    al_stop_timer(battle_timer);
    al_flush_event_queue(timer_queue);
    
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_timer_event_source(ani_timer));
    al_start_timer(ani_timer);
    
    while(1) {
        
        ALLEGRO_EVENT ev;
        
        al_wait_for_event(queue, &ev);
        
        if(ev.type == ALLEGRO_EVENT_TIMER) {
            draw_background();
            
            frame++;
            
            al_draw_bitmap(characters[chara].animation_object.animAndar[anim_index], x, y, ALLEGRO_FLIP_HORIZONTAL);
            draw_monster(mon);
            
            x = x + change_x;
            y = y + change_y;
            
            if(frame % change == 0) {
                anim_index++;
                
                if(anim_index >= PER1_ANIM_WALK) {
                    anim_index = 0;
                }
            }
            
            al_flip_display();
            
            if(frame >= interactions) {
                
                if(backing) {
                    break;
                }
                
                else {
                    backing = true;
                    frame = 19;
                }
                
                al_flush_event_queue(queue);
                al_pause_event_queue(queue, true);
                //al_stop_timer(ani_timer);
                
                
                int z = 0;
                
                for(z = 0; z < PER1_ANIM_ATACK; z++) {
                    draw_background();
                    al_draw_bitmap(characters[chara].animation_object.animAtaque[z], x, y, ALLEGRO_FLIP_HORIZONTAL);
                    draw_monster(mon);
                    al_flip_display();
                    al_rest(0.1);
                }
                
                z--;
                
                for( ; z >= 0; z--) {
                    draw_background();
                    al_draw_bitmap(characters[chara].animation_object.animAtaque[z], x, y, ALLEGRO_FLIP_HORIZONTAL);
                    draw_monster(mon);
                    al_flip_display();
                    al_rest(0.1);
                }
                
                change_x = -change_x;
                change_y = -change_y;
                
                if(!al_is_event_queue_empty(queue)) {
                    printf("HOOOOOWWWW???/");
                }
                
                //al_start_timer(ani_timer);
                al_pause_event_queue(queue, false);
                
            }
        }
    }
    
    //Destroy the timer and starts again the battle one
    al_destroy_event_queue(queue);
    al_destroy_timer(ani_timer);
    al_start_timer(battle_timer);
}


//MARK: Question Funx


void random_question_positions(int answer_pos, int quantity) {
    
    double y_values[quantity];
    
    int x;
    
    for(x = 0; x < quantity; x++) {
        y_values[x] = 290 + (85 * x);
    }
    
    questions_pos_y[0] = y_values[answer_pos];
    y_values[answer_pos] = 0;
    
    x = 1;
    
    while(x < quantity) {
        
        int num = rand_lim(quantity - 1);
        
        if(y_values[num] != 0) {
            questions_pos_y[x] = y_values[num];
            y_values[num] = 0;
            
            x++;
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
    
    int answer_pos;
    if(the_question) {
        
        answer_pos = rand_lim(current_queston->ans_num - 1);
        random_question_positions(answer_pos, the_question->ans_num);
        draw_question_box();
        draw_question_arrow(arrow_pos);
        al_flip_display();
        
        while(1) {
            
            ALLEGRO_EVENT ev;
            
            if(!al_event_queue_is_empty(keyboard_queue)) {
                
                al_get_next_event(keyboard_queue, &ev);
                
                
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
                    
                    draw_question_box();
                    draw_question_arrow(arrow_pos);
                    al_flush_event_queue(keyboard_queue);
                    al_flip_display();
                }
                
            }
          
            al_rest(0.02);
        }
        
    }
    
    else {
        printf("Failed to load question. Reloading from question 1...\n");
        
        if(head_question->next != NULL) {
            current_queston = head_question->next;
            
            return show_question();
        }
        
        else {
            printf("No questions found.");
            return 0;
        }
    }
    
}


//MARK: Monster Thread

void *t_monster_animations(ALLEGRO_THREAD *thr, void *dados) {
    
    while(is_on_battle) {
        
        update_animations_monsters();
        al_rest(MONSTER_ANIM_DELAY);
        
    }
    
    
    return NULL;
}

//MARK: Battle


void level_up_characters_if_needed() {
    
    for(int x = 0; x < char_quantity; x++) {
        if(characters[x].exp >= exp_needed_for_level(characters[x].level)) {
            characters[x].exp = characters[x].exp - exp_needed_for_level(characters[x].level);
            characters[x].level++;
            
            set_char_values_for_level(x, characters[x].level);
        }
    }
}


void add_exp_to_characters() {
    
    int exp = 0;
    
    for(int x = 0; x < mon_quantity; x++) {
        exp += monsters[x].exp;
    }
    
    for(int x = 0; x < char_quantity; x++) {
        characters[x].exp += exp;
    }
    
    level_up_characters_if_needed();
    
}

void position_characters_for_battle() {
    
    characters[0].animation_object.x = 200;
    characters[0].animation_object.y = 430;
    characters[0].animation_object.invert = true;
    
    if(char_quantity > 1) {
        
        characters[1].animation_object.x = 0;
        characters[1].animation_object.y = 0;
        characters[1].animation_object.invert = true;
        
        if(char_quantity > 2) {
            
            characters[2].animation_object.x = 0;
            characters[2].animation_object.y = 0;
            characters[2].animation_object.invert = true;
            
        }
    }
    
}


bool character_can_use_skill(int chara, int value) {
    
    switch (chara) {
        case 0:
            
            if((characters[chara].sp - characters[chara].skills[value].mana_cost) > 0) {
                return true;
            }
            
            else {
                return false;
            }
            
            break;
    }
    
    printf("Invalid value for character.\n");
    return false;
}


void heal_character(int chara, int value, bool is_hp) {
    
    if(value < 0) {
        printf("Invalid value for healing.\n");
        return;
    }
    
    switch (chara) {
        case 0:
            
            if(is_hp) {
                
                characters[chara].hp += value;
                
                if(characters[chara].hp > characters[chara].maxhp) {
                    characters[chara].hp = characters[chara].maxhp;
                }
            }
            
            else {
                
                characters[chara].sp += value;
                
                if(characters[chara].sp > characters[chara].maxsp) {
                    characters[chara].sp = characters[chara].maxsp;
                }
                
            }
            break;
        default:
            break;
    }
    
}


void use_item_on_character(int chara, int item_place) {
    
    execute_fx_animation(1.5, 001, chara, false);
    
    heal_character(chara, inventory->items[get_item_index_from_inventory(item_place)].hp, true);
    heal_character(chara, inventory->items[get_item_index_from_inventory(item_place)].sp, false);
    
}



void damage_monster(int monster, int value) {
    
    monsters[monster].hp -= value;
    
}


void use_skill_on_monster(int monster, int chara, int skill_idx) {
    
    execute_fx_animation(1, characters[chara].skills[skill_idx].fx_id, monster, true);
    
    if(characters[chara].skills[skill_idx].mana_cost <= characters[chara].sp) {
        
        if(characters[chara].skills[skill_idx].atk > 0) {
            monsters[monster].hp -= ((double)characters[chara].skills[skill_idx].atk * characters[chara].atk - monsters[monster].def);
            
        }
        
        if(characters[chara].skills[skill_idx].matak > 0) {
            monsters[monster].hp -= (characters[chara].skills[skill_idx].matak * characters[chara].matak - monsters[monster].mdef);
        }
        
    }
    
    characters[chara].sp -= characters[chara].skills[skill_idx].mana_cost;
    printf("%d", characters[chara].sp);
}


bool check_characters_death() {
    
    for(int x = 0; x < char_quantity; x++) {
        if(characters[x].hp > 0 && characters[x].id >= 0) {
            return false;
        }
    }
    
    return true;
}


bool check_monsters_death() {
    
    for(int x = 0; x < mon_quantity; x++) {
        if(monsters[x].hp > 0 && monsters[x].id >= 0) {
            return false;
        }
    }
    
    return true;
}


//returns -1 if the user press ESC
int battle_selection_loop(int quantity, bool monster_selection) {
    
    
    //ATACAR MONSTRO
    bool monster_selected = false;
    
    // maior valor à esquerda
    int selected = 0;
    
    while(monsters[selected].hp <= 0 && monster_selection) {
        
        selected++;
        
        if(selected >= quantity) {
            selected = 0;
            break;
        }
    }
    
    //DRAWING ALL EXCEPT THE MENU
    al_clear_to_color(al_map_rgb(255, 255, 255));
    draw_background();
    draw_all_characters();
    draw_all_monsters();
    draw_life_bars();
    if(monster_selection)
        draw_arrow_monster(selected);
    else draw_arrow_character(selected);
    al_flip_display();
    
    while(!monster_selected) {
        
        ALLEGRO_EVENT ev;
        
        if(!al_event_queue_is_empty(keyboard_queue)) {
            
            al_get_next_event(keyboard_queue, &ev);
            
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                
                switch (ev.keyboard.keycode) {
                        
                    case ALLEGRO_KEY_LEFT:
                        
                        selected++;
                        
                        if(selected >= quantity) {
                            selected = 0;
                        }
                        
                        while(monsters[selected].hp <= 0 && monster_selection) {
                            
                            selected++;
                            
                            if(selected >= quantity) {
                                selected = 0;
                                break;
                            }
                        }
                        
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        selected--;
                        
                        if(selected < 0) {
                            selected = quantity - 1;
                        }
                        
                        while(monsters[selected].hp <= 0 && monster_selection) {
                            
                            selected--;
                            
                            if(selected < 0) {
                                selected = quantity - 1;
                            }
                        }
                        
                        break;
                    case ALLEGRO_KEY_ENTER:
                        monster_selected = true;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        return -1;
                        //voltar? ele terá que responder denovo se voltar?
                        break;
                }
                
                //UPDATE VISUAL OF SELECT MONSTER ARROW
                draw_background();
                draw_all_characters();
                draw_all_monsters();
                draw_life_bars();
                
                if(monster_selection)
                    draw_arrow_monster(selected);
                else draw_arrow_character(selected);
                al_flip_display();
            }
            
        }
            
            
        else if(!al_event_queue_is_empty(timer_queue)) {
            
            al_get_next_event(timer_queue, &ev);
            
            draw_background();
            draw_all_characters();
            draw_all_monsters();
            draw_life_bars();
            
            if(monster_selection)
                draw_arrow_monster(selected);
            else draw_arrow_character(selected);
            al_flip_display();
        }
        
        al_rest(0.01);
    }
    
    return selected;
}


void draw_main_battle() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_background();
    draw_character_info(0);
    draw_arrow_with(0, 0);
    draw_all_characters();
    draw_all_monsters();
    draw_life_bars();
}

// -1 -> ERROR
// 1 -> VICTORY
// 2 -> GAME OVER
// 3 -> ESCAPE

int begin_battle(int mon_id, int mon_num) {
    
    //SEt this boolean so the thread knows when the battle ended.
    is_on_battle = true;
    
    //Thread to update monster animations
    monster_anim_thread = al_create_thread(t_monster_animations, NULL);
    
    int return_result = 0;
    bool char_invert = characters[0].animation_object.invert;
    double prev_x = characters[0].animation_object.x, prev_y = characters[0].animation_object.y;
    
    al_stop_timer(world_timer);
    al_start_timer(battle_timer);
    
    //set background for battle
    load_battle_background_at_stage(current_stage);
    
    //Set new position for the battle
    position_characters_for_battle();
    
    //clear previous monsters from the array and set quantity to global value
    clear_monsters();
    mon_quantity = mon_num;
    printf("mon quan = %d", mon_quantity);
    
    create_monster(mon_id, 0);
    
    if(mon_num >= 2) {
        
        create_monster(mon_id, 1);
        
        if (mon_num > 2) {
            
            create_monster(mon_id, 2);
        }
        
    }
    
    //values for character acting
    int character_acting = -1;
    
    //values for the selected option and enter pressed
    bool selected_x = false;
    bool selected_y = false;
    bool enter_pressed = false;
    
    bool battle_on = true;
    
    //Fade in
    fade_in();
    
    //Start song
    stop_tracks();
    play_battle_track();
    
    //Fade out
    fade_out_for(draw_main_battle);
    
    //Start thread
    al_start_thread(monster_anim_thread);
    
    draw_arrow_with(selected_x, selected_y);
    
    //MARK: Main loop battle
    while(battle_on) {
        
        character_acting++;
        
        if(character_acting >= char_quantity) {
            character_acting = 0;
        }
        
        //Wait for command
        while (!enter_pressed) {
            
            ALLEGRO_EVENT ev;
            
            if(!al_event_queue_is_empty(keyboard_queue)) {
                al_get_next_event(keyboard_queue, &ev);
                
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
                            show_exit_screen(true);
                            break;
                    }
                    
                    //printf("Flip key\n");
                    //al_flip_display();
                }
            }
            
            else if(!al_event_queue_is_empty(timer_queue)) {
                al_get_next_event(timer_queue, &ev);
                al_flush_event_queue(timer_queue);
                
                if (ev.type == ALLEGRO_EVENT_TIMER) {
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    draw_background();
                    draw_character_info(character_acting);
                    draw_arrow_with(selected_x, selected_y);
                    update_animation_character(character_acting);
                    draw_all_monsters();
                    //update_animations_monsters();
                    draw_life_bars();
                    //printf("Flip timer\n");
                    al_flip_display();
                }
            }
            
            al_rest(0.01);
        }
        
        //BEGIN QUESTION
        int result = show_question();
        
        
        if(result == 0) {
            return -1;
            //sem perguntas! END ALLEGRO??
        }
        
        //MARK:Answer right: execute action
        else if(result == 1) {
            //EXECUTAR AÇÃO
            //MARK:Action: simple attack
            if(!selected_x && !selected_y) {
                
                int selected = battle_selection_loop(mon_num, true);
                
                if(selected >=0) {
                    
                    animate_attack_on_monster(character_acting, selected, 1.5);
                    
                    if(characters[character_acting].atk - monsters[selected].def > 0)
                        monsters[selected].hp -= characters[character_acting].atk - monsters[selected].def;
                    
                    draw_life_bars();
                }
            }
            
            //MARK:Action: use item
            else if(selected_x && !selected_y) {
                //USAR ITEM
                
                if(inventory->items[0].id == 0) {
                    //NENHUM ITEM NO INVENTARIO
                    printf("No items\n.");
                    show_text(2, true);
                }
                
                else {
                    
                    load_item_list();
                    
                    bool item_selected = false, cancell = false;
                    int selected = 0;
                    
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    draw_background();
                    draw_all_characters();
                    draw_all_monsters();
                    draw_life_bars();
                    draw_character_info(character_acting);
                    draw_arrow_items(selected);
                    al_flip_display();
                    
                    //Wait for selected item
                    while(!item_selected && !cancell) {
                        
                        ALLEGRO_EVENT ev;
                        if(!al_event_queue_is_empty(keyboard_queue)) {
                            
                            al_get_next_event(keyboard_queue, &ev);
                            
                            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                                
                                switch (ev.keyboard.keycode) {
                                        
                                    case ALLEGRO_KEY_UP:
                                        selected--;
                                        
                                        if(selected < 0) {
                                            selected = item_quantity - 1;
                                        }
                                        break;
                                    case ALLEGRO_KEY_DOWN:
                                        selected++;
                                        
                                        if(selected >= item_quantity) {
                                            selected = 0;
                                        }
                                        break;
                                    case ALLEGRO_KEY_ENTER:
                                        item_selected = true;
                                        break;
                                    case ALLEGRO_KEY_ESCAPE:
                                        cancell = true;
                                        //voltar? ele terá que responder denovo se voltar?
                                        break;
                                }
                                
                                //UPDATE VISUAL OF SELECT ITEM ARROW
                                draw_background();
                                draw_all_characters();
                                draw_all_monsters();
                                draw_life_bars();
                                draw_character_info(character_acting);
                                draw_arrow_items(selected);
                                al_flip_display();
                            }
                        }
                    }
                    
                    if(!cancell) {
                        
                        //get selected character
                        int char_sel = battle_selection_loop(mon_num, false);
                        
                        if(char_sel >= 0) {
                            
                            //Removes the item from inventory
                            remove_item_from_inventory(selected);
                            
                            //Uses item on char
                            use_item_on_character(char_sel, selected);
                            
                        }
                    }
                }
                
            }
            
            //MARK:Action: use skill
            else if(!selected_x && selected_y) {
                //USAR HABILIDADE
                
                if(characters[character_acting].skills[0].id == 0) {
                    //NENHUMA SKILL DISPONIVEL
                    printf("No skills\n.");
                    show_text(1, true);
                }
                
                else {
                    
                    load_skill_list(character_acting);
                    
                    bool skill_selected = false, cancell = false;
                    int selected = 0;
                    
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    draw_background();
                    draw_all_characters();
                    draw_all_monsters();
                    draw_life_bars();
                    draw_character_info(character_acting);
                    draw_arrow_skills(selected);
                    al_flip_display();
                    
                    //Wait for selected skill
                    while(!skill_selected && !cancell) {
                        
                        ALLEGRO_EVENT ev;
                        if(!al_event_queue_is_empty(keyboard_queue)) {
                            
                            al_get_next_event(keyboard_queue, &ev);
                            
                            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                                
                                switch (ev.keyboard.keycode) {
                                        
                                    case ALLEGRO_KEY_UP:
                                        selected--;
                                        
                                        if(selected < 0) {
                                            selected = skill_quantity - 1;
                                        }
                                        break;
                                    case ALLEGRO_KEY_DOWN:
                                        selected++;
                                        
                                        if(selected >= skill_quantity) {
                                            selected = 0;
                                        }
                                        break;
                                    case ALLEGRO_KEY_ENTER:
                                        if(character_can_use_skill(character_acting, selected))
                                            skill_selected = true;
                                        else {
                                            printf("%d", characters[character_acting].sp);
                                            printf("Not enough mana to use skill!\n");
                                            
                                            show_text(3, true);
                                            //No mana for skill!
                                        }
                                        
                                        break;
                                    case ALLEGRO_KEY_ESCAPE:
                                        cancell = true;
                                        //voltar? ele terá que responder denovo se voltar?
                                        break;
                                }
                                
                                //UPDATE VISUAL OF SELECT ITEM ARROW
                                draw_background();
                                draw_all_characters();
                                draw_all_monsters();
                                draw_life_bars();
                                draw_character_info(character_acting);
                                draw_arrow_skills(selected);
                                al_flip_display();
                            }
                        }
                    }
                    
                    if(!cancell) {
                        
                        int target;
                        
                        //Skill used on monster
                        if(characters[character_acting].skills[selected].target_monster) {
                            target = battle_selection_loop(mon_num, true);
                            use_skill_on_monster(target, character_acting, selected);
                        }
                        
                        //Skill used on character
                        else  {
                            target = battle_selection_loop(mon_num, false);
                            //MARK: NEED FUNCTION FOR HEALING
                            //use_skill_on_character(target, character_acting, selected);
                        }
                        
                    }
                    
                }
            }
            
            //MARK:Action: escape fight
            else if(selected_x && selected_y) {
                
                if(!boss_fight) {
                    
                    //FUGIR
                    battle_on = false;
                    return_result = 3;
                }
                
                else {
                    show_text(6, true);
                }
            }
        }
        
        //MARK:Answer wrong: suffer attack
        else if(result == 2) {
            //SOFRER ATAQUE
            if(boss_fight)
                execute_fx_animation(1.5, 004, rand_lim(char_quantity - 1), false);
            else execute_fx_animation(1.5, 003, rand_lim(char_quantity - 1), false);
            
            if(monsters[0].atk > monsters[0].matak) {
                if(monsters[0].atk - characters[character_acting].def > 0) {
                    characters[character_acting].hp -= (monsters[0].atk - characters[character_acting].def);
                }
            }
            
            else {
                if(monsters[0].matak - characters[character_acting].mdef > 0) {
                    characters[character_acting].hp -= (monsters[0].matak - characters[character_acting].mdef);
                }
            }
            
        }
        
        //MARK:Cancell action
        else if(result == 3) {
            //IGNORAR
        }
        
        enter_pressed = false;
        
        
        //CHECK BATTLE STATUS
        //MARK:Checking battle status
        if(check_characters_death()) {
            //GAME OVER!!
            battle_on = false;
            return_result = 2;
        }
        
        else if(check_monsters_death()) {
            //BATTLE WON!
            add_exp_to_characters();
            battle_on = false;
            return_result = 1;
        }
    }
    
    //Stops the thread
    is_on_battle = false;
    
    characters[0].animation_object.invert = char_invert;
    
    printf("Ending battle");
    
    al_flush_event_queue(timer_queue);
    draw_main_battle();
    al_flip_display();
    
    //Stops battle timer and restart world timer
    al_stop_timer(battle_timer);
    al_start_timer(world_timer);
    
    //Fade in
    double radius = window_width * 2;
    
    for(int x = 0; x < FADE_MAX; x++) {
        
        draw_background();
        al_draw_circle((double)window_width / 2, (double)window_height / 2, window_width / 2, al_map_rgb(0, 0, 0), (radius / FADE_MAX) * x);
        al_flip_display();
        al_rest(0.01);
    }
    
    //Stop battle music
    stop_tracks();
    play_stage_track();
    
    //In case it was a boss fight
    boss_fight = false;
    
    //Set original position of character before the battle
    characters[0].animation_object.x = prev_x;
    characters[0].animation_object.y = prev_y;
    
    //Destroy thread
    al_destroy_thread(monster_anim_thread);
    
    //battle return result
    return return_result;
}


//MARK: Outside


void execute_action_for_object(int obj) {
    
    int battle_result;
    
    switch (objects[obj].object_type) {
        case NPC:
            show_text(objects[obj].action_id, false);
            break;
        case CHEST:
            add_item_to_inventory(objects[obj].action_id);
            
            //CHANGE CHEST BITMAP TO OPEN AND DEACTIVATES ITEM!
            
            break;
        case MONSTER:
            
            if(objects[obj].action_id > 99) {
                boss_fight = true;
            }
            
            battle_result = begin_battle(objects[obj].action_id, objects[obj].action_quantity);
            
            if(battle_result == 2) {
                show_gameover_screen();
            }
            
            else if(battle_result == 1 || battle_result == 3) {
                //add_exp_to_characters();
                boss_defeated[(objects[obj].action_id / 100) - 1] = true;
                clear_all_objects();
                fade_out_for(draw_main_character_on_position);
            }
            
            break;
        default:
            break;
    }
    
}

int check_object_at_area(int x1,int x2,int y1,int y2) {
    
    for(int z = 0; z < objects_quantity; z++) {
        
        int ox1 = objects[z].x1;
        int ox2 = objects[z].x2;
        int oy1 = objects[z].y1;
        int oy2 = objects[z].y2;
        
        if(ox1 > x1 && ox1 < x2) {
            if(oy1 > y1 && oy1 < y2) {
                return z;
            }
            
            else if(oy2 > y1 && oy2 < y2) {
                return z;
            }
        }
        
        else if (ox2 > x1 && ox2 < x2) {
            if(oy1 > y1 && oy1 < y2) {
                return z;
            }
            
            else if(oy2 > y1 && oy2 < y2) {
                return z;
            }
        }
        
    }
    
    return -1;
    
}

void check_character_interaction() {
    
    int x1, x2, y1, y2;
    
    y1 = characters[0].animation_object.y - ((CHAR_INTERACTION_SIZE - CHARACTER_HEIGHT) / 2);
    y2 = y1 + CHAR_INTERACTION_SIZE;
    
    //Character is looking right
    if(characters[0].animation_object.invert) {
        
        x1 = characters[0].animation_object.x + CHARACTER_WIDTH;
        x2 = x1 + CHAR_INTERACTION_SIZE;
    }
    
    //Character is looking left
    else {
        x2 = characters[0].animation_object.x;
        x1 = x2 - CHAR_INTERACTION_SIZE;
    }
    
    int index = check_object_at_area(x1,x2,y1,y2);
    
    if(index < 0) {
        //No objects on area
        printf("No objects in the area.\n");
    }
    
    else {
        execute_action_for_object(index);
    }
    
}

//Returns 1 to advance stage
//Returns -1 to regress stage
//Returns 2 to reset keys after battle
//Returns 3 for gameover
//Returns 0 for nothing
int walk_character(bool up, bool down, bool left, bool right) {
    
    //Move up
    if(up && !down) {
        printf("%f\n", (WALK_SPEED_Y / (1 / WORLD_TIMER)));
        characters[0].animation_object.y -= (WALK_SPEED_Y / (1 / WORLD_TIMER));
        
        //Max y to go up
        if(characters[0].animation_object.y < MAX_WALK_Y) {
            characters[0].animation_object.y = MAX_WALK_Y;
        }
    }
    
    //Move down
    else if(!up && down) {
        characters[0].animation_object.y += (WALK_SPEED_Y / (1 / WORLD_TIMER));
        
        //Leaving the screen on bottom
        if(characters[0].animation_object.y + CHARACTER_HEIGHT > window_height) {
            characters[0].animation_object.y = window_height - CHARACTER_HEIGHT;
        }
    }
    
    //Turn the character to the left
    if(left && !right) {
        characters[0].animation_object.x -= (WALK_SPEED_X / (1 / WORLD_TIMER));
        characters[0].animation_object.invert = false;
        
        //Advance stage
        if(characters[0].animation_object.x <= 1) {
            characters[0].animation_object.x = 1;
            return 1;
        }
        
    }
    
    //Turn the character to the right
    else if(!left && right) {
        characters[0].animation_object.x += (WALK_SPEED_X / (1 / WORLD_TIMER));
        characters[0].animation_object.invert = true;
        
        //Return to previous stage
        if(characters[0].animation_object.x + CHARACTER_WIDTH >= window_width) {
            characters[0].animation_object.x = window_width - CHARACTER_WIDTH;
            return -1;
        }
    }
    
    //No movement, meaning no walk animation
    if(!up && !down && !left && !right) {
        characters[0].animation_object.currentState = IDLE;
        characters[0].animation_object.walkCount = 0;
        characters[0].animation_object.walkIndex = 0;
    }
    
    //Movement, update animation walk count and count to monster
    else {
        characters[0].animation_object.currentState = WALKING;
        characters[0].animation_object.walkCount++;
        
        //If the option of monsters appear on walk is on
        if(monsters_on_walk) {
            walk_interations++;
            
            //Event of monster appearing
            if(walk_interations >= interations_to_monster) {
                walk_interations = 0;
                set_new_interations_to_monster();
                
                int num = rand_lim(MAX_MONSTERS);
                if(num == 0)
                    num = 1;
                
                int result = begin_battle(monster_id_on_stage(current_stage), num);
                
                if(result == 2) {
                    show_gameover_screen();
                    return 3;
                }
                
                fade_out_for(draw_main_character_on_position);
                
                return 2;
            }
        }
    }
    
    //Change animation status for walknig
    if(characters[0].animation_object.walkCount >= STEPS_WALK_ANIM) {
        characters[0].animation_object.walkCount = 0;
        characters[0].animation_object.walkIndex++;
        
        if(characters[0].animation_object.walkIndex >= PER1_ANIM_WALK) {
            characters[0].animation_object.walkIndex = 0;
        }
    }
    
    //Perform the drawing
    draw_main_character_on_position();
    draw_world_objects();
    al_flip_display();
    
    return 0;
}

void scenario() {
    
    play_stage_track();
    
    load_objects_on_stage(current_stage);
    load_stage(current_stage);
    
    //Initial animation
    fade_out_for(draw_main_character_on_position);
    
    //Start timer for world
    al_start_timer(world_timer);
    
    bool up = false, down = false, left = false, right = false;
    
    while (!game_over) {
        
        ALLEGRO_EVENT ev;
        
        if(!al_event_queue_is_empty(keyboard_queue)) {
            al_get_next_event(keyboard_queue, &ev);
            
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                
                switch (ev.keyboard.keycode) {
                        
                    case ALLEGRO_KEY_UP:
                        up = true;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        down = true;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        left = true;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        right = true;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        check_character_interaction();
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        show_exit_screen(true);
                        break;
                }
                
            }
            
            else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
                
                switch (ev.keyboard.keycode) {
                        
                    case ALLEGRO_KEY_UP:
                        up = false;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        down = false;
                        break;
                    case ALLEGRO_KEY_LEFT:
                        left = false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        right = false;
                        break;
                }
                
            }
        }
        
        else if(!al_event_queue_is_empty(timer_queue)) {
            al_get_next_event(timer_queue, &ev);
            
            if (ev.type == ALLEGRO_EVENT_TIMER) {
                int action = walk_character(up, down, left, right);
                
                //Reset keys after battle
                if(action == 2) {
                    up = false;
                    down = false;
                    left = false;
                    right = false;
                }
                
                else if(action == 1) {
                    if(current_stage + 1 < MAX_STAGES) {
                        current_stage++;
                        characters[0].animation_object.x = START_X_RIGHT;
                        characters[0].animation_object.y = START_Y;
                        load_stage(current_stage);
                        
                        //INSERT TRANSICTION HERE!
                    }
                    //ENDING OF THE GAME
                    else {
                        
                    }
                }
                
                else if(action == -1) {
                    if(current_stage - 1 >= 0) {
                        current_stage--;
                        characters[0].animation_object.x = START_X_LEFT;
                        characters[0].animation_object.y = START_Y;
                        load_stage(current_stage);
                        
                        //INSERT TRANSICTION HERE!
                    }
                    
                    //BEGINNING OF THE GAME
                    else {
                        
                    }
                }
                
                //GAME OVER
                else if(action == 3) {
                    return;
                }
            }
        }
        
        //Rest to not force CPU with infinite processes
        al_rest(0.01);
    }
    
}

//MARK: Main menu


void fade_in_main_menu() {
    
    for(double f = 1.0; f > 0.0; f = f - 0.05) {
        
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(sup_screen, al_map_rgba_f(f, f, f, f), 0, 0, 0);
        al_rest(0.05);
        al_flip_display();
    }
    
    al_rest(0.03);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();
    al_rest(0.5);
}


// 0 - NEW GAME
// 1 - CONTINUE
// 2 - OPTIONS
// 3 - EXIT

int show_main_menu() {
    
    int selected = 0, max = 4;
    
    for(double f = 1.0; f > 0.0; f = f - 0.05) {
        
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(sup_screen, al_map_rgba_f(f, f, f, f), 0, 0, 0);
        al_rest(0.02);
        al_flip_display();
    }
    
    al_destroy_bitmap(sup_screen);
    sup_screen = al_load_bitmap("startscreen.jpg");
    
    if(!sup_screen) {
        printf("Failed to load main menu bitmap.");
        return 3;
    }
    
    for(double f = .0; f < 1.0; f = f + 0.05) {
        
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_tinted_bitmap(sup_screen, al_map_rgba_f(f, f, f, f), 0, 0, 0);
        al_rest(0.02);
        al_flip_display();
    }
    
    al_rest(0.05);
    al_draw_bitmap(sup_screen, 0, 0, 0);
    draw_main_menu_arrow(selected);
    al_flip_display();
    
    while(1) {
        
        if(!al_event_queue_is_empty(keyboard_queue)) {
           
            ALLEGRO_EVENT ev;
            al_get_next_event(keyboard_queue, &ev);
            
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                
                switch (ev.keyboard.keycode) {
                        
                    case ALLEGRO_KEY_UP:
                        selected--;
                        
                        if(selected < 0)
                            selected = max - 1;
                        
                        break;
                    case ALLEGRO_KEY_DOWN:
                        selected++;
                        
                        if(selected >= max)
                            selected = 0;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        
                        return selected;
                        break;
                    case ALLEGRO_KEY_ESCAPE:
                        show_exit_screen(false);
                        break;
                }
                
                al_draw_bitmap(sup_screen, 0, 0, 0);
                draw_main_menu_arrow(selected);
                al_flip_display();
            }
            
        }
        
        al_rest(0.01);
    }
    
    
}



//MARK: Main


//Comentado para trabalhar fora do Mac OS
int main(int argc, char **argv) {
    
    //Comentado para trabalhar no Mac OS
    //int main(void) {
    
    // Init
    if (!begin_allegro_init()) {
        return -1;
    }
    
    //Register event sources
    al_register_event_source(timer_queue, al_get_timer_event_source(battle_timer));
    al_register_event_source(timer_queue, al_get_timer_event_source(world_timer));
    al_register_event_source(keyboard_queue, al_get_keyboard_event_source());
    
    
    // Main loop
    while(1) {
        
        //Show main screen and wait for selection
        int res = show_main_menu();
        
        //New game
        if(res == 0) {
            fade_in_main_menu();
            scenario();
        }
        
        //Continue game
        else if(res == 1) {
            fade_in_main_menu();
            load_saved_game();
            scenario();
        }
        
        //Options
        else if(res == 2) {
            //OPTIONS
        }
        
        //Exit
        else if(res == 3) {
            fade_in_main_menu();
            destroy_allegro();
            return 0;
        }
        
    }
    return 0;
    
}