// header include
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<time.h>
#include <Windows.h>

// all const
#define map_size_x 16
#define map_size_y 32
#define Hunger_limits 100
#define player_inventory_limits 8
#define NPC_amount 10
#define Monster_amount 10

// map related define
#define TEST_MAP "maps/test_map.txt"
#define MAZE_MAP "maps/maze_map.txt"

//music relate define
#define BGM "musics/bgm.wav"
#define SYSTEM_OK "musics/systemOK.wav"
#define UNLOCK "musics/unlock.wav"
#define VICTORY "musics/victory.wav"
#define MUSIC_STOP NULL
#define BGM_MODE 0
#define SE_MODE 1
#define GAME_TITLE "The Adventure of ASCII"

int map[map_size_x][map_size_y] = {0};//id = type * 10 + type_id
char title[map_size_x][map_size_y] = {0};
int map_id=0;
void title_screen();

int statement_info[7]={1,1,0,1,1,1,0};//1 basic, 2 item select, 3 map, 4 act mode, 5 money, 6 turn, 7 position

int item_select=0;
char *item_name[50]={"Null","HP Potion","Mp potion","Food","Sword","Shield"};//1 hp potion, 2 mp potion,3 food , 4 sword, 5 shield

int magic_choose=0;
char *magic_name[50]={"Fire Ball"};

char *map_name[3]={"Test_map","Maze"};
char *act_mode_name[5]={"None","Walk","Talk","Attack","Magic"};
char *mapPath[5]={"maps/test_map.txt","maps/maze_map.txt"};

char block[10]={' ','#','+','=','|','E','?','%'};//0 none, 1 block, 2 door, 3 roof&floor, 4 pillar, 5 Exit, 6 quest_mark, 7 fire sball
char *NPCs_name[26]={"Abor","Burnt","Casse","Deva","Evenu","Fulisuq","Galise","Holiken","Illie","Juxin","Knought","Louey","Monika","Noile","Olivu","Purver","Qouider","Roghtir","Shger","Touile","Uyer","Vetaz","Wiuyte","Xodu","Yuui","Zoizon"};
typedef struct player
{
    char skin;
    int x,y;
    int hp,max_hp;
    int mp,max_mp;
    int hunger,attack,shield;
    int addition_attack,addition_shield;
    int addition_max_hp,addition_max_mp;
    int money;
    int id;
    int can_move;
    int dead;
    int map_id;
    int inventory[player_inventory_limits];
}Player;
Player player_game;
void initPlayer(char skin,int map_id, int x, int y, int hp, int max_hp, int mp, int max_mp, int hunger, int attack, int shield,int money);

typedef struct NPC
{
    char skin;
    int x,y;
    int hp,max_hp;
    int attack,shield;
    int id;
    int map_id;
}NPC;
NPC npc_game[NPC_amount];
void initNpc(int id, char skin,int map_id, int x, int y, int hp, int max_hp,int attack, int shield);

typedef struct Monster
{
    char skin;
    int x,y;
    int hp,max_hp;
    int attack,shield;
    int id;
    int map_id;
}Monster;
Monster monster_game[Monster_amount];
void initMonster(int id, char skin,int map_id, int x, int y, int hp, int max_hp,int attack, int shield);

int movingX[4] = {-1, 1, 0, 0};
int movingY[4] = {0, 0, -1, 1};
int turnpc_game[1]={0};

void turn_a_round();
// player survive
void hunger_system();
void hp_detect();
void attack_detect();
void shield_detect();
void player_behave();

void item_give(int id, int item_id, int slot);
void item_select_next();
void item_use();

// map & outlook related
void init_map();
void clear_map_board();
void print_screen();
void print_map();
void print_statement();
void state_info_shown();
void skin_change(int id);

void map_creature_clear();
void teleport_map(int id, int next_map_id);

//maze_func
void initMaze(int height, int width);
void create_maze(int x, int y);
int count_mark(int x, int y);
int detect_all_mark();

// creature related
void player_set();
void player_act(int direction);
void find_player();

void magic_system();
void magic_spawn(int magic_num, int direction);

void NPC_move(int id,int direction);
void find_NPC(int id);
void rand_NPC_move();

void set_position(int x,int y,int mov_x,int mov_y);
void spawn_position(int x,int y,int id);
void rand_spawn(int id);
int collision_define(int x, int y, int next_x, int next_y);

void talk(int talker,int listener,int way);
void damage(int attacker, int defender);

// game system functions
int read_game();
void save_game();
void delete_save();
void save_map(char *map_path);
void load_map(char *map_path);
void load_creature();

void command_box();
void cheat_box();

void act_select();
void keyboard_move();
int F_number_questions();
int waiting_time=100;
int act_selected[1]={0};//1 for move, 2 for talk, 3 for attack

void setGameTitle(char *name);
void playMusic(char *musicPath, int mode);



