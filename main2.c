#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#define tracks 8
void playlist(struct musicas);     
struct musicas{
   ALLEGRO_SAMPLE *musica[tracks];
}; 
int main(){

   ALLEGRO_DISPLAY *display = NULL;

   if(!al_init()){
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
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
   
   playlist1.musica[0] = al_load_sample( "musicas/batalha/track01.ogg" );
   playlist1.musica[1] = al_load_sample( "musicas/batalha/track02.ogg" );
   playlist1.musica[2] = al_load_sample( "musicas/batalha/track03.ogg" );
   playlist1.musica[3] = al_load_sample( "musicas/batalha/track04.ogg" );
   playlist1.musica[4] = al_load_sample( "musicas/batalha/track05.ogg" );
   playlist1.musica[5] = al_load_sample( "musicas/batalha/track06.ogg" );
   playlist1.musica[6] = al_load_sample( "musicas/batalha/track07.ogg" );
   playlist1.musica[7] = al_load_sample( "musicas/batalha/track08.ogg" );
  

   
 
 
   display = al_create_display(640, 480);
 
   if(!display){  
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }
 
   /* Loop the sample until the display closes. */


   int y = 0;
   while(!y){

   scanf("%d",&y);
   playlist(musicas);

 
   }

   
 
   al_destroy_display(display);
   int i;
   for(i =0;i<8;i++){
   al_destroy_sample(musicas[i]);
   }
   
   return 0;
}

void playlist(struct musicas){
      int i = rand() % 7;
      al_play_sample(musicas[i], 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

}