#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h> 
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>
#include <stdlib.h>


#define tracks 8
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer;

struct Playlist{
   ALLEGRO_SAMPLE *musica[tracks];
};
void play(struct Playlist *playlist){
      time_t t;
      srand((unsigned)time(&t));

      int i;
      i = rand() % 7;
      al_play_sample(playlist->musica[i], 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

}
int iniciarjogo()
{
   if(!al_init()){
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
   al_init_primitives_addon();
   al_install_keyboard();
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
   event_queue = al_create_event_queue();
   if(!event_queue){
      printf("moios o event queue");
      return -1;
   }
   timer = al_create_timer(1.0/60);

   return 1;
}

int main(){

   iniciarjogo();

   struct Playlist Batalha;
   
   Batalha.musica[0] = al_load_sample( "musicas/batalha/track01.ogg" );
   Batalha.musica[1] = al_load_sample( "musicas/batalha/track02.ogg" );
   Batalha.musica[2] = al_load_sample( "musicas/batalha/track03.ogg" );
   Batalha.musica[3] = al_load_sample( "musicas/batalha/track04.ogg" );
   Batalha.musica[4] = al_load_sample( "musicas/batalha/track05.ogg" );
   Batalha.musica[5] = al_load_sample( "musicas/batalha/track06.ogg" );
   Batalha.musica[6] = al_load_sample( "musicas/batalha/track07.ogg" );
   Batalha.musica[7] = al_load_sample( "musicas/batalha/track08.ogg" );
 
   display = al_create_display(640, 480);
 
   if(!display){  
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }
 
   /* Loop the sample until the display closes. */


   bool sair = false;

   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_keyboard_event_source());
   al_start_timer(timer);
   play(&Batalha);

   while(!sair){
   ALLEGRO_EVENT ev;
   al_wait_for_event(event_queue,&ev);
   

      if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
         switch(ev.keyboard.keycode)
         {
            case ALLEGRO_KEY_ESCAPE:
            sair = true;
            break;
         }

     }
     else if(ev.type == ALLEGRO_EVENT_TIMER){
      
     }
   }

   
 
   al_destroy_display(display);
   int i;
   for(i =0;i<8;i++){
   al_destroy_sample(Batalha.musica[i]);
   }
   
   return 0;
}