//functions define
//init
void init_map(){
    int i,j;
    for(i=0;i<map_size_x;i++){
        for(j=0;j<map_size_y;j++){
            if(j==0||j==map_size_y-1)map[i][j]=4;
            if(i==0||i==map_size_x-1)map[i][j]=3;
        }
    }
    map[map_size_x/2][map_size_y-1]=5;
}
void clear_map_board(){
    int i, j;
    for(i=0;i<map_size_x;i++){
        for(j=0;j<map_size_y;j++)map[i][j]=0;
    }
}
void initPlayer(char skin,int map_id, int x, int y, int hp, int max_hp, int mp, int max_mp, int hunger, int attack, int shield,int money){
    player_game.skin=skin;
    player_game.map_id=map_id;
    player_game.x = x;
    player_game.y = y;
    player_game.hp = hp;
    player_game.max_hp = max_hp;
    player_game.mp = mp;
    player_game.max_mp = max_mp;
    player_game.hunger = hunger;
    player_game.attack = attack;
    player_game.shield = shield;
    player_game.money=money;
    player_game.id=10;
    player_game.addition_attack=0;
    player_game.addition_shield=0;
    player_game.addition_max_hp=0;
    player_game.addition_max_mp=0;
}
void initNpc(int id, char skin,int map_id, int x, int y, int hp, int max_hp,int attack, int shield){
    npc_game[id].skin='N';
    npc_game[id].map_id=map_id;
    npc_game[id].x=x;
    npc_game[id].y=y;
    npc_game[id].hp=hp;
    npc_game[id].max_hp=max_hp;
    npc_game[id].attack=attack;
    npc_game[id].shield=shield;
    npc_game[id].id=id+20;
}
void initMonster(int id, char skin,int map_id, int x, int y, int hp, int max_hp,int attack, int shield){
    monster_game[id].skin='M';
    monster_game[id].map_id=map_id;
    monster_game[id].x=x;
    monster_game[id].y=y;
    monster_game[id].hp=hp;
    monster_game[id].max_hp=max_hp;
    monster_game[id].attack=attack;
    monster_game[id].shield=shield;
    monster_game[id].id=id+30;
}

void act_select(){
    int selection;
    while(act_selected[0]<=0){
        printf("What do you want to do? (f1 move, f2 talk, f3 attack) \n");
        act_selected[0]=F_number_questions();
        if (act_selected[0]>3)continue;
        else print_screen();
    }
    keyboard_move();
}
void keyboard_move(){
    if(GetAsyncKeyState(VK_UP) != 0)player_act(0);
    if(GetAsyncKeyState(VK_DOWN) != 0)player_act(1);
    if(GetAsyncKeyState(VK_LEFT) != 0)player_act(2);
    if(GetAsyncKeyState(VK_RIGHT) != 0)player_act(3);
    if(GetAsyncKeyState(VK_TAB) != 0){
            item_select_next();
            print_screen();
    }
    if(GetAsyncKeyState(VK_LMENU) != 0){//Left alt
            act_selected[0]=0;
            print_screen();
    }
    if(GetAsyncKeyState('W') != 0){
            act_selected[0]=1;
            print_screen();
    }
    if(GetAsyncKeyState('T') != 0){
            act_selected[0]=2;
            print_screen();
    }
    if(GetAsyncKeyState('A') != 0){
            act_selected[0]=3;
            print_screen();
    }
    if(GetAsyncKeyState('M') != 0){
            act_selected[0]=4;
            print_screen();
            magic_system();
    }
    if(GetAsyncKeyState('I') != 0){
            state_info_shown();
            print_screen();
    }
    if(GetAsyncKeyState('E') != 0){
            item_use();
            print_screen();
    }
    if(GetAsyncKeyState(VK_CONTROL) != 0)command_box();
    Sleep(waiting_time);
}
int F_number_questions(){
     while(1)
    {
        if(GetAsyncKeyState(VK_F1) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 1;
        }
        else if(GetAsyncKeyState(VK_F2) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 2;
        }
        else if(GetAsyncKeyState(VK_F3) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 3;
        }
        else if(GetAsyncKeyState(VK_F4) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 4;
        }
        else if(GetAsyncKeyState(VK_F5) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 5;
        }
        else if(GetAsyncKeyState(VK_F6) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 6;
        }
        else if(GetAsyncKeyState(VK_F7) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 7;
        }
        else if(GetAsyncKeyState(VK_F8) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 8;
        }
        else if(GetAsyncKeyState(VK_F9) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 9;
        }
        else if(GetAsyncKeyState(VK_F10) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 10;
        }
        else if(GetAsyncKeyState(VK_F11) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 11;
        }
        else if(GetAsyncKeyState(VK_F12) != 0){
            playMusic(SYSTEM_OK,SE_MODE);
            return 12;
        }
    }
}

void turn_a_round(){
    int i,rand_num;
    turnpc_game[0]++;
    rand_num=rand()%3+1;
    for(i=0;i<rand_num;i++)rand_NPC_move();
    print_screen();
    player_behave();
}

void print_screen(){
    system("cls");
    print_map();
    print_statement();
}

