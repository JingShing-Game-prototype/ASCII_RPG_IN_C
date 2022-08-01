#include"RPG_Funcs.h"

int main(){
    int direction=0,confirm=0,i,is_playing=1;
    srand((unsigned int)time(NULL));
    system("chcp 65001 & cls");
    setGameTitle(GAME_TITLE);
    title_screen();
    playMusic(BGM, BGM_MODE);
    if(read_game()==0){
        printf("Pleaze enter the one character as your player: ");
        scanf(" %c",&player_game.skin);
        playMusic(SYSTEM_OK,SE_MODE);
        initPlayer(player_game.skin,map_id,1,1,10,10,0,10,10,0,0,10);
        init_map();
        rand_spawn(player_game.id);
        for(i=0;i<10;i++){
            if (i%3==0){
                initNpc(i,map_id,0,0,0,10,10,1,0);
                rand_spawn(npc_game[i].id);
            }
         }
         for(i=0;i<player_inventory_limits;i++){
            item_give(player_game.id,0,i);
         }
    }
    player_game.can_move=1;player_game.dead=0;
    print_screen();
    while(is_playing>0){
        act_select();
    }
}