void print_map(){
    int i,j;
    for(i=0;i<map_size_x;i++){
        for(j=0;j<map_size_y;j++){
            if(map[i][j]==player_game.id){
                if(player_game.hp>=5)printf("\033[33m%c\033[m",player_game.skin);//player
                else printf("\033[31m%c\033[m",player_game.skin);
            }
            else if(map[i][j]/10==2)printf("%c",npc_game[map[i][j]%10].skin);//NPC
            else if(map[i][j]/10==3)printf("%c",monster_game[map[i][j]%10].skin);//Monster
            else if(map[i][j]/10==0){
                    if(map[i][j]==7)printf("\033[31m%c\033[m",block[map[i][j]]);
                    else printf("%c",block[map[i][j]]);
            }
            else if(map[i][j]/10==-2) printf("%c",npc_game[map[i][j]*-1%10].skin);//dead body
            else printf("U");
        }
        printf("\n");
    }
}

void set_position(int x,int y,int next_x,int next_y){
    map[next_x][next_y]=map[x][y];
    map[x][y]=0;
}

void spawn_position(int x,int y,int id){
    map[x][y]=id;
}

int collision_define(int x, int y, int next_x, int next_y){
    if(next_x>map_size_x||next_y>map_size_y||next_x<0||next_y<0){
        if(map[x][y]==player_game.id)printf("You Bump the Boudary!\n");
        return 0;
    }
    if(map[next_x][next_y]!=0){
        if(map[x][y]==player_game.id){
            if(map[next_x][next_y]/10==2){//Npc to player, not specific
            print_screen();
            printf("\nBump into %s!\n",NPCs_name[map[next_x][next_y]%10]);
            }
            else if(map[next_x][next_y]/10==-2)printf("Bump into %s's dead body\n",NPCs_name[map[next_x][next_y]*-1%10]);
            else if(map[next_x][next_y]==5){
                if(map_id==0)teleport_map(player_game.id,1);
                else if(map_id==1)teleport_map(player_game.id,0);
            }
            else printf("Bump into something!\n");
            talk(map[next_x][next_y],map[x][y],0);
            system("pause");
        }
        return 0;
    }
    return 1;
}

void player_act(int direction){
    int x,y,next_x,next_y;
    int last_slash_block;
    //find_player();
    x=player_game.x;
    y=player_game.y;
    if (x>-1&&y>-1){
        next_x=x+movingX[direction];
        next_y=y+movingY[direction];
        switch (act_selected[0]){
        case 1:
            if(player_game.can_move){
                if(collision_define(x,y,next_x,next_y)){
                    set_position(x,y,next_x,next_y);
                    player_game.x=next_x;
                    player_game.y=next_y;
                    turn_a_round();
                }
            }
            break;
        case 2:
            if(map[next_x][next_y]/10==2)talk(map[next_x][next_y],player_game.id,1);
            break;
        case 3:
            if(player_game.inventory[item_select]==4){
                last_slash_block=map[next_x][next_y];
                map[next_x][next_y]=2;
                print_screen();
                playMusic(UNLOCK,SE_MODE);
                system("pause");
                map[next_x][next_y]=last_slash_block;
                print_screen();
            }
            if(map[next_x][next_y]>9)damage(player_game.id,map[next_x][next_y]);
            system("Pause");
            break;
        case 4:
            if(player_game.mp>0){
                    switch (magic_choose){
                    case 1:
                        printf("You shoot a fire ball.\n");
                        magic_spawn(magic_choose,direction);
                        break;
                    }
            }
            else printf("You have no mana.\n");
            break;
        }
    }
    else if(player_game.dead){
            print_screen();
            printf("Player can't act anymore!\n\nPlayer died already!\n");
    }
    else {
            print_screen();
            printf("can't find the player\n");
    }
}
void find_player(){
    int i,j,k=0;
    for(i=0;i<map_size_x;i++){
        for(j=0;j<map_size_y;j++){
            if(map[i][j]==player_game.id){
                player_game.x=i;
                player_game.y=j;
                k++;
                break;
            }
        }
    }
    if(k==0)player_game.x=player_game.y=-1;
}

void NPC_move(int id,int direction){
    int x,y,next_x,next_y;
    //find_NPC(npc_game[id%10].id);
    x=npc_game[id%10].x;
    y=npc_game[id%10].y;
    if (x>-1&&y>-1){
        next_x=x+movingX[direction];
        next_y=y+movingY[direction];
        if(collision_define(x,y,next_x,next_y)){
                set_position(x,y,next_x,next_y);
                        npc_game[id%10].x=next_x;
                        npc_game[id%10].y=next_y;
        }
    }
}
void find_NPC(int id){
    int i,j,k=0;
    for(i=0;i<map_size_x;i++){
        for(j=0;j<map_size_y;j++){
            if(map[i][j]==id){
                npc_game[id%10].x=i;
                npc_game[id%10].y=j;
                k++;
                break;
            }
        }
    }
    if(k==0)npc_game[id%10].x=npc_game[id%10].y=-1;
}
void rand_NPC_move(){
    int rand_num=rand()%10;
    while(npc_game[rand_num].id==0){
            rand_num=rand()%10;
    }
    if(rand()%2)NPC_move(rand_num,rand()%4);
}

void rand_spawn(int id){
    int i,j,rand_x=0,rand_y=0;
    while(map[rand_x][rand_y]!=0){
        rand_x=rand()%map_size_x;
        rand_y=rand()%map_size_y;
    }
    map[rand_x][rand_y]=id;
    if(id/10==1){
        player_game.x=rand_x;
        player_game.y=rand_y;
    }
    if(id/10==2){
        npc_game[id%10].x=rand_x;
        npc_game[id%10].y=rand_y;
    }
    if(id/10==3){
        monster_game[id%10].x=rand_x;
        monster_game[id%10].y=rand_y;
    }
}

void talk(int talker,int listener,int way){
    int talk_line = rand()%100,purpose;
    purpose=way;
    if(talker/10==2){
        if(listener/10==1){
            printf("==============================\n");
            if(purpose==0){
                printf("\n%s: ",NPCs_name[talker%10]);
                if(talk_line>=90)printf("How a beauty!");
                else if(talk_line>=80)printf("Howdy!");
                else if(talk_line>=70)printf("Hello!");
                else if(talk_line>40)printf("Get away!");
                else if(talk_line>20)printf("Get out!");
                else if(talk_line>0)printf("Don't bother me!");
                printf("\n");
            }
            if(purpose==1){
                print_screen();
                while(purpose>-1){
                printf("What's your purpose? (f1 talk, f2 trade; f3 leave)");
                purpose=F_number_questions()-1;
                if(purpose==2)purpose=-1;
                if(purpose<0)break;
                printf("\n%s: ",NPCs_name[talker%10]);
                if(purpose==0){
                    printf("I have nothing to talk with you.\n\n");
                }
                if(purpose==1){
                    while(purpose>-1){
                    printf("What's you want to get? (f1 money, f2 food, f3 healing, f4 mp; f5 to quit)\nfood: 10/10g\nhp: 2/10g\nmp: 2/10g\n");
                    purpose=F_number_questions()-1;
                    if(purpose==4)purpose=-1;
                    if(purpose<0)break;
                    printf("\n%s: ",NPCs_name[talker%10]);
                    if(purpose==0){
                        if(player_game.money<20){
                                printf("You silly begger...\n");
                                player_game.money+=10;
                        }
                        else printf("what a greedy man. You can't get any money form me.\n");
                        if(rand()%2==0){
                                printf("I can't bear you any more.\n");
                                teleport_map(player_game.id,1);
                        }
                    }
                    if(player_game.money>0){
                        if(purpose==1){
                                if(player_game.hunger<Hunger_limits){
                                printf("Please enjoy your meal.\n");
                                player_game.money-=10;
                                player_game.hunger+=10;
                                }
                                else printf("You are not hungry.\n");
                            }
                        if(purpose==2){
                                if(player_game.hp<player_game.max_hp){
                                printf("I will heal you.\n");
                                player_game.money-=10;
                                player_game.hp+=2;
                                }
                                else printf("You are healthy.\n");
                            }
                        if(purpose==3){
                                if(player_game.mp<player_game.max_mp){
                                printf("I will give you my mplayer_game.\n");
                                player_game.money-=10;
                                player_game.mp+=2;
                                }
                                else printf("You are full of mplayer_game.\n");
                            }
                        }
                        else printf("I can't help you. If you don't have any money.\n");
                        printf("\n");
                        system("pause");
                        print_screen();
                    }
                }
                if(purpose>1)continue;
            }
            printf("\n");
            printf("==============================\n");
            }
        }
    }
}
void damage(int attacker, int defender){
    if(attacker==player_game.id){
        if(defender/10==2){
            if(npc_game[defender%10].shield-player_game.attack<0)npc_game[defender%10].hp-=(player_game.attack-npc_game[defender%10].shield);
            if(npc_game[defender%10].hp<=0){
                npc_game[defender%10].id*=-1;
                map[npc_game[defender%10].x][npc_game[defender%10].y]=npc_game[defender%10].id;
                npc_game[defender%10].skin='&';
            }
            turn_a_round();
            if(npc_game[defender%10].hp<=0)printf("Player had slayed %s\n",NPCs_name[defender%10]);
            else if(npc_game[defender%10].shield-player_game.attack<0)printf("Player caused %s %d damage\n",NPCs_name[defender%10],player_game.attack-npc_game[defender%10].shield);
            else printf("Player can't caused any damage on %s\n",NPCs_name[defender%10]);
        }
        else if(defender/10==3){
            if(monster_game[defender%10].shield-player_game.attack<0)monster_game[defender%10].hp-=(player_game.attack-monster_game[defender%10].shield);
            if(monster_game[defender%10].hp<=0){
                monster_game[defender%10].id*=-1;
                monster_game[defender%10].skin='&';
            }
            turn_a_round();
            if(monster_game[defender%10].shield-player_game.attack<0)printf("Player caused Monster %d damage\n",player_game.attack-monster_game[defender%10].shield);
            else printf("Player can't caused any damage\n");
        }
    }
}

int read_game(){
    int read,i,j;
    FILE *gameFile;
    gameFile = fopen( "rpg_save.txt","r" );
    if( gameFile != NULL ){
        printf("Do you want to read the last game? (f1 yes, f2 no)\n");
        read=F_number_questions()%2;
    }
    if(read){
        if( gameFile == NULL ){
        printf( "open failure\n\n" );
        fclose(gameFile);
        system("pause");
        return 0;
        }
        else{
                fscanf(gameFile,"Map");fscanf(gameFile,",");
                fscanf(gameFile,"%d",&map_id);fscanf(gameFile,",");
                fscanf(gameFile,"\n");fscanf(gameFile,"Player");
                fscanf(gameFile,",");fscanf(gameFile, "%c",&player_game.skin);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.id);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.map_id);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.x);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.y);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.hp);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.max_hp);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.mp);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.max_mp);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.hunger);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.attack);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.shield);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.money);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.addition_attack);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.addition_shield);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.addition_max_hp);
                fscanf(gameFile,",");fscanf(gameFile, "%d",&player_game.addition_max_mp);
                fscanf(gameFile,",");
                fscanf(gameFile, "Inventory");fscanf(gameFile, ",");
                for(i=0;i<player_inventory_limits;i++){
                    fscanf(gameFile, "%d",&player_game.inventory[i]);
                    fscanf(gameFile, ",");
                }
                fscanf(gameFile,"\n");
                for(i=0;i<NPC_amount;i++){
                    fscanf(gameFile,"Npc");
                    fscanf(gameFile,",");fscanf(gameFile, "%c",&npc_game[i].skin);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&npc_game[i].id);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&npc_game[i].map_id);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&npc_game[i].x);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&npc_game[i].y);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&npc_game[i].hp);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&npc_game[i].max_hp);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&npc_game[i].attack);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&npc_game[i].shield);
                    fscanf(gameFile,",");
                    fscanf(gameFile,"\n");
                }
                for(i=0;i<Monster_amount;i++){
                    fscanf(gameFile,"Monster");
                    fscanf(gameFile,",");fscanf(gameFile, "%c",&monster_game[i].skin);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&monster_game[i].id);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&monster_game[i].map_id);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&monster_game[i].x);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&monster_game[i].y);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&monster_game[i].hp);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&monster_game[i].max_hp);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&monster_game[i].attack);
                    fscanf(gameFile,",");fscanf(gameFile, "%d",&monster_game[i].shield);
                    fscanf(gameFile,",");
                    fscanf(gameFile,"\n");
                }
                fscanf(gameFile,"\n");
            printf("Loading...\n");
            fclose(gameFile);
            load_map(mapPath[map_id]);
            load_creature();
            printf("Load is done!\n\n");
            return 1;
        }
    }
    return 0;
}
void save_game(){
    int read,i,j;
    FILE *gameFile;
    gameFile = fopen("rpg_save.txt","w");
    fprintf(gameFile, "Map,");
    fprintf(gameFile, "%d,",map_id);
    fprintf(gameFile,"\n");
    fprintf(gameFile, "Player,");
    fprintf(gameFile, "%c,",player_game.skin);
    fprintf(gameFile, "%d,",player_game.id);
    fprintf(gameFile, "%d,",player_game.map_id);
    fprintf(gameFile, "%d,",player_game.x);
    fprintf(gameFile, "%d,",player_game.y);
    fprintf(gameFile, "%d,",player_game.hp);
    fprintf(gameFile, "%d,",player_game.max_hp);
    fprintf(gameFile, "%d,",player_game.mp);
    fprintf(gameFile, "%d,",player_game.max_mp);
    fprintf(gameFile, "%d,",player_game.hunger);
    fprintf(gameFile, "%d,",player_game.attack);
    fprintf(gameFile, "%d,",player_game.shield);
    fprintf(gameFile, "%d,",player_game.money);
    fprintf(gameFile, "%d,",player_game.addition_attack);
    fprintf(gameFile, "%d,",player_game.addition_shield);
    fprintf(gameFile, "%d,",player_game.addition_max_hp);
    fprintf(gameFile, "%d,",player_game.addition_max_mp);
    fprintf(gameFile, "Inventory,");
    for(i=0;i<player_inventory_limits;i++){
        fprintf(gameFile, "%d,",player_game.inventory[i]);
    }
    fprintf(gameFile,"\n");
    for(i=0;i<NPC_amount;i++){
        fprintf(gameFile, "Npc,");
        fprintf(gameFile, "%c,",npc_game[i].skin);
        fprintf(gameFile, "%d,",npc_game[i].id);
        fprintf(gameFile, "%d,",npc_game[i].map_id);
        fprintf(gameFile, "%d,",npc_game[i].x);
        fprintf(gameFile, "%d,",npc_game[i].y);
        fprintf(gameFile, "%d,",npc_game[i].hp);
        fprintf(gameFile, "%d,",npc_game[i].max_hp);
        fprintf(gameFile, "%d,",npc_game[i].attack);
        fprintf(gameFile, "%d,",npc_game[i].shield);
        fprintf(gameFile,"\n");
    }
    for(i=0;i<Monster_amount;i++){
        fprintf(gameFile, "Monster,");
        fprintf(gameFile, "%c,",monster_game[i].skin);
        fprintf(gameFile, "%d,",monster_game[i].id);
        fprintf(gameFile, "%d,",monster_game[i].map_id);
        fprintf(gameFile, "%d,",monster_game[i].x);
        fprintf(gameFile, "%d,",monster_game[i].y);
        fprintf(gameFile, "%d,",monster_game[i].hp);
        fprintf(gameFile, "%d,",monster_game[i].max_hp);
        fprintf(gameFile, "%d,",monster_game[i].attack);
        fprintf(gameFile, "%d,",monster_game[i].shield);
        fprintf(gameFile,"\n");
    }
    fprintf(gameFile,"\n");
    printf("\nHas been saving...\n");
    fclose(gameFile);
    save_map(mapPath[map_id]);
    printf("Saving is done!\n\n");
}
void delete_save(){
   int ret;
   char filename[] = "rpg_save.txt";
   ret = remove(filename);
   if(ret == 0)printf("File deleted successfully\n\n");
   else printf("Error: unable to delete the file\n\n");
}
void save_map(char *map_path){
    int i, j;
    FILE *map_file = NULL;
    if((map_file = fopen(map_path, "w")) != NULL){
    for(i=0;i<map_size_x;i++){
        for(int j=0;j<map_size_y;j++){
                if(map[i][j]>=10)fprintf(map_file, "0,");
                else fprintf(map_file, "%d,",map[i][j]);
            }
        fprintf(map_file,"\n");
        }
        fclose(map_file);
    }
}
void load_map(char *map_path){
    int i, j;
    FILE *map_file = NULL;
    if((map_file = fopen(map_path, "r")) != NULL){
    for(i=0;i<map_size_x;i++){
            for(j=0;j<map_size_y;j++){
                    fscanf(map_file,"%d",&map[i][j]);
                    fscanf(map_file,",");
            }
            fscanf(map_file,"\n");
        }
        fclose(map_file);
        map_creature_clear();
    }
}
void load_creature(){
    int i;
    if(player_game.map_id==map_id){
        spawn_position(player_game.x,player_game.y,player_game.id);
    }
    for(i=0;i<NPC_amount;i++){
        if(npc_game[i].id!=0){
            if(npc_game[i].map_id==map_id){
                spawn_position(npc_game[i].x,npc_game[i].y,npc_game[i].id);
            }
        }
    }
    for(i=0;i<Monster_amount;i++){
        if(monster_game[i].id!=0){
            if(monster_game[i].map_id==map_id){
                spawn_position(monster_game[i].x,monster_game[i].y,monster_game[i].id);
            }
        }
    }
}
void title_screen(){//need fixed
     printf("+-----------------------------+\n"
            "|                             |\n"
            "|                             |\n"
            "|                             |\n"
            "|   The Adventure of ASCII    |\n"
            "|                             |\n"
            "|                             |\n"
            "|=============================|\n"
            "|                             |\n"
            "|                             |\n"
            "|                             |\n"
            "|                             |\n"
            "|     [2021       JiXun]      |\n"
            "|                             |\n"
            "|                             |\n"
            "+-----------------------------+\n\n");
    system("pause");
    playMusic(SYSTEM_OK,SE_MODE);
    system("cls");
}

void print_statement(){
    int i;
    printf("==============================\n");
    if(statement_info[0]){
            printf("Hp: %d/%d, Mp: %d/%d, Hunger: %d/%d",player_game.hp,player_game.max_hp,player_game.mp,player_game.max_mp,player_game.hunger,Hunger_limits);
        if(player_game.attack>0){
            printf(", Atk: ");
            for(i=0;i<player_game.attack;i++)printf("+");
        }
        if(player_game.shield>0){
            printf(", Shield: ");
            for(i=0;i<player_game.shield;i++)printf("+");
        }
    }
    if(statement_info[1])printf("\nItem slot%d select[\033[47m\033[30m %s \033[m\033[m]",item_select+1,item_name[player_game.inventory[item_select]]);
    if(statement_info[2])printf("\nNow at: %s",map_name[map_id]);
    if(statement_info[3])printf("\nAct: [%s] mode",act_mode_name[act_selected[0]]);
    if(statement_info[4])printf("\nMoney: %d",player_game.money);
    if(statement_info[5])printf("\nTurn: %d",turnpc_game[0]);
    if(statement_info[6])printf("\nPosition: (%d, %d)",player_game.x,player_game.y);
    printf("\n==============================\n");
}
void state_info_shown(){
    int point;
    printf("What are you want to close or open?\n(f1 basic, f2 item select, f3 map, f4 act mode, f5 money, f6 turn, f7 position)");
    point=F_number_questions()-1;
    if(point<7){
        statement_info[point]=(statement_info[point]+1)%2;
        print_screen();
    }
}

void skin_change(int id){
    if(id==player_game.id){
        printf("Pleaze enter the one character as your player: ");
        scanf("%s",&player_game.skin);
    }
}

void hunger_system(){
    if(turnpc_game[0]%10==0)if(rand()%2)player_game.hunger--;
    if(player_game.hunger<10)printf("Warning ! you are starving !\n");
    if(player_game.hunger<5)if(rand()%2)player_game.hp--;
}
void hp_detect(){
    player_game.max_hp=10+player_game.addition_max_hp;
    if(player_game.hp<=0){
            print_screen();
            printf("you are died.\n");
            player_game.dead=1;
            player_game.can_move=0;
    }
    else if(player_game.hp>0){
        player_game.dead=0;
        player_game.can_move=1;
    }
}
void attack_detect(){
    player_game.attack=player_game.addition_attack;
    if(player_game.inventory[item_select]==4){
            player_game.attack++;
    }
}
void shield_detect(){
    player_game.shield=player_game.addition_shield;
    if(player_game.inventory[item_select]==5){
            player_game.shield++;
    }
}
void player_behave(){
    hunger_system();
    hp_detect();
    attack_detect();
    shield_detect();
}

void command_box(){
    int confirm,command_number;
    printf("Do you want to use command? (f1 yes, f2 no)\n");
    confirm=F_number_questions()%2;
    if(confirm){
        printf("What's your need? (f1 quit, f2 save, f3 load, f4 skin, f5 delete save)\n");
        command_number=F_number_questions();
        switch (command_number){
            case 1:
                printf("Do you want to quit the game? (f1 yes, f2 no)\n");
                confirm=F_number_questions()%2;
                if(confirm){
                    system("pause");
                    exit(0);
                }
                break;
            case 2:
                printf("Do you want to save the game? (f1 yes, f2 no)\n");
                confirm=F_number_questions()%2;
                if(confirm)save_game();
                break;
            case 3:
                printf("Do you want to load the game? (f1 yes, f2 no)\n");
                confirm=F_number_questions()%2;
                if(confirm)if(read_game())print_screen();
                hp_detect();
                break;
            case 4:
                printf("Do you want to change the skin? (f1 yes, f2 no)\n");
                confirm=F_number_questions()%2;
                if(confirm)skin_change(player_game.id);
                print_screen();
                break;
            case 5:
                printf("Do you want to delete save file? (f1 yes, f2 no)\n");
                confirm=F_number_questions()%2;
                if(confirm)delete_save();
                break;
            case 6:
                printf("Do you want to change wait-key time? (f1 yes, f2 no)\n");
                confirm=F_number_questions()%2;
                if(confirm){
                    printf("What interval time you want? (suggested upper 500) ");
                    scanf("%d",&waiting_time);
                }
                break;
            case 7:
                cheat_box();
                break;
        }
    }
}
void cheat_box(){
    int confirm,cheat_number, item_id, mode, cheat_actived=0;
    printf("Do you want to cheat? (f1 yes, f2 no)\n");
    confirm=F_number_questions()%2;
    if(confirm){
        printf("You cheater...\n");
        cheat_actived=1;
    while(cheat_actived){
    printf("What's your need? (f1 health edit, f2 mp edit, f3 hunger edit, f4 atk set, f5 def set, f6 item give)\n");
    cheat_number=F_number_questions();
    switch (cheat_number){
        case 1:
            printf("Hp: What mode do you prefer? (f1 full, f2 edit)");
            mode=F_number_questions();
            if(mode==1){
            player_game.hp=player_game.max_hp;
            printf("Hp full actived\n");
            }
            if(mode==2){
                printf("HP: ");
                scanf("%d",&player_game.hp);
            }
            break;
        case 2:
            printf("MP: What mode do you prefer? (f1 full, f2 edit)");
            mode=F_number_questions();
            if(mode==1){
            player_game.mp=player_game.max_mp;
            printf("Mp full actived\n");
            }
            if(mode==2){
                printf("MP: ");
                scanf("%d",&player_game.mp);
            }
            break;
        case 3:
            printf("Hunger: What mode do you prefer? (f1 full, f2 edit)");
            mode=F_number_questions();
            if(mode==1){
            player_game.hunger=Hunger_limits;
            printf("Hunger full actived\n");
            }
            if(mode==2){
                printf("Hunger: ");
                scanf("%d",&player_game.hunger);
            }
            break;
        case 4:
            printf("Atk: ");
            scanf("%d",&player_game.attack);
            break;
        case 5:
            printf("Def: ");
            scanf("%d",&player_game.shield);
            break;
        case 6:
            printf("Input item id: ");
            scanf("%d",&item_id);
            item_give(player_game.id,item_id,item_select);
            break;
            }
        printf("Continue? (f1 continue, f2 quit)\n");
        cheat_actived=F_number_questions()%2;
        }
    }
}

void playMusic(char *musicPath, int mode){
    if(mode == BGM_MODE){
        PlaySound(musicPath, NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
    }
    else if(mode == SE_MODE){
        PlaySound(musicPath, NULL, SND_SYNC|SND_FILENAME);
    }
}
void setGameTitle(char *name){
    char commandBuffer[64] = "TITLE ";
    strcat(commandBuffer, name);
    system(commandBuffer);
}

//maze
void initMaze(int height, int width){
    int i, j;
    for(i=0;i<2 * height + 1;i++){
        for(j=0;j<2 * width + 1;j++){
            if(i%2==1&&j%2==1)map[i][j]=6;
            else map[i][j]=1;
        }
    }
    map[1][0]=0;
    map[2*height-1][2*width]=5;
}
void create_maze(int x, int y){
    if(map[x][y]==6)map[x][y]=0;
    int count,direction;
    int next_x,next_y;
    while(1){
    count=count_mark(x,y);
    if(count==0)return;
        while(1){
        direction=rand()%4;
        next_x=x+movingX[direction]*2;
        next_y=y+movingY[direction]*2;
        if(map[next_x][next_y]==6)break;
    }
    map[(x+next_x)/2][(y+next_y)/2]=0;
    map[next_x][next_y]=0;
    create_maze(next_x,next_y);
    }
}
int count_mark(int x, int y){
    int count=0,i;
    for(i=0;i<4;i++){
        if(map[x+movingX[i]*2][y+movingY[i]*2]==6)count++;
    }
    return count;
}
int detect_all_mark(int height, int width){
    int count,i,j;
    for(i=0;i<2 * height + 1;i++){
        for(j=0;j<2 * width + 1;j++){
            if(map[i][j]==6)count++;
        }
    }
    return count;
}

void map_creature_clear(){//broke
    int i;
    if(player_game.map_id!=map_id)map[player_game.x][player_game.y]=0;
    else if(player_game.map_id==map_id)map[player_game.x][player_game.y]=player_game.id;
    for(i=0;i<NPC_amount;i++){
        if(npc_game[i].skin!=NULL&&npc_game[i].id!=0){
        if(npc_game[i].map_id!=map_id)map[npc_game[i].x][npc_game[i].y]=0;
        else if(npc_game[i].map_id==map_id)map[npc_game[i].x][npc_game[i].y]=npc_game[i].id;
        }
    }
    for(i=0;i<Monster_amount;i++){
        if(monster_game[i].skin!=NULL&&monster_game[i].id!=0){
        if(monster_game[i].map_id!=map_id)map[monster_game[i].x][monster_game[i].y]=0;
        else if(monster_game[i].map_id==map_id)map[monster_game[i].x][monster_game[i].y]=monster_game[i].id;
        }
    }
}
void teleport_map(int id, int next_map_id){
    int i, j;
    printf("\nYou are now going to %s\n\n",map_name[next_map_id]);
    system("pause");
    if(next_map_id==0){
        if(player_game.id==id){
            save_map(mapPath[map_id]);
            clear_map_board();
            map_id=next_map_id;
            player_game.x=map_size_x/2;
            player_game.y=map_size_y-2;
            player_game.map_id=next_map_id;
            load_map(mapPath[next_map_id]);
        }
    }
    else if(next_map_id==1){
        if(player_game.id==id){
            save_map(mapPath[map_id]);
            clear_map_board();
            map_creature_clear();
            map_id=next_map_id;
            player_game.map_id=next_map_id;
            initMaze(7,15);
            create_maze(1,1);
            player_game.x=1;
            player_game.y=0;
        }
        map_creature_clear();
    }
    print_screen();
}

void item_give(int id, int item_id, int slot){
    if(id==player_game.id){
        if(slot<player_inventory_limits){
            player_game.inventory[slot]=item_id;
        }
    }
}
void item_select_next(){
    item_select=(item_select+1)%8;
    attack_detect();
    shield_detect();
}
void item_use(){
    //1 hp potion, 2 mp potion,3 food, 4 sword, 5 shield
    switch (player_game.inventory[item_select]){
    case 0:
        break;
    case 1:
        if(player_game.hp<player_game.max_hp){
            player_game.inventory[item_select]=0;
            player_game.hp+=5;
        }
        break;
    case 2:
        if(player_game.mp<player_game.max_mp){
            player_game.inventory[item_select]=0;
            player_game.mp+=5;
        }
        break;
    case 3:
        if(player_game.hunger<Hunger_limits){
            player_game.inventory[item_select]=0;
            player_game.hunger+=30;
        }
        break;
    }
}

void magic_system(){
    printf("Please choose what magic you want to use:\n");
    printf("F1 Fire Ball(need direction)\n");
    magic_choose=0;
    while(magic_choose==0){
    magic_choose=F_number_questions();
    if (magic_choose>5)continue;
    }
}
void magic_spawn(int magic_num, int direction){
    int last_x,last_y,last_block,last_time=0;
    last_x=player_game.x;
    last_y=player_game.y;
    switch (magic_num){
    case 1:
        while(last_time<5){
        last_x+=movingX[direction];
        last_y+=movingY[direction];
        last_block=map[last_x][last_y];
        map[last_x][last_y]=7;
        print_screen();
        system("pause");
        map[last_x][last_y]=last_block;
        last_time++;
        }
        break;
    }
}
